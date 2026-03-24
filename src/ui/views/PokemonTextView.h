#pragma once


class BattlePokemon;
class Player;
struct pokemonMove;

namespace PokemonTextView
{
    void DisplayStats(const BattlePokemon&);

    void DisplayIVs(const BattlePokemon&);
    void DisplayEVs(const BattlePokemon&);

    void DisplayLearnableMoves(const BattlePokemon&);
    void DisplayLearnedMoves(const BattlePokemon&);
    void DisplayLearnedMovesExpanded(const BattlePokemon&);
    void DisplayMovesInBattle(const BattlePokemon&, const BattlePokemon&);

    void DisplayPlayerPokemon(const Player&);
    std::string_view DisplayPokemonStatus(const BattlePokemon&);

    std::string_view CalculateStatusMoveEffectiveness(const BattlePokemon&, const BattlePokemon&, const pokemonMove&);
    std::string_view CalculateDamageMoveEffectiveness(const BattlePokemon&, const BattlePokemon&, const pokemonMove&);
}