#include <iostream>

#include "../data/Pokemon.h"
#include "../data/Move.h"
#include "../moves/MoveEffectEnums.h"
#include "../data/StringToTypes.h"

#include "BattleCalculations.h"

BattleCalculations::BattleCalculations(BattleContext& context, RandomEngine& rng) : m_context(context), m_rng(rng) {}

int BattleCalculations::CalculateCriticalHitStageModifier(size_t element)
{
	if (element > 3)
	{
		element = 3;
	}

	return m_arr_CriticalHitStageMultiplier[element];
}

double BattleCalculations::CalculateCriticalHit(BattlePokemon* source)
{
	int critModNumber = CalculateCriticalHitStageModifier(source->GetCriticalHitStage());

	std::uniform_int_distribution<int> critmoddistributor(0, 10000);

	double randomNumber{ static_cast<double>(critmoddistributor(m_rng.GetGenerator())) };

	if (randomNumber >= critModNumber)
	{
		m_context.flags.isCriticalHit = false;
		
		return 1;
	}
	else
	{
		m_context.flags.isCriticalHit = true;
		return 1.5;
	}
}

double BattleCalculations::CalculateStageModifier(size_t element)
{
	element += 6;
	return m_arr_StageMultiplier[element];
}

double BattleCalculations::CalculateTypeEffectiveness(BattlePokemon::pokemonMove* currentMove, BattlePokemon* target)
{
	size_t moveType = static_cast<size_t>(currentMove->mp_move->GetMoveTypeEnum());
	size_t defensiveTypeOne = static_cast<size_t>(target->GetTypeOneEnum());
	size_t defensiveTypeTwo = static_cast<size_t>(target->GetTypeTwoEnum());

	TypeEffect effect1 = typeEffectivenessChart[moveType][defensiveTypeOne];
	TypeEffect effect2 = (defensiveTypeTwo == 18) ? TypeEffect::Normal : typeEffectivenessChart[moveType][defensiveTypeTwo];

	double moveEffectiveness = MultiplyEffectiveness(effect1, effect2);

	using Effectiveness = BattleStateFlags::Effectiveness;

	if (moveEffectiveness == 0.0)
	{
		m_context.flags.currentEffectiveness = Effectiveness::No;
	}
	else if (moveEffectiveness > 0.0 && moveEffectiveness < 1.0)
	{
		m_context.flags.currentEffectiveness = Effectiveness::Less;
	}
	else if (moveEffectiveness > 1.0)
	{
		m_context.flags.currentEffectiveness = Effectiveness::Super;
	}
	else
	{
		m_context.flags.currentEffectiveness = Effectiveness::Normal;
	}

	if (currentMove->mp_move->GetMoveEffectEnum() == MoveEffect::OHKO && moveEffectiveness != 0.0)
	{
		m_context.flags.currentEffectiveness = Effectiveness::OHKO;
	}

	return moveEffectiveness;
}

double BattleCalculations::CalculateAccuracyModifiers(int element)
{
	element += 6;
	return m_arr_AccuracyStageMultiplier[element];
}

bool BattleCalculations::CalculateHitChance(BattlePokemon::pokemonMove* currentMove, BattlePokemon* source, BattlePokemon* target)
{
	if (
		(target->IsSemiInvulnerableFromFly() && (currentMove->mp_move->GetMoveEffectEnum() != MoveEffect::Gust && currentMove->mp_move->GetName() != "Thunder")) ||
		(target->IsSemiInvulnerableFromDig() && (currentMove->mp_move->GetMoveEffectEnum() != MoveEffect::Earthquake && currentMove->mp_move->GetName() != "Fissure"))
		)
	{
		return false;
	}

	int adjustedStages = (source->GetAccuracyStage() - target->GetEvasionStage());

	if (adjustedStages < -6)
	{
		adjustedStages = -6;
	}
	else if (adjustedStages > 6)
	{
		adjustedStages = 6;
	}

	double moveAccuracy = static_cast<double>(currentMove->mp_move->GetAccuracy());

	double accuracyMod{ 0 };

	if (currentMove->mp_move->GetMoveEffectEnum() != MoveEffect::OHKO)
	{
		accuracyMod = moveAccuracy * CalculateAccuracyModifiers(adjustedStages);
	}
	else
	{
		accuracyMod = (source->GetLevel() - target->GetLevel()) + 30; // for OHKO moves
	}

	if (accuracyMod >= 100.0)
	{
		return true;
	}

	else
	{
		std::uniform_real_distribution<double> randomModDistributor(0.0, 100.0);
		double roll{ randomModDistributor(m_rng.GetGenerator()) };
		
		return (roll < accuracyMod);
	}
}

void BattleCalculations::CalculateDamage(Player* targetPlayer, BattlePokemon::pokemonMove* currentMove, BattlePokemon* source, BattlePokemon* target)
{
	m_context.damageTaken = 0;

	double damage{ 0 };

	double effectiveness = CalculateTypeEffectiveness(currentMove, target);

	if (currentMove->mp_move->GetMoveEffectEnum() == MoveEffect::Struggle)
	{
		effectiveness = 1;
	}

	if ((currentMove->mp_move->GetMoveEffectEnum() == MoveEffect::OHKO) && effectiveness != 0)
	{
		damage = target->GetCurrentHP();
		target->DamageCurrentHP(static_cast<int>(damage));
		m_context.damageTaken = damage;
		return;
	}

	if (effectiveness == 0)
	{
		return;
	}

	double critical = CalculateCriticalHit(source);

	double sourceAttack{ 0.0 };
	double targetDefense{ 0.0 };

	double stagemultiplier{ 0.0 };

	if (currentMove->mp_move->GetCategoryEnum() == Category::Physical)
	{
		if (m_context.flags.isCriticalHit && (source->GetAttackStage() < 0))
		{
			stagemultiplier = CalculateStageModifier(0);
			sourceAttack = source->GetAttack() * stagemultiplier;
		}
		else
		{
			stagemultiplier = CalculateStageModifier(source->GetAttackStage());
			sourceAttack = source->GetAttack() * stagemultiplier;
		}

		if (m_context.flags.isCriticalHit && (source->GetDefenseStage() > 0))
		{
			stagemultiplier = CalculateStageModifier(0);
			targetDefense = target->GetDefense() * stagemultiplier;
		}
		else
		{
			stagemultiplier = CalculateStageModifier(target->GetDefenseStage());
			targetDefense = target->GetDefense() * stagemultiplier;
		}
	}

	else if (currentMove->mp_move->GetCategoryEnum() == Category::Special)
	{
		if (m_context.flags.isCriticalHit && (source->GetSpecialAttackStage() < 0))
		{
			stagemultiplier = CalculateStageModifier(0);
			sourceAttack = source->GetSpecialAttack() * stagemultiplier;
		}
		else
		{
			stagemultiplier = CalculateStageModifier(source->GetSpecialAttackStage());
			sourceAttack = source->GetSpecialAttack() * stagemultiplier;
		}

		if (m_context.flags.isCriticalHit && (source->GetSpecialDefenseStage() > 0))
		{
			stagemultiplier = CalculateStageModifier(0);
			targetDefense = target->GetSpecialDefense() * stagemultiplier;
		}
		else
		{
			stagemultiplier = CalculateStageModifier(target->GetSpecialDefenseStage());
			targetDefense = target->GetSpecialDefense() * stagemultiplier;
		}
	}

	double stab{};

	if ((currentMove->mp_move->GetMoveTypeEnum() == source->GetTypeOneEnum()) || (currentMove->mp_move->GetMoveTypeEnum() == source->GetTypeTwoEnum())
		&& currentMove->mp_move->GetMoveEffectEnum() != MoveEffect::Struggle)
	{
		stab = 1.5;
	}
	else
	{
		stab = 1;
	}

	double burn{ 1 };

	if (source->GetStatus() == Status::Burned)
	{
		burn = 0.5;
	}

	std::uniform_int_distribution<int> damagemoddistributor(85, 100);
	double damagemod{ static_cast<double>(damagemoddistributor(m_rng.GetGenerator())) };

	double random{ (damagemod / 100.0) };

	double powerModifier{ 1 };

	if (target->IsSemiInvulnerableFromFly() && (currentMove->mp_move->GetMoveEffectEnum() == MoveEffect::Gust))
	{
		powerModifier = 2;
	}

	int currentMovePower{ currentMove->mp_move->GetPower() };

	if (currentMove->mp_move->GetMoveEffectEnum() == MoveEffect::LowKick)
	{
		currentMovePower = CalculateLowKickPower(target);
	}

	damage = floor(floor(floor(floor(floor(floor(floor(floor(floor(2 * source->GetLevel() / 5 + 2) * (currentMovePower * powerModifier) * (static_cast<double>(sourceAttack) / static_cast<double>(targetDefense)) / 50) + 2) * critical) * random) * stab) * effectiveness) * burn));

	if ((currentMove->mp_move->GetMoveEffectEnum() == MoveEffect::Stomp || currentMove->mp_move->GetMoveEffectEnum() == MoveEffect::BodySlam) && target->HasUsedMinimize())
	{
		damage *= 2;
	}

	if (currentMove->mp_move->GetMoveEffectEnum() == MoveEffect::Earthquake && target->IsSemiInvulnerableFromDig())
	{
		damage *= 2;
	}

	if (targetPlayer->HasReflect() && !m_context.flags.isCriticalHit && currentMove->mp_move->GetCategoryEnum() == Category::Physical)
	{
		damage = floor(damage / 2);
	}

	if (targetPlayer->HasLightScreen() && !m_context.flags.isCriticalHit && currentMove->mp_move->GetCategoryEnum() == Category::Special)
	{
		damage = floor(damage / 2);
	}

	if (damage == 0 && effectiveness != 0)
	{
		damage = 1;
	}

	if (damage > target->GetCurrentHP())
	{
		damage = target->GetCurrentHP();
	}

	if (target->HasSubstitute() && !currentMove->mp_move->CanBypassSubstitute())
	{
		target->DamageSubstitute(static_cast<int>(damage));
	}

	else
	{
		target->DamageCurrentHP(static_cast<int>(damage));
	}

	bool isMultiStrike = currentMove->mp_move->GetMoveEffectEnum() == MoveEffect::MultiAttack ||
						 currentMove->mp_move->GetMoveEffectEnum() == MoveEffect::DoubleHit ||
						 currentMove->mp_move->GetMoveEffectEnum() == MoveEffect::Twineedle;

	if (target->IsBiding() && !isMultiStrike)
	{
		target->AddBideDamage(static_cast<int>(damage));
	}

	m_context.damageTaken = damage;

	std::cout << damage << " damage inflicted.\n";
}

// Calculate power of low kick based on target Pokemon's weight (in kg)
int BattleCalculations::CalculateLowKickPower(BattlePokemon* target)
{
	double pokemonWeight = target->GetPokemonDatabasePointer()->GetPokemonWeight();

	if (pokemonWeight >= 0.1 && pokemonWeight <= 9.9)
	{
		return 20;
	}

	if (pokemonWeight >= 10 && pokemonWeight <= 24.9)
	{
		return 40;
	}

	if (pokemonWeight >= 25 && pokemonWeight <= 49.9)
	{
		return 60;
	}

	if (pokemonWeight >= 50 && pokemonWeight <= 99.9)
	{
		return 80;
	}

	if (pokemonWeight >= 100 && pokemonWeight <= 199.9)
	{
		return 100;
	}

	if (pokemonWeight >= 200)
	{
		return 120;
	}

	return 0;
}