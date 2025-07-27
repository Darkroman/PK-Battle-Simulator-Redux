#include "Pokemon.h"
#include "Move.h"

#include "StringToTypes.h"
#include "../moves/MoveEffectEnums.h"

#include <iomanip>
#include <filesystem>
#include <iostream>
#include <fstream>

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

void Database::DisplayPokemon() const
{
    int colCount{};

    for (size_t i = 0; i < pokedex.size(); ++i)
    {
        ++colCount;

        size_t pokeNum = i + 1;

        if (pokeNum < 10)
        {
            std::cout << pokeNum << ":   ";
            std::cout << std::setw(15) << std::left << pokedex[i].GetName();
        }
        else if (pokeNum >= 10 && pokeNum <= 99)
        {
            std::cout << pokeNum << ":  ";
            std::cout << std::setw(15) << std::left << pokedex[i].GetName();
        }
        else
        {
            std::cout << pokeNum << ": ";
            std::cout << std::setw(15) << std::left << pokedex[i].GetName();
        }

        if (colCount % 6 == 0)
        {
            std::cout << '\n';
        }
    }
    std::cout << '\n';
}

void Database::DisplayMoves() const
{
    int colCount{};

    for (size_t i = 0; i < movedex.size(); ++i)
    {
        ++colCount;

        size_t moveNum = i + 1;

        if (moveNum < 10)
        {
            std::cout << moveNum << ":   ";
            std::cout << std::setw(15) << std::left << movedex[i].GetName();
        }
        else if (moveNum >= 10 && moveNum <= 99)
        {
            std::cout << moveNum << ":  ";
            std::cout << std::setw(15) << std::left << movedex[i].GetName();
        }
        else
        {
            std::cout << moveNum << ": ";
            std::cout << std::setw(15) << std::left << movedex[i].GetName();
        }

        if (colCount % 6 == 0)
        {
            std::cout << '\n';
        }
    }
    std::cout << '\n';
}