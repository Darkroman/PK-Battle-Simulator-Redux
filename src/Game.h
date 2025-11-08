#pragma once

#include "data/Database.h"
#include "entities/AIPlayer.h"
#include "entities/HumanPlayer.h"
#include "entities/ai strategies/EasyAIStrategy.h"

#include "ui/Menu.h"
#include "battle/RandomEngine.h"
#include "battle/BattleContext.h"
#include "battle/BattleManager.h"
#include "ui/MoveResultsText.h"
#include "ui/BattleTextMenu.h"
#include "ui/StatusEffectText.h"

#include <iostream>

void RunGame()
{
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;

    start = std::chrono::high_resolution_clock::now();
    Database& db = Database::GetInstance();
    end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> ms = (end - start) * 1000;

    std::cout << "Time it took to load database: " << ms.count() << "ms" << "\n\n";

    RandomEngine rng;

    EasyAIStrategy easyStrat(rng);
    IAIStrategy& aiStrat = easyStrat;

    std::vector<std::unique_ptr<Player>> players;
    players.push_back(std::make_unique<HumanPlayer>("Player One"));
    players.push_back(std::make_unique<AIPlayer>("Player Two", aiStrat));

    BattleContext context(players);

    BattleTextMenu battleMenu(context);
    IBattleMenuUI& battleMenuUI = battleMenu;
    MoveResultsText moveResultsText(context);
    IMoveResultsUI& moveResultsUI = moveResultsText;
    StatusEffectText statusEffectText(context);
    IStatusEffectUI& statusEffectUI = statusEffectText;

    Menu menu(players, rng);

    bool exit = false;
    while (exit == false)
    {
        exit = menu.RunMenu();

        if (exit == true)
        {
            return;
        }

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
            
        BattleManager battleManager(context, rng, battleMenu, moveResultsText, statusEffectText);

        exit = battleManager.RunBattleLoop();

        exit = battleMenu.AnnounceWinner();

        battleManager.ResetValues();
    }
}