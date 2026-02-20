#include <iostream>
#include <set>
#include <unordered_map>

#include "../../battle/BattleContext.h"
#include "../Player.h"
#include "../ai strategies/IAIStrategy.h"
#include "../../moves/MoveEffectEnums.h"
#include "../../data/StringToTypes.h"
#include "../../data/Pokemon.h"
#include "../PlayerDecisionOutcome.h"

#include "AIController.h"

AIController::AIController(IAIStrategy& strat) :
	m_aiStrategy(strat) {}

PlayerDecisionOutcome AIController::ChooseAction(Player& player, BattlePokemon& selfMon)
{
	PlayerDecisionOutcome decision{};

    if (m_aiStrategy.WantsToSwitch(player, selfMon))
    {
		decision.action = BattleAction::SwitchPokemon;
		decision.chosenPokemon = SwitchAction(player, selfMon);
    }
    else
    {
		decision.chosenMove = FightAction(player, selfMon);

		if (decision.chosenMove == &selfMon.Struggle())
		{
			decision.action = BattleAction::Struggle;
		}
		else
		{
			decision.action = BattleAction::Fight;
		}

		return decision;
    }
}

BattlePokemon* AIController::PromptForSwitch(Player& player, BattlePokemon& selfMon)
{
	BattlePokemon* selectedPokemon = SwitchAction(player, selfMon);
	return selectedPokemon;
}

BattlePokemon::pokemonMove* AIController::FightAction(Player& player, BattlePokemon& selfMon)
{
   BattlePokemon::pokemonMove* selectedMove = &m_aiStrategy.ChooseMove(player, selfMon);
   return selectedMove;
}

BattlePokemon* AIController::SwitchAction(Player& player, BattlePokemon& selfMon)
{
    BattlePokemon* selectedPokemon = m_aiStrategy.ChooseSwitch(player, selfMon);
	return selectedPokemon;
}

BattleAction AIController::ForfeitAction(Player&)
{
	return BattleAction::Forfeit;
}

void AIController::OnBattleStart(Player& self, BattleContext& context)
{
	if (&self == context.playerOne)
	{
		memory.selfPlayer = context.playerOne;
		memory.opponentPlayer = context.playerTwo;
	}
	else if (&self == context.playerTwo)
	{
		memory.selfPlayer = context.playerTwo;
		memory.opponentPlayer = context.playerOne;
	}

	GetOpponentParty(*memory.opponentPlayer);
	InitEstimatedStatRanges();
	PrecomputeRealStats(*memory.opponentPlayer);
}

void AIController::OnActivePokemonChanged(BattleContext& context)
{
	if (memory.selfPlayer == context.playerOne)
	{
		UpdateActivePokemon(*context.playerTwoCurrentPokemon);
	}
	else if (memory.selfPlayer == context.playerTwo)
	{
		UpdateActivePokemon(*context.playerOneCurrentPokemon);
	}
}

void AIController::GetOpponentParty(Player& opponent)
{
    for (size_t i = 0; i < memory.opponentMemory.size(); ++i)
    {
        memory.opponentMemory.at(i).pokemon = &(opponent.GetBelt(i + 1));
    }
}

void AIController::InitEstimatedStatRanges()
{
	int minIV{ 0 };
	int maxIV{ 31 };
	int minEV{ 0 };
	int maxEV{ 252 };

	auto SetStatRange = [&](int base, int level, bool isHP) -> std::pair<int, int>
		{
			int min{}, max{};
			if (isHP)
			{
				min = ((minIV + 2 * base + (minEV / 4)) * level / 100) + 10 + level;
				max = ((maxIV + 2 * base + (maxEV / 4)) * level / 100) + 10 + level;
			}
			else
			{
				min = (((minIV + 2 * base + (minEV / 4)) * level / 100) + 5);
				max = (((maxIV + 2 * base + (maxEV / 4)) * level / 100) + 5);
			}

			return { min, max };
		};

	for (size_t i = 0; i < memory.opponentMemory.size(); ++i)
	{
		if (memory.opponentMemory.at(i).pokemon->GetPokemonDatabasePointer() == nullptr)
		{
			continue;
		}

		int level{ memory.opponentMemory.at(i).pokemon->GetLevel() };

		int baseHP{ memory.opponentMemory.at(i).pokemon->GetPokemonDatabasePointer()->GetBaseHP() };
		memory.opponentMemory.at(i).hpRange = SetStatRange(baseHP, level, true);

		int baseAtk{ memory.opponentMemory.at(i).pokemon->GetPokemonDatabasePointer()->GetBaseAttack() };
		memory.opponentMemory.at(i).attackRange = SetStatRange(baseAtk, level, false);

		int baseDef{ memory.opponentMemory.at(i).pokemon->GetPokemonDatabasePointer()->GetBaseDefense() };
		memory.opponentMemory.at(i).defenseRange = SetStatRange(baseDef, level, false);

		int baseSpA{ memory.opponentMemory.at(i).pokemon->GetPokemonDatabasePointer()->GetBaseSpecialAttack() };
		memory.opponentMemory.at(i).specialAttackRange = SetStatRange(baseSpA, level, false);

		int baseSpD{ memory.opponentMemory.at(i).pokemon->GetPokemonDatabasePointer()->GetBaseSpecialDefense() };
		memory.opponentMemory.at(i).specialDefenseRange = SetStatRange(baseSpD, level, false);

		int baseSpeed{ memory.opponentMemory.at(i).pokemon->GetPokemonDatabasePointer()->GetBaseSpeed() };
		memory.opponentMemory.at(i).speedRange = SetStatRange(baseSpeed, level, false);
	}
}

void AIController::PrecomputeRealStats(Player& target)
{
	std::set<int> hpSet, atkSet, defSet, spaSet, spdSet, speedSet;

	for (size_t i = 0; i < 6; ++i)
	{
		auto& poke = target.GetBelt(i + 1);

		if (poke.GetPokemonNameView().empty())
		{
			continue;
		}

		int level = poke.GetLevel();
		int baseHP = poke.GetPokemonDatabasePointer()->GetBaseHP();
		int baseAtk = poke.GetPokemonDatabasePointer()->GetBaseAttack();
		int baseDef = poke.GetPokemonDatabasePointer()->GetBaseDefense();
		int baseSpA = poke.GetPokemonDatabasePointer()->GetBaseSpecialAttack();
		int baseSpD = poke.GetPokemonDatabasePointer()->GetBaseSpecialDefense();
		int baseSpeed = poke.GetPokemonDatabasePointer()->GetBaseSpeed();

		for (int ivevTotal = 0; ivevTotal <= 94; ++ivevTotal)
		{
			hpSet.insert(((2 * baseHP + ivevTotal) * level) / 100 + 10 + level);
			atkSet.insert(((2 * baseAtk + ivevTotal) * level) / 100 + 5);
			defSet.insert(((2 * baseDef + ivevTotal) * level) / 100 + 5);
			spaSet.insert(((2 * baseSpA + ivevTotal) * level) / 100 + 5);
			spdSet.insert(((2 * baseSpD + ivevTotal) * level) / 100 + 5);
			speedSet.insert(((2 * baseSpeed + ivevTotal) * level) / 100 + 5);
		}

		memory.opponentMemory.at(i).realHPValues = std::vector<int>(hpSet.begin(), hpSet.end());
		memory.opponentMemory.at(i).realAttackValues = std::vector<int>(atkSet.begin(), atkSet.end());
		memory.opponentMemory.at(i).realDefenseValues = std::vector<int>(defSet.begin(), defSet.end());
		memory.opponentMemory.at(i).realSpecialAttackValues = std::vector<int>(spaSet.begin(), spaSet.end());
		memory.opponentMemory.at(i).realSpecialDefenseValues = std::vector<int>(spdSet.begin(), spdSet.end());
		memory.opponentMemory.at(i).realSpeedValues = std::vector<int>(speedSet.begin(), speedSet.end());
	}
}

void AIController::UpdateObservedMoves(BattlePokemon::pokemonMove& currentMove)
{
	auto it = FindActivePokemonSlot();

	if (it == memory.opponentMemory.end())
	{
		return;
	}

	auto& moves = it->observedMoves.moves;
	auto& revealed = it->observedMoves.revealed;

	for (size_t moveSlot = 0; moveSlot < moves.size(); ++moveSlot)
	{
		if (moves.at(moveSlot) != nullptr && moves.at(moveSlot)->GetName() == currentMove.GetName())
		{
			return;
		}

		if (moves.at(moveSlot) == nullptr)
		{
			moves.at(moveSlot) = &currentMove;
			revealed.at(moveSlot) = true;
			return;
		}
	}
}

void AIController::UpdateActivePokemon(BattlePokemon& activeOpponentMon)
{
	memory.activeOpponent.opponentActivePokemon = &activeOpponentMon;
}

void AIController::OnMoveResolved(BattleContext& context)
{
	if (!memory.selfPlayer || !memory.opponentPlayer)
		return;

	bool amIAttacker = (memory.selfPlayer == context.attackingPlayer);
	bool amIDefender = (memory.selfPlayer == context.defendingPlayer);

	auto* attackingPokemon = context.attackingPokemon;
	auto* defendingPokemon = context.defendingPokemon;
	auto* moveUsed = context.currentMove;

	if (!attackingPokemon || !defendingPokemon || !moveUsed)
		return;

	if (amIAttacker)
	{
		UpdateEnemyHPandDefenseStats(context, *memory.opponentPlayer, *moveUsed, *attackingPokemon, *defendingPokemon);
	}

	if (amIDefender)
	{
		UpdateEnemyOffenseStats(context, *moveUsed, *defendingPokemon, *attackingPokemon);
	}
}

void AIController::OnFirstMoveResolved(BattleContext& context)
{
	if (!memory.selfPlayer || !memory.opponentPlayer)
		return;

	// if switch happened
	if (!context.playerOneCurrentMove || !context.playerTwoCurrentMove)
	{
		return;
	}

	// If move priorities do not match, skip speed inferences
	if (context.playerOneCurrentMove->GetPriority() != context.playerTwoCurrentMove->GetPriority())
	{
		return;
	}

	UpdateEnemySpeedStats(context, *memory.selfPlayer, *context.playerOneCurrentPokemon, *context.playerTwoCurrentPokemon);
}

void AIController::UpdateEnemyOffenseStats(BattleContext& context, BattlePokemon::pokemonMove& currentMove, BattlePokemon& source, BattlePokemon& target)
{
	if (currentMove.GetCategoryEnum() == Category::Status || context.damageTaken == 0 || source.IsFainted())
	{
		return;
	}

	int observedDamage{ context.damageTaken };

	int effectiveness{ context.effectiveness };

	int currentMovePower{ currentMove.GetPower() };

	int powerModifier{ context.initialPowerMultiplier };

	if (powerModifier > 10)
	{
		currentMovePower = currentMovePower * powerModifier / 10;
	}

	auto GetStageRatio = [](int stage) -> std::pair<int, int>
		{
			if (stage < 0)
			{
				return { 2, -stage + 2 };
			}

			if (stage == 0)
			{
				return { 2, 2 };
			}

			return { stage + 2, 2 };
		};

	bool isPhysical{ currentMove.GetCategoryEnum() == Category::Physical };

	int baseSourceDefense{ isPhysical ? source.GetDefense() : source.GetSpecialDefense() };

	int sourceStage{ isPhysical ? source.GetDefenseStage() : source.GetSpecialDefenseStage() };

	bool isCriticalHit{ context.flags.isCriticalHit };

	if (isCriticalHit)
	{
		// If own defense stage is greater than 0, clamp to 0
		sourceStage = std::min(sourceStage, 0);
	}

	auto [defNumerator, defDenominator] = GetStageRatio(sourceStage);
	int sourceDefense{ baseSourceDefense * defNumerator / defDenominator };

	auto poke = FindActivePokemonSlot();

	std::vector<int> otherMultipliers{};
	otherMultipliers.reserve(3); // max other multipliers is 3 (as of gen 1 moves)

	if (memory.selfPlayer->HasLightScreen() && !isCriticalHit && !isPhysical)
	{
		otherMultipliers.push_back(2048);
	}

	if (memory.selfPlayer->HasReflect() && !isCriticalHit && isPhysical)
	{
		otherMultipliers.push_back(2048);
	}

	if (currentMove.GetMoveEffectEnum() == MoveEffect::Earthquake && source.IsSemiInvulnerableFromDig())
	{
		otherMultipliers.push_back(8192);
	}

	if ((currentMove.GetMoveEffectEnum() == MoveEffect::Stomp || currentMove.GetMoveEffectEnum() == MoveEffect::BodySlam) && source.HasUsedMinimize())
	{
		otherMultipliers.push_back(8192);
	}

	int other{ 4096 };
	for (auto mult : otherMultipliers)
	{
		other = (other * mult + 2048) / 4096;
	}

	int damageReversed = observedDamage * other / 4096;

	std::vector<int> multipliers{};
	multipliers.reserve(4); // max multipliers is 4 (burn, crit, stab, and effectiveness)

	if (target.GetStatus() == Status::Burned && isPhysical)
	{
		multipliers.push_back(2048);
	}

	multipliers.push_back(effectiveness);

	bool hasStab = (currentMove.GetMoveTypeEnum() == source.GetTypeOneEnum() ||
		currentMove.GetMoveTypeEnum() == source.GetTypeTwoEnum()) &&
		currentMove.GetMoveEffectEnum() != MoveEffect::Struggle;
	multipliers.push_back(hasStab ? 6144 : 4096);

	multipliers.push_back(isCriticalHit ? 6144 : 4096);

	for (auto mult : multipliers)
	{
		damageReversed = (damageReversed * 4096) / mult;
	}

	int targetStage{ isPhysical ? target.GetAttackStage() : target.GetSpecialAttackStage() };

	if (isCriticalHit)
	{
		// if opponent's attack stage is less than 0, clamp to 0
		targetStage = std::max(targetStage, 0);
	}

	std::pair<int, int> targetStageRatio{ GetStageRatio(targetStage) };

	damageReversed = damageReversed * targetStageRatio.second / targetStageRatio.first;

	std::vector<int>& targetAttackValues = isPhysical ? poke->realAttackValues : poke->realSpecialAttackValues;

	std::vector<int> newAttackValues{};
	newAttackValues.reserve(targetAttackValues.size());

	int level = target.GetLevel();

	for (auto atk : targetAttackValues)
	{
		int attackStat = atk;

		attackStat = attackStat * targetStageRatio.first / targetStageRatio.second;

		int damage = (((((2 * level / 5) + 2) * currentMovePower * attackStat) / sourceDefense) / 50) + 2;

		for (auto mult : multipliers)
		{
			damage = (damage * mult) / 4096;
		}

		bool valid = false;
		for (int randomMod = 85; randomMod <= 100; ++randomMod)
		{
			int scaled = (damage * randomMod + 50) / 100;
			if (scaled == context.damageTaken)
			{
				valid = true;
				break;
			}
		}

		if (valid)
		{
			newAttackValues.push_back(atk);
		}
	}

	if (!newAttackValues.empty())
	{
		if (isPhysical)
		{
			poke->realAttackValues = std::move(newAttackValues);
			poke->attackRange = { poke->realAttackValues.front(), poke->realAttackValues.back() };
		}
		else
		{
			poke->realSpecialAttackValues = std::move(newAttackValues);
			poke->specialAttackRange = { poke->realSpecialAttackValues.front(), poke->realSpecialAttackValues.back() };
		}
	}
}

void AIController::UpdateEnemyHPandDefenseStats(BattleContext& context, Player& targetPlayer, BattlePokemon::pokemonMove& currentMove, BattlePokemon& source, BattlePokemon& target)
{
	if (context.effectiveness == 0 || context.pixelsLost == 0 || target.IsFainted())
		return;

	const int HP_BAR_WIDTH{ context.HP_BAR_WIDTH };

	int pixelsLost{ context.pixelsLost };

	int currentMovePower{ currentMove.GetPower() };

	int powerModifier{ context.initialPowerMultiplier };

	if (powerModifier > 10)
		currentMovePower = currentMovePower * powerModifier / 10;

	auto GetStageRatio = [](int stage) -> std::pair<int, int>
		{
			if (stage < 0)
			{
				return { 2, -stage + 2 };
			}
			if (stage == 0)
			{
				return { 2, 2 };
			}

			return { stage + 2, 2 };
		};

	bool isPhysical{ currentMove.GetCategoryEnum() == Category::Physical };

	int baseSourceAttack{ isPhysical ? source.GetAttack() : source.GetSpecialAttack() };

	int sourceStage{ isPhysical ? source.GetAttackStage() : source.GetSpecialAttackStage() };

	bool isCriticalHit{ context.flags.isCriticalHit };

	if (isCriticalHit)
	{
		// If attacker's attack stage is less than 0, clamp to 0
		sourceStage = std::max(sourceStage, 0);
	}

	auto [atkNumerator, atkDenominator] = GetStageRatio(sourceStage);
	int sourceAttack{ baseSourceAttack * atkNumerator / atkDenominator };

	auto poke = FindActivePokemonSlot();

	// Core multipliers: Burn, Crit, STAB, Effectiveness
	std::vector<int> multipliers{};
	multipliers.reserve(4); // 4 is max multipliers it can hold

	if (isCriticalHit) multipliers.push_back(6144);

	bool hasStab = (currentMove.GetMoveTypeEnum() == source.GetTypeOneEnum() ||
		currentMove.GetMoveTypeEnum() == source.GetTypeTwoEnum()) &&
		currentMove.GetMoveEffectEnum() != MoveEffect::Struggle;

	if (hasStab)
	{
		multipliers.push_back(6144);
	}

	multipliers.push_back(context.effectiveness);

	if (source.GetStatus() == Status::Burned && isPhysical)
	{
		multipliers.push_back(2048);
	}

	// other multipliers (stomp and bodyslam vs minimize, earthquake vs dig, reflect and light screen)
	std::vector<int> otherMultipliers{};
	otherMultipliers.reserve(3); // max other multipliers it can do as of gen 1 (minimize/dig, reflect and light screen)

	if ((currentMove.GetMoveEffectEnum() == MoveEffect::Stomp || currentMove.GetMoveEffectEnum() == MoveEffect::BodySlam) && target.HasUsedMinimize())
	{
		otherMultipliers.push_back(8192);
	}

	if (currentMove.GetMoveEffectEnum() == MoveEffect::Earthquake && target.IsSemiInvulnerableFromDig())
	{
		otherMultipliers.push_back(8192);
	}

	if (targetPlayer.HasReflect() && !isCriticalHit && isPhysical)
	{
		otherMultipliers.push_back(2048);
	}

	if (targetPlayer.HasLightScreen() && !isCriticalHit && !isPhysical)
	{
		otherMultipliers.push_back(2048);
	}

	int targetStage{ isPhysical ? target.GetDefenseStage() : target.GetSpecialDefenseStage() };

	if (isCriticalHit)
	{
		// If defender's defense stage is greater than 0, clamp to 0
		targetStage = std::min(targetStage, 0);
	}

	std::pair<int, int> targetStageRatio{ GetStageRatio(targetStage) };

	auto ApplyCritAwareStage = [&](int def)
		{
			def = def * targetStageRatio.first / targetStageRatio.second;

			return def;
		};

	auto IsPixelDamagePossible = [&](int hp, int baseDamage) -> bool
		{
			int damage = baseDamage;

			for (int mult : multipliers)
				damage = (damage * mult) / 4096;

			int other{ 4096 };
			for (int mult : otherMultipliers)
				other = (other * mult + 2048) / 4096;

			damage = damage * other / 4096;
			damage = std::max(damage, 1);

			int randomMax = damage;
			int randomMin = std::max((damage * 85 + 50) / 100, 1);

			int minPixels = (randomMin * HP_BAR_WIDTH) / hp;
			int maxPixels = (randomMax * HP_BAR_WIDTH + hp - 1) / hp;

			return pixelsLost >= minPixels && pixelsLost <= maxPixels;
		};

	// Filter defense values
	std::vector<int>& targetDefenseValues = isPhysical ? poke->realDefenseValues : poke->realSpecialDefenseValues;
	std::vector<int> newDefenseValues{};
	newDefenseValues.reserve(targetDefenseValues.size());

	int level{ target.GetLevel() };

	for (auto def : targetDefenseValues)
	{
		if (isPhysical && def < poke->defenseRange.first) continue;
		if (!isPhysical && def < poke->specialDefenseRange.first) continue;

		bool valid{ false };
		for (int hp : poke->realHPValues)
		{
			int damage = (((((2 * level / 5) + 2) * currentMovePower * sourceAttack) / ApplyCritAwareStage(def)) / 50) + 2;
			if (IsPixelDamagePossible(hp, damage))
			{
				valid = true;
				break;
			}
		}

		if (valid) newDefenseValues.push_back(def);
	}

	if (!newDefenseValues.empty())
	{
		if (isPhysical)
		{
			poke->realDefenseValues = std::move(newDefenseValues);
			poke->defenseRange = { poke->realDefenseValues.front(), poke->realDefenseValues.back() };
		}
		else
		{
			poke->realSpecialDefenseValues = std::move(newDefenseValues);
			poke->specialDefenseRange = { poke->realSpecialDefenseValues.front(), poke->realSpecialDefenseValues.back() };
		}
	}

	// Filter HP values
	std::vector<int> newHPValues{};
	newHPValues.reserve(poke->realHPValues.size());

	for (int hp : poke->realHPValues)
	{
		if (hp < poke->hpRange.first) continue;

		bool valid{ false };
		for (int def : (isPhysical ? poke->realDefenseValues : poke->realSpecialDefenseValues))
		{
			int damage = (((((2 * level / 5) + 2) * currentMovePower * sourceAttack) / ApplyCritAwareStage(def)) / 50) + 2;
			if (IsPixelDamagePossible(hp, damage))
			{
				valid = true;
				break;
			}
		}

		if (valid)
		{
			newHPValues.push_back(hp);
		}
	}

	if (!newHPValues.empty())
	{
		poke->realHPValues = std::move(newHPValues);
		poke->hpRange = { poke->realHPValues.front(), poke->realHPValues.back() };
	}
}

void AIController::UpdateEnemySpeedStats(BattleContext& context, Player& self, BattlePokemon& source, BattlePokemon& target)
{
	auto poke = FindActivePokemonSlot();

	// --- Stage ratio helper ---
	auto GetStageRatio = [](int stage) -> std::pair<int, int>
		{
			if (stage < 0)
			{
				return { 2, -stage + 2 };
			}
			
			if (stage == 0)
			{
				return { 2, 2 };
			}

			return { stage + 2, 2 };
		};

	// --- Compute OUR effective speed ---
	bool isParalyzed{ source.GetStatus() == Status::Paralyzed };

	int myBaseSpeed{ source.GetSpeed() };
	auto myStageRatio{ GetStageRatio(source.GetSpeedStage()) };

	int myEffectiveSpeed{ myBaseSpeed * myStageRatio.first / myStageRatio.second };

	if (isParalyzed)
	{
		myEffectiveSpeed /= 2;
	}

	// --- Compute opponent multiplier ---
	// opponentEffectiveSpeed = rawSpeed * multiplier.first / multiplier.second

	auto opponentStageRatio = GetStageRatio(target.GetSpeedStage());

	std::pair<int, int> opponentMultiplier = opponentStageRatio;

	if (target.GetStatus() == Status::Paralyzed)
	{
		opponentMultiplier.first *= 1;
		opponentMultiplier.second *= 2;
	}

	int& minRawSpeed = poke->speedRange.first;
	int& maxRawSpeed = poke->speedRange.second;

	// We solve inequalities on:
	// rawSpeed * opponentMultiplier.first / opponentMultiplier.second
	// compared against myEffectiveSpeed

	int numerator = myEffectiveSpeed * opponentMultiplier.second;
	int denominator = opponentMultiplier.first;

	if (self.IsFirst())
	{
		// We moved first:
		// opponentEffectiveSpeed <= myEffectiveSpeed
		//
		// rawSpeed <= floor(numerator / denominator)

		int newMax = numerator / denominator;

		maxRawSpeed = std::min(maxRawSpeed, newMax);
	}
	else
	{
		// Opponent moved first:
		// opponentEffectiveSpeed > myEffectiveSpeed
		//
		// rawSpeed >= ceil((numerator + 1) / denominator)

		int newMin = (numerator + denominator) / denominator;

		minRawSpeed = std::max(minRawSpeed, newMin);
	}

	// Clamp in case range inverted
	if (minRawSpeed > maxRawSpeed)
		minRawSpeed = maxRawSpeed;
}

std::array<PersistentMemory, 6>::iterator AIController::FindActivePokemonSlot()
{
	auto it = std::find_if(
		memory.opponentMemory.begin(),
		memory.opponentMemory.end(),
		[this](const PersistentMemory& mem)
		{
			return memory.activeOpponent.opponentActivePokemon == mem.pokemon;
		});

	return it;
}