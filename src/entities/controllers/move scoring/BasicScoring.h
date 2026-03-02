#pragma once

class Player;
class BattlePokemon;
struct pokemonMove;
class BattleCalculations;
class RandomEngine;

namespace BasicScoring
{
	int BaseDamageScoring(Player& self, Player& targetPlayer, pokemonMove& move, BattlePokemon& selfMon, BattlePokemon& targetMon);
	int BaseStatusScoring(Player& self, Player& targetPlayer, pokemonMove& move, BattlePokemon& selfMon, BattlePokemon& targetMon);

	int CheckDamageImmunity(pokemonMove& move, Player& self, BattlePokemon& targetMon);
	int CheckOHKOLevel(BattlePokemon& selfMon, BattlePokemon& targetMon);
	int CheckExplosionLastPokemon(Player& self, Player& targetPlayer);
	int DreamEaterSleepCheck(BattlePokemon& targetMon);

	int SleepCheck(pokemonMove& move, BattlePokemon& targetMon);
	int PoisonCheck(pokemonMove& move, BattlePokemon& targetMon);
	int ParalyzeCheck(pokemonMove& move, BattlePokemon& targetMon);
	int ConfuseCheck(BattlePokemon& targetMon);

	int SelfAttackBoostCheck(BattlePokemon& selfMon);
	int SelfDefenseBoostCheck(BattlePokemon& selfMon);
	int GrowthCheck(BattlePokemon& selfMon);
	int SelfSpecialDefenseBoostCheck(BattlePokemon& selfMon);
	int SelfSpeedBoostCheck(BattlePokemon& selfMon);
	int SelfEvasionBoostCheck(BattlePokemon& selfMon);

	int TargetAttackReductionCheck(BattlePokemon& targetMon);
	int TargetDefenseReductionCheck(BattlePokemon& targetMon);
	int TargetSpeedReductionCheck(BattlePokemon& targetMon);
	int TargetAccuracyReductionCheck(BattlePokemon& targetMon);

	int HazeCheck(BattlePokemon& selfMon, BattlePokemon targetMon);
	int ForceSwitchCheck(Player& targetPlayer);
	int HealCheck(BattlePokemon& selfMon);
	int ReflectCheck(Player& self);
	int LightScreenCheck(Player& self);
	int MistCheck(Player& self);
	int FocusEnergyCheck(BattlePokemon& selfMon);
	int SubstituteCheck(BattlePokemon& selfMon);
	int LeechSeedCheck(BattlePokemon& targetMon);
	int DisableCheck(BattlePokemon& targetMon);
}