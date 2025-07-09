#include "TurnManager.h"
#include "BattleContext.h"
#include "RandomEngine.h"
#include "../ui/interfaces/IBattleMenuUI.h"
#include "../ui/interfaces/IMoveResultsUI.h"
#include "../ui/interfaces/IStatusEffectUI.h"

TurnManager::TurnManager(BattleContext& context, RandomEngine& rng, IStatusEffectUI& statusEffectUI, IBattleMenuUI& battleMenuUI, IMoveResultsUI& moveResultsUI)
	: m_context(context)
	, m_rng(rng)
	, m_battleMenuUI(battleMenuUI)
	, m_moveResultsUI(moveResultsUI)
	, m_statusEffectUI(statusEffectUI)
	, m_statusManager(context, rng, statusEffectUI)
	, m_winChecker(context, m_turnProcessor, battleMenuUI)
	, m_calculations(context, rng)
	, m_turnProcessor(context, m_calculations, rng, statusEffectUI, m_statusManager, battleMenuUI, moveResultsUI, m_winChecker)
	, m_postTurnProcessor(context, rng, statusEffectUI, m_statusManager, m_winChecker)
{}

bool TurnManager::RunBattleLoop()
{
	int turnCount{ 0 };
	bool winCondition{ false };

	m_battleMenuUI.ThrowOutFirstPokemon();

	while (winCondition == false)
	{
		++turnCount;
		m_battleMenuUI.DisplayTurnNumber(turnCount);

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

		m_battleMenuUI.NewLine();

		m_turnProcessor.SwapRoles();

		m_turnProcessor.ExecuteTurn(winCondition);
		if (winCondition) { continue; }

		m_battleMenuUI.NewLine();

		m_turnProcessor.DeterminePostTurnDamageOrder();

		m_postTurnProcessor.ProcessAllPostTurnEffects(winCondition);
		if (winCondition) { continue; }

		m_context.playerOne->SetHasSwitched(false);
		m_context.playerTwo->SetHasSwitched(false);
		m_context.damageTaken = 0;

		m_battleMenuUI.NewLine();

#if !defined NDEBUG
		// ResetHPandPPForTesting();
#endif
	}

	turnCount = 0;
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