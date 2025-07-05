#pragma once

struct BattleContext;
class TurnProcessor;
class IBattleMenuUI;
class Player;
class BattlePokemon;

class WinChecker
{
public:
    WinChecker(BattleContext& context, TurnProcessor& turnProcessor, IBattleMenuUI& battleMenuUI);

    bool CheckWinCondition(Player* sourcePlayer, Player* targetPlayer);
    bool CheckWinOrSwitch(Player* sourcePlayer, Player* targetPlayer, BattlePokemon* targetPokemon);

private:
    BattleContext& m_context;
    TurnProcessor& m_turnProcessor;
    IBattleMenuUI& m_battleMenuUI;
};
