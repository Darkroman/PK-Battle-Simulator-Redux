#pragma once

struct BattleContext;
class BattleCalculations;
class RandomEngine;
class IMoveResultsUI;
class IStatusEffectUI;

#include "BattleStatusManager.h"

class IBattleMenuUI;

#include "WinChecker.h"

class TurnProcessor
{
public:
	TurnProcessor(BattleContext&, BattleCalculations&, RandomEngine&, IStatusEffectUI&, BattleStatusManager&, IBattleMenuUI&, IMoveResultsUI&, WinChecker&);

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
	IStatusEffectUI& m_statusEffectUI;
	BattleStatusManager& m_statusManager;
	IBattleMenuUI& m_battleMenuUI;
	IMoveResultsUI& m_resultsUI;
	WinChecker& m_winChecker;

};