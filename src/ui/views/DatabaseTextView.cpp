#include <iostream>
#include <iomanip>

#include "DatabaseTextView.h"
#include "../../data/Database.h"
#include "../../data/Pokemon.h"
#include "../../data/Move.h"

namespace DatabaseTextView
{
	void DisplayAllPokemon(const Database& db)
	{
        int colCount{};

        const auto& pokedex = db.GetPokedexVector();

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

    void DisplayAllMoves(const Database& db)
    {
        int colCount{};

        const auto& movedex = db.GetMovedexVector();

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
}