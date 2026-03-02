#pragma once

struct pokemonMove;
class BattlePokemon;

#include "../battle/BattleAction.h"

struct PlayerDecisionOutcome
{
    BattleAction action{ BattleAction::None };
    pokemonMove* chosenMove{ nullptr };
    BattlePokemon* chosenPokemon{ nullptr };
};
