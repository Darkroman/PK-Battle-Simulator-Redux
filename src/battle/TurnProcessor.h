#pragma once

struct BattleContext;
class BattleCalculations;
class RandomEngine;
struct IMoveResultsUI;

#include "BattleStatusManager.h"

class IBattleMenuUI;

#include "WinChecker.h"

class TurnProcessor
{
public:
	TurnProcessor(BattleContext&, BattleCalculations&, RandomEngine&, IBattleMenuUI&, IMoveResultsUI&);

	void DetermineWhoGoesFirst();
	void DeterminePostTurnDamageOrder();
	void ExecuteTurn(bool&);
	void SwapRoles();
	void SetFirst(Player*, Player*);

	void PerformSwitch(Player*, BattlePokemon*&);

private:
	BattleContext& m_context;
	BattleCalculations& m_calculations;
	RandomEngine& m_rng;
	BattleStatusManager m_statusManager;
	IBattleMenuUI& m_battleMenuUI;
	WinChecker m_winChecker;
	IMoveResultsUI& m_resultsUI;
};