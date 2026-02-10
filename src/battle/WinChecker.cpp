#include "WinChecker.h"

#include "BattleContext.h"
#include "SwitchExecutor.h"
#include "../ui/interfaces/IBattleMenuUI.h"
#include "../entities/AIPlayer.h"

WinChecker::WinChecker(BattleContext& context, SwitchExecutor& switchExecutor, IBattleMenuUI& battleMenuUI)
    : m_context(context)
	, m_switchExecutor(switchExecutor)
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
			m_battleMenuUI.SwitchPokemonOption(*targetPlayer, *targetPokemon);
			targetPlayer->SetPokemonToSwitchTo(m_battleMenuUI.GetChosenPokemon());
		}

		if (targetPokemon == m_context.playerOneCurrentPokemon)
		{
			m_switchExecutor.ExecuteSwitch(m_context.playerOne, m_context.playerOneCurrentPokemon);
			targetPlayer->SetHasSwitched(true);
			return false;
		}

		if (targetPokemon == m_context.playerTwoCurrentPokemon)
		{
			m_switchExecutor.ExecuteSwitch(m_context.playerTwo, m_context.playerTwoCurrentPokemon);
			targetPlayer->SetHasSwitched(true);
			return false;
		}
	}
	else
	{
		return winCondition;
	}
	
	return winCondition;
}