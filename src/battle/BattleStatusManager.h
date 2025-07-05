#pragma once

#include "BattleContext.h"
#include "RandomEngine.h"

class BattleStatusManager
{
public:
	BattleStatusManager(BattleContext&, RandomEngine&);

	bool CheckPerformativeStatus(BattleContext&);
	bool SleepStatus(BattlePokemon*);
	bool FrozenStatus(BattlePokemon*);
	bool FlinchStatus(BattleContext&);
	bool ConfusedStatus(BattleContext&);
	bool ParalysisStatus(BattleContext&);

	void ThrashStop();
	void ThrashConfuse();
	void ThrashReset();

	void BideStop();
	void BideReset();

	void CheckSubstituteCondition(Player*, BattlePokemon*);

	void RageCheck(BattleContext&);

	void ResetPokemonTurnStatuses(BattleContext&);

	void CheckFaintCondition(Player* sourcePlayer, Player* targetPlayer, BattlePokemon* sourcePokemon, BattlePokemon* targetPokemon);

private:
	BattleContext& m_context;
	RandomEngine& m_rng;
};