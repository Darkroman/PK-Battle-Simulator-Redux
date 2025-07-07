#pragma once

struct BattleContext;
class RandomEngine;
class IBattleMenuUI;
struct IMoveResultsUI;

#include "BattleCalculations.h"
#include "BattleStatusManager.h"
#include "TurnProcessor.h"
#include "PostTurnEffectProcessor.h"
#include "WinChecker.h"

class TurnManager
{
public:
	TurnManager(BattleContext&, RandomEngine&, IBattleMenuUI&, IMoveResultsUI&);

	bool RunBattleLoop();
	void ResetValues();

private:
	BattleContext& m_context;
	RandomEngine& m_rng;
	IBattleMenuUI& m_battleMenuUI;
	BattleCalculations m_calculations;
	BattleStatusManager m_statusManager;
	TurnProcessor m_turnProcessor;
	WinChecker m_winChecker;
	PostTurnEffectProcessor m_postTurnProcessor;
	IMoveResultsUI& m_moveResultsUI;
};