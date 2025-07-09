#pragma once

struct BattleContext;
class RandomEngine;
class IBattleMenuUI;
class IMoveResultsUI;
class IStatusEffectUI;

#include "BattleCalculations.h"
#include "BattleStatusManager.h"
#include "TurnProcessor.h"
#include "PostTurnEffectProcessor.h"
#include "WinChecker.h"

class TurnManager
{
public:
	TurnManager(BattleContext&, RandomEngine&, IStatusEffectUI&, IBattleMenuUI&, IMoveResultsUI&);

	bool RunBattleLoop();
	void ResetValues();

private:
	BattleContext& m_context;
	RandomEngine& m_rng;
	IBattleMenuUI& m_battleMenuUI;
	IMoveResultsUI& m_moveResultsUI;
	IStatusEffectUI& m_statusEffectUI;
	BattleStatusManager m_statusManager;
	WinChecker m_winChecker;
	BattleCalculations m_calculations;
	TurnProcessor m_turnProcessor;
	PostTurnEffectProcessor m_postTurnProcessor;
};