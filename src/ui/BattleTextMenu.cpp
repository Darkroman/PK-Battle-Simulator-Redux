#include "BattleTextMenu.h"
#include "../battle/BattleContext.h"
#include "../data/InputValidation.h"
#include "../entities/AIPlayer.h"
#include <iostream>
#include <limits>
#include <string>

BattleTextMenu::BattleTextMenu(BattleContext& context) : m_context(context) {}

void BattleTextMenu::ThrowOutFirstPokemon()
{
    std::cout << m_context.playerOne->GetPlayerNameView() << " chose " << m_context.playerOne->GetBelt(1)->GetPokemonNameView() << "!\n";
    std::cout << m_context.playerTwo->GetPlayerNameView() << " chose " << m_context.playerTwo->GetBelt(1)->GetPokemonNameView() << "!\n\n";

    m_context.playerOneCurrentPokemon = m_context.playerOne->GetBelt(1);
    m_context.playerTwoCurrentPokemon = m_context.playerTwo->GetBelt(1);
}

void BattleTextMenu::DisplayFightingPokemon()
{
    std::cout << m_context.playerOne->GetPlayerNameView() << "'s Pokemon: " << m_context.playerOneCurrentPokemon->GetNameView() << " ("
        << m_context.playerOneCurrentPokemon->GetCurrentHP() << "/" << m_context.playerOneCurrentPokemon->GetMaxHP() << ")\n";

    std::cout << m_context.playerTwo->GetPlayerNameView() << "'s Pokemon: " << m_context.playerTwoCurrentPokemon->GetNameView() << " ("
        << m_context.playerTwoCurrentPokemon->GetCurrentHP() << "/" << m_context.playerTwoCurrentPokemon->GetMaxHP() << ")\n\n";
}

void BattleTextMenu::PlayerOneMakeSelection()
{
    if (m_context.playerOneCurrentPokemon->IsCharging() || m_context.playerOneCurrentPokemon->IsRecharging() ||
        m_context.playerOneCurrentPokemon->IsThrashing() || m_context.playerOneCurrentPokemon->IsBiding())
    {
        return;
    }

    if (!m_context.playerOne->IsHuman())
    {
        AIPlayer* aiPlayer = static_cast<AIPlayer*>(m_context.playerOne);
        aiPlayer->ChooseAction(m_context.playerOneCurrentPokemon, m_context);
    }
    else
    {
        std::cout << m_context.playerOne->GetPlayerNameView() << " choose your action\n";

        MakeASelectionLoop(m_context.playerOne, m_context.playerOneCurrentPokemon);

        if (m_context.playerOne->HasForfeited())
            return;
    }
    m_context.playerOneCurrentMove = m_context.selectedMove;
    m_context.selectedMove = nullptr;
}

void BattleTextMenu::PlayerTwoMakeSelection()
{
    if (m_context.playerTwoCurrentPokemon->IsCharging() || m_context.playerTwoCurrentPokemon->IsRecharging() ||
        m_context.playerTwoCurrentPokemon->IsThrashing() || m_context.playerTwoCurrentPokemon->IsBiding())
    {
        return;
    }

    if (!m_context.playerTwo->IsHuman())
    {
        AIPlayer* aiPlayer = static_cast<AIPlayer*>(m_context.playerTwo);
        aiPlayer->ChooseAction(m_context.playerTwoCurrentPokemon, m_context);
    }
    else
    {
        std::cout << m_context.playerTwo->GetPlayerNameView() << " choose your action\n";

        MakeASelectionLoop(m_context.playerTwo, m_context.playerTwoCurrentPokemon);

        if (m_context.playerTwo->HasForfeited())
            return;
    }
    m_context.playerTwoCurrentMove = m_context.selectedMove;
    m_context.selectedMove = nullptr;
}

void BattleTextMenu::MakeASelectionLoop(Player* player, BattlePokemon* currentPokemon)
{
    bool exit = false;
    while (!exit)
    {
        std::cout << "1. Fight \t";
        std::cout << "2. Switch Pokemon";
        std::cout << ((player->CanSwitch()) ? " \t" : "(X) \t");
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
            exit = Fight(player, currentPokemon);
            break;

        case 2:
            if (!player->CanSwitch())
            {
                std::cout << "You aren't able to switch Pokemon right now!\n";
                break;
            }
            exit = SwitchPokemonOption(player, currentPokemon);
            break;

        case 3:
            Forfeit(player);
            exit = true;
            break;

        default:
            std::cout << "Invalid input!\n\n";
            break;
        }
    }
}

bool BattleTextMenu::Fight(Player* player, BattlePokemon* currentPokemon)
{
    bool struggle = false;

    while (true)
    {
        std::cout << currentPokemon->GetName() << "'s current moves\n";
        currentPokemon->DisplayMovesInBattle();

        struggle = CheckPPCountForStruggle(currentPokemon);

        if (struggle)
        {
            std::cout << "You are out of PP for all moves. All you can do is Struggle.\n\n";
            m_context.selectedMove = currentPokemon->Struggle();
            return true;
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
            return false;
        }

        if (choice > 4)
        {
            std::cout << "Invalid input!\n\n";
            continue;
        }

        if (choice > 0 && struggle)
        {
            m_context.selectedMove = currentPokemon->Struggle();
            return true;
        }

        if (currentPokemon->GetMove(choice)->mp_move == nullptr)
        {
            std::cout << "There is no move there!\n\n";
            continue;
        }

        if (currentPokemon->GetMove(choice)->b_isDisabled)
        {
            std::cout << "This move is currently disabled!\n\n";
            continue;
        }

        if (currentPokemon->GetPP(choice) <= 0)
        {
            std::cout << "There's no PP left for this move!\n\n";
            continue;
        }

        if (currentPokemon->GetMove(choice)->mp_move != nullptr)
        {
            m_context.selectedMove = currentPokemon->GetMove(choice);
            std::cout << player->GetPlayerNameView() << " has chosen " << m_context.selectedMove->mp_move->GetName() << "\n\n";
            return true;
        }
    }
}

bool BattleTextMenu::SwitchPokemonOption(Player* currentPlayer, BattlePokemon* currentPokemon)
{
    std::cout << "Choose Pokemon to switch out! 0 to cancel.\n";

    bool exit = false;
    while (!exit)
    {
        currentPlayer->DisplayPlayerPokemon();

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

        if (choice == 0 && currentPokemon->IsFainted())
        {
            std::cout << "Your " << currentPokemon->GetNameView() << " is fainted. You must select another pokemon to take its place!\n\n";
            continue;
        }
        else if (choice == 0)
        {
            return false;
        }

        if (choice > 6)
        {
            std::cout << "Invalid input!\n\n";
            continue;
        }

        if (!currentPlayer->GetBelt(choice)->HasPokemon())
        {
            std::cout << "No Pokemon there!\n\n";
            continue;
        }

        if (currentPlayer->GetBelt(choice)->IsFainted())
        {
            std::cout << "A fainted Pokemon cannot fight!\n\n";
            continue;
        }

        if (currentPlayer->GetBelt(choice) == currentPokemon)
        {
            std::cout << "That pokemon is already in play!\n\n";
            continue;
        }

        if (choice != 0)
        {
            exit = true;

            currentPlayer->SetIsSwitching(true);

            currentPlayer->SetPokemonToSwitchTo(currentPlayer->GetBelt(choice));

            return exit;
        }
    }
    return exit;
}

void BattleTextMenu::SwitchOutMsg(Player* player, BattlePokemon* pokemon)
{
    std::cout << player->GetPlayerNameView() << " switches out " << pokemon->GetNameView() << "\n";
}

void BattleTextMenu::PlayerChoosesMsg(Player* player, BattlePokemon* pokemon)
{
    std::cout << player->GetPlayerNameView() << " chooses " << pokemon->GetNameView() << "\n";
}

void BattleTextMenu::Forfeit(Player* sourcePlayer)
{
    std::cout << sourcePlayer->GetPlayerNameView() << " has forfeited!\n\n";
    sourcePlayer->SetForfeit(true);
}

bool BattleTextMenu::CheckPPCountForStruggle(BattlePokemon* pokemon)
{
    int zeroPPCount{ 0 }, disabledCount{ 0 };

    for (size_t i = 0; i < 4; ++i)
    {
        if (pokemon->GetMove(i + 1)->mp_move != nullptr)
        {
            if (pokemon->GetMove(i + 1)->m_currentPP == 0)
            {
                ++zeroPPCount;
            }
            if (pokemon->GetMove(i + 1)->b_isDisabled)
            {
                ++disabledCount;
            }
        }
    }

    if (zeroPPCount + disabledCount >= pokemon->GetMoveCount())
    {
        return true;
    }
    return false;
}

bool BattleTextMenu::AnnounceWinner()
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

    if (m_context.playerOne->HasWon())
    {
        std::cout << "\n" << m_context.playerOne->GetPlayerNameView() << " wins!\n\n";
        m_context.playerOne->SetWinCondition(false);
        return false;
    }

    if (m_context.playerTwo->HasWon())
    {
        std::cout << "\n" << m_context.playerTwo->GetPlayerNameView() << " wins!\n\n";
        m_context.playerTwo->SetWinCondition(false);
        return false;
    }

    return false;
}

void BattleTextMenu::NewLine()
{
    std::cout << '\n';
}

void BattleTextMenu::DisplayTurnNumber(int turnCount)
{
    std::cout << "TURN #" << turnCount << "\n\n";
}