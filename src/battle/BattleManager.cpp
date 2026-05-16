#include "BattleManager.h"

#include "BattleContext.h"
#include "RandomEngine.h"
#include "../ui/interfaces/IBattleAnnouncerUI.h"
#include "../ui/interfaces/IMoveResultsUI.h"
#include "../ui/interfaces/IStatusEffectUI.h"
#include "../entities/Player.h"
#include "BattleAIManager.h"
#include "../entities/controllers/IPlayerController.h"
#include "../entities/controllers/AIController.h"

BattleManager::BattleManager(BattleContext& context, RandomEngine& rng, IBattleAnnouncerUI& battleAnnouncerUI, IMoveResultsUI& moveResultsUI, IStatusEffectUI& statusEffectUI)
	: m_context(context)
	, m_rng(rng)
	, m_battleAnnouncerUI(battleAnnouncerUI)
	, m_moveResultsUI(moveResultsUI)
	, m_statusEffectUI(statusEffectUI)
	, m_calculations(context, rng)
	, m_switchExecutor(context, moveResultsUI)
	, m_winChecker(context, m_switchExecutor)
	, m_statusEffectProcessor(context, rng, statusEffectUI)
	, m_moveExecutor(context, m_calculations, m_statusEffectProcessor, moveResultsUI, rng, m_switchExecutor)
	, m_turnProcessor(context, m_calculations, m_statusEffectProcessor, m_winChecker, m_switchExecutor, m_moveExecutor)
	, m_postTurnProcessor(context, m_calculations, statusEffectUI, m_statusEffectProcessor, m_winChecker)
{
}

void BattleManager::AssignFirstPokemon()
{
	for (auto& pokemon : m_context.playerOne->GetBeltArray())
	{
		if (pokemon.HasPokemon())
		{
			m_context.playerOneCurrentPokemon = &pokemon;
			break;
		}
	}

	for (auto& pokemon : m_context.playerTwo->GetBeltArray())
	{
		if (pokemon.HasPokemon())
		{
			m_context.playerTwoCurrentPokemon = &pokemon;
			break;
		}
	}
}

void BattleManager::ApplyPlayerOneAction()
{
	PlayerDecisionOutcome decision{};

	if (m_context.playerOneCurrentPokemon->IsCharging() || m_context.playerOneCurrentPokemon->IsRecharging() ||
		m_context.playerOneCurrentPokemon->IsThrashing() || m_context.playerOneCurrentPokemon->IsBiding())
	{
		return;
	}

	decision = m_context.playerOne->GetController().ChooseAction(*m_context.playerOne, *m_context.playerTwo, *m_context.playerOneCurrentPokemon, *m_context.playerTwoCurrentPokemon, m_rng);

	switch (decision.action)
	{
		case BattleAction::Fight:
			m_context.playerOneCurrentMove = decision.chosenMove;
			break;

		case BattleAction::Struggle:
			m_context.playerOneCurrentMove = decision.chosenMove;
			break;

		case BattleAction::SwitchPokemon:
			m_context.playerOneCurrentMove = nullptr;
			m_context.playerOne->SetIsSwitching(true);
			m_context.playerOne->SetPokemonToSwitchTo(decision.chosenPokemon);
			break;

		case BattleAction::Forfeit:
			m_context.playerOne->SetForfeit(true);
			m_context.vec_outOfPokemon.emplace_back(m_context.playerOne);
			break;
	}
}

void BattleManager::ApplyPlayerTwoAction()
{
	PlayerDecisionOutcome decision{};

	if (m_context.playerTwoCurrentPokemon->IsCharging() || m_context.playerTwoCurrentPokemon->IsRecharging() ||
		m_context.playerTwoCurrentPokemon->IsThrashing() || m_context.playerTwoCurrentPokemon->IsBiding())
	{
		return;
	}

	decision = m_context.playerTwo->GetController().ChooseAction(*m_context.playerTwo, *m_context.playerOne, *m_context.playerTwoCurrentPokemon, *m_context.playerOneCurrentPokemon, m_rng);

	switch (decision.action)
	{
	case BattleAction::Fight:
		m_context.playerTwoCurrentMove = decision.chosenMove;
		break;

	case BattleAction::Struggle:
		m_context.playerTwoCurrentMove = decision.chosenMove;
		break;

	case BattleAction::SwitchPokemon:
		m_context.playerTwoCurrentMove = nullptr;
		m_context.playerTwo->SetIsSwitching(true);
		m_context.playerTwo->SetPokemonToSwitchTo(decision.chosenPokemon);
		break;

	case BattleAction::Forfeit:
		m_context.playerTwo->SetForfeit(true);
		m_context.vec_outOfPokemon.emplace_back(m_context.playerTwo);
		break;
	}
}

bool BattleManager::RunBattleLoop()
{
	m_context.battleTurn = 0;
	bool winCondition{ false };

	AssignFirstPokemon();

	BattleAIProcedures::InitAIPlayers(m_context);

	m_battleAnnouncerUI.ThrowOutFirstPokemon(m_context);

	while (winCondition == false)
	{
		++m_context.battleTurn;

		m_battleAnnouncerUI.DisplayTurnNumber(m_context.battleTurn);

		m_battleAnnouncerUI.DisplayFightingPokemon(m_context);

		ApplyPlayerOneAction();

		if (m_context.playerOne->HasForfeited())
		{
			winCondition = m_winChecker.CheckWinCondition(*m_context.playerTwo, *m_context.playerOne);
			return winCondition;
		}

		ApplyPlayerTwoAction();

		if (m_context.playerTwo->HasForfeited())
		{
			winCondition = m_winChecker.CheckWinCondition(*m_context.playerOne, *m_context.playerTwo);
			return winCondition;
		}

		m_turnProcessor.DetermineTurnOrder();

		m_turnProcessor.ExecuteTurn(winCondition);
		if (winCondition) { continue; }

		if (!(m_context.attackingPokemon->IsFainted() || m_context.defendingPokemon->IsFainted()))
		{
			m_battleAnnouncerUI.NewLine();
		}
		
		BattleAIProcedures::RefineEnemyModelFirstTurn(m_context);

		m_turnProcessor.SwapRoles();

		m_turnProcessor.ExecuteTurn(winCondition);
		if (winCondition) { continue; }

		BattleAIProcedures::RefineEnemyModelSecondTurn(m_context);

		m_postTurnProcessor.ProcessAllPostTurnEffects(winCondition);
		if (winCondition) { continue; }

		m_context.playerOne->SetHasSwitched(false);
		m_context.playerTwo->SetHasSwitched(false);

		m_context.flags.ResetBattleFlags();

		m_battleAnnouncerUI.NewLine();
	}

	m_context.battleTurn = 0;
	return winCondition;
}

void BattleManager::ResetValues()
{
	m_context.vec_outOfPokemon.clear();

	m_context.playerOne->ResetValues();
	m_context.playerTwo->ResetValues();

	for (auto& pokemon : m_context.playerOne->GetBeltArray())
	{
		if (!pokemon.HasPokemon())
		{
			continue;
		}
		pokemon.ResetValues();
	}

	for (auto& pokemon : m_context.playerTwo->GetBeltArray())
	{
		if (!pokemon.HasPokemon())
		{
			continue;
		}
		pokemon.ResetValues();
	}

	for (const auto* aiPlayers : m_context.vec_aiPlayers)
	{
		aiPlayers->GetAIController().ResetObservedMoves();
	}
}