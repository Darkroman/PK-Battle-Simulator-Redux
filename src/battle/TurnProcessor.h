#pragma once

struct BattleContext;
class BattleCalculations;
class RandomEngine;
class StatusEffectProcessor;
class WinChecker;
class SwitchExecutor;
class MoveExecutor;

class Player;

class TurnProcessor
{
public:
	TurnProcessor(BattleContext&, BattleCalculations&, RandomEngine&, StatusEffectProcessor&, WinChecker&, SwitchExecutor&, MoveExecutor&);

	void DetermineWhoGoesFirst();
	void SetFirst(Player&, Player&);
	void ExecuteTurn(bool&);
	void SwapRoles();

private:
	BattleContext& m_context;
	BattleCalculations& m_calculations;
	RandomEngine& m_rng;
	StatusEffectProcessor& m_statusProcessor;
	WinChecker& m_winChecker;
	SwitchExecutor& m_switchExecutor;
	MoveExecutor& m_moveExecutor;
};