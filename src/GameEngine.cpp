#include "data/Database.h"
#include "entities/Player.h"
#include "entities/controllers/AIController.h"
#include "entities/controllers/HumanControllerConsole.h"

#include "GameEngine.h"

GameEngine::GameEngine()
    : db(Database::GetInstance()),
    rng(),
    easyStrategy(rng),
    aiStrat(easyStrategy),
    players([&] {
        std::vector<std::unique_ptr<Player>> v;
        v.push_back(std::make_unique<Player>("Player One"));
        v.push_back(std::make_unique<Player>("Player Two"));
        v.at(0)->SetController(std::make_unique<HumanControllerConsole>(), ControllerType::Human);
        v.at(1)->SetController(std::make_unique<AIController>(aiStrat), ControllerType::AI);
        return v;
        }()),
    context(players),
    textSink(),
    battleAnnouncerText(),
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

    context.vec_aiPlayers.clear();
    if (context.playerOne->IsAI()) context.vec_aiPlayers.push_back(context.playerOne);
    if (context.playerTwo->IsAI()) context.vec_aiPlayers.push_back(context.playerTwo);
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
                    battleManager.emplace(context, rng, battleAnnouncerText, moveResultsText, statusEffectText);
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
                battleAnnouncerText.AnnounceWinner(context);
                battleManager->ResetValues();
                battleManager.reset();
                currentState = AppState::MainMenu;

                break;

            case AppState::Exit:
                return;

        }
    }
}