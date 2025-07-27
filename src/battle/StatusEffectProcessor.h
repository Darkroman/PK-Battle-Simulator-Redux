#pragma once

struct BattleContext;
class RandomEngine;
class IStatusEffectUI;
class Player;
class BattlePokemon;

class StatusEffectProcessor
{
public:
	StatusEffectProcessor(BattleContext&, RandomEngine&, IStatusEffectUI&);

	bool CheckPerformativeStatus();
	bool SleepStatus();
	bool FrozenStatus();
	bool FlinchStatus();
	bool ConfusedStatus();
	bool ParalysisStatus();

	void ThrashStop();
	void ThrashConfuse();
	void ThrashReset();

	void BideStop();
	void BideReset();

	void CheckSubstituteCondition(Player*, BattlePokemon*);

	void RageCheck();

	void ResetPokemonTurnStatuses();

	void CheckFaintCondition(Player* sourcePlayer, Player* targetPlayer, BattlePokemon* sourcePokemon, BattlePokemon* targetPokemon);

private:
	BattleContext& m_context;
	RandomEngine& m_rng;
	IStatusEffectUI& m_statusEffectUI;
};