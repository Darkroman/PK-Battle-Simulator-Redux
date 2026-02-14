#pragma once

#include "BattlePokemon.h"
#include "../battle/BattleAction.h"

struct BattleDecision
{
    BattleAction action{ BattleAction::None };
    BattlePokemon::pokemonMove* chosenMove{ nullptr };
    BattlePokemon* chosenPokemon{ nullptr };
};
