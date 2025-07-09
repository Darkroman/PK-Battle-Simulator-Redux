#include "TurnProcessor.h"
#include "BattleContext.h"
#include "BattleCalculations.h"
#include "RandomEngine.h"
#include "WinChecker.h"
#include "TurnUtils.h"
#include "MoveExecutor.h"

TurnProcessor::TurnProcessor(BattleContext& context, BattleCalculations& calculations, RandomEngine& rng, BattleStatusManager& statusManager, WinChecker& winChecker, TurnUtils& turnUtils, MoveExecutor& moveExecutor)
	: m_context(context)
	, m_calculations(calculations)
	, m_rng(rng)
	, m_statusManager(statusManager)
	, m_winChecker(winChecker)
	, m_turnUtils(turnUtils)
	, m_moveExecutor(moveExecutor)
	{}

void TurnProcessor::DetermineWhoGoesFirst()
{
	double stageMultiplierOne = m_calculations.CalculateStageModifier(m_context.playerOneCurrentPokemon->GetSpeedStage());
	double stageMultiplierTwo = m_calculations.CalculateStageModifier(m_context.playerTwoCurrentPokemon->GetSpeedStage());

	double playerOneSpeed = std::floor(m_context.playerOneCurrentPokemon->GetSpeed() * stageMultiplierOne);
	double playerTwoSpeed = std::floor(m_context.playerTwoCurrentPokemon->GetSpeed() * stageMultiplierTwo);

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

	const auto* moveOne = m_context.playerOneCurrentMove->mp_move;
	const auto* moveTwo = m_context.playerTwoCurrentMove->mp_move;

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

void TurnProcessor::DeterminePostTurnDamageOrder()
{
	double playerOneSpeed = std::floor(m_context.playerOneCurrentPokemon->GetSpeed() * m_calculations.CalculateStageModifier(m_context.playerOneCurrentPokemon->GetSpeedStage()));

	double playerTwoSpeed = std::floor(m_context.playerTwoCurrentPokemon->GetSpeed() * m_calculations.CalculateStageModifier(m_context.playerTwoCurrentPokemon->GetSpeedStage()));

	if (m_context.playerOneCurrentPokemon->GetStatus() == Status::Paralyzed)
	{
		playerOneSpeed /= 2;
	}

	if (m_context.playerTwoCurrentPokemon->GetStatus() == Status::Paralyzed)
	{
		playerTwoSpeed /= 2;
	}

	if ((playerOneSpeed > playerTwoSpeed) && (m_context.attackingPlayer != m_context.playerOne))
	{
		m_context.attackingPlayer = m_context.playerOne;
		m_context.defendingPlayer = m_context.playerTwo;
		m_context.attackingPokemon = m_context.playerOneCurrentPokemon;
		m_context.defendingPokemon = m_context.playerTwoCurrentPokemon;
	}
	else if ((playerTwoSpeed > playerOneSpeed) && (m_context.attackingPlayer == m_context.playerOne))
	{
		m_context.attackingPlayer = m_context.playerTwo;
		m_context.defendingPlayer = m_context.playerOne;
		m_context.attackingPokemon = m_context.playerTwoCurrentPokemon;
		m_context.defendingPokemon = m_context.playerOneCurrentPokemon;
	}
}

void TurnProcessor::ExecuteTurn(bool& winCondition)
{
	if (m_context.attackingPlayer->IsSwitching())
	{
		m_turnUtils.PerformSwitch(m_context.attackingPlayer, m_context.attackingPokemon);
		if (m_context.defendingPokemon->IsBound())
		{
			m_context.defendingPokemon->SetBoundTurnCount(m_context.defendingPokemon->GetBoundCounter());
		}
		return;
	}
	else if (m_context.attackingPlayer->HasSwitched())
	{
		m_context.attackingPlayer->SetHasSwitched(false);
		return;
	}

	else if (!m_context.attackingPokemon->IsFainted())
	{
		if (!m_statusManager.CheckPerformativeStatus())
		{
			return;
		}

		m_moveExecutor.ExecuteMove();
	}

	m_statusManager.RageCheck();

	winCondition = m_winChecker.CheckWinCondition(m_context.attackingPlayer, m_context.defendingPlayer);
}

void TurnProcessor::SwapRoles()
{
	std::swap(m_context.attackingPlayer, m_context.defendingPlayer);
	std::swap(m_context.attackingPokemon, m_context.defendingPokemon);
	m_context.currentMove = (m_context.playerOneCurrentMove == m_context.currentMove) ? m_context.playerTwoCurrentMove : m_context.playerOneCurrentMove;
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