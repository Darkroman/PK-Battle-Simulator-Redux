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

	int observedDamage = context.damageTaken;

	int effectiveness{ context.effectiveness };

	int currentMovePower{ currentMove.GetPower() };

	int powerModifier{ context.initialPowerMultiplier };

	if (powerModifier > 10)
	{
		currentMovePower = currentMovePower * powerModifier / 10;
	}

	auto GetStageRatio = [&](int stage) -> std::pair<int, int>
		{
			if (stage < 0)
			{
				return { 2, -stage + 2 };
			}

			else if (stage == 0)
			{
				return { 2, 2 };
			}

			else
			{
				return { stage + 2, 2 };
			}
		};

	bool isPhysical = currentMove.GetCategoryEnum() == Category::Physical;

	std::pair<int, int> sourceStageRatio = isPhysical ? GetStageRatio(source.GetDefenseStage()) : GetStageRatio(source.GetSpecialDefenseStage());
	std::pair<int, int> targetStageRatio = isPhysical ? GetStageRatio(target.GetAttackStage()) : GetStageRatio(target.GetSpecialAttackStage());

	int sourceDefense{ 0 };

	if (context.flags.isCriticalHit && (source.GetDefenseStage() > 0 || source.GetSpecialDefenseStage() > 0))
	{
		sourceDefense = isPhysical ? source.GetDefense() : source.GetSpecialDefense();
	}
	else
	{
		sourceDefense = (isPhysical ? source.GetDefense() * sourceStageRatio.first / sourceStageRatio.second : source.GetSpecialDefense()) * sourceStageRatio.first / sourceStageRatio.second;
	}

	auto poke = FindActivePokemonSlot();

	std::vector<int> multipliers{};
	multipliers.reserve(6); // max multipliers that can exist at a time

	bool isCriticalHit = context.flags.isCriticalHit;

	if (memory.selfPlayer->HasLightScreen() && !isCriticalHit && currentMove.GetCategoryEnum() == Category::Special)
	{
		observedDamage = (observedDamage * 2048) >> 10;
		multipliers.push_back(2048);
	}

	if (memory.selfPlayer->HasReflect() && !isCriticalHit && currentMove.GetCategoryEnum() == Category::Physical)
	{
		observedDamage = (observedDamage * 2048) >> 10;
		multipliers.push_back(2048);
	}

	if (currentMove.GetMoveEffectEnum() == MoveEffect::Earthquake && source.IsSemiInvulnerableFromDig())
	{
		observedDamage /= 2;
		multipliers.push_back(8192);
	}

	if ((currentMove.GetMoveEffectEnum() == MoveEffect::Stomp || currentMove.GetMoveEffectEnum() == MoveEffect::BodySlam) && source.HasUsedMinimize())
	{
		observedDamage /= 2;
		multipliers.push_back(8192);
	}

	if (target.GetStatus() == Status::Burned && currentMove.GetCategoryEnum() == Category::Physical)
	{
		observedDamage = (observedDamage * 2048) >> 10;
		multipliers.push_back(2048);
	}

	// reverse the effectiveness
	observedDamage = (observedDamage * 4096) / context.effectiveness;
	multipliers.push_back(effectiveness);

	// reverse stab
	bool isStab = currentMove.GetMoveTypeEnum() == target.GetTypeOneEnum() || currentMove.GetMoveTypeEnum() == target.GetTypeTwoEnum();
	int stab = isStab ? 6144 : 4096;
	observedDamage = (observedDamage * 4096) / stab;
	multipliers.push_back(stab);

	// reverse crit
	int critical = context.flags.isCriticalHit ? 6144 : 4096;
	multipliers.push_back(critical);

	observedDamage = (observedDamage * 4096) / critical;

	auto ReverseStageMultiplier = [&](int dmg)
		{
			if (context.flags.isCriticalHit && (target.GetAttackStage() < 0 || target.GetSpecialAttackStage() < 0))
			{

			}
			else
			{
				dmg = (dmg * targetStageRatio.second) / targetStageRatio.first;
			}
			return dmg;
		};

	auto ApplyCritAwareAttackStage = [&](int atk)
		{
			if (context.flags.isCriticalHit && (target.GetAttackStage() < 0 || target.GetSpecialAttackStage() < 0))
			{

			}
			else
			{
				atk = atk * (targetStageRatio.first / targetStageRatio.second);
			}
			return atk;
		};

	int neutralDamage = ReverseStageMultiplier(observedDamage);

	int level{ target.GetLevel() };

	std::vector<int> targetAttackValues = isPhysical ? poke->realAttackValues : poke->realSpecialAttackValues;

	std::vector<int> newAttackValues{};
	newAttackValues.reserve(targetAttackValues.size());

	for (auto& atk : targetAttackValues)
	{
		int damage = (((((2 * level / 5) + 2) * currentMovePower * ApplyCritAwareAttackStage(atk)) / sourceDefense) / 50) + 2;

		for (auto& mult : multipliers)
		{
			damage = (damage * mult) >> 12;
		}

		if (damage < context.damageTaken)
		{
			continue;
		}

		bool valid{ false };
		for (int randomMod = 85; randomMod <= 100; ++randomMod)
		{
			if ((damage * randomMod) / 100 == context.damageTaken)
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

	if (isPhysical)
	{
		if (!newAttackValues.empty())
		{
			poke->realAttackValues = std::move(newAttackValues);
			poke->attackRange = { poke->realAttackValues.front(), poke->realAttackValues.back() };
		}
	}
	else
	{
		if (!newAttackValues.empty())
		{
			poke->realSpecialAttackValues = std::move(newAttackValues);
			poke->specialAttackRange = { poke->realSpecialAttackValues.front(), poke->realSpecialAttackValues.back() };
		}
	}
}

void AIController::UpdateEnemyHPandDefenseStats(BattleContext& context, Player& targetPlayer, BattlePokemon::pokemonMove& currentMove, BattlePokemon& source, BattlePokemon& target)
{
	if (context.effectiveness == 0 || context.pixelsLost == 0 || target.IsFainted())
	{
		return;
	}

	const int HP_BAR_WIDTH = context.HP_BAR_WIDTH;

	int pixelsLost = context.pixelsLost;

	int baseDamage{ 0 };

	int effectiveness{ context.effectiveness };

	int currentMovePower{ currentMove.GetPower() };

	int powerModifier{ context.initialPowerMultiplier };

	if (powerModifier > 10)
	{
		currentMovePower = currentMovePower * powerModifier / 10;
	}

	auto GetStageRatio = [&](int stage) -> std::pair<int, int>
		{
			if (stage < 0)
			{
				return { 2, -stage + 2 };
			}

			else if (stage == 0)
			{
				return { 2, 2 };
			}

			else
			{
				return { stage + 2, 2 };
			}
		};

	bool isPhysical = currentMove.GetCategoryEnum() == Category::Physical;

	std::pair<int, int> sourceStageRatio = isPhysical ? GetStageRatio(source.GetAttackStage()) : GetStageRatio(source.GetSpecialAttackStage());
	std::pair<int, int> targetStageRatio = isPhysical ? GetStageRatio(target.GetDefenseStage()) : GetStageRatio(target.GetSpecialDefenseStage());

	int sourceAttack{ 0 };

	if (context.flags.isCriticalHit && (source.GetAttackStage() < 0 || source.GetSpecialAttackStage() < 0))
	{
		sourceAttack = isPhysical ? source.GetAttack() : source.GetSpecialAttack();
	}
	else
	{
		sourceAttack = (isPhysical ? source.GetAttack() * sourceStageRatio.first / sourceStageRatio.second : source.GetSpecialAttack()) * sourceStageRatio.first / sourceStageRatio.second;
	}

	auto poke = FindActivePokemonSlot();

	std::vector<int> multipliers{};
	multipliers.reserve(6); // max multipliers that can exist at a time

	if (context.flags.isCriticalHit)
	{
		multipliers.push_back(6144);
	}

	bool hasStab = (currentMove.GetMoveTypeEnum() == source.GetTypeOneEnum() ||
		currentMove.GetMoveTypeEnum() == source.GetTypeTwoEnum())
		&& currentMove.GetMoveEffectEnum() != MoveEffect::Struggle;

	if (hasStab)
	{
		multipliers.push_back(6144);
	}

	multipliers.push_back(effectiveness);

	if (source.GetStatus() == Status::Burned && currentMove.GetCategoryEnum() == Category::Physical)
	{
		multipliers.push_back(2048);
	}

	if ((currentMove.GetMoveEffectEnum() == MoveEffect::Stomp || currentMove.GetMoveEffectEnum() == MoveEffect::BodySlam) && target.HasUsedMinimize())
	{
		multipliers.push_back(8192);
	}

	if (currentMove.GetMoveEffectEnum() == MoveEffect::Earthquake && target.IsSemiInvulnerableFromDig())
	{
		multipliers.push_back(8192);
	}

	if (targetPlayer.HasReflect() && !context.flags.isCriticalHit && currentMove.GetCategoryEnum() == Category::Physical)
	{
		multipliers.push_back(2048);
	}

	if (targetPlayer.HasLightScreen() && !context.flags.isCriticalHit && currentMove.GetCategoryEnum() == Category::Special)
	{
		multipliers.push_back(2048);
	}

	int level{ target.GetLevel() };

	std::vector<int> targetDefenseValues = isPhysical ? poke->realDefenseValues : poke->realSpecialDefenseValues;

	std::vector<int> newDefenseValues{};
	newDefenseValues.reserve(targetDefenseValues.size());

	auto ApplyCritAwareStage = [&](int def)
		{
			if (context.flags.isCriticalHit && (target.GetDefenseStage() > 0 || target.GetSpecialDefenseStage() > 0))
			{

			}
			else
			{
				def = def * targetStageRatio.first / targetStageRatio.second;
			}
			return def;
		};

	auto IsPixelDamagePossible = [&](int hp, int baseDamage) -> bool
		{
			int damage = baseDamage;
			for (int mult : multipliers)
				damage = (damage * mult) >> 12;

			damage = std::max(damage, 1);

			int randomMax = damage;
			int randomMin = std::max((damage * 85) / 100, 1);

			int minPixels = (randomMin * HP_BAR_WIDTH) / hp;
			int maxPixels = ((randomMax * HP_BAR_WIDTH + hp - 1) / hp);

			return (minPixels >= pixelsLost && pixelsLost <= maxPixels);
		};

	for (auto& def : targetDefenseValues)
	{
		if (isPhysical && def < poke->defenseRange.first)
		{
			continue;
		}
		else if (!isPhysical && def < poke->specialDefenseRange.first)
		{
			continue;
		}

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
		if (valid)
		{
			newDefenseValues.push_back(def);
		}
	}

	if (isPhysical)
	{
		if (!newDefenseValues.empty())
		{
			poke->realDefenseValues = std::move(newDefenseValues);
			poke->defenseRange = { poke->realDefenseValues.front(), poke->realDefenseValues.back() };
		}

	}
	else
	{
		if (!newDefenseValues.empty())
		{
			poke->realSpecialDefenseValues = std::move(newDefenseValues);
			poke->specialDefenseRange = { poke->realSpecialDefenseValues.front(), poke->realSpecialDefenseValues.back() };
		}
	}


	std::vector<int> newHPValues{};
	newHPValues.reserve(poke->realHPValues.size());

	for (int hp : poke->realHPValues)
	{
		if (hp < poke->hpRange.first)
		{
			continue;
		}

		bool valid{ false };
		for (int def : poke->realDefenseValues)
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

	auto MulRatio = [](std::pair<int, int> a, std::pair<int, int> b)
		{
			return std::pair<int, int>{ a.first* b.first, a.second* b.second };
		};

	auto TightenRawFromEff = [](int eff, std::pair<int, int> ratio, bool isFirst) -> int
		{
			int num;

			if (isFirst)
			{
				num = (eff + 1) * ratio.second - 1;
			}
			else
			{
				num = eff * ratio.second + ratio.first - 1;
			}

			return num / ratio.first;
		};

	auto myStage = GetStageRatio(source.GetSpeedStage());
	int myEff = source.GetSpeed() * myStage.first / myStage.second;

	if (source.GetStatus() == Status::Paralyzed)
	{
		myEff /= 2;
	}

	// Compute opponent's combined stage + paralysis ratio
	auto opStage = GetStageRatio(target.GetSpeedStage());
	auto opStatus = (target.GetStatus() == Status::Paralyzed) ? std::pair<int, int>{ 1, 2 } : std::pair<int, int>{ 1, 1 };
	auto opRatio = MulRatio(opStage, opStatus);

	int& rawMin = poke->speedRange.first;
	int& rawMax = poke->speedRange.second;

	if (self.IsFirst())
	{
		int newMax = TightenRawFromEff(myEff, opRatio, true);

		if (newMax < rawMax)
		{
			rawMax = std::max(rawMin, newMax);
		}
	}
	else
	{
		int newMin = TightenRawFromEff(myEff, opRatio, false);

		if (newMin > rawMin)
		{
			rawMin = std::min(rawMax, newMin);
		}
	}
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