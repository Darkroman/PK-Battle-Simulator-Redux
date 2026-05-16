#include <iostream>
#include <chrono>
#include <thread>

#include "GameEngine.h"

#include "data/Database.h"
#include "entities/Player.h"
#include "entities/controllers/AIController.h"
#include "entities/controllers/HumanControllerConsole.h"

#include "ui/ConsoleTextSink.h"
#include "ui/MoveResultsText.h"
#include "ui/BattleAnnouncerText.h"
#include "ui/StatusEffectText.h"

#include "ui/BattleAnnouncerHeadless.h"
#include "ui/MoveResultsHeadless.h"
#include "ui/StatusEffectHeadless.h"

GameEngine::GameEngine()
    : db(Database::GetInstance()),
    rng(),
    players([&] {
        std::vector<std::unique_ptr<Player>> v;
        v.push_back(std::make_unique<Player>("Player One"));
        v.push_back(std::make_unique<Player>("Player Two"));
        v.at(0)->SetController(std::make_unique<HumanControllerConsole>(), ControllerType::Human);
        v.at(1)->SetController(std::make_unique<AIController>(Difficulty::Easy), ControllerType::AI);
        return v;
        }()),
    context(players)
    //textSink(std::make_unique<ConsoleTextSink>()),
    //battleAnnouncer(std::make_unique<BattleAnnouncerHeadless>()),
    //moveResults(std::make_unique<MoveResultsHeadless>()),
    //statusEffect(std::make_unique<StatusEffectHeadless>())
{
    SetupUI(false);
    Bootstrap();
}

void GameEngine::SetupUI(bool headless)
{
    if (headless)
    {
        battleAnnouncer = std::make_unique<BattleAnnouncerHeadless>();
        moveResults = std::make_unique<MoveResultsHeadless>();
        statusEffect = std::make_unique<StatusEffectHeadless>();
    }
    else
    {
        textSink = std::make_unique<ConsoleTextSink>();
        battleAnnouncer = std::make_unique<BattleAnnouncerText>();
        moveResults = std::make_unique<MoveResultsText>(*textSink);
        statusEffect = std::make_unique<StatusEffectText>(*textSink);
    }
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
    if (context.playerOne->IsAI()) context.vec_aiPlayers.emplace_back(context.playerOne);
    if (context.playerTwo->IsAI()) context.vec_aiPlayers.emplace_back(context.playerTwo);
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
                    menu.emplace(players);
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
                    battleManager.emplace(context, rng, *battleAnnouncer, *moveResults, *statusEffect);
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
                battleAnnouncer->AnnounceWinner(context);
                battleManager->ResetValues();
                battleManager.reset();
                currentState = AppState::MainMenu;

                break;

            case AppState::Simulate:
            {
                RunSimulate();

                currentState = AppState::MainMenu;

                break;
            }

            case AppState::Exit:
                return;

        }
    }
}

void GameEngine::RunSimulate()
{
    simIterations = 100000;

    if (simIterations <= 0)
    {
        std::cerr << "Simulation iterations must be greater than zero.\n";
        return;
    }

    unsigned int numThreads = std::thread::hardware_concurrency();

    if (numThreads == 0)
    {
        numThreads = 1;
    }

    numThreads = std::min(numThreads, simIterations);

    unsigned int baseBattlesPerThread = simIterations / numThreads;
    unsigned int remainder = simIterations % numThreads;

    std::atomic<int> pOneVictories{};
    std::atomic<int> pTwoVictories{};

    std::vector<std::thread> workers;
    workers.reserve(numThreads);

    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;

    start = std::chrono::high_resolution_clock::now();

    for (unsigned int t = 0; t < numThreads; ++t)
    {
        workers.emplace_back([&, t]()
        {
            RandomEngine localRng(t);

            std::vector<std::unique_ptr<Player>> localPlayers;
            localPlayers.reserve(players.size());

            for (const auto& playerPtr : players)
            {
                localPlayers.push_back(std::make_unique<Player>(*playerPtr));
            }

            BattleContext localContext(localPlayers);

            BattleManager localManager(localContext, localRng, *battleAnnouncer, *moveResults, *statusEffect);

            int localP1Wins = 0;
            int localP2Wins = 0;

            localContext.playerOne = localPlayers[0].get();
            localContext.playerTwo = localPlayers[1].get();

            localContext.vec_aiPlayers.clear();
            if (localContext.playerOne->IsAI()) localContext.vec_aiPlayers.emplace_back(localContext.playerOne);
            if (localContext.playerTwo->IsAI()) localContext.vec_aiPlayers.emplace_back(localContext.playerTwo);

            unsigned int battlesForThisThread =
            baseBattlesPerThread + (t < remainder ? 1 : 0);

            for (unsigned int i = 0; i < battlesForThisThread; ++i)
            {
                localManager.RunBattleLoop();

                if (localPlayers[0]->HasWon())
                {
                    ++localP1Wins;
                }
                else if (localPlayers[1]->HasWon())
                {
                    ++localP2Wins;
                }

                localManager.ResetValues();
            }

            pOneVictories += localP1Wins;
            pTwoVictories += localP2Wins;
        });
    }

    for (auto& th : workers) th.join();

    end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = (end - start);

    std::cout << "Time it took to do simulation: " << elapsed.count() << " seconds" << "\n\n";

    std::cout << "Player One has won " << pOneVictories << " times\n";
    std::cout << "Player Two has won " << pTwoVictories << " times\n";
}