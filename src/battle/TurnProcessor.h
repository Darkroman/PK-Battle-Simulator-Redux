#pragma once

struct BattleContext;
class BattleCalculations;
class RandomEngine;
class IMoveResultsUI;
class IStatusEffectUI;
class TurnUtils;
class MoveExecutor;

#include "BattleStatusManager.h"

class IBattleMenuUI;

#include "WinChecker.h"

class TurnProcessor
{
public:
	TurnProcessor(BattleContext&, BattleCalculations&, RandomEngine&, BattleStatusManager&, WinChecker&, TurnUtils&, MoveExecutor&);

	void DetermineWhoGoesFirst();
	void DeterminePostTurnDamageOrder();
	void ExecuteTurn(bool&);
	void SwapRoles();
	void SetFirst(Player*, Player*);

private:
	BattleContext& m_context;
	BattleCalculations& m_calculations;
	RandomEngine& m_rng;
	BattleStatusManager& m_statusManager;
	WinChecker& m_winChecker;
	TurnUtils& m_turnUtils;
	MoveExecutor& m_moveExecutor;
};