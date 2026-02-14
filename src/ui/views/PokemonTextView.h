#pragma once

// Forward declarations only
class BattlePokemon;
class Player;

namespace PokemonTextView
{
    // Full stat block (name, types, level, stats, IVs, EVs)
    void DisplayStats(const BattlePokemon&);

    // Individual sections (used by menus that drill down)
    void DisplayIVs(const BattlePokemon&);
    void DisplayEVs(const BattlePokemon&);

    // Move-related displays
    void DisplayLearnableMoves(const BattlePokemon&);
    void DisplayLearnedMoves(const BattlePokemon&);
    void DisplayMovesInBattle(const BattlePokemon&);

    void DisplayPlayerPokemon(const Player&);
    std::string_view DisplayPokemonStatus(const BattlePokemon&);
}
