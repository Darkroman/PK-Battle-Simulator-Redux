#include "TurnManager.h"
#include "BattleContext.h"
#include "RandomEngine.h"
#include "../ui/interfaces/IBattleMenuUI.h"
#include "../ui/interfaces/IMoveResultsUI.h"

TurnManager::TurnManager(BattleContext& context, RandomEngine& rng, IBattleMenuUI& battleMenuUI, IMoveResultsUI& moveResultsUI)
	: m_context(context)
	, m_rng(rng)
	, m_battleMenuUI(battleMenuUI)
	, m_calculations(context, rng)
	, m_statusManager(context, rng)
	, m_turnProcessor(context, m_calculations, rng, battleMenuUI, moveResultsUI)
	, m_winChecker(context, m_turnProcessor, battleMenuUI)
	, m_postTurnProcessor(context, m_statusManager, m_winChecker, rng)
	, m_moveResultsUI(moveResultsUI)
{}

bool TurnManager::RunBattleLoop()
{
	int turnCount{ 0 };
	bool winCondition{ false };

	m_battleMenuUI.ThrowOutFirstPokemon();

	while (winCondition == false)
	{
		++turnCount;
		std::cout << "TURN #" << turnCount << "\n\n";

		m_battleMenuUI.DisplayFightingPokemon();

		m_battleMenuUI.PlayerOneMakeSelection();

		if (m_context.playerOne->HasForfeited())
		{
			winCondition = m_winChecker.CheckWinCondition(m_context.playerTwo, m_context.playerOne);
			return false;
		}

		m_battleMenuUI.PlayerTwoMakeSelection();

		if (m_context.playerTwo->HasForfeited())
		{
			winCondition = m_winChecker.CheckWinCondition(m_context.playerOne, m_context.playerTwo);
			return false;
		}

		m_turnProcessor.DetermineWhoGoesFirst();

		m_turnProcessor.ExecuteTurn(winCondition);
		if (winCondition) { continue; }

		std::cout << '\n';

		m_turnProcessor.SwapRoles();

		m_turnProcessor.ExecuteTurn(winCondition);
		if (winCondition) { continue; }

		m_turnProcessor.DeterminePostTurnDamageOrder();

		std::cout << '\n';

		m_postTurnProcessor.ProcessAllPostTurnEffects();
		if (winCondition) { break; }

		m_context.playerOne->SetHasSwitched(false);
		m_context.playerTwo->SetHasSwitched(false);
		m_context.damageTaken = 0;

		std::cout << '\n';

#if !defined NDEBUG
		// ResetHPandPPForTesting();
#endif
	}

	turnCount = 0;
	return false;
}

bool TurnManager::AnnounceWinner()
{
	if (m_context.playerOne->HasWon() && m_context.playerTwo->HasWon())
	{
		std::cout << "Both players Pokemon have fainted in the same turn!\n";
		std::cout << "It is a tie game!\n\n";
		ResetValues();
		return false;
	}

	if (m_context.playerOne->HasWon())
	{
		std::cout << m_context.playerOne->GetPlayerNameView() << " wins!\n\n";
		ResetValues();
		return false;
	}

	if (m_context.playerTwo->HasWon())
	{
		std::cout << m_context.playerTwo->GetPlayerNameView() << " wins!\n\n";
		ResetValues();
		return false;
	}

	return false;
}

void TurnManager::ResetValues()
{
	m_context.playerOne->ResetValues();
	m_context.playerTwo->ResetValues();

	for (size_t i = 1; i <= m_context.playerOne->GetPokemonCount(); ++i)
	{
		if (m_context.playerOne->GetBelt(i) != nullptr)
		{
			m_context.playerOne->GetBelt(i)->ResetValues();
		}

	}

	for (size_t i = 1; i <= m_context.playerTwo->GetPokemonCount(); ++i)
	{
		if (m_context.playerTwo->GetBelt(i) != nullptr)
		{
			m_context.playerTwo->GetBelt(i)->ResetValues();
		}

	}
}