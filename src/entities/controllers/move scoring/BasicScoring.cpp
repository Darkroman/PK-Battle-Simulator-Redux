#include <algorithm>

#include "BasicScoring.h"

#include "AIMoveClassifier.h"
#include "ScoringResultsStruct.h"
#include "../../../data/StringToTypes.h"
#include "../../Player.h"
#include "../AIController.h"

namespace BasicScoring
{
	int BaseDamageScoring(ScoringResults& results, const Player& self, const Player& targetPlayer, const pokemonMove& move, const BattlePokemon& selfMon, const BattlePokemon& targetMon)
	{
		int delta{};

		auto tags = results.tag;

		delta += CheckDamageImmunity(move, self, targetMon); // score -10 if immune

		switch (tags)
		{
		case AIScoreTag::OHKO:
			delta += CheckOHKOLevel(selfMon, targetMon); // score -10 if attacker level less than defender
			break;

		case AIScoreTag::SelfFaintingDamage:
			delta += CheckExplosionLastPokemon(self, targetPlayer); // if attacker on last pokemon and defender on last pokemon score -1, if defender not on last pokemon score -10
			break;

		case AIScoreTag::DreamEater:
			delta += DreamEaterSleepCheck(targetMon); // score -8 if defender is not sleeping
			break;
		}

		return delta;
	}

	int BaseStatusScoring(ScoringResults& results, const Player& self, const Player& targetPlayer, const pokemonMove& move, const BattlePokemon& selfMon, const BattlePokemon& targetMon)
	{
		int delta{};

		auto tags = results.tag;

		switch (tags)
		{
			// Status Effects
		case AIScoreTag::Sleep:
			delta += SleepCheck(move, targetMon); // if defender already has status, or move is sleep powder or spore and is grass type, score -10
			break;

		case AIScoreTag::Poison:
			delta += PoisonCheck(move, targetMon); // if defender already has status, is immune to poison, or is grass type and move is poison powder, score -10
			break;

		case AIScoreTag::Paralyze:
			delta += ParalyzeCheck(move, targetMon); // if defender already has status, is immune to paralysis, or is ground type and move is thunder wave, or is grass type and move is stun spore, score -10
			break;

		case AIScoreTag::Confuse:
			delta += ConfuseCheck(targetMon); // if defender is already confused, score -10
			break;

			// Stat Stage boosts
		case AIScoreTag::SelfAttackBoost:
			delta += SelfAttackBoostCheck(selfMon); // if attacker is +6 in attack, score -10
			break;

		case AIScoreTag::SelfDefenseBoost:
			delta += SelfDefenseBoostCheck(selfMon); // if attacker is +6 in defense, score -10
			break;

		case AIScoreTag::Growth:
			delta += GrowthCheck(selfMon); // if attacker is +6 in attack and +6 in special attack, score -10
			break;

		case AIScoreTag::SelfSpDBoost:
			delta += SelfSpecialDefenseBoostCheck(selfMon); // if attacker is +6 in special defense, score -10
			break;

		case AIScoreTag::SelfSpeedBoost:
			delta += SelfSpeedBoostCheck(selfMon); // if attacker is +6 in speed, score -10
			break;

		case AIScoreTag::SelfEvasionBoost:
			delta += SelfEvasionBoostCheck(selfMon); // if attacker is +6 in evasion, score -10
			break;

			// Stat Stage reductions
		case AIScoreTag::TargetAttackDecrease:
			delta += TargetAttackReductionCheck(targetMon); // if defender is -6 in attack, score -10
			break;

		case AIScoreTag::TargetDefenseDecrease:
			delta += TargetDefenseReductionCheck(targetMon); // if defender is -6 in defense, score -10
			break;

		case AIScoreTag::TargetSpeedDecrease:
			delta += TargetSpeedReductionCheck(targetMon); // if defender is -6 in speed, score -10
			break;

		case AIScoreTag::TargetAccuracyDecrease:
			delta += TargetAccuracyReductionCheck(targetMon); // if defender is -6 in accuracy, score -10
			break;

			// Other status moves
		case AIScoreTag::Haze:
			delta += HazeCheck(selfMon, targetMon); // if attacker has any stage greater than 0, or defender has any stage less than 0, score -10
			break;

		case AIScoreTag::ForceSwitch:
			delta += ForceSwitchCheck(targetPlayer); // if defender has only 1 pokemon left, score -10
			break;

		case AIScoreTag::Healing:
			delta += HealCheck(selfMon); // if attacker is full hp, score -8
			break;

		case AIScoreTag::Rest:
			delta += RestCheck(selfMon); // if attacker is at full hp, score -8
			break;

		case AIScoreTag::Reflect:
			delta += ReflectCheck(self); // if attacking player already has reflect, score -8
			break;

		case AIScoreTag::LightScreen:
			delta += LightScreenCheck(self); // if attacking player already has light screen, score -8
			break;

		case AIScoreTag::Mist:
			delta += MistCheck(self); // if attacking player already has mist, score -8
			break;

		case AIScoreTag::FocusEnergy:
			delta += FocusEnergyCheck(selfMon); // if attacker already has focus energy, score -10
			break;

		case AIScoreTag::Substitute:
			delta += SubstituteCheck(selfMon); // if attacker has substitute already score -8, or if attacker's hp percent is less than 26, score -10
			break;

		case AIScoreTag::Seed:
			delta += LeechSeedCheck(targetMon); // if defender is already seeded, or is grass type, score -10
			break;

		case AIScoreTag::Disable:
			delta += DisableCheck(targetMon); // if defender already has a move disabled, score -8
			break;
		}

		return delta;
	}

	int CheckDamageImmunity(const pokemonMove& move, const Player& self, const BattlePokemon& targetMon)
	{
		int effectiveness = self.GetAIController().AICalculateMoveTypeEffectiveness(move, targetMon);

		int delta{};

		if (effectiveness == 0)
		{
			delta -= 10;
		}

		return delta;
	}

	int CheckOHKOLevel(const BattlePokemon& selfMon, const BattlePokemon& targetMon)
	{
		int delta{};

		unsigned int attackerLevel{ selfMon.GetLevel() };
		unsigned int defenderLevel{ targetMon.GetLevel() };

		if (attackerLevel < defenderLevel)
		{
			delta -= 10;
		}

		return delta;
	}

	int CheckExplosionLastPokemon(const Player& self, const Player& targetPlayer)
	{
		int delta{};

		if (self.GetPokemonLeft() > 1)
		{
			return delta;
		}

		if (targetPlayer.GetPokemonLeft() > 1)
		{
			delta -= 10;
		}
		else
		{
			delta -= 1;
		}

		return delta;
	}

	int DreamEaterSleepCheck(const BattlePokemon& targetMon)
	{
		int delta{};

		if (targetMon.GetStatus() != Status::Sleeping)
		{
			delta -= 8;
		}

		return delta;
	}

	int SleepCheck(const pokemonMove& move, const BattlePokemon& targetMon)
	{
		int delta{};

		bool isGrassType = targetMon.GetTypeOneEnum() == PokemonType::Grass || targetMon.GetTypeTwoEnum() == PokemonType::Grass;
		bool isPowderImmune = isGrassType && move.GetMoveEffectEnum() == MoveEffect::SleepPowder;

		if (targetMon.GetStatus() != Status::Normal || isPowderImmune)
		{
			delta -= 10;
		}

		return delta;
	}

	int PoisonCheck(const pokemonMove& move, const BattlePokemon& targetMon)
	{
		int delta{};

		bool typeIsImmune = targetMon.GetTypeOneEnum() == PokemonType::Poison
			|| targetMon.GetTypeTwoEnum() == PokemonType::Poison
			|| targetMon.GetTypeOneEnum() == PokemonType::Steel
			|| targetMon.GetTypeTwoEnum() == PokemonType::Steel;

		bool isGrassType = targetMon.GetTypeOneEnum() == PokemonType::Grass || targetMon.GetTypeTwoEnum() == PokemonType::Grass;
		bool isPowderImmune = isGrassType && move.GetMoveEffectEnum() == MoveEffect::PoisonPowder;

		if (targetMon.GetStatus() != Status::Normal || typeIsImmune || isPowderImmune)
		{
			delta -= 10;
		}

		return delta;
	}

	int ParalyzeCheck(const pokemonMove& move, const BattlePokemon& targetMon)
	{
		int delta{};

		bool typeIsImmune = targetMon.GetTypeOneEnum() == PokemonType::Electric
			|| targetMon.GetTypeTwoEnum() == PokemonType::Electric;

		bool isThunderWaveImmune = (targetMon.GetTypeOneEnum() == PokemonType::Ground
			|| targetMon.GetTypeTwoEnum() == PokemonType::Ground)
			&& move.GetName() == "Thunder Wave";

		bool isGrassType = targetMon.GetTypeOneEnum() == PokemonType::Grass || targetMon.GetTypeTwoEnum() == PokemonType::Grass;
		bool isPowderImmune = isGrassType && move.GetMoveEffectEnum() == MoveEffect::PoisonPowder;

		if (targetMon.GetStatus() != Status::Normal || typeIsImmune || isThunderWaveImmune || isPowderImmune)
		{
			delta -= 10;
		}

		return delta;
	}

	int ConfuseCheck(const BattlePokemon& targetMon)
	{
		int delta{};

		if (targetMon.IsConfused())
		{
			delta -= 5;
		}

		return delta;
	}

	int SelfAttackBoostCheck(const BattlePokemon& selfMon)
	{
		int delta{};

		if (selfMon.GetAttackStage() == 12)
		{
			delta -= 10;
		}

		return delta;
	}

	int SelfDefenseBoostCheck(const BattlePokemon& selfMon)
	{
		int delta{};

		if (selfMon.GetDefenseStage() == 12)
		{
			delta -= 10;
		}

		return delta;
	}

	int GrowthCheck(const BattlePokemon& selfMon)
	{
		int delta{};

		if (selfMon.GetAttackStage() == 12 && selfMon.GetSpecialAttackStage() == 12)
		{
			delta -= 10;
		}

		return delta;
	}

	int SelfSpecialDefenseBoostCheck(const BattlePokemon& selfMon)
	{
		int delta{};

		if (selfMon.GetSpecialDefenseStage() == 12)
		{
			delta -= 10;
		}

		return delta;
	}

	int SelfSpeedBoostCheck(const BattlePokemon& selfMon)
	{
		int delta{};

		if (selfMon.GetSpeedStage() == 12)
		{
			delta -= 10;
		}

		return delta;
	}

	int SelfEvasionBoostCheck(const BattlePokemon& selfMon)
	{
		int delta{};

		if (selfMon.GetEvasionStage() == 12)
		{
			delta -= 10;
		}

		return delta;
	}

	int TargetAttackReductionCheck(const BattlePokemon& targetMon)
	{
		int delta{};

		if (targetMon.GetAttackStage() == 0)
		{
			delta -= 10;
		}

		return delta;
	}

	int TargetDefenseReductionCheck(const BattlePokemon& targetMon)
	{
		int delta{};

		if (targetMon.GetDefenseStage() == 0)
		{
			delta -= 10;
		}

		return delta;
	}

	int TargetSpeedReductionCheck(const BattlePokemon& targetMon)
	{
		int delta{};

		if (targetMon.GetSpeedStage() == 0)
		{
			delta -= 10;
		}

		return delta;
	}

	int TargetAccuracyReductionCheck(const BattlePokemon& targetMon)
	{
		int delta{};

		if (targetMon.GetAccuracyStage() == 0)
		{
			delta -= 10;
		}

		return delta;
	}

	int HazeCheck(const BattlePokemon& selfMon, const BattlePokemon targetMon)
	{
		int delta{};

		auto selfStats = { selfMon.GetAttackStage(), selfMon.GetDefenseStage(), selfMon.GetSpecialAttackStage(), selfMon.GetSpecialDefenseStage(), selfMon.GetSpeedStage(), selfMon.GetAccuracyStage(), selfMon.GetEvasionStage() };
		bool selfHasBuff = std::any_of(selfStats.begin(), selfStats.end(), [](size_t s) { return s > 6; });

		auto targetStats = { targetMon.GetAttackStage(), targetMon.GetDefenseStage(), targetMon.GetSpecialAttackStage(), targetMon.GetSpecialDefenseStage(), targetMon.GetSpeedStage(), targetMon.GetAccuracyStage(), targetMon.GetEvasionStage() };
		bool targetHasDebuff = std::any_of(targetStats.begin(), targetStats.end(), [](size_t s) { return s < 6; });

		if (selfHasBuff || targetHasDebuff)
		{
			delta -= 10;
		}

		return delta;
	}

	int ForceSwitchCheck(const Player& targetPlayer)
	{
		int delta{};

		if (targetPlayer.GetPokemonLeft() < 2)
		{
			delta -= 10;
		}

		return delta;
	}

	int HealCheck(const BattlePokemon& selfMon)
	{
		int delta{};

		if (selfMon.GetCurrentHP() == selfMon.GetMaxHP())
		{
			delta -= 8;
		}

		return delta;
	}

	int RestCheck(const BattlePokemon& selfMon)
	{
		int delta{};

		if (selfMon.GetCurrentHP() == selfMon.GetMaxHP())
		{
			delta -= 8;
		}

		return delta;
	}

	int ReflectCheck(const Player& self)
	{
		int delta{};

		if (self.HasReflect())
		{
			delta -= 8;
		}

		return delta;
	}

	int LightScreenCheck(const Player& self)
	{
		int delta{};

		if (self.HasLightScreen())
		{
			delta -= 8;
		}

		return delta;
	}

	int MistCheck(const Player& self)
	{
		int delta{};

		if (self.HasMist())
		{
			delta -= 8;
		}

		return delta;
	}

	int FocusEnergyCheck(const BattlePokemon& selfMon)
	{
		int delta{};

		if (selfMon.HasFocusEnergy())
		{
			delta -= 10;
		}

		return delta;
	}

	int SubstituteCheck(const BattlePokemon& selfMon)
	{
		int delta{};

		if (selfMon.HasSubstitute())
		{
			delta -= 8;
		}

		unsigned int hpPercent{ selfMon.GetCurrentHP() * 100 / selfMon.GetMaxHP() };

		if (hpPercent < 26)
		{
			delta -= 10;
		}

		return delta;
	}

	int LeechSeedCheck(const BattlePokemon& targetMon)
	{
		int delta{};

		bool typeIsImmune = targetMon.GetTypeOneEnum() == PokemonType::Grass || targetMon.GetTypeTwoEnum() == PokemonType::Grass;

		if (targetMon.IsSeeded() || typeIsImmune)
		{
			delta -= 10;
		}

		return delta;
	}

	int DisableCheck(const BattlePokemon& targetMon)
	{
		int delta{};

		if (targetMon.MoveIsDisabled())
		{
			delta -= 8;
		}

		return delta;
	}
}