#pragma once

struct BattleContext;
class SwitchExecutor;
class Player;
class BattlePokemon;

class WinChecker
{
public:
    WinChecker(BattleContext& context, SwitchExecutor& switchExecutor);

    bool CheckWinCondition(Player& sourcePlayer, Player& targetPlayer);
    bool CheckWinOrSwitch(Player& sourcePlayer, Player& targetPlayer, BattlePokemon& targetPokemon);

private:
    BattleContext& m_context;
    SwitchExecutor& m_switchExecutor;

};
