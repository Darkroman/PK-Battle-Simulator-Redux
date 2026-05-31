#include "Database.h"

Database::Database(){}

Database& Database::GetInstance()
{
    static Database instance;
    return instance;
}

void Database::TestingPokemonAndMoves(Pokemon*& pkmn1, Pokemon*& pkmn2)
{
    Pokemon* test1 = new Pokemon(152, "Poketest1", 255, 100, 100, 100, 100, 100, "Bug", ConvertStringToPokemonType("Bug"), "None", ConvertStringToPokemonType("None"), 10);
    Pokemon* test2 = new Pokemon(153, "Poketest2", 255, 100, 100, 100, 100, 100, "Psychic", ConvertStringToPokemonType("Psychic"), "None", ConvertStringToPokemonType("None"), 10);

    for (size_t i = 1; i < 165; ++i)
    {
        //test1->DebugAddMove(i);
    }

    for (size_t i = 1; i < 165; ++i)
    {
        //test1->DebugAddMove(i);
    }

    pkmn1 = test1;
    pkmn2 = test2;
}

std::span<const Pokemon> Database::GetPokedexView() const
{
    return pokedex;
}

std::span<const Move> Database::GetMovedexView() const
{
    return movedex;
}

const Pokemon* Database::GetPointerToPokedexNumber(size_t index) const
{
    return &(pokedex[index]);
}

const Move* Database::GetPointerToMovedexNumber(size_t index) const
{
    return &(movedex[index]);
}