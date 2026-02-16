#pragma once

struct BattleContext;
class BattleCalculations;
class RandomEngine;
class StatusEffectProcessor;
class WinChecker;
class SwitchExecutor;
class MoveExecutor;

class BattlePokemon;
class Player;

class TurnProcessor
{
public:
	TurnProcessor(BattleContext&, BattleCalculations&, StatusEffectProcessor&, WinChecker&, SwitchExecutor&, MoveExecutor&);

	void DetermineTurnOrder();
	void ExecuteTurn(bool&);
	void SwapRoles();

private:
	BattleContext& m_context;
	BattleCalculations& m_calculations;
	StatusEffectProcessor& m_statusProcessor;
	WinChecker& m_winChecker;
	SwitchExecutor& m_switchExecutor;
	MoveExecutor& m_moveExecutor;
};