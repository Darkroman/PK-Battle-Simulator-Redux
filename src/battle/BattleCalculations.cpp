#include "../ui/interfaces/IMoveResultsUI.h"
#include "../data/Pokemon.h"
#include "../moves/MoveEffectEnums.h"
#include "../data/StringToTypes.h"
#include "../battle/RandomEngine.h"
#include "../battle/BattleContext.h"

#include "BattleCalculations.h"

#include <iostream>

BattleCalculations::BattleCalculations(BattleContext& context, RandomEngine& rng, IMoveResultsUI& resultsUI) : m_context(context), m_rng(rng), m_resultsUI(resultsUI) {}

void BattleCalculations::CalculateCriticalHit(BattlePokemon* source)
{
	size_t stage = source->GetCriticalHitStage();
	if (stage > 3) stage = 3;

	int threshold = m_arr_CriticalHitStageThresholds[stage];

	std::uniform_int_distribution<int> critmoddistributor(0, 23);
	int roll{ critmoddistributor(m_rng.GetGenerator()) };

	m_context.flags.isCriticalHit = (roll < threshold);
}

std::pair<int, int> BattleCalculations::GetStageRatio(int stage)
{
	stage = std::clamp(stage, -6, 6);
	return m_arr_StageRatio[static_cast<size_t>(stage + 6)];
}

int BattleCalculations::CalculateTypeEffectiveness(BattlePokemon::pokemonMove* currentMove, BattlePokemon* target)
{
	size_t moveType = static_cast<size_t>(currentMove->GetMoveTypeEnum());
	size_t defensiveTypeOne = static_cast<size_t>(target->GetTypeOneEnum());
	size_t defensiveTypeTwo = static_cast<size_t>(target->GetTypeTwoEnum());

	uint16_t effect1 = typeChart[moveType][defensiveTypeOne];
	uint16_t effect2 = (defensiveTypeTwo == 18) ? 4096 : typeChart[moveType][defensiveTypeTwo];

	int moveEffectiveness = MultiplyEffectiveness(effect1, effect2);

	using Effectiveness = BattleStateFlags::Effectiveness;

	if (moveEffectiveness == 0)
	{
		m_context.flags.currentEffectiveness = Effectiveness::No;
	}
	else if (moveEffectiveness > 0 && moveEffectiveness < 4096)
	{
		m_context.flags.currentEffectiveness = Effectiveness::Less;
	}
	else if (moveEffectiveness > 4096)
	{
		m_context.flags.currentEffectiveness = Effectiveness::Super;
	}
	else
	{
		m_context.flags.currentEffectiveness = Effectiveness::Normal;
	}

	if (currentMove->GetMoveEffectEnum() == MoveEffect::OHKO && moveEffectiveness != 0)
	{
		m_context.flags.currentEffectiveness = Effectiveness::OHKO;
	}

	return moveEffectiveness;
}

bool BattleCalculations::CalculateHitChance(BattlePokemon::pokemonMove* currentMove, BattlePokemon* source, BattlePokemon* target)
{
	if (
		(target->IsSemiInvulnerableFromFly() && (currentMove->GetMoveEffectEnum() != MoveEffect::Gust && currentMove->GetName() != "Thunder")) ||
		(target->IsSemiInvulnerableFromDig() && (currentMove->GetMoveEffectEnum() != MoveEffect::Earthquake && currentMove->GetName() != "Fissure"))
		)
	{
		return false;
	}

	int adjustedStages = (source->GetAccuracyStage() - target->GetEvasionStage());

	adjustedStages = std::clamp(adjustedStages, -6, 6);
	const auto& [numerator, denominator] = m_arr_accuracyStageRatio[static_cast<size_t>(adjustedStages + 6)];

	int moveAccuracy = currentMove->GetAccuracy();

	int accuracyMod{};

	if (currentMove->GetMoveEffectEnum() != MoveEffect::OHKO)
	{
		accuracyMod = (moveAccuracy * numerator) / denominator;

	}
	else
	{
		accuracyMod = (source->GetLevel() - target->GetLevel()) + 30; // for OHKO moves
	}

	if (accuracyMod >= 100)
	{
		return true;
	}

	else
	{
		std::uniform_int_distribution<int> roll(0, 99);
		int rollOutcome{ roll(m_rng.GetGenerator()) };

		return rollOutcome < accuracyMod;
	}
}

void BattleCalculations::CalculateDamage(Player* targetPlayer, BattlePokemon::pokemonMove* currentMove, BattlePokemon* source, BattlePokemon* target)
{
	m_context.flags.hitSubstitute = false;

	m_context.damageTaken = 0;

	int baseDamage{ 0 };

	int effectiveness = CalculateTypeEffectiveness(currentMove, target);

	if (currentMove->GetMoveEffectEnum() == MoveEffect::Struggle)
	{
		effectiveness = 4096;
	}

	if ((currentMove->GetMoveEffectEnum() == MoveEffect::OHKO) && effectiveness != 0)
	{
		baseDamage = target->GetCurrentHP();
		target->DamageCurrentHP(baseDamage);
		m_context.damageTaken = baseDamage;
		return;
	}

	if (effectiveness == 0)
	{
		return;
	}

	int sourceAttack{ 0 };
	int targetDefense{ 0 };

	if (currentMove->GetCategoryEnum() == Category::Physical)
	{
		if (m_context.flags.isCriticalHit && (source->GetAttackStage() < 0))
		{
			auto [numerator, denominator] = GetStageRatio(0);
			sourceAttack = source->GetAttack() * numerator / denominator;
		}
		else
		{
			auto [numerator, denominator] = GetStageRatio(source->GetAttackStage());
			sourceAttack = source->GetAttack() * numerator / denominator;
		}

		if (m_context.flags.isCriticalHit && (target->GetDefenseStage() > 0))
		{
			auto [numerator, denominator] = GetStageRatio(0);
			targetDefense = target->GetDefense() * numerator / denominator;
		}
		else
		{
			auto [numerator, denominator] = GetStageRatio(target->GetDefenseStage());
			targetDefense = target->GetDefense() * numerator / denominator;
		}
	}

	else if (currentMove->GetCategoryEnum() == Category::Special)
	{
		if (m_context.flags.isCriticalHit && (source->GetSpecialAttackStage() < 0))
		{
			auto [numerator, denominator] = GetStageRatio(0);
			sourceAttack = source->GetSpecialAttack() * numerator / denominator;
		}
		else
		{
			auto [numerator, denominator] = GetStageRatio(source->GetSpecialAttackStage());
			sourceAttack = source->GetSpecialAttack() * numerator / denominator;
		}

		if (m_context.flags.isCriticalHit && (target->GetSpecialDefenseStage() > 0))
		{
			auto [numerator, denominator] = GetStageRatio(0);
			targetDefense = target->GetSpecialDefense() * numerator / denominator;
		}
		else
		{
			auto [numerator, denominator] = GetStageRatio(target->GetSpecialDefenseStage());
			targetDefense = target->GetSpecialDefense() * numerator / denominator;
		}
	}

	int currentMovePower{ currentMove->GetPower() };

	if (currentMove->GetMoveEffectEnum() == MoveEffect::LowKick)
	{
		currentMovePower = CalculateLowKickPower(target);
	}

	int powerModifier = m_context.initialPowerMultiplier;

	if (powerModifier > 10)
	{
		currentMovePower = currentMovePower * powerModifier / 10;
	}

	int level = source->GetLevel();

	baseDamage = (((((2 * level / 5) + 2) * currentMovePower * sourceAttack) / targetDefense) / 50) + 2;

	CalculateCriticalHit(source);

	if (m_context.flags.isCriticalHit)
	{
		baseDamage = (baseDamage * 6144) >> 12;
	}

	// From my research it seems the random damage roll is handled like this in the actual games Gen 5+
	std::uniform_int_distribution<int> dist(0, 255);
	int roll = dist(m_rng.GetGenerator());
	int randPercent = 100 - (roll % 16);
	int finalDamage = (baseDamage * randPercent) / 100;

	bool hasStab = (currentMove->GetMoveTypeEnum() == source->GetTypeOneEnum() ||
		currentMove->GetMoveTypeEnum() == source->GetTypeTwoEnum())
		&& currentMove->GetMoveEffectEnum() != MoveEffect::Struggle;

	if (hasStab)
	{
		finalDamage = (finalDamage * 6144) >> 12;
	}

	finalDamage = (finalDamage * effectiveness) >> 12;

	if (source->GetStatus() == Status::Burned && currentMove->GetCategoryEnum() == Category::Physical)
	{
		finalDamage = (finalDamage * 2048) >> 12;
	}

	if ((currentMove->GetMoveEffectEnum() == MoveEffect::Stomp || currentMove->GetMoveEffectEnum() == MoveEffect::BodySlam) && target->HasUsedMinimize())
	{
		finalDamage *= 2;
	}

	if (currentMove->GetMoveEffectEnum() == MoveEffect::Earthquake && target->IsSemiInvulnerableFromDig())
	{
		finalDamage *= 2;
	}

	if (targetPlayer->HasReflect() && !m_context.flags.isCriticalHit && currentMove->GetCategoryEnum() == Category::Physical)
	{
		finalDamage = (finalDamage * 2048) >> 12;
	}

	if (targetPlayer->HasLightScreen() && !m_context.flags.isCriticalHit && currentMove->GetCategoryEnum() == Category::Special)
	{
		finalDamage = (finalDamage * 2048) >> 12;
	}

	if (finalDamage < 1 && effectiveness != 0)
	{
		finalDamage = 1;
	}

	if (finalDamage > target->GetCurrentHP())
	{
		finalDamage = target->GetCurrentHP();
	}

	if (target->HasSubstitute() && !currentMove->CanBypassSubstitute())
	{
		target->DamageSubstitute(finalDamage);
		m_context.flags.hitSubstitute = true;
	}

	else
	{
		target->DamageCurrentHP(finalDamage);
		m_context.flags.hitSubstitute = false;
	}

	bool isMultiStrike = currentMove->GetMoveEffectEnum() == MoveEffect::MultiAttack ||
						 currentMove->GetMoveEffectEnum() == MoveEffect::DoubleHit ||
						 currentMove->GetMoveEffectEnum() == MoveEffect::Twineedle;

	if (target->IsBiding() && !isMultiStrike && !target->HasSubstitute())
	{
		target->AddBideDamage(finalDamage);
	}

	m_context.damageTaken = finalDamage;

	m_resultsUI.DisplayDirectDamageInflictedMsg(finalDamage);
}

// Calculate power of low kick based on target Pokemon's weight (in hectograms)
int BattleCalculations::CalculateLowKickPower(BattlePokemon* target)
{
	int pokemonWeight = target->GetPokemonDatabasePointer()->GetPokemonWeightHg();

	if (pokemonWeight <= 0)
	{
		return 0;
	}

	if (pokemonWeight < 100)
	{
		return 20;
	}

	if (pokemonWeight < 250)
	{
		return 40;
	}

	if (pokemonWeight < 500)
	{
		return 60;
	}

	if (pokemonWeight < 1000)
	{
		return 80;
	}

	if (pokemonWeight < 2000)
	{
		return 100;
	}

	return 120;
}