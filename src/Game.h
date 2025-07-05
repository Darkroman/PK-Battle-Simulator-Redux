#pragma once
#include "ui/Menu.h"
#include "battle/RandomEngine.h"
#include "battle/BattleContext.h"
#include "battle/TurnManager.h"
#include "battle/TurnProcessor.h"
#include "ui/MoveResultsText.h"
#include "ui/BattleTextMenu.h"

void RunGame()
{
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;

    start = std::chrono::high_resolution_clock::now();
    Database& db = Database::GetInstance();
    end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> ms = (end - start) * 1000;

    std::cout << "Time it took to load database: " << ms.count() << "ms" << "\n\n";
    
    std::vector<Player> players{ Player("Player One"), Player("Player Two") };

    Menu menu(players);

    BattleContext context(players);
    RandomEngine rng;
    BattleTextMenu battleMenu(context);
    IBattleMenuUI& battleMenuUI = battleMenu;
    MoveResultsText moveResultsText(context);
    IMoveResultsUI& moveResultsUI = moveResultsText;

    TurnManager turnManager(context, rng, battleMenu, moveResultsText);

    bool exit = false;
    while (exit == false)
    {
        exit = menu.RunMenu();

        if (exit == true)
        {
            return;
        }

        exit = turnManager.RunBattleLoop();

        exit = turnManager.AnnounceWinner();

        //exit = battle.StartBattle();

        //exit = battle.AnnounceWinner();
    }
}