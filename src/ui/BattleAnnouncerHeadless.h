#pragma once

#include "interfaces/IBattleAnnouncerUI.h"

#include "../battle/BattleContext.h"
#include "../entities/Player.h"

#include <iostream>

class BattleAnnouncerHeadless : public IBattleAnnouncerUI
{
public:
    void ThrowOutFirstPokemon(const BattleContext&) const override {}
    void DisplayFightingPokemon(const BattleContext&) const override {}
    bool AnnounceWinner(const BattleContext& context) override
    {
        /*
    if (m_context.playerOne->HasWon() && m_context.playerTwo->HasWon())
    {
        std::cout << "Both players Pokemon have fainted in the same turn!\n";
        std::cout << "It is a tie game!\n\n";
        m_context.playerOne->SetWinCondition(false);
        m_context.playerTwo->SetWinCondition(false);
        return false;
    }
    */

        if (context.playerOne->HasWon())
        {
            std::cout << '\n' << context.playerOne->GetPlayerNameView() << " wins!\n\n";
            return false;
        }

        if (context.playerTwo->HasWon())
        {
            std::cout << '\n' << context.playerTwo->GetPlayerNameView() << " wins!\n\n";
            return false;
        }

        return false;
    }

    void NewLine() const override {}
    void DisplayTurnNumber(int) const override {}
};