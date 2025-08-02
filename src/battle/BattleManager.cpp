#include "BattleContext.h"
#include "RandomEngine.h"
#include "../ui/interfaces/IBattleMenuUI.h"
#include "../ui/interfaces/IMoveResultsUI.h"
#include "../ui/interfaces/IStatusEffectUI.h"

#include "BattleManager.h"

BattleManager::BattleManager(BattleContext& context, RandomEngine& rng, IBattleMenuUI& battleMenuUI, IMoveResultsUI& moveResultsUI, IStatusEffectUI& statusEffectUI)
	: m_context(context)
	, m_rng(rng)
	, m_battleMenuUI(battleMenuUI)
	, m_moveResultsUI(moveResultsUI)
	, m_statusEffectUI(statusEffectUI)
	, m_calculations(context, rng, moveResultsUI)
	, m_switchExecutor(context, battleMenuUI)
	, m_winChecker(context, m_switchExecutor, battleMenuUI)
	, m_statusEffectProcessor(context, rng, statusEffectUI)
	, m_moveExecutor(context, m_calculations, m_statusEffectProcessor, moveResultsUI, battleMenuUI, statusEffectUI, rng, m_switchExecutor)
	, m_turnProcessor(context, m_calculations, rng, m_statusEffectProcessor, m_winChecker, m_switchExecutor, m_moveExecutor)
	, m_postTurnProcessor(context, m_calculations, statusEffectUI, m_statusEffectProcessor, m_winChecker)
{
}

bool BattleManager::RunBattleLoop()
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

		m_postTurnProcessor.DeterminePostTurnDamageOrder();

		m_postTurnProcessor.ProcessAllPostTurnEffects(winCondition);
		if (winCondition) { continue; }

		m_context.playerOne->SetHasSwitched(false);
		m_context.playerTwo->SetHasSwitched(false);

		m_context.flags.ResetBattleFlags();

		m_battleMenuUI.NewLine();

#if !defined NDEBUG
		// ResetHPandPPForTesting();
#endif
	}

	turnCount = 0;
	return false;
}

void BattleManager::ResetValues()
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