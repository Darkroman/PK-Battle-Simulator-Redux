#include "data/Database.h"
#include "entities/AIPlayer.h"
#include "entities/HumanPlayer.h"

#include "GameEngine.h"

GameEngine::GameEngine()
    : db(Database::GetInstance()),
    rng(),
    easyStrategy(rng),
    aiStrat(easyStrategy),
    players([&] {
        std::vector<std::unique_ptr<Player>> v;
        v.push_back(std::make_unique<HumanPlayer>("Player One"));
        v.push_back(std::make_unique<AIPlayer>("Player Two", aiStrat));
        return v;
        }()),
    context(players),
    textSink(),
    battleTextMenu(),
    moveResultsText(textSink),
    statusEffectText(textSink)
{
    Bootstrap();
}

void GameEngine::Bootstrap()
{
    context.playerOne = players[0].get();
    context.playerTwo = players[1].get();
}

void GameEngine::PresetupBattle()
{
    context.playerOne = players[0].get();
    context.playerTwo = players[1].get();

    if (context.playerOne->IsHuman())
    {
        context.aiPlayerOne = nullptr;
    }
    else
    {
        context.aiPlayerOne = static_cast<AIPlayer*>(context.playerOne);
    }

    if (context.playerTwo->IsHuman())
    {
        context.aiPlayerTwo = nullptr;
    }
    else
    {
        context.aiPlayerTwo = static_cast<AIPlayer*>(context.playerTwo);
    }
}

void GameEngine::Run()
{
    while (currentState != AppState::Exit)
    {
        switch (currentState)
        {
        case AppState::MainMenu:
            if (!menu)
            {
                menu.emplace(players, rng, aiStrat);
            }

            if (menu->RunMenu())
            {
                currentState = AppState::Exit;
            }
            else
            {
                menu.reset();
                currentState = AppState::InitBattle;
            }

            break;

        case AppState::InitBattle:
            PresetupBattle();

            if (!battleManager)
            {
                battleManager.emplace(context, rng, battleTextMenu, moveResultsText, statusEffectText);
            }

            currentState = AppState::Battle;

            break;

        case AppState::Battle:
            if (battleManager->RunBattleLoop())
            {
                currentState = AppState::Victory;
            }

            break;

        case AppState::Victory:
            battleTextMenu.AnnounceWinner(context);
            battleManager->ResetValues();
            battleManager.reset();
            currentState = AppState::MainMenu;

            break;

        case AppState::Exit:
            return;

        }
    }
}