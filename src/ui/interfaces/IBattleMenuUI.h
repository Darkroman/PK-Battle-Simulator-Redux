#pragma once

#include <string>
#include "../../entities/Player.h"
#include "../../entities/BattlePokemon.h"

class IBattleMenuUI
{
public:
    virtual ~IBattleMenuUI() = default;

    virtual void ThrowOutFirstPokemon() = 0;
    virtual void DisplayFightingPokemon() = 0;
    virtual void PlayerOneMakeSelection() = 0;
    virtual void PlayerTwoMakeSelection() = 0;
    virtual bool SwitchPokemonOption(Player* currentPlayer, BattlePokemon* currentPokemon) = 0;

protected:
    virtual void MakeASelectionLoop(Player* player, BattlePokemon* currentPokemon) = 0;
    virtual bool Fight(Player* player, BattlePokemon* currentPokemon) = 0;
};