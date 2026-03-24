#include "../data/Pokemon.h"
#include "../moves/MoveEffectEnums.h"
#include "../data/StringToTypes.h"
#include "RandomEngine.h"
#include "BattleContext.h"
#include "Typechart.h"
#include "../entities/Player.h"

#include "BattleCalculations.h"

BattleCalculations::BattleCalculations(BattleContext& context, RandomEngine& rng) : m_context(context), m_rng(rng) {}

int BattleCalculations::CalculatePokemonSpeed(BattlePokemon& pokemon)
{
	auto [numerator, denominator] = GetStageRatio(pokemon.GetSpeedStage());

	int speed = pokemon.GetSpeed() * numerator / denominator;

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

bool BattleCalculations::CalculateCriticalHit(BattleContext& ctx,BattlePokemon& source)
{
	size_t stage = source.GetCriticalHitStage();
	if (stage > 3) stage = 3;

	int threshold = m_arr_CriticalHitStageThresholds[stage];

	std::uniform_int_distribution<int> critmoddistributor(0, 23);
	int roll{ critmoddistributor(m_rng.GetGenerator()) };

	ctx.flags.isCriticalHit = (roll < threshold);

	return ctx.flags.isCriticalHit;
}

std::pair<int, int> BattleCalculations::GetStageRatio(int stage)
{
	stage = std::clamp(stage, -6, 6);
	return m_arr_StageRatio[static_cast<size_t>(stage + 6)];
}

int BattleCalculations::MultiplyEffectiveness(uint16_t effect1, uint16_t effect2)
{
	if (effect1 == 0 || effect2 == 0)
	{
		return 0;
	}

	int product = static_cast<int>(effect1 * effect2);
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

	uint16_t effect1 = typeChart[moveType][defensiveTypeOne];
	uint16_t effect2 = (defensiveTypeTwo == 18) ? 4096 : typeChart[moveType][defensiveTypeTwo];

	ctx.effectiveness = MultiplyEffectiveness(effect1, effect2);

	int moveEffectiveness = ctx.effectiveness;

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

bool BattleCalculations::CalculateHitChance(pokemonMove& currentMove, BattlePokemon& source, BattlePokemon& target)
{
	if (
		(target.IsSemiInvulnerableFromFly() && (currentMove.GetMoveEffectEnum() != MoveEffect::Gust && currentMove.GetName() != "Thunder")) ||
		(target.IsSemiInvulnerableFromDig() && (currentMove.GetMoveEffectEnum() != MoveEffect::Earthquake && currentMove.GetName() != "Fissure"))
		)
	{
		return false;
	}

	int adjustedStages = (source.GetAccuracyStage() - target.GetEvasionStage());

	adjustedStages = std::clamp(adjustedStages, -6, 6);
	const auto& [numerator, denominator] = m_arr_accuracyStageRatio[static_cast<size_t>(adjustedStages + 6)];

	int moveAccuracy = currentMove.GetAccuracy();

	int accuracyMod{};

	if (currentMove.GetMoveEffectEnum() != MoveEffect::OHKO)
	{
		accuracyMod = (moveAccuracy * numerator) / denominator;

	}
	else
	{
		accuracyMod = (source.GetLevel() - target.GetLevel()) + 30; // for OHKO moves
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

int BattleCalculations::CalculateDamage(BattleContext& ctx, Player& targetPlayer, pokemonMove& currentMove, BattlePokemon& source, BattlePokemon& target)
{
	int effectiveness = ctx.effectiveness;

	if (effectiveness == 0)
	{
		return 0;
	}

	int baseDamage{ 0 };

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

	int baseSourceAttack{ isPhysical ? source.GetAttack() : source.GetSpecialAttack() };
	int baseTargetDefense{ isPhysical ? target.GetDefense() : target.GetSpecialDefense() };

	int sourceStage{ isPhysical ? source.GetAttackStage() : source.GetSpecialAttackStage() };
	int targetStage{ isPhysical ? target.GetDefenseStage() : target.GetSpecialDefenseStage() };

	if (isCritical)
	{
		// If attacker's attack stage is less than 0, clamp to 0
		sourceStage = std::max(sourceStage, 0);
		
		// If defender's defense stage is greater than 0, clamp to 0
		targetStage = std::min(targetStage, 0);
	}

	auto [atkNumerator, atkDenominator] = GetStageRatio(sourceStage);
	int sourceAttack{ baseSourceAttack * atkNumerator / atkDenominator };
	
	auto [defNumerator, defDenominator] = GetStageRatio(targetStage);
	int targetDefense{ baseTargetDefense * defNumerator / defDenominator };
	// END:

	int currentMovePower{ currentMove.GetPower() };

	if (currentMove.GetMoveEffectEnum() == MoveEffect::LowKick)
	{
		currentMovePower = CalculateLowKickPower(target);
	}

	int powerModifier = ctx.initialPowerMultiplier;

	if (powerModifier > 10)
	{
		currentMovePower = currentMovePower * powerModifier / 10;
	}

	int level = source.GetLevel();

	// Damage formula: (((((2 * level / 5) + 2) * currentMovePower * sourceAttack) / targetDefense) / 50) + 2
	// Truncates int after every division
	int step1 = (2 * level / 5) + 2;
	int step2 = step1 * currentMovePower;
	int step3 = step2 * sourceAttack;
	int step4 = step3 / targetDefense;
	baseDamage = step4 / 50 + 2;
	
	int interimDamage = baseDamage;

	if (isCritical)
	{
		interimDamage = interimDamage * 6144 / 4096;
	}

	std::uniform_int_distribution<int> dist(85, 100);
	int randPercent = dist(m_rng.GetGenerator());

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

	int other{ 4096 };

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

	int finalDamage = interimDamage * other / 4096;

	if (effectiveness != 0)
	{
		finalDamage = std::max(1, finalDamage);
	}

	/*
	if (finalDamage > target.GetCurrentHP())
	{
		finalDamage = target.GetCurrentHP();
	}
	*/
	
	return finalDamage;
}

void BattleCalculations::ApplyDamage(Player& targetPlayer, pokemonMove& currentMove, BattlePokemon& source, BattlePokemon& target, int damage)
{
	const int HP_BAR_WIDTH = m_context.HP_BAR_WIDTH;

	if (damage == 0)
	{
		return;
	}

	int currentPixel = target.GetCurrentHP() * HP_BAR_WIDTH / target.GetMaxHP();
	m_context.prevPixels = currentPixel;

	if (target.HasSubstitute() && !currentMove.CanBypassSubstitute())
	{
		target.DamageSubstitute(damage);
		m_context.flags.hitSubstitute = true;
	}

	else
	{
		target.DamageCurrentHP(damage);
		m_context.flags.hitSubstitute = false;
		m_context.damageTaken = damage;
		int newPixel = target.GetCurrentHP() * HP_BAR_WIDTH / target.GetMaxHP();
		m_context.pixelsLost = currentPixel - newPixel;
		m_context.damageInPixels = m_context.prevPixels - newPixel;
	}

	if (target.IsBiding() && !m_context.flags.hitSubstitute)
	{
		target.AddBideDamage(damage);
	}
}

// Calculate power of low kick based on target Pokemon's weight (in hectograms)
int BattleCalculations::CalculateLowKickPower(BattlePokemon& target)
{
	int pokemonWeight = target.GetPokemonDatabasePointer()->GetPokemonWeightHg();

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