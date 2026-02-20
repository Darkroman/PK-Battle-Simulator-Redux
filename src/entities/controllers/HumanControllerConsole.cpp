#include <iostream>
#include <string>

#include "../../common/InputValidation.h"
#include "../../ui/views/PokemonTextView.h"
#include "../Player.h"

#include "HumanControllerConsole.h"

PlayerDecisionOutcome HumanControllerConsole::ChooseAction(Player& player, BattlePokemon& currentPokemon)
{
    PlayerDecisionOutcome decision{};

    std::cout << player.GetPlayerNameView() << " choose your action\n";

    BattleAction action = BattleAction::None;
    while (action == BattleAction::None)
    {
        std::cout << "1. Fight \t";
        std::cout << "2. Switch Pokemon";
        std::cout << ((player.CanSwitch()) ? " \t" : "(X) \t");
        std::cout << "3. Forfeit\n";

        std::string input;
        std::cout << "Option: ";
        std::getline(std::cin >> std::ws, input);
        std::cout << '\n';

        if (!IsDigits(input) || input.size() > 10)
        {
            std::cout << "Invalid input!\n\n";
            continue;
        }

        int choice = std::stoi(input);

        switch (choice)
        {

        case 1:
            decision.action = BattleAction::Fight;
            decision.chosenMove = FightAction(player, currentPokemon);

            if (!decision.chosenMove)
            {
                continue;
            }

            break;

        case 2:
            if (!player.CanSwitch())
            {
                std::cout << "You aren't able to switch Pokemon right now!\n";
                break;
            }
            decision.action = BattleAction::SwitchPokemon;
            decision.chosenPokemon = SwitchAction(player, currentPokemon);

            if (!decision.chosenPokemon)
            {
                continue;
            }

            break;

        case 3:
            decision.action = ForfeitAction(player);
            break;

        default:
            std::cout << "Invalid input!\n\n";
            break;
        }

        return decision;
    }
}

BattlePokemon* HumanControllerConsole::PromptForSwitch(Player& currentPlayer, BattlePokemon& currentPokemon)
{
   BattlePokemon* selectedPokemon = SwitchAction(currentPlayer, currentPokemon);
    return selectedPokemon;
}

BattlePokemon::pokemonMove* HumanControllerConsole::FightAction(Player& player, BattlePokemon& currentPokemon)
{
    bool struggle = currentPokemon.CheckPPCountForStruggle();

    BattlePokemon::pokemonMove* selectedMove{ nullptr };

    if (struggle)
    {
        std::cout << "You are out of PP for all moves. All you can do is Struggle.\n\n";
        
        return selectedMove = &currentPokemon.Struggle();
    }

    while (true)
    {
        std::cout << currentPokemon.GetName() << "'s current moves\n";
        PokemonTextView::DisplayMovesInBattle(currentPokemon);

        std::string input;
        std::cout << "Option (0 to cancel): ";
        std::getline(std::cin >> std::ws, input);
        std::cout << '\n';

        if (!IsDigits(input) || input.size() > 10)
        {
            std::cout << "Invalid input!\n\n";
            continue;
        }

        int choice = std::stoi(input);

        if (choice == 0)
        {
            return nullptr;
        }

        if (choice > 4)
        {
            std::cout << "Invalid input!\n\n";
            continue;
        }

        if (choice > 0 && struggle)
        {
            return selectedMove = &currentPokemon.Struggle();
        }

        if (currentPokemon.GetMove(choice).b_isDisabled)
        {
            std::cout << "This move is currently disabled!\n\n";
            continue;
        }

        if (currentPokemon.GetPP(choice) <= 0)
        {
            std::cout << "There's no PP left for this move!\n\n";
            continue;
        }

        if (!currentPokemon.GetMove(choice).IsActive())
        {
            std::cout << "There is no move there!\n\n";
            continue;
        }
        else
        {
            selectedMove = &currentPokemon.GetMove(choice);
            std::cout << player.GetPlayerNameView() << " has chosen " << selectedMove->GetName() << "\n\n";
            return selectedMove;
        }
    }
}

BattlePokemon* HumanControllerConsole::SwitchAction(Player& currentPlayer, BattlePokemon& currentPokemon)
{
    std::cout << "Choose Pokemon to switch out! 0 to cancel.\n";

    BattlePokemon* selectedPokemon{};

    while (true)
    {
        PokemonTextView::DisplayPlayerPokemon(currentPlayer);

        std::string input;
        std::cout << "Option: ";
        std::getline(std::cin >> std::ws, input);
        std::cout << '\n';

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
            return selectedPokemon;
        }

        if (choice > 6)
        {
            std::cout << "Invalid input!\n\n";
            continue;
        }

        if (!currentPlayer.GetBelt(choice).HasPokemon())
        {
            std::cout << "No Pokemon there!\n\n";
            continue;
        }

        if (currentPlayer.GetBelt(choice).IsFainted())
        {
            std::cout << "A fainted Pokemon cannot fight!\n\n";
            continue;
        }

        if (&currentPlayer.GetBelt(choice) == &currentPokemon)
        {
            std::cout << "That pokemon is already in play!\n\n";
            continue;
        }

        if (choice != 0)
        {
            selectedPokemon = &currentPlayer.GetBelt(choice);
            return selectedPokemon;
        }
    }
}

BattleAction HumanControllerConsole::ForfeitAction(Player& sourcePlayer)
{
    std::cout << sourcePlayer.GetPlayerNameView() << " has forfeited!\n";
    return BattleAction::Forfeit;
}
