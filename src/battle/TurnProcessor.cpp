#include "TurnProcessor.h"

#include "BattleContext.h"
#include "BattleCalculations.h"
#include "RandomEngine.h"
#include "StatusEffectProcessor.h"
#include "WinChecker.h"
#include "SwitchExecutor.h"
#include "MoveExecutor.h"

TurnProcessor::TurnProcessor(BattleContext& context, BattleCalculations& calculations, StatusEffectProcessor& statusProcessor, WinChecker& winChecker, SwitchExecutor& switchExecutor, MoveExecutor& moveExecutor)
	: m_context(context)
	, m_calculations(calculations)
	, m_statusProcessor(statusProcessor)
	, m_winChecker(winChecker)
	, m_switchExecutor(switchExecutor)
	, m_moveExecutor(moveExecutor)
	{}

void TurnProcessor::DetermineTurnOrder()
{
	if (m_context.playerOne->IsSwitching() && m_context.playerTwo->IsSwitching())
	{
		m_calculations.RandomizeTurnOrder();
		return;
	}

	constexpr int SWITCH_PRIORITY{ 6 };

	const auto* moveOne = m_context.playerOneCurrentMove;
	const auto* moveTwo = m_context.playerTwoCurrentMove;

	auto moveOnePriority = (m_context.playerOne->IsSwitching()) ? SWITCH_PRIORITY : moveOne->GetPriority();
	auto moveTwoPriority = (m_context.playerTwo->IsSwitching()) ? SWITCH_PRIORITY : moveTwo->GetPriority();

	if (moveOnePriority > moveTwoPriority)
	{
		m_calculations.SetFirst(*m_context.playerOne, *m_context.playerTwo);
		return;
	}
	else if (moveOnePriority < moveTwoPriority)
	{
		m_calculations.SetFirst(*m_context.playerTwo, *m_context.playerOne);
		return;
	}

	int playerOneSpeed = m_calculations.CalculatePokemonSpeed(*m_context.playerOneCurrentPokemon);
	int playerTwoSpeed = m_calculations.CalculatePokemonSpeed(*m_context.playerTwoCurrentPokemon);

	if (playerOneSpeed > playerTwoSpeed)
	{
		m_calculations.SetFirst(*m_context.playerOne, *m_context.playerTwo);
		return;
	}
	else if (playerTwoSpeed > playerOneSpeed)
	{
		m_calculations.SetFirst(*m_context.playerTwo, *m_context.playerOne);
		return;
	}
	else
	{
		m_calculations.RandomizeTurnOrder();
	}
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
		m_switchExecutor.ExecuteSwitch(*m_context.attackingPlayer, m_context.attackingPokemon);

		return;
	}
	else if (m_context.attackingPlayer->HasSwitched())
	{
		m_context.attackingPlayer->SetHasSwitched(false);
		return;
	}

	else if (!m_context.attackingPokemon->IsFainted() && !m_context.defendingPokemon->IsFainted())
	{
		if (!m_statusProcessor.CheckPerformativeStatus())
		{
			return;
		}

		m_moveExecutor.ExecuteMove();
	}

	m_statusProcessor.RageCheck();

	winCondition = m_winChecker.CheckWinCondition(*m_context.attackingPlayer, *m_context.defendingPlayer);
}

void TurnProcessor::SwapRoles()
{
	std::swap(m_context.attackingPlayer, m_context.defendingPlayer);
	std::swap(m_context.attackingPokemon, m_context.defendingPokemon);
	m_context.currentMove = (m_context.playerOneCurrentMove == m_context.currentMove) ? m_context.playerTwoCurrentMove : m_context.playerOneCurrentMove;
}