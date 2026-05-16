#pragma once

class Player;
class BattlePokemon;
struct pokemonMove;
class BattleCalculations;
class RandomEngine;
struct ScoringResults;

namespace BasicScoring
{
	int BaseDamageScoring(ScoringResults&, const Player& self, const Player& targetPlayer, const pokemonMove& move, const BattlePokemon& selfMon, const BattlePokemon& targetMon);
	int BaseStatusScoring(ScoringResults&, const Player& self, const Player& targetPlayer, const pokemonMove& move, const BattlePokemon& selfMon, const BattlePokemon& targetMon);

	int CheckDamageImmunity(const pokemonMove& move, const Player& self, const BattlePokemon& targetMon);
	int CheckOHKOLevel(const BattlePokemon& selfMon, const BattlePokemon& targetMon);
	int CheckExplosionLastPokemon(const Player& self, const Player& targetPlayer);
	int DreamEaterSleepCheck(const BattlePokemon& targetMon);

	int SleepCheck(const pokemonMove& move, const BattlePokemon& targetMon);
	int PoisonCheck(const pokemonMove& move, const BattlePokemon& targetMon);
	int ParalyzeCheck(const pokemonMove& move, const BattlePokemon& targetMon);
	int ConfuseCheck(const BattlePokemon& targetMon);

	int SelfAttackBoostCheck(const BattlePokemon& selfMon);
	int SelfDefenseBoostCheck(const BattlePokemon& selfMon);
	int GrowthCheck(const BattlePokemon& selfMon);
	int SelfSpecialDefenseBoostCheck(const BattlePokemon& selfMon);
	int SelfSpeedBoostCheck(const BattlePokemon& selfMon);
	int SelfEvasionBoostCheck(const BattlePokemon& selfMon);

	int TargetAttackReductionCheck(const BattlePokemon& targetMon);
	int TargetDefenseReductionCheck(const BattlePokemon& targetMon);
	int TargetSpeedReductionCheck(const BattlePokemon& targetMon);
	int TargetAccuracyReductionCheck(const BattlePokemon& targetMon);

	int HazeCheck(const BattlePokemon& selfMon, const BattlePokemon targetMon);
	int ForceSwitchCheck(const Player& targetPlayer);
	int HealCheck(const BattlePokemon& selfMon);
	int RestCheck(const BattlePokemon& selfMon);
	int ReflectCheck(const Player& self);
	int LightScreenCheck(const Player& self);
	int MistCheck(const Player& self);
	int FocusEnergyCheck(const BattlePokemon& selfMon);
	int SubstituteCheck(const BattlePokemon& selfMon);
	int LeechSeedCheck(const BattlePokemon& targetMon);
	int DisableCheck(const BattlePokemon& targetMon);
}