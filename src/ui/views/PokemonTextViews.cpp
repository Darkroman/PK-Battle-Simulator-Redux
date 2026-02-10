#include <iostream>
#include <iomanip>

#include "../../entities/Player.h"
#include "../../data/Database.h"
#include "../../data/Pokemon.h"
#include "../../data/Move.h"

#include "PokemonTextView.h"

namespace PokemonTextView
{
    void DisplayStats(const BattlePokemon& pokemon)
    {
        std::cout << "Pokemon: " << pokemon.GetName() << '\n';

        if (!pokemon.HasNickname())
        {
            std::cout << "No nickname\n";
        }
        else
        {
            std::cout << "Nickname: " << pokemon.GetNickname() << '\n';
        }

        std::cout << "Type: "
            << pokemon.GetTypeOne()
            << "/"
            << pokemon.GetTypeTwo()
            << '\n';

        std::cout << "Level: " << pokemon.GetLevel() << '\n';

        std::cout << "HP: " << pokemon.GetMaxHP()
            << " -- [IV: " << pokemon.GetHPIV() << "]"
            << " -- [EV: " << pokemon.GetHPEV() << "]\n";

        std::cout << "Attack: " << pokemon.GetAttack()
            << " -- [IV: " << pokemon.GetAttackIV() << "]"
            << " -- [EV: " << pokemon.GetAttackEV() << "]\n";

        std::cout << "Defense: " << pokemon.GetDefense()
            << " -- [IV: " << pokemon.GetDefenseIV() << "]"
            << " -- [EV: " << pokemon.GetDefenseEV() << "]\n";

        std::cout << "Special Attack: " << pokemon.GetSpecialAttack()
            << " -- [IV: " << pokemon.GetSpecialAttackIV() << "]"
            << " -- [EV: " << pokemon.GetSpecialAttackEV() << "]\n";

        std::cout << "Special Defense: " << pokemon.GetSpecialDefense()
            << " -- [IV: " << pokemon.GetSpecialDefenseIV() << "]"
            << " -- [EV: " << pokemon.GetSpecialDefenseEV() << "]\n";

        std::cout << "Speed: " << pokemon.GetSpeed()
            << " -- [IV: " << pokemon.GetSpeedIV() << "]"
            << " -- [EV: " << pokemon.GetSpeedEV() << "]\n";
    }

    void DisplayLearnableMoves(const BattlePokemon& pokemon)
    {
        const Pokemon* basePokemon = pokemon.GetPokemonDatabasePointer();
        auto& db = Database::GetInstance();

        int colCount = 0;

        for (auto it = basePokemon->MovelistBegin();
            it != basePokemon->MovelistEnd();
            ++it)
        {
            ++colCount;

            size_t moveIndex = basePokemon->FetchMoveNumber(it);
            Move* move = db.GetPointerToMovedexNumber(moveIndex);

            size_t displayIndex = moveIndex + 1;

            if (displayIndex < 10)
            {
                std::cout << move->GetMoveIndex() << ":   ";
            }
            else if (displayIndex <= 99)
            {
                std::cout << move->GetMoveIndex() << ":  ";
            }
            else
            {
                std::cout << move->GetMoveIndex() << ": ";
            }

            std::cout << std::setw(15) << std::left << move->GetName();

            if (colCount % 6 == 0)
            {
                std::cout << '\n';
            }
        }

        std::cout << '\n';
    }

    void DisplayIVs(const BattlePokemon& pokemon)
    {
        std::cout << "IVs:\n";
        std::cout << "HP:      "         << pokemon.GetHPIV() << '\n';
        std::cout << "Attack:  "         << pokemon.GetAttackIV() << '\n';
        std::cout << "Defense: "         << pokemon.GetDefenseIV() << '\n';
        std::cout << "Special Attack: "  << pokemon.GetSpecialAttackIV() << '\n';
        std::cout << "Special Defense: " << pokemon.GetSpecialDefenseIV() << '\n';
        std::cout << "Speed:   "         << pokemon.GetSpeedIV() << '\n';
    }

    void DisplayEVs(const BattlePokemon& pokemon)
    {
        std::cout << "HP EV: "              << pokemon.GetHPEV() << '\n';
        std::cout << "Attack EV: "          << pokemon.GetAttackEV() << '\n';
        std::cout << "Defense EV: "         << pokemon.GetDefenseEV() << '\n';
        std::cout << "Special Attack EV: "  << pokemon.GetSpecialAttackEV() << '\n';
        std::cout << "Special Defense EV: " << pokemon.GetSpecialDefenseEV() << '\n';
        std::cout << "Speed EV: "           << pokemon.GetSpeedEV() << "\n\n";
    }

    void DisplayLearnedMoves(const BattlePokemon& pokemon)
    {
        for (size_t moveSlot = 1; moveSlot < 5; ++moveSlot)
        {
            if (pokemon.GetMove(moveSlot)->IsActive())
            {
                std::cout << pokemon.GetMove(moveSlot)->GetName() << "/";
            }
            else
            {
                std::cout << "---/";
            }
        }
        std::cout << '\n';
    }

    void DisplayMovesInBattle(const BattlePokemon& pokemon)
    {
        for (size_t moveSlot = 1; moveSlot < 5; ++moveSlot)
        {
            auto* move = pokemon.GetMove(moveSlot);
            if (move->IsActive())
            {
                if (move->b_isDisabled)
                {
                    std::cout << moveSlot << ". " << move->GetName() << " (Disabled!)\n";
                }
                else
                {
                    std::cout << moveSlot << ". " << move->GetName() << " PP(" << move->m_currentPP << "/" << move->m_maxPP << ")" << "\n";
                }
            }
            else
            {
                std::cout << moveSlot << ". " << "---\n";
            }
        }
    }

    void DisplayPlayerPokemon(const Player& player)
    {
        int count{ 1 };
        std::cout << "---" << player.GetPlayerName() << "'s Pokemon---\n";

        for (const auto& p : player.GetBeltArray())
        {
            if (!p.HasPokemon())
            {
                std::cout << count << ". ---\n";
            }
            else
            {
                std::cout << count << ". " << p.GetPokemonNameView() << " HP(" << p.GetCurrentHP() << "/" << p.GetMaxHP() << ") " << "- Level: " << p.GetLevel() << " Moves: ";
                DisplayLearnedMoves(p);
            }
            ++count;
        }
        std::cout << '\n';

        /*
        int count{ 1 };

        std::cout << "---" << this->GetPlayerName() << "'s Pokemon---\n";

        for (const auto& p : belt)
        {
            if (!p.HasPokemon())
            {
                std::cout << count << ". ---\n";
            }
            else
            {
                std::cout << count << ". " << p.GetPokemonNameView() << " HP(" << p.GetCurrentHP() << "/" << p.GetMaxHP() << ") " << "- Level: " << p.GetLevel() << " Moves: ";
                PokemonTextView::DisplayLearnedMoves(p);
            }
            ++count;
        }
        std::cout << '\n';
        */
    }
}
