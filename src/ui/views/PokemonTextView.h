#pragma once

#include <cstddef>

// Forward declarations only
class BattlePokemon;
class Player;

namespace PokemonTextView
{
    // Full stat block (name, types, level, stats, IVs, EVs)
    void DisplayStats(const BattlePokemon& pokemon);

    // Individual sections (used by menus that drill down)
    void DisplayIVs(const BattlePokemon& pokemon);
    void DisplayEVs(const BattlePokemon& pokemon);

    // Move-related displays
    void DisplayLearnableMoves(const BattlePokemon& pokemon);
    void DisplayLearnedMoves(const BattlePokemon& pokemon);
    void DisplayMovesInBattle(const BattlePokemon& pokemon);

    void DisplayPlayerPokemon(const Player& player);
}
