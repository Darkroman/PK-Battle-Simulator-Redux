#pragma once

#include "ui/Menu.h"
#include "battle/RandomEngine.h"
#include "battle/BattleContext.h"
#include "battle/TurnManager.h"
#include "battle/TurnProcessor.h"
#include "battle/PostTurnEffectProcessor.h"
#include "ui/MoveResultsText.h"
#include "ui/BattleTextMenu.h"
#include "ui/StatusEffectText.h"
#include "entities/ai strategies/EasyAIStrategy.h"
#include "battle/TurnUtils.h"
#include "battle/MoveExecutor.h"

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
    BattleCalculations calculations(context, rng);

    BattleTextMenu battleMenu(context);
    IBattleMenuUI& battleMenuUI = battleMenu;
    MoveResultsText moveResultsText(context);
    IMoveResultsUI& moveResultsUI = moveResultsText;
    StatusEffectText statusEffectText(context);
    IStatusEffectUI& statusEffectUI = statusEffectText;

    BattleStatusManager statusManager(context, rng, statusEffectText);
    TurnUtils turnUtils(context, battleMenu);
    WinChecker winChecker(context, turnUtils, battleMenu);
    MoveExecutor moveExecutor(context, calculations, statusManager, moveResultsText, battleMenu, statusEffectText, rng, turnUtils);
    TurnProcessor turnProcessor(context, calculations, rng, statusManager, winChecker, turnUtils, moveExecutor);
    PostTurnEffectProcessor postTurn(context, statusEffectText, statusManager, winChecker);

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

        TurnManager turnManager(context, battleMenu, winChecker, turnProcessor, postTurn);

        exit = turnManager.RunBattleLoop();

        exit = battleMenu.AnnounceWinner();

        turnManager.ResetValues();
    }
}