#include "SwitchExecutor.h"

#include "BattleContext.h"
#include "../ui/interfaces/IMoveResultsUI.h"
#include "../entities/controllers/AIController.h"

SwitchExecutor::SwitchExecutor(BattleContext& context, IMoveResultsUI& moveResultsUI) :
	m_context(context), m_moveResultsUI(moveResultsUI) {}

void SwitchExecutor::ExecuteSwitch(Player& player, BattlePokemon*& pokemon)
{
	m_moveResultsUI.SwitchOutMsg(player.GetPlayerNameView(), pokemon->GetPokemonNameView());
	pokemon->ResetStatsOnSwitch();
	pokemon = player.GetPokemonToSwitchTo();

	BattlePokemon* newPokemon{ nullptr };

	if (&player == m_context.playerOne)
	{
		if (m_context.playerTwoCurrentPokemon->IsBound())
		{
			m_context.playerTwoCurrentPokemon->SetBound(false);
			m_context.playerTwo->SetCanSwitch(true);
			m_context.playerTwoCurrentPokemon->ResetBoundCounter();
			m_context.playerTwoCurrentPokemon->SetBoundTurnCount(0);
		}

		m_context.playerOneCurrentPokemon = pokemon;
		newPokemon = m_context.playerOneCurrentPokemon;
		m_moveResultsUI.PlayerChoosesMsg(player.GetPlayerNameView(), newPokemon->GetPokemonNameView());
	}
	else if (&player == m_context.playerTwo)
	{
		if (m_context.playerOneCurrentPokemon->IsBound())
		{
			m_context.playerOneCurrentPokemon->SetBound(false);
			m_context.playerOne->SetCanSwitch(true);
			m_context.playerOneCurrentPokemon->ResetBoundCounter();
			m_context.playerOneCurrentPokemon->SetBoundTurnCount(0);
		}

		m_context.playerTwoCurrentPokemon = pokemon;
		newPokemon = m_context.playerTwoCurrentPokemon;
		m_moveResultsUI.PlayerChoosesMsg(player.GetPlayerNameView(), newPokemon->GetPokemonNameView());
	}

	for (auto player : m_context.vec_aiPlayers)
	{
		player->GetAIController().OnActivePokemonChanged(m_context);
	}

	player.SetIsSwitching(false);
}