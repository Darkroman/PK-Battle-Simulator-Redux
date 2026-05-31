#include "BattleCalculations.h"

#include "../data/Pokemon.h"
#include "../moves/MoveEffectEnums.h"
#include "../data/StringToTypes.h"
#include "RandomEngine.h"
#include "BattleContext.h"
#include "Typechart.h"
#include "StageRatios.h"
#include "../entities/Player.h"

BattleCalculations::BattleCalculations(BattleContext& context, RandomEngine& rng) : m_context(context), m_rng(rng) {}

unsigned int BattleCalculations::CalculatePokemonSpeed(const BattlePokemon& pokemon)
{
	auto [numerator, denominator] = GetStageRatio(pokemon.GetSpeedStage());

	unsigned int speed = pokemon.GetSpeed() * numerator / denominator;

	if (pokemon.GetStatus() == Status::Paralyzed)
	{
		speed /= 2;
	}

	return speed;
}

void BattleCalculations::RandomizeTurnOrder()
{
	std::array<Player*, 2> activePlayers{ m_context.playerOne, m_context.playerTwo };
	std::shuffle(activePlayers.begin(), activePlayers.end(), m_rng.GetGenerator());

	SetFirst(*activePlayers[0], *activePlayers[1]);
}

void BattleCalculations::RandomizePostTurnOrder()
{
	std::vector<BattlePokemon*> activePokemon{ m_context.attackingPokemon, m_context.defendingPokemon };
	std::shuffle(activePokemon.begin(), activePokemon.end(), m_rng.GetGenerator());

	if (activePokemon[0] != m_context.attackingPokemon) {
		std::swap(m_context.attackingPlayer, m_context.defendingPlayer);
		std::swap(m_context.attackingPokemon, m_context.defendingPokemon);
	}
}

void BattleCalculations::SetFirst(Player& first, Player& second)
{
	first.SetFirst(true);
	second.SetFirst(false);
	m_context.attackingPlayer = &first;
	m_context.defendingPlayer = &second;
	m_context.attackingPokemon = (&first == m_context.playerOne) ? m_context.playerOneCurrentPokemon : m_context.playerTwoCurrentPokemon;
	m_context.defendingPokemon = (&second == m_context.playerOne) ? m_context.playerOneCurrentPokemon : m_context.playerTwoCurrentPokemon;
	m_context.currentMove = (&first == m_context.playerOne) ? m_context.playerOneCurrentMove : m_context.playerTwoCurrentMove;
}

bool BattleCalculations::CalculateCriticalHit(BattleContext& ctx, const BattlePokemon& source)
{
	size_t stage = source.GetCriticalHitStage();
	if (stage > 3) stage = 3;

	int threshold = m_arr_CriticalHitStageThresholds[stage];

	std::uniform_int_distribution<int> critmoddistributor(0, 23);
	int roll{ critmoddistributor(m_rng.GetGenerator()) };

	ctx.flags.isCriticalHit = (roll < threshold);

	return ctx.flags.isCriticalHit;
}

unsigned int BattleCalculations::MultiplyEffectiveness(unsigned int effect1, unsigned effect2)
{
	if (effect1 == 0 || effect2 == 0)
	{
		return 0;
	}

	unsigned int product = effect1 * effect2;
	return (product / 4096);
}

void BattleCalculations::CalculateTypeEffectiveness(BattleContext& ctx, const pokemonMove& currentMove, const BattlePokemon& target)
{
	using Effectiveness = BattleStateFlags::Effectiveness;

	ctx.effectiveness = 4096;
	ctx.flags.currentEffectiveness = Effectiveness::Normal;

	if (currentMove.GetMoveEffectEnum() == MoveEffect::Struggle)
	{
		return;
	}

	size_t moveType = static_cast<size_t>(currentMove.GetMoveTypeEnum());
	size_t defensiveTypeOne = static_cast<size_t>(target.GetTypeOneEnum());
	size_t defensiveTypeTwo = static_cast<size_t>(target.GetTypeTwoEnum());

	unsigned int effect1 = typeChart[moveType][defensiveTypeOne];
	unsigned int effect2 = (defensiveTypeTwo == 18) ? 4096 : typeChart[moveType][defensiveTypeTwo];

	ctx.effectiveness = MultiplyEffectiveness(effect1, effect2);

	unsigned int moveEffectiveness = ctx.effectiveness;

	if (moveEffectiveness == 0)
	{
		ctx.flags.currentEffectiveness = Effectiveness::No;
	}
	else if (moveEffectiveness > 0 && moveEffectiveness < 4096)
	{
		ctx.flags.currentEffectiveness = Effectiveness::Less;
	}
	else if (moveEffectiveness > 4096)
	{
		ctx.flags.currentEffectiveness = Effectiveness::Super;
	}
	else
	{
		ctx.flags.currentEffectiveness = Effectiveness::Normal;
	}

	if (currentMove.GetMoveEffectEnum() == MoveEffect::OHKO && moveEffectiveness != 0)
	{
		ctx.flags.currentEffectiveness = Effectiveness::OHKO;
	}
}

bool BattleCalculations::CalculateHitChance(const pokemonMove& currentMove, const BattlePokemon& source, const BattlePokemon& target)
{
	if (
		(target.IsSemiInvulnerableFromFly() && (currentMove.GetMoveEffectEnum() != MoveEffect::Gust && currentMove.GetName() != "Thunder")) ||
		(target.IsSemiInvulnerableFromDig() && (currentMove.GetMoveEffectEnum() != MoveEffect::Earthquake && currentMove.GetName() != "Fissure"))
		)
	{
		return false;
	}

	size_t sourceAccuracy = source.GetAccuracyStage();
	size_t targetEvasion = target.GetEvasionStage();

	int netStage{ static_cast<int>(sourceAccuracy) - static_cast<int>(targetEvasion) };
	int targetIndex = netStage + 6;
	size_t adjustedStages = static_cast<size_t>(std::clamp(targetIndex, 0, 12));

	const auto& [numerator, denominator] = GetAccuracyStageRatio(adjustedStages);

	int moveAccuracy = currentMove.GetAccuracy();

	int accuracyMod{};

	if (currentMove.GetMoveEffectEnum() != MoveEffect::OHKO)
	{
		accuracyMod = (moveAccuracy * numerator) / denominator;

	}
	else
	{
		accuracyMod = (source.GetLevel() - target.GetLevel()) + 30; // for OHKO moves

		if (accuracyMod < 0)
		{
			accuracyMod = 0;
		}
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

unsigned int BattleCalculations::CalculateDamage(BattleContext& ctx, const Player& targetPlayer, const pokemonMove& currentMove, const BattlePokemon& source, BattlePokemon& target)
{
	unsigned int effectiveness = ctx.effectiveness;

	if (effectiveness == 0)
	{
		return 0;
	}

	unsigned int baseDamage{ 0 };

	if ((currentMove.GetMoveEffectEnum() == MoveEffect::OHKO) && effectiveness != 0)
	{
		baseDamage = target.GetCurrentHP();
		target.DamageCurrentHP(baseDamage);
		ctx.damageTaken = baseDamage;
		return baseDamage;
	}

	bool isCritical{ CalculateCriticalHit(ctx, source) };

	// START: Calculate total attack and defense values of attacker and defender
	bool isPhysical{ currentMove.GetCategoryEnum() == Category::Physical };

	unsigned int baseSourceAttack{ isPhysical ? source.GetAttack() : source.GetSpecialAttack() };
	unsigned int baseTargetDefense{ isPhysical ? target.GetDefense() : target.GetSpecialDefense() };

	size_t sourceStage{ isPhysical ? source.GetAttackStage() : source.GetSpecialAttackStage() };
	size_t targetStage{ isPhysical ? target.GetDefenseStage() : target.GetSpecialDefenseStage() };

	if (isCritical)
	{
		// If attacker's attack stage is less than 6, clamp to 6
		sourceStage = std::max(sourceStage, (size_t)6);
		
		// If defender's defense stage is greater than 6, clamp to 6
		targetStage = std::min(targetStage, (size_t)6);
	}

	auto [atkNumerator, atkDenominator] = GetStageRatio(sourceStage);
	unsigned int sourceAttack{ baseSourceAttack * atkNumerator / atkDenominator };
	
	auto [defNumerator, defDenominator] = GetStageRatio(targetStage);
	unsigned int targetDefense{ baseTargetDefense * defNumerator / defDenominator };
	// END:

	unsigned int currentMovePower{ currentMove.GetPower() };

	if (currentMove.GetMoveEffectEnum() == MoveEffect::LowKick)
	{
		currentMovePower = CalculateLowKickPower(target);
	}

	unsigned int powerModifier = ctx.initialPowerMultiplier;

	if (powerModifier > 10)
	{
		currentMovePower = currentMovePower * powerModifier / 10;
	}

	unsigned int level = source.GetLevel();

	// Damage formula: (((((2 * level / 5) + 2) * currentMovePower * sourceAttack) / targetDefense) / 50) + 2
	// Truncates int after every division
	unsigned int step1 = (2 * level / 5) + 2;
	unsigned int step2 = step1 * currentMovePower;
	unsigned int step3 = step2 * sourceAttack;
	unsigned int step4 = step3 / targetDefense;
	baseDamage = step4 / 50 + 2;
	
	unsigned int interimDamage = baseDamage;

	if (isCritical)
	{
		interimDamage = interimDamage * 6144 / 4096;
	}

	std::uniform_int_distribution<unsigned int> dist(85, 100);
	unsigned int randPercent = dist(m_rng.GetGenerator());

	interimDamage = interimDamage * randPercent / 100;

	bool hasStab = (currentMove.GetMoveTypeEnum() == source.GetTypeOneEnum() ||
		currentMove.GetMoveTypeEnum() == source.GetTypeTwoEnum())
		&& currentMove.GetMoveEffectEnum() != MoveEffect::Struggle;

	if (hasStab)
	{
		interimDamage = interimDamage * 6144 / 4096;
	}

	interimDamage = interimDamage * effectiveness / 4096;

	if (source.GetStatus() == Status::Burned && isPhysical)
	{
		interimDamage = interimDamage * 2048 / 4096;
	}

	unsigned int other{ 4096 };

	if ((currentMove.GetMoveEffectEnum() == MoveEffect::Stomp || currentMove.GetMoveEffectEnum() == MoveEffect::BodySlam) && target.HasUsedMinimize())
	{
		other = (other * 8192 + 2048) / 4096;
	}

	if (currentMove.GetMoveEffectEnum() == MoveEffect::Earthquake && target.IsSemiInvulnerableFromDig())
	{
		other = (other * 8192 + 2048) / 4096;
	}

	if (targetPlayer.HasReflect() && !isCritical && isPhysical)
	{
		other = (other * 2048 + 2048) / 4096;
	}

	if (targetPlayer.HasLightScreen() && !isCritical && !isPhysical)
	{
		other = (other * 2048 + 2048) / 4096;
	}

	unsigned int finalDamage = interimDamage * other / 4096;

	if (effectiveness != 0)
	{
		finalDamage = std::max((unsigned int)1, finalDamage);
	}

	/*
	if (finalDamage > target.GetCurrentHP())
	{
		finalDamage = target.GetCurrentHP();
	}
	*/
	
	return finalDamage;
}

void BattleCalculations::ApplyDamage(const pokemonMove& currentMove, BattlePokemon& target, unsigned int damage)
{
	const unsigned int HP_BAR_WIDTH = m_context.HP_BAR_WIDTH;

	if (damage == 0)
	{
		return;
	}

	bool hitSubstitute = target.HasSubstitute() && !currentMove.CanBypassSubstitute();
	m_context.flags.hitSubstitute = hitSubstitute;
	damage = (hitSubstitute ? std::min(damage, target.GetSubstituteHP()) : std::min(damage, target.GetCurrentHP()));

	unsigned int currentPixel = target.GetCurrentHP() * HP_BAR_WIDTH / target.GetMaxHP();
	m_context.prevPixels = currentPixel;

	if (hitSubstitute)
	{
		target.DamageSubstitute(damage);
	}
	else
	{
		target.DamageCurrentHP(damage);
		m_context.damageTaken = damage;
		unsigned int newPixel = target.GetCurrentHP() * HP_BAR_WIDTH / target.GetMaxHP();
		m_context.pixelsLost = currentPixel - newPixel;
		m_context.damageInPixels = m_context.prevPixels - newPixel;
	}

	if (target.IsBiding() && !hitSubstitute)
	{
		target.AddBideDamage(damage);
	}
}

// Calculate power of low kick based on target Pokemon's weight (in hectograms)
unsigned int BattleCalculations::CalculateLowKickPower(const BattlePokemon& target)
{
	unsigned int pokemonWeight = target.GetPokemonDatabasePointer()->GetPokemonWeightHg();

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