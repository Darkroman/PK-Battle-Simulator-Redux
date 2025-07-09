#include "TurnUtils.h"

#include "../entities/Player.h"
#include "../entities/BattlePokemon.h"
#include "BattleContext.h"
#include "../ui/interfaces/IBattleMenuUI.h"

TurnUtils::TurnUtils(BattleContext& context, IBattleMenuUI& battleMenuUI) :
	m_context(context), m_battleMenuUI(battleMenuUI) {}

void TurnUtils::PerformSwitch(Player* player, BattlePokemon*& pokemon)
{
	m_battleMenuUI.SwitchOutMsg(player, pokemon);
	pokemon->ResetStatsOnSwitch();
	pokemon = player->GetPokemonToSwitchTo();

	if (player == m_context.playerOne)
	{
		m_context.playerOneCurrentPokemon = pokemon;
	}
	else if (player == m_context.playerTwo)
	{
		m_context.playerTwoCurrentPokemon = pokemon;
	}

	player->SetIsSwitching(false);
	m_battleMenuUI.PlayerChoosesMsg(player, pokemon);
}