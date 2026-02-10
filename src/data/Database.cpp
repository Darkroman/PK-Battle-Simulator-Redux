#include "Pokemon.h"
#include "Move.h"
#include "StringToTypes.h"

#include "Database.h"

Database::Database()
{
    pokedex.reserve(151); //Number of Pokemon
    movedex.reserve(165); //Number of moves

    LoadEmbedMoves();
    LoadEmbedPokemon();
    LoadEmbedLearnset();

#if !defined NDEBUG
        TestingPokemonAndMoves();
#endif
}

Database& Database::GetInstance()
{
    static Database instance;
    return instance;
}

void Database::TestingPokemonAndMoves()
{
    pokedex.emplace_back(152, "Poketest", 100, 100, 100, 100, 100, 100, "Ground", ConvertStringToPokemonType("Ground"), "None", ConvertStringToPokemonType("None"), 1);

    std::vector<size_t> tmp_movelist2{};
    tmp_movelist2.reserve(165);
    for (size_t i = 1; i < 165; ++i)
    {
        tmp_movelist2.emplace_back(i);
    }

    pokedex[151].CopyMoveListVector(tmp_movelist2);
}

const std::vector<Pokemon>& Database::GetPokedexVector() const
{
    return pokedex;
}

const std::vector<Move>& Database::GetMovedexVector() const
{
    return movedex;
}

Pokemon* Database::GetPointerToPokedexNumber(size_t index)
{
    return &(pokedex[index]);
}

Move* Database::GetPointerToMovedexNumber(size_t index)
{
    return &(movedex[index]);
}

std::vector<Pokemon>::iterator Database::PokedexBegin()
{
    return pokedex.begin();
}

std::vector<Pokemon>::iterator Database::PokedexEnd()
{
    return pokedex.end();
}

std::vector<Pokemon>::const_iterator Database::cPokedexBegin() const
{
    return pokedex.cbegin();
}

std::vector<Pokemon>::const_iterator Database::cPokedexEnd() const
{
    return pokedex.cend();
}

std::vector<Move>::iterator Database::MovedexBegin()
{
    return movedex.begin();
}

std::vector<Move>::iterator Database::MovedexEnd()
{
    return movedex.end();
}

std::vector<Move>::const_iterator Database::cMovedexBegin() const
{
    return movedex.cbegin();
}

std::vector<Move>::const_iterator Database::cMovedexEnd() const
{
    return movedex.cend();
}