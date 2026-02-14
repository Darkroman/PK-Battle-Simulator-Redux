#pragma once

#include "BattlePokemon.h"
#include "../battle/BattleAction.h"

struct PlayerDecisionOutcome
{
    BattleAction action{ BattleAction::None };
    BattlePokemon::pokemonMove* chosenMove{ nullptr };
    BattlePokemon* chosenPokemon{ nullptr };
};
