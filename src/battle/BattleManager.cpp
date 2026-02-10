#include "BattleContext.h"
#include "RandomEngine.h"
#include "../ui/interfaces/IBattleMenuUI.h"
#include "../ui/interfaces/IMoveResultsUI.h"
#include "../ui/interfaces/IStatusEffectUI.h"
#include "../entities/AIPlayer.h"
#include "BattleAIManager.h"

#include "BattleManager.h"

BattleManager::BattleManager(BattleContext& context, RandomEngine& rng, IBattleMenuUI& battleMenuUI, IMoveResultsUI& moveResultsUI, IStatusEffectUI& statusEffectUI)
	: m_context(context)
	, m_rng(rng)
	, m_battleMenuUI(battleMenuUI)
	, m_moveResultsUI(moveResultsUI)
	, m_statusEffectUI(statusEffectUI)
	, m_calculations(context, rng)
	, m_switchExecutor(context, battleMenuUI)
	, m_winChecker(context, m_switchExecutor, battleMenuUI)
	, m_statusEffectProcessor(context, rng, statusEffectUI)
	, m_moveExecutor(context, m_calculations, m_statusEffectProcessor, moveResultsUI, battleMenuUI, rng, m_switchExecutor)
	, m_turnProcessor(context, m_calculations, rng, m_statusEffectProcessor, m_winChecker, m_switchExecutor, m_moveExecutor)
	, m_postTurnProcessor(context, m_calculations, statusEffectUI, m_statusEffectProcessor, m_winChecker)
{
}

void BattleManager::AssignFirstPokemon()
{
	m_context.playerOneCurrentPokemon = m_context.playerOne->GetBelt(1);
	m_context.playerTwoCurrentPokemon = m_context.playerTwo->GetBelt(1);
}

void BattleManager::ApplyPlayerOneAction(BattleAction action)
{
	switch (action)
	{
		case BattleAction::Fight:
			m_context.playerOneCurrentMove = m_battleMenuUI.GetChosenMove();
			break;

		case BattleAction::Struggle:
			m_context.playerOneCurrentMove = m_context.playerOneCurrentPokemon->Struggle();
			break;

		case BattleAction::SwitchPokemon:
			m_context.playerOne->SetIsSwitching(true);
			m_context.playerOne->SetPokemonToSwitchTo(m_battleMenuUI.GetChosenPokemon());
			break;

		case BattleAction::Forfeit:
			m_context.playerOne->SetForfeit(true);
			break;
	}
}

void BattleManager::ApplyPlayerTwoAction(BattleAction action)
{
	switch (action)
	{
	case BattleAction::Fight:
		m_context.playerTwoCurrentMove = m_battleMenuUI.GetChosenMove();
		break;

	case BattleAction::Struggle:
		m_context.playerTwoCurrentMove = m_context.playerTwoCurrentPokemon->Struggle();
		break;

	case BattleAction::SwitchPokemon:
		m_context.playerTwo->SetIsSwitching(true);
		m_context.playerTwo->SetPokemonToSwitchTo(m_battleMenuUI.GetChosenPokemon());
		break;

	case BattleAction::Forfeit:
		m_context.playerTwo->SetForfeit(true);
		break;
	}
}

bool BattleManager::RunBattleLoop()
{
	int turnCount{ 0 };
	bool winCondition{ false };

	BattleAIProcedures::InitAIPlayers(m_context);

	m_battleMenuUI.ThrowOutFirstPokemon(m_context);

	AssignFirstPokemon();

	while (winCondition == false)
	{
		BattleAIProcedures::UpdateEnemyActivePokemon(m_context);

		++turnCount;
		m_battleMenuUI.DisplayTurnNumber(turnCount);

		m_battleMenuUI.DisplayFightingPokemon(m_context);

		e_battleAction = m_battleMenuUI.PlayerOneMakeSelection(m_context);

		ApplyPlayerOneAction(e_battleAction);

		if (m_context.playerOne->HasForfeited())
		{
			winCondition = m_winChecker.CheckWinCondition(m_context.playerTwo, m_context.playerOne);
			return winCondition;
		}

		e_battleAction = m_battleMenuUI.PlayerTwoMakeSelection(m_context);

		ApplyPlayerTwoAction(e_battleAction);

		if (m_context.playerTwo->HasForfeited())
		{
			winCondition = m_winChecker.CheckWinCondition(m_context.playerOne, m_context.playerTwo);
			return winCondition;
		}

		m_turnProcessor.DetermineWhoGoesFirst();

		m_turnProcessor.ExecuteTurn(winCondition);
		if (winCondition) { continue; }

		BattleAIProcedures::RefineEnemyModelFirstTurn(m_context);

		m_battleMenuUI.NewLine();

		m_turnProcessor.SwapRoles();

		m_turnProcessor.ExecuteTurn(winCondition);
		if (winCondition) { continue; }

		BattleAIProcedures::RefineEnemyModelSecondTurn(m_context);

		m_battleMenuUI.NewLine();

		m_postTurnProcessor.DeterminePostTurnDamageOrder();

		m_postTurnProcessor.ProcessAllPostTurnEffects(winCondition);
		if (winCondition) { continue; }

		m_context.playerOne->SetHasSwitched(false);
		m_context.playerTwo->SetHasSwitched(false);

		m_context.flags.ResetBattleFlags();

		m_battleMenuUI.NewLine();

#if !defined NDEBUG
		//ResetHPandPPForTesting();
#endif
	}

	turnCount = 0;
	return winCondition;
}

void BattleManager::ResetValues()
{
	m_context.playerOne->ResetValues();
	m_context.playerTwo->ResetValues();
	m_context.aiPlayerOne = nullptr;
	m_context.aiPlayerTwo = nullptr;

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