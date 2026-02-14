#pragma once

#include "entities/ai strategies/EasyAIStrategy.h"

#include "battle/RandomEngine.h"
#include "battle/BattleContext.h"
#include "ui/ConsoleTextSink.h"
#include "ui/MoveResultsText.h"
#include "ui/BattleAnnouncerText.h"
#include "ui/StatusEffectText.h"
#include "ui/Menu.h"
#include "battle/BattleManager.h"

#include <optional>

class Database;

enum class AppState { MainMenu, InitBattle, Battle, Victory, Exit };

class GameEngine
{
public:
    GameEngine();
    void Run();

private:
    void Bootstrap();
    void PresetupBattle();

    Database& db;
    RandomEngine rng;

    EasyAIStrategy easyStrategy;
    IAIStrategy& aiStrat;

    std::vector<std::unique_ptr<Player>> players;
    BattleContext context;

    ConsoleTextSink textSink;

    BattleAnnouncerText battleAnnouncerText;
    MoveResultsText moveResultsText;
    StatusEffectText statusEffectText;

    AppState currentState = AppState::MainMenu;
    std::optional<Menu> menu;
    std::optional<BattleManager> battleManager;
};