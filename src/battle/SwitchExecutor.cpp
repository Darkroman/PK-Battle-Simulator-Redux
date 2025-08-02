#include "SwitchExecutor.h"

#include "BattleContext.h"
#include "../ui/interfaces/IBattleMenuUI.h"

SwitchExecutor::SwitchExecutor(BattleContext& context, IBattleMenuUI& battleMenuUI) :
	m_context(context), m_battleMenuUI(battleMenuUI) {}

void SwitchExecutor::ExecuteSwitch(Player* player, BattlePokemon*& pokemon)
{
	m_battleMenuUI.SwitchOutMsg(player, pokemon);
	pokemon->ResetStatsOnSwitch();
	pokemon = player->GetPokemonToSwitchTo();

	if (player == m_context.playerOne)
	{
		if (m_context.playerTwoCurrentPokemon->IsBound())
		{
			m_context.playerTwoCurrentPokemon->SetBound(false);
			m_context.playerTwo->SetCanSwitch(true);
			m_context.playerTwoCurrentPokemon->ResetBoundCounter();
			m_context.playerTwoCurrentPokemon->SetBoundTurnCount(0);
		}

		m_context.playerOneCurrentPokemon = pokemon;
	}
	else if (player == m_context.playerTwo)
	{
		if (m_context.playerOneCurrentPokemon->IsBound())
		{
			m_context.playerOneCurrentPokemon->SetBound(false);
			m_context.playerOne->SetCanSwitch(true);
			m_context.playerOneCurrentPokemon->ResetBoundCounter();
			m_context.playerOneCurrentPokemon->SetBoundTurnCount(0);
		}

		m_context.playerTwoCurrentPokemon = pokemon;
	}

	player->SetIsSwitching(false);
	m_battleMenuUI.PlayerChoosesMsg(player, pokemon);
}