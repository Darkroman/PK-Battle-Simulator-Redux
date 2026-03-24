#include <iostream>
#include <iomanip>

#include "DatabaseTextView.h"
#include "../../data/Database.h"
#include "../../data/Pokemon.h"
#include "../../data/Move.h"
#include "../../data/StringToTypes.h"

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

            std::cout
                << std::right << std::setw(3) << pokeNum << ": "
                << std::left << std::setw(15) << pokedex[i].GetName();

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

            std::cout
                << std::right << std::setw(3) << moveNum << ": "
                << std::left << std::setw(15) << movedex[i].GetName();

            if (colCount % 6 == 0)
            {
                std::cout << '\n';
            }
        }
        std::cout << '\n';
    }

    void DisplayMovesWithZeroPower(const Database& db)
    {
        const auto& movedex = db.GetMovedexVector();

        for (const auto& move : movedex)
        {
            if (move.GetPower() == 0 && move.GetCategoryEnum() != Category::Status)
            {
                std::cout << move.GetName() << " - Power: " << move.GetPower() << " - Category: " << move.GetCategory() << '\n';
            }
        }
    }
}