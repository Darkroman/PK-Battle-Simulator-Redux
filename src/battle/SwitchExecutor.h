#pragma once

struct BattleContext;
class IBattleMenuUI;
class Player;
class BattlePokemon;

class SwitchExecutor
{
public:
	SwitchExecutor(BattleContext&, IBattleMenuUI&);

	void ExecuteSwitch(Player* player, BattlePokemon*& pokemon);

private:
	BattleContext& m_context;
	IBattleMenuUI& m_battleMenuUI;
};