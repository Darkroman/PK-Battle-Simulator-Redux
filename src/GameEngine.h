#pragma once

#include <optional>

#include "entities/Player.h"
#include "battle/RandomEngine.h"
#include "battle/BattleContext.h"
#include "ui/interfaces/ITextSink.h"
#include "ui/interfaces/IBattleAnnouncerUI.h"
#include "ui/interfaces/IMoveResultsUI.h"
#include "ui/interfaces/IStatusEffectUI.h"
#include "common/AppState.h"
#include "ui/Menu.h"
#include "battle/BattleManager.h"

class Database;

class GameEngine
{
public:
    GameEngine();
    void Run();
    void RunSimulate();

private:
    void SetupUI(bool headless);
    void Bootstrap();
    void PresetupBattle();

    Database& db;
    RandomEngine rng;

    std::vector<std::unique_ptr<Player>> players;
    BattleContext context;

    std::unique_ptr<ITextSink> textSink;
    std::unique_ptr<IBattleAnnouncerUI> battleAnnouncer;
    std::unique_ptr<IMoveResultsUI> moveResults;
    std::unique_ptr<IStatusEffectUI> statusEffect;

    AppState currentState = AppState::MainMenu;
    std::optional<Menu> menu;
    std::optional<BattleManager> battleManager;

    unsigned int simIterations{ 1 };
    int playerOneVictories{};
    int playerTwoVictories{};
};