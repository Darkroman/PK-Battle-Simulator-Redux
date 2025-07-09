#pragma once

#include "ui/Menu.h"
#include "battle/RandomEngine.h"
#include "battle/BattleContext.h"
#include "battle/TurnManager.h"
#include "battle/TurnProcessor.h"
#include "ui/MoveResultsText.h"
#include "ui/BattleTextMenu.h"
#include "ui/StatusEffectText.h"
#include "entities/ai strategies/EasyAIStrategy.h"

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

    Menu menu(players, rng);

    bool exit = false;
    while (exit == false)
    {
        exit = menu.RunMenu();

        if (exit == true)
        {
            return;
        }

        BattleContext context(players);
        BattleTextMenu battleMenu(context);
        IBattleMenuUI& battleMenuUI = battleMenu;
        MoveResultsText moveResultsText(context);
        IMoveResultsUI& moveResultsUI = moveResultsText;
        StatusEffectText statusEffectText(context);
        IStatusEffectUI& statusEffectUI = statusEffectText;

        TurnManager turnManager(context, rng, statusEffectText, battleMenu, moveResultsText);

        exit = turnManager.RunBattleLoop();

        exit = battleMenu.AnnounceWinner();

        turnManager.ResetValues();
    }
}