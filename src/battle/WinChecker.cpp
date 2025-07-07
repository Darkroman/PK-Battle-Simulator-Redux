#include "WinChecker.h"
#include "TurnProcessor.h"
#include "../ui/interfaces/IBattleMenuUI.h"
#include "BattleContext.h"
#include "../entities/Player.h"
#include "../entities/AIPlayer.h"
#include "../entities/BattlePokemon.h"
#include <iostream>

WinChecker::WinChecker(BattleContext& context, TurnProcessor& turnProcessor, IBattleMenuUI& battleMenuUI)
    : m_context(context)
	, m_turnProcessor(turnProcessor)
	, m_battleMenuUI(battleMenuUI)
	{}

bool WinChecker::CheckWinCondition(Player* sourcePlayer, Player* targetPlayer)
{
    if (targetPlayer->GetFaintedCount() == targetPlayer->GetPokemonCount() || targetPlayer->HasForfeited())
    {
        sourcePlayer->SetWinCondition(true);
        return true;
    }

    if (sourcePlayer->GetFaintedCount() == sourcePlayer->GetPokemonCount() || sourcePlayer->HasForfeited())
    {
        targetPlayer->SetWinCondition(true);
        return true;
    }

    return false;
}

bool WinChecker::CheckWinOrSwitch(Player* sourcePlayer, Player* targetPlayer, BattlePokemon* targetPokemon)
{
	bool winCondition{ false };

	if (!targetPokemon->IsFainted())
	{
		return false;
	}

	winCondition = CheckWinCondition(sourcePlayer, targetPlayer);

	if (!winCondition)
	{
		if (!targetPlayer->IsHuman())
		{
			auto* aiPlayer = static_cast<AIPlayer*>(targetPlayer);
			aiPlayer->ChooseSwitch(*targetPokemon);
		}
		else
		{
			m_battleMenuUI.SwitchPokemonOption(targetPlayer, targetPokemon);
		}

		if (targetPokemon == m_context.playerOneCurrentPokemon)
		{
			m_turnProcessor.PerformSwitch(m_context.playerOne, m_context.playerOneCurrentPokemon);
			targetPlayer->SetHasSwitched(true);
			return false;
		}

		if (targetPokemon == m_context.playerTwoCurrentPokemon)
		{
			m_turnProcessor.PerformSwitch(m_context.playerTwo, m_context.playerTwoCurrentPokemon);
			targetPlayer->SetHasSwitched(true);
			return false;
		}
	}
	else
	{
		return true;
	}
	
	return false;
}