#pragma once

struct BattleContext;
class IMoveResultsUI;
class Player;
class BattlePokemon;

class SwitchExecutor
{
public:
	SwitchExecutor(BattleContext&, IMoveResultsUI&);

	void ExecuteSwitch(Player& player, BattlePokemon*& pokemon);

private:
	BattleContext& m_context;
	IMoveResultsUI& m_moveResultsUI;
};