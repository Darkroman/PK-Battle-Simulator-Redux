#pragma once

struct BattleContext;
class IBattleMenuUI;
class TurnUtils;
class Player;
class BattlePokemon;

class WinChecker
{
public:
    WinChecker(BattleContext& context, TurnUtils& turnUtils, IBattleMenuUI& battleMenuUI);

    bool CheckWinCondition(Player* sourcePlayer, Player* targetPlayer);
    bool CheckWinOrSwitch(Player* sourcePlayer, Player* targetPlayer, BattlePokemon* targetPokemon);

private:
    BattleContext& m_context;
    TurnUtils& m_turnUtils;
    IBattleMenuUI& m_battleMenuUI;

};
