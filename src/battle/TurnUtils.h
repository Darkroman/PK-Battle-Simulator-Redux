#pragma once

struct BattleContext;
class IBattleMenuUI;
class Player;
class BattlePokemon;

class TurnUtils
{
public:
	TurnUtils(BattleContext&, IBattleMenuUI&);

	void PerformSwitch(Player* player, BattlePokemon*& pokemon);

private:
	BattleContext& m_context;
	IBattleMenuUI& m_battleMenuUI;
};
