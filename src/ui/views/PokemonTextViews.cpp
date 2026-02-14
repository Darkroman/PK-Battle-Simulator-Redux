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
        std::cout << "Pokemon: " << pokemon.GetPokemonNameView() << '\n';

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

        std::cout << std::left << std::setw(17) << "HP: " << std::setw(3) << pokemon.GetMaxHP()

            << " -- [IV: " << std::right <<  std::setw(2) << pokemon.GetHPIV() << "]"
            << " -- [EV: " <<                std::setw(3) << pokemon.GetHPEV() << "]\n";

        std::cout << std::left << std::setw(17) << "Attack: " << std::setw(3) << pokemon.GetAttack()

            << " -- [IV: " << std::right << std::setw(2) << pokemon.GetAttackIV() << "]"
            << " -- [EV: " <<               std::setw(3) << pokemon.GetAttackEV() << "]\n";

        std::cout << std::left << std::setw(17) << "Defense: " << std::setw(3) << pokemon.GetDefense()

            << " -- [IV: " << std::right << std::setw(2) << pokemon.GetDefenseIV() << "]"
            << " -- [EV: " <<               std::setw(3) << pokemon.GetDefenseEV() << "]\n";

        std::cout << std::left << std::setw(17) << "Special Attack: " << std::setw(3) << pokemon.GetSpecialAttack()

            << " -- [IV: " << std::right << std::setw(2) << pokemon.GetSpecialAttackIV() << "]"
            << " -- [EV: " <<               std::setw(3) << pokemon.GetSpecialAttackEV() << "]\n";

        std::cout << std::left << std::setw(17) << "Special Defense: " << std::setw(3) << pokemon.GetSpecialDefense()

            << " -- [IV: " << std::right << std::setw(2) << pokemon.GetSpecialDefenseIV() << "]"
            << " -- [EV: " <<               std::setw(3) << pokemon.GetSpecialDefenseEV() << "]\n";

        std::cout << std::left << std::setw(17) << "Speed: " << std::setw(3) << pokemon.GetSpeed()

            << " -- [IV: " << std::right << std::setw(2) << pokemon.GetSpeedIV() << "]"
            << " -- [EV: " <<               std::setw(3) << pokemon.GetSpeedEV() << "]\n";
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

            std::cout
                << std::right << std::setw(3) << displayIndex << ": "
                << std::left << std::setw(15) << move->GetName();

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
        std::cout << std::left << std::setw(20) << "HP IV: "              << std::right << std::setw(2) << pokemon.GetHPIV() << '\n';
        std::cout << std::left << std::setw(20) << "Attack IV: "          << std::right << std::setw(2) << pokemon.GetAttackIV() << '\n';
        std::cout << std::left << std::setw(20) << "Defense IV: "         << std::right << std::setw(2) << pokemon.GetDefenseIV() << '\n';
        std::cout << std::left << std::setw(20) << "Special Attack IV: "  << std::right << std::setw(2) << pokemon.GetSpecialAttackIV() << '\n';
        std::cout << std::left << std::setw(20) << "Special Defense IV: " << std::right << std::setw(2) << pokemon.GetSpecialDefenseIV() << '\n';
        std::cout << std::left << std::setw(20) << "Speed IV: "           << std::right << std::setw(2) << pokemon.GetSpeedIV() << '\n';
    }

    void DisplayEVs(const BattlePokemon& pokemon)
    {
        std::cout << std::left << std::setw(20) << "HP EV: "              << std::right << std::setw(3) << pokemon.GetHPEV() << '\n';
        std::cout << std::left << std::setw(20) << "Attack EV: "          << std::right << std::setw(3) << pokemon.GetAttackEV() << '\n';
        std::cout << std::left << std::setw(20) << "Defense EV: "         << std::right << std::setw(3) << pokemon.GetDefenseEV() << '\n';
        std::cout << std::left << std::setw(20) << "Special Attack EV: "  << std::right << std::setw(3) << pokemon.GetSpecialAttackEV() << '\n';
        std::cout << std::left << std::setw(20) << "Special Defense EV: " << std::right << std::setw(3) << pokemon.GetSpecialDefenseEV() << '\n';
        std::cout << std::left << std::setw(20) << "Speed EV: "           << std::right << std::setw(3) << pokemon.GetSpeedEV() << "\n\n";
    }

    void DisplayLearnedMoves(const BattlePokemon& pokemon)
    {
        for (size_t moveSlot = 1; moveSlot < 5; ++moveSlot)
        {
            if (moveSlot != 1)
            {
                std::cout << " ";
            }

            if (pokemon.GetMove(moveSlot).IsActive())
            {
                std::cout << std::format("{:<12}", pokemon.GetMove(moveSlot).GetName());
                std::cout << " ";
            }
            else
            {
                std::cout << std::format("{:^12}", "---");
                std::cout << " ";
            }

            if (moveSlot != 4)
            {
                std::cout << "/";
            }
        }
        std::cout << '\n';
    }

    void DisplayMovesInBattle(const BattlePokemon& pokemon)
    {
        for (size_t moveSlot = 1; moveSlot < 5; ++moveSlot)
        {
            const auto& move = pokemon.GetMove(moveSlot);
            if (move.IsActive())
            {
                if (move.b_isDisabled)
                {
                    std::cout << moveSlot << ". "
                        << std::left << std::setw(13) << move.GetName()
                        << std::format("{:^11}", "(Disabled!)")
                        << '\n';
                }
                else
                {
                    std::cout << moveSlot << ". "
                        << std::left << std::setw(12) << move.GetName() << " PP("
                        << std::right << std::setw(2) << move.m_currentPP << "/"
                        << std::right << std::setw(2) << move.m_maxPP << ")" << '\n';
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
                std::string_view status = DisplayPokemonStatus(p);
                std::cout
                    << count << ". "
                    << std::left << std::setw(11) << p.GetPokemonNameView()
                    << " HP("
                    << std::right << std::setw(3) << p.GetCurrentHP() << "/"
                    << std::right << std::setw(3) << p.GetMaxHP() << ") - "
                    << std::right << std::setw(4) << status
                    << " - Level: "
                    << std::right << std::setw(3) << p.GetLevel()
                    << " - Moves: ";
                DisplayLearnedMoves(p);
            }
            ++count;
        }
        std::cout << '\n';
    }

    std::string_view DisplayPokemonStatus(const BattlePokemon& pokemon)
    {
        Status currentStatus = pokemon.GetStatus();

        switch (currentStatus)
        {
            case Status::Burned:
                return "BRN";
                break;

            case Status::Frozen:
                return "FRZ";
                break;

            case Status::Paralyzed:
                return "PAR";
                break;

            case Status::Poisoned:
                return "PSN";
                break;

            case Status::Badly_Poisoned:
                return "BPSN";
                break;

            case Status::Sleeping:
                return "SLP";
                break;
        }

        if (pokemon.IsFainted())
        {
            return "FNT";
        }
        else if (pokemon.IsConfused())
        {
            return "CNF";
        }
        else
        {
            return "NOR";
        }
        
    }
}
