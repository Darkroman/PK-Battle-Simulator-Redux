#include "TurnProcessor.h"

#include "BattleContext.h"
#include "BattleCalculations.h"
#include "RandomEngine.h"
#include "StatusEffectProcessor.h"
#include "WinChecker.h"
#include "SwitchExecutor.h"
#include "MoveExecutor.h"

TurnProcessor::TurnProcessor(BattleContext& context, BattleCalculations& calculations, RandomEngine& rng, StatusEffectProcessor& statusProcessor, WinChecker& winChecker, SwitchExecutor& switchExecutor, MoveExecutor& moveExecutor)
	: m_context(context)
	, m_calculations(calculations)
	, m_rng(rng)
	, m_statusProcessor(statusProcessor)
	, m_winChecker(winChecker)
	, m_switchExecutor(switchExecutor)
	, m_moveExecutor(moveExecutor)
	{}

void TurnProcessor::DetermineWhoGoesFirst()
{
	auto [numerator1, denominator1] = m_calculations.GetStageRatio(m_context.playerOneCurrentPokemon->GetSpeedStage());
	int playerOneSpeed = m_context.playerOneCurrentPokemon->GetSpeed() * numerator1 / denominator1;

	auto [numerator2, denominator2] = m_calculations.GetStageRatio(m_context.playerTwoCurrentPokemon->GetSpeedStage());
	int playerTwoSpeed = m_context.playerTwoCurrentPokemon->GetSpeed() * numerator2 / denominator2;

	if (m_context.playerOneCurrentPokemon->GetStatus() == Status::Paralyzed)
	{
		playerOneSpeed /= 2;
	}
	if (m_context.playerTwoCurrentPokemon->GetStatus() == Status::Paralyzed)
	{
		playerTwoSpeed /= 2;
	}

	if (m_context.playerOne->IsSwitching() && !m_context.playerTwo->IsSwitching())
	{
		SetFirst(m_context.playerOne, m_context.playerTwo);
		return;
	}
	if (m_context.playerTwo->IsSwitching() && !m_context.playerOne->IsSwitching())
	{
		SetFirst(m_context.playerTwo, m_context.playerOne);
		return;
	}
	if (m_context.playerOne->IsSwitching() && m_context.playerTwo->IsSwitching())
	{
		SetFirst(m_context.playerOne, m_context.playerTwo);
		return;
	}

	const auto* moveOne = m_context.playerOneCurrentMove;
	const auto* moveTwo = m_context.playerTwoCurrentMove;

	if (moveOne->GetPriority() > moveTwo->GetPriority())
	{
		SetFirst(m_context.playerOne, m_context.playerTwo);
		return;
	}
	if (moveTwo->GetPriority() > moveOne->GetPriority())
	{
		SetFirst(m_context.playerTwo, m_context.playerOne);
		return;
	}

	if (playerOneSpeed > playerTwoSpeed)
	{
		SetFirst(m_context.playerOne, m_context.playerTwo);
		return;
	}
	if (playerTwoSpeed > playerOneSpeed)
	{
		SetFirst(m_context.playerTwo, m_context.playerOne);
		return;
	}

	std::uniform_int_distribution<int> firstDist(1, 2);
	int firstMod = firstDist(m_rng.GetGenerator());

	if (firstMod == 2)
	{
		SetFirst(m_context.playerTwo, m_context.playerOne);
	}
	else
	{
		SetFirst(m_context.playerOne, m_context.playerTwo);
	}
}

void TurnProcessor::SetFirst(Player* first, Player* second)
{
	first->SetFirst(true);
	second->SetFirst(false);
	m_context.attackingPlayer = first;
	m_context.defendingPlayer = second;
	m_context.attackingPokemon = (first == m_context.playerOne) ? m_context.playerOneCurrentPokemon : m_context.playerTwoCurrentPokemon;
	m_context.defendingPokemon = (second == m_context.playerOne) ? m_context.playerOneCurrentPokemon : m_context.playerTwoCurrentPokemon;
	m_context.currentMove = (first == m_context.playerOne) ? m_context.playerOneCurrentMove : m_context.playerTwoCurrentMove;
}

void TurnProcessor::ExecuteTurn(bool& winCondition)
{
	auto& ctx = m_context;

	if (ctx.currentMove)
	{
		if (ctx.currentMove->GetName() != "Counter")
		{
			ctx.damageTaken = 0;
		}
	}
	

	ctx.pixelsLost = 0;
	ctx.initialPowerMultiplier = 10;
	ctx.effectiveness = 4096;

	ctx.flags.ResetBattleFlags();

	if (m_context.attackingPlayer->IsSwitching())
	{
		m_switchExecutor.ExecuteSwitch(m_context.attackingPlayer, m_context.attackingPokemon);

		return;
	}
	else if (m_context.attackingPlayer->HasSwitched())
	{
		m_context.attackingPlayer->SetHasSwitched(false);
		return;
	}

	else if (!m_context.attackingPokemon->IsFainted())
	{
		if (!m_statusProcessor.CheckPerformativeStatus())
		{
			return;
		}

		m_moveExecutor.ExecuteMove();
	}

	m_statusProcessor.RageCheck();

	winCondition = m_winChecker.CheckWinCondition(m_context.attackingPlayer, m_context.defendingPlayer);
}

void TurnProcessor::SwapRoles()
{
	std::swap(m_context.attackingPlayer, m_context.defendingPlayer);
	std::swap(m_context.attackingPokemon, m_context.defendingPokemon);
	m_context.currentMove = (m_context.playerOneCurrentMove == m_context.currentMove) ? m_context.playerTwoCurrentMove : m_context.playerOneCurrentMove;
}