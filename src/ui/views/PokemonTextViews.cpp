#include <iostream>
#include <iomanip>

#include "../../entities/Player.h"
#include "../../data/Database.h"
#include "../../data/Pokemon.h"
#include "../../data/Move.h"
#include "../../data/StringToTypes.h"
#include "../../moves/MoveEffectEnums.h"
#include "../../battle/Typechart.h"

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
                std::cout << std::format("{:<13}", pokemon.GetMove(moveSlot).GetName());
                std::cout << " ";
            }
            else
            {
                std::cout << std::format("{:^13}", "---");
                std::cout << " ";
            }

            if (moveSlot != 4)
            {
                std::cout << "/";
            }
        }
        std::cout << '\n';
    }

    void DisplayLearnedMovesExpanded(const BattlePokemon& pokemon)
    {
        for (size_t moveSlot = 1; moveSlot < 5; ++moveSlot)
        {
            const auto& move = pokemon.GetMove(moveSlot);
            if (move.IsActive())
            {
                   std::cout << moveSlot << ". "
                << std::left << std::setw(13) << move.GetName() << " PP("
                << std::right << std::setw(2) << move.m_currentPP << "/"
                << std::right << std::setw(2) << move.m_maxPP << ") "
                << std::left << "- Power: " << std::setw(4) << move.GetPower()
                << std::left << "- Accuracy: " << std::setw(4) << move.GetAccuracy()
                << std::left << "- Type: " << std::setw(9) << move.GetMoveType()
                << std::left << "- Category: " << std::setw(9) << move.GetCategory()
                << '\n';
                
            }
            else
            {
                std::cout << moveSlot << ". " << "---\n";
            }
        }
    }

    void DisplayMovesInBattle(const BattlePokemon& pokemon, const BattlePokemon& target)
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
                    bool isStatus = move.GetCategoryEnum() == Category::Status;

                    std::cout << moveSlot << ". "
                        << std::left << std::setw(13) << move.GetName() << " PP("
                        << std::right << std::setw(2) << move.m_currentPP << "/"
                        << std::right << std::setw(2) << move.m_maxPP << ") "
                        << std::left << "- Type: " << std::setw(9) << move.GetMoveType()
                        << std::left << "- Category: " << std::setw(9) << move.GetCategory()
                        << std::left << "- Effectiveness: " << std::setw(15) << (isStatus ? CalculateStatusMoveEffectiveness(pokemon, target, move) : CalculateDamageMoveEffectiveness(pokemon, target, move))
                        << '\n';
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

    std::string_view CalculateStatusMoveEffectiveness(const BattlePokemon& source, const BattlePokemon& target, const pokemonMove& currentMove)
    {
        bool isGrassImmune = (currentMove.GetMoveEffectEnum() == MoveEffect::PoisonPowder
            || currentMove.GetMoveEffectEnum() == MoveEffect::StunSpore
            || currentMove.GetMoveEffectEnum() == MoveEffect::SleepPowder
            || currentMove.GetMoveEffectEnum() == MoveEffect::LeechSeed)
            && (target.GetTypeOneEnum() == PokemonType::Grass || target.GetTypeTwoEnum() == PokemonType::Grass);

        bool isElectricImmune = (currentMove.GetMoveEffectEnum() == MoveEffect::Paralyze || currentMove.GetMoveEffectEnum() == MoveEffect::StunSpore)
            && (target.GetTypeOneEnum() == PokemonType::Electric || target.GetTypeTwoEnum() == PokemonType::Electric);

        bool isThunderWaveImmune = (currentMove.GetMoveTypeEnum() == PokemonType::Electric
            && (target.GetTypeOneEnum() == PokemonType::Ground || target.GetTypeTwoEnum() == PokemonType::Ground));

        bool isPoisonImmune = (currentMove.GetMoveTypeEnum() == PokemonType::Poison)
            && ((target.GetTypeOneEnum() == PokemonType::Poison || target.GetTypeTwoEnum() == PokemonType::Poison)
            || (target.GetTypeOneEnum() == PokemonType::Steel || target.GetTypeTwoEnum() == PokemonType::Steel));

        if (isGrassImmune || isElectricImmune || isThunderWaveImmune || isPoisonImmune)
        {
            return "Immune";
        }
        else
        {
            return "Effective";
        }
    }

    std::string_view CalculateDamageMoveEffectiveness(const BattlePokemon& source, const BattlePokemon& target, const pokemonMove& currentMove)
    {
        if (currentMove.GetMoveEffectEnum() == MoveEffect::Struggle)
        {
            return "Effective";
        }

        size_t moveType = static_cast<size_t>(currentMove.GetMoveTypeEnum());
        size_t defensiveTypeOne = static_cast<size_t>(target.GetTypeOneEnum());
        size_t defensiveTypeTwo = static_cast<size_t>(target.GetTypeTwoEnum());

        uint16_t effect1 = typeChart[moveType][defensiveTypeOne];
        uint16_t effect2 = (defensiveTypeTwo == 18) ? 4096 : typeChart[moveType][defensiveTypeTwo];

        int product = static_cast<int>(effect1 * effect2);
        int moveEffectiveness = (product / 4096);

        if (moveEffectiveness == 0)
        {
            return "Immune";
        }
        else if (moveEffectiveness > 0 && moveEffectiveness < 4096)
        {
            return "Not Effective";
        }
        else if (moveEffectiveness > 4096)
        {
            return "Super Effective";
        }
        else
        {
            return "Effective";
        }
    }
}