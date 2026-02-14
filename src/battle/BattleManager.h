#pragma once

struct BattleContext;
class RandomEngine;
class IBattleAnnouncerUI;
class IMoveResultsUI;
class IStatusEffectUI;

#include "BattleCalculations.h"
#include "SwitchExecutor.h"
#include "WinChecker.h"
#include "StatusEffectProcessor.h"
#include "MoveExecutor.h"
#include "TurnProcessor.h"
#include "PostTurnEffectProcessor.h"

class BattleManager
{
public:
	BattleManager(BattleContext& context, RandomEngine& rng, IBattleAnnouncerUI& battleAnnouncerUI, IMoveResultsUI& moveResultsUI, IStatusEffectUI& statusEffectUI);

	void AssignFirstPokemon();
	void ApplyPlayerOneAction();
	void ApplyPlayerTwoAction();
	bool RunBattleLoop();
	void ResetValues();

private:
	BattleContext& m_context;
	RandomEngine& m_rng;
	IBattleAnnouncerUI& m_battleAnnouncerUI;
	IMoveResultsUI& m_moveResultsUI;
	IStatusEffectUI& m_statusEffectUI;

	BattleCalculations m_calculations;
	SwitchExecutor m_switchExecutor;
	WinChecker m_winChecker;
	StatusEffectProcessor m_statusEffectProcessor;
	MoveExecutor m_moveExecutor;
	TurnProcessor m_turnProcessor;
	PostTurnEffectProcessor m_postTurnProcessor;

	BattleAction e_battleAction{};
};