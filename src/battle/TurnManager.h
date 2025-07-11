#pragma once

struct BattleContext;
class RandomEngine;
class IBattleMenuUI;
class IMoveResultsUI;
class IStatusEffectUI;

#include "BattleCalculations.h"
#include "BattleStatusManager.h"
#include "TurnUtils.h"
#include "WinChecker.h"
#include "MoveExecutor.h"
#include "TurnProcessor.h"
#include "PostTurnEffectProcessor.h"

class TurnManager
{
public:
	TurnManager(BattleContext& context, RandomEngine& rng, IBattleMenuUI& battleMenuUI, IMoveResultsUI& moveResultsUI, IStatusEffectUI& statusEffectUI);

	bool RunBattleLoop();
	void ResetValues();

private:
	BattleContext& m_context;
	RandomEngine& m_rng;
	IBattleMenuUI& m_battleMenuUI;
	IMoveResultsUI& m_moveResultsUI;
	IStatusEffectUI& m_statusEffectUI;

	BattleCalculations m_calculations;
	TurnUtils m_turnUtils;
	WinChecker m_winChecker;
	BattleStatusManager m_statusManager;
	MoveExecutor m_moveExecutor;
	TurnProcessor m_turnProcessor;
	PostTurnEffectProcessor m_postTurnProcessor;
};