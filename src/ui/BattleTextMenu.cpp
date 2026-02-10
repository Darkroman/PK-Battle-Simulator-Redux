#include "BattleTextMenu.h"
#include "../battle/BattleContext.h"
#include "../common/InputValidation.h"
#include "../entities/AIPlayer.h"
#include "views/PokemonTextView.h"

#include <iostream>

void BattleTextMenu::ThrowOutFirstPokemon(const BattleContext& context) const
{
    std::cout << context.playerOne->GetPlayerNameView() << " chose " << context.playerOne->GetBelt(1)->GetPokemonNameView() << "!\n";
    std::cout << context.playerTwo->GetPlayerNameView() << " chose " << context.playerTwo->GetBelt(1)->GetPokemonNameView() << "!\n\n";
}

void BattleTextMenu::DisplayFightingPokemon(const BattleContext& context) const
{
    auto PrintPercent = [&](int currentHP, int maxHP)
    {
        if (currentHP >= maxHP)
        {
            std::cout << "100%";
            return;
        }

        const int HP_BAR_WIDTH = context.HP_BAR_WIDTH;

        int pixelsOfHP = currentHP * HP_BAR_WIDTH / maxHP;
        int quarterPercent = pixelsOfHP * 25;

        int integer = quarterPercent / 100;
        int decimal = quarterPercent % 100;

        std::cout << integer << ".";
        if (decimal < 10) std::cout << "0";
        std::cout << decimal << "%";
    };

    auto PrintPokemon = [&](Player& player, BattlePokemon& pokemon, bool isAI)
        {
            if (isAI)
            {
                std::cout << player.GetPlayerNameView() << "'s Pokemon: " << pokemon.GetNameView() << " (";
                PrintPercent(pokemon.GetCurrentHP(), pokemon.GetMaxHP());
                std::cout << "/100%)\n";
            }
            else
            {
                std::cout << player.GetPlayerNameView() << "'s Pokemon: " << pokemon.GetNameView() << " ("
                          << pokemon.GetCurrentHP() << "/" << pokemon.GetMaxHP() << ")\n";
            }
        };

    bool isOneAI = context.aiPlayerOne;
    PrintPokemon(*context.playerOne, *context.playerOneCurrentPokemon, isOneAI);

    bool isTwoAI = context.aiPlayerTwo;
    PrintPokemon(*context.playerTwo, *context.playerTwoCurrentPokemon, isTwoAI);
}

BattleAction BattleTextMenu::PlayerOneMakeSelection(const BattleContext& context)
{
    BattleAction action{ BattleAction::None };

    if (context.playerOneCurrentPokemon->IsCharging() || context.playerOneCurrentPokemon->IsRecharging() ||
        context.playerOneCurrentPokemon->IsThrashing() || context.playerOneCurrentPokemon->IsBiding())
    {
        return BattleAction::None;
    }

    if (!context.playerOne->IsHuman())
    {
        AIPlayer* aiPlayer = static_cast<AIPlayer*>(context.playerOne);
        selectedMove = aiPlayer->ChooseAction(context.playerOneCurrentPokemon, context);
        return BattleAction::Fight;
    }
    else
    {
        std::cout << context.playerOne->GetPlayerNameView() << " choose your action\n";
        return action = MakeASelectionLoop(*context.playerOne, *context.playerOneCurrentPokemon);
    }
}

BattleAction BattleTextMenu::PlayerTwoMakeSelection(const BattleContext& context)
{
    BattleAction action{ BattleAction::None };

    if (context.playerTwoCurrentPokemon->IsCharging() || context.playerTwoCurrentPokemon->IsRecharging() ||
        context.playerTwoCurrentPokemon->IsThrashing() || context.playerTwoCurrentPokemon->IsBiding())
    {
        return BattleAction::None;
    }

    if (!context.playerTwo->IsHuman())
    {
        AIPlayer* aiPlayer = static_cast<AIPlayer*>(context.playerTwo);
        selectedMove = aiPlayer->ChooseAction(context.playerTwoCurrentPokemon, context);
        return BattleAction::Fight;
    }
    else
    {
        std::cout << context.playerTwo->GetPlayerNameView() << " choose your action\n";

        return action = MakeASelectionLoop(*context.playerTwo, *context.playerTwoCurrentPokemon);
    }
}

BattleAction BattleTextMenu::MakeASelectionLoop(Player& player, BattlePokemon& currentPokemon)
{
    BattleAction action = BattleAction::None;
    while (action == BattleAction::None)
    {
        std::cout << "1. Fight \t";
        std::cout << "2. Switch Pokemon";
        std::cout << ((player.CanSwitch()) ? " \t" : "(X) \t");
        std::cout << "3. Forfeit\n";

        std::string input;
        std::cout << "Option: ";
        std::cin >> input;

        if (!IsDigits(input) || input.size() > 10)
        {
            std::cout << "Invalid input!\n\n";
            continue;
        }

        int choice = std::stoi(input);

        switch (choice)
        {
        case 1:
            action = Fight(player, currentPokemon);
            break;

        case 2:
            if (!player.CanSwitch())
            {
                std::cout << "You aren't able to switch Pokemon right now!\n";
                break;
            }
            action = SwitchPokemonOption(player, currentPokemon);
            break;

        case 3:
            action = Forfeit(player);
            break;

        default:
            std::cout << "Invalid input!\n\n";
            break;
        }

        if (action == BattleAction::None)
        {
            continue;
        }
        else
        {
            return action;
        }
    }
}

BattleAction BattleTextMenu::Fight(Player& player, BattlePokemon& currentPokemon)
{
    bool struggle = false;

    while (true)
    {
        std::cout << currentPokemon.GetName() << "'s current moves\n";
        PokemonTextView::DisplayMovesInBattle(currentPokemon);

        //struggle = CheckPPCountForStruggle(currentPokemon);

        if (struggle)
        {
            std::cout << "You are out of PP for all moves. All you can do is Struggle.\n\n";
            //selectedMove = currentPokemon.Struggle();
            return BattleAction::Struggle;
        }

        std::string input;
        std::cout << "Option (0 to cancel): ";
        std::cin >> input;
        std::cout << '\n';

        if (!IsDigits(input) || input.size() > 10)
        {
            std::cout << "Invalid input!\n\n";
            continue;
        }

        int choice = std::stoi(input);

        if (choice == 0)
        {
            return BattleAction::None;
        }

        if (choice > 4)
        {
            std::cout << "Invalid input!\n\n";
            continue;
        }

        if (choice > 0 && struggle)
        {
            //selectedMove = currentPokemon.Struggle();
            return BattleAction::Struggle;
        }

        if (!currentPokemon.GetMove(choice)->IsActive())
        {
            std::cout << "There is no move there!\n\n";
            continue;
        }

        if (currentPokemon.GetMove(choice)->b_isDisabled)
        {
            std::cout << "This move is currently disabled!\n\n";
            continue;
        }

        if (currentPokemon.GetPP(choice) <= 0)
        {
            std::cout << "There's no PP left for this move!\n\n";
            continue;
        }

        if (currentPokemon.GetMove(choice)->IsActive())
        {
            selectedMove = currentPokemon.GetMove(choice);
            std::cout << player.GetPlayerNameView() << " has chosen " << selectedMove->GetName() << "\n\n";
            return BattleAction::Fight;
        }
    }
}

BattleAction BattleTextMenu::SwitchPokemonOption(Player& currentPlayer, BattlePokemon& currentPokemon)
{
    std::cout << "Choose Pokemon to switch out! 0 to cancel.\n";

    BattleAction action{ BattleAction::None };
    while (true)
    {
        PokemonTextView::DisplayPlayerPokemon(currentPlayer);

        std::string input;
        std::cout << "Option: ";
        std::cin >> input;
        std::cout << '\n';

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (!IsDigits(input) || input.size() > 10)
        {
            std::cout << "Invalid input!\n\n";
            continue;
        }

        int choice = std::stoi(input);

        if (choice == 0 && currentPokemon.IsFainted())
        {
            std::cout << "Your " << currentPokemon.GetNameView() << " is fainted. You must select another pokemon to take its place!\n\n";
            continue;
        }
        else if (choice == 0)
        {
            return BattleAction::None;
        }

        if (choice > 6)
        {
            std::cout << "Invalid input!\n\n";
            continue;
        }

        if (!currentPlayer.GetBelt(choice)->HasPokemon())
        {
            std::cout << "No Pokemon there!\n\n";
            continue;
        }

        if (currentPlayer.GetBelt(choice)->IsFainted())
        {
            std::cout << "A fainted Pokemon cannot fight!\n\n";
            continue;
        }

        if (currentPlayer.GetBelt(choice) == &currentPokemon)
        {
            std::cout << "That pokemon is already in play!\n\n";
            continue;
        }

        if (choice != 0)
        {
            selectedPokemon = currentPlayer.GetBelt(choice);
            return BattleAction::SwitchPokemon;
        }
    }
}

BattleAction BattleTextMenu::Forfeit(Player& sourcePlayer)
{
    std::cout << sourcePlayer.GetPlayerNameView() << " has forfeited!\n\n";
    return BattleAction::Forfeit;
}

void BattleTextMenu::SwitchOutMsg(const Player& player, const BattlePokemon& pokemon) const
{
    std::cout << player.GetPlayerNameView() << " switches out " << pokemon.GetNameView() << "\n";
}

void BattleTextMenu::PlayerChoosesMsg(const Player& player, const BattlePokemon& pokemon) const
{
    std::cout << player.GetPlayerNameView() << " chooses " << pokemon.GetNameView() << "\n";
}

bool BattleTextMenu::CheckPPCountForStruggle(BattlePokemon& pokemon)
{
    int zeroPPCount{ 0 }, disabledCount{ 0 };

    for (size_t i = 0; i < 4; ++i)
    {
        if (pokemon.GetMove(i + 1)->IsActive())
        {
            if (pokemon.GetMove(i + 1)->m_currentPP == 0)
            {
                ++zeroPPCount;
            }
            if (pokemon.GetMove(i + 1)->b_isDisabled)
            {
                ++disabledCount;
            }
        }
    }

    if (zeroPPCount + disabledCount >= pokemon.GetMoveCount())
    {
        return true;
    }
    return false;
}

bool BattleTextMenu::AnnounceWinner(const BattleContext& context)
{
    /*
    if (m_context.playerOne->HasWon() && m_context.playerTwo->HasWon())
    {
        std::cout << "Both players Pokemon have fainted in the same turn!\n";
        std::cout << "It is a tie game!\n\n";
        m_context.playerOne->SetWinCondition(false);
        m_context.playerTwo->SetWinCondition(false);
        return false;
    }
    */

    if (context.playerOne->HasWon())
    {
        std::cout << '\n' << context.playerOne->GetPlayerNameView() << " wins!\n\n";
        return false;
    }

    if (context.playerTwo->HasWon())
    {
        std::cout << '\n' << context.playerTwo->GetPlayerNameView() << " wins!\n\n";
        return false;
    }

    return false;
}

void BattleTextMenu::NewLine() const
{
    std::cout << '\n';
}

void BattleTextMenu::DisplayTurnNumber(int turnCount) const
{
    std::cout << "TURN #" << turnCount << "\n\n";
}

BattlePokemon::pokemonMove* BattleTextMenu::GetChosenMove() const
{
    return selectedMove;
}

BattlePokemon* BattleTextMenu::GetChosenPokemon() const
{
    return selectedPokemon;
}