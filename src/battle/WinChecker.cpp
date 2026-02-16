#include "WinChecker.h"

#include "BattleContext.h"
#include "SwitchExecutor.h"
#include "../ui/interfaces/IMoveResultsUI.h"
#include "../entities/controllers/AIController.h"

WinChecker::WinChecker(BattleContext& context, SwitchExecutor& switchExecutor)
    : m_context(context)
	, m_switchExecutor(switchExecutor)
	{}

bool WinChecker::CheckWinCondition(Player& sourcePlayer, Player& targetPlayer)
{
	if (m_context.vec_outOfPokemon.empty())
	{
		return false;
	}

	if (&targetPlayer == m_context.vec_outOfPokemon[0])
	{
		sourcePlayer.SetWinCondition(true);
		return true;
	}

	if (&sourcePlayer == m_context.vec_outOfPokemon[0])
	{
		targetPlayer.SetWinCondition(true);
		return true;
	}
}

bool WinChecker::CheckWinOrSwitch(Player& sourcePlayer, Player& targetPlayer, BattlePokemon& targetPokemon)
{
	bool winCondition{ false };

	if (!targetPokemon.IsFainted())
	{
		return false;
	}

	winCondition = CheckWinCondition(sourcePlayer, targetPlayer);

	if (!winCondition)
	{
		BattlePokemon* newPokemon = targetPlayer.GetController().PromptForSwitch(targetPlayer, targetPokemon);
		targetPlayer.SetPokemonToSwitchTo(newPokemon);

		if (&targetPokemon == m_context.playerOneCurrentPokemon)
		{
			m_switchExecutor.ExecuteSwitch(*m_context.playerOne, m_context.playerOneCurrentPokemon);
			targetPlayer.SetHasSwitched(true);
			return false;
		}

		if (&targetPokemon == m_context.playerTwoCurrentPokemon)
		{

			m_switchExecutor.ExecuteSwitch(*m_context.playerTwo, m_context.playerTwoCurrentPokemon);
			targetPlayer.SetHasSwitched(true);
			return false;
		}

		for (auto player : m_context.vec_aiPlayers)
		{
			player->GetAIController().OnActivePokemonChanged(m_context);
		}
	}
	else
	{
		return winCondition;
	}
	
	return winCondition;
}