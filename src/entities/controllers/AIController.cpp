#include <utility>
#include <set>
#include <algorithm>

#include "../../battle/BattleContext.h"
#include "../../battle/Typechart.h"
#include "../Player.h"
#include "../../moves/MoveEffectEnums.h"
#include "../../data/StringToTypes.h"
#include "../../data/Pokemon.h"
#include "../PlayerDecisionOutcome.h"
#include "move scoring/AIMoveScoring.h"
#include "switch logic/AISwitchLogic.h"

#include "AIController.h"

AIController::AIController(Difficulty difficulty)
	: m_difficulty(difficulty)
 {}

PlayerDecisionOutcome AIController::ChooseAction(Player& player, Player& targetPlayer, BattlePokemon& selfMon, BattlePokemon& targetMon, RandomEngine& rng)
{
	PlayerDecisionOutcome decision{};

	if (AISwitchLogic::WantsToSwitch(player, targetPlayer, selfMon, targetMon))
	{
		decision.action = BattleAction::SwitchPokemon;
		decision.chosenPokemon = SwitchAction(player, targetPlayer, selfMon, targetMon);

		if (decision.chosenPokemon != nullptr)
		{
			return decision;
		}
	}

	decision.chosenMove = FightAction(player, targetPlayer, selfMon, targetMon, rng);

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

BattlePokemon* AIController::PromptForSwitch(Player& player, Player& targetPlayer, BattlePokemon& selfMon, BattlePokemon& targetMon)
{
	BattlePokemon* selectedPokemon = SwitchActionPostKO(player, targetPlayer, selfMon, targetMon);
	return selectedPokemon;
}

Difficulty AIController::GetDifficulty()
{
	return m_difficulty;
}

pokemonMove* AIController::FightAction(Player& player, Player& targetPlayer, BattlePokemon& selfMon, BattlePokemon& targetMon, RandomEngine& rng)
{
	pokemonMove* selectedMove = AIMoveScoring::GetWinningMove(player, targetPlayer, selfMon, targetMon, rng);

    return selectedMove;
}

BattlePokemon* AIController::SwitchAction(Player& player, Player& targetPlayer, BattlePokemon& selfMon, BattlePokemon& targetMon)
{
	BattlePokemon* selectedPokemon = AISwitchLogic::ChooseSwitch(player, targetPlayer, selfMon, targetMon);

	return selectedPokemon;
}

BattlePokemon* AIController::SwitchActionPostKO(Player& player, Player& targetPlayer, BattlePokemon& selfMon, BattlePokemon& targetMon)
{
	BattlePokemon* selectedPokemon = AISwitchLogic::ChoosePostKOSwitch(player, targetPlayer, selfMon, targetMon);

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
	//InitEstimatedStatRanges();
}

void AIController::OnActivePokemonChanged(BattleContext& context)
{
	if (memory.selfPlayer == context.playerOne)
	{
		UpdateOpponentActivePokemon(*context.playerTwoCurrentPokemon);
	}
	else if (memory.selfPlayer == context.playerTwo)
	{
		UpdateOpponentActivePokemon(*context.playerOneCurrentPokemon);
	}
}

void AIController::GetOpponentParty(Player& opponent)
{
    for (size_t i = 0; i < memory.opponentMemory.size(); ++i)
    {
        memory.opponentMemory.at(i).pokemon = &(opponent.GetBelt(i + 1));
    }
}

std::array<pokemonMove*, 4> AIController::GetObservedMoves()
{
	auto it = FindActivePokemonSlot();

	if (it == memory.opponentMemory.end() || it->pokemon->IsFainted())
	{
		std::array<pokemonMove*, 4> empty{};
		return empty;
	}

	return it->observedMoves.moves;
}

void AIController::UpdateObservedMoves(pokemonMove& currentMove)
{
	auto it = FindActivePokemonSlot();

	if (it == memory.opponentMemory.end() || it->pokemon->IsFainted())
	{
		return;
	}

	memory.activeOpponent.opponentLastUsedMove = &currentMove;

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

void AIController::ResetObservedMoves()
{
	for (auto& pokemon : memory.opponentMemory)
	{
		for (auto& move : pokemon.observedMoves.moves)
		{
			move = nullptr;
		}
		for (auto& move : pokemon.observedMoves.revealed)
		{
			move = false;
		}
	}
}

void AIController::UpdateOpponentActivePokemon(BattlePokemon& activeOpponentMon)
{
	memory.activeOpponent.opponentActivePokemon = &activeOpponentMon;
}

void AIController::OnMoveResolved(BattleContext& context)
{
	if (!memory.selfPlayer || !memory.opponentPlayer)
	{
		return;
	}

	// if switch happened
	if (context.currentMove == nullptr)
	{
		return;
	}

	bool amIAttacker = (memory.selfPlayer == context.attackingPlayer);
	bool amIDefender = (memory.selfPlayer == context.defendingPlayer);

	BattlePokemon& attackingPokemon = *context.attackingPokemon;
	BattlePokemon& defendingPokemon = *context.defendingPokemon;
	pokemonMove& moveUsed = *context.currentMove;

	if (amIAttacker)
	{
		//UpdateEnemyHPandDefenseStats(context, moveUsed, attackingPokemon, defendingPokemon);
	}

	if (amIDefender)
	{
		//UpdateEnemyOffenseStats(context, moveUsed, defendingPokemon, attackingPokemon);
		UpdateObservedMoves(moveUsed);	
	}
}

int AIController::AICalculatePokemonTypeEffectiveness(const BattlePokemon& source, const BattlePokemon& target)
{
	size_t sourceTypeOne = static_cast<size_t>(source.GetTypeOneEnum());
	size_t sourceTypeTwo = static_cast<size_t>(source.GetTypeTwoEnum());
	size_t defensiveTypeOne = static_cast<size_t>(target.GetTypeOneEnum());
	size_t defensiveTypeTwo = static_cast<size_t>(target.GetTypeTwoEnum());

	uint16_t effect1 = typeChart[sourceTypeOne][defensiveTypeOne];
	uint16_t effect2 = (defensiveTypeTwo == 18) ? 4096 : typeChart[sourceTypeOne][defensiveTypeTwo];
	uint16_t effect3 = (sourceTypeTwo == 18)    ? 4096 : typeChart[sourceTypeTwo][defensiveTypeOne];
	uint16_t effect4 = (sourceTypeTwo == 18)    ? 4096 : (defensiveTypeTwo == 18) ? 4096 : typeChart[sourceTypeTwo][defensiveTypeTwo];

	int productOne = effect1 * effect2 / 4096;
	int productTwo = effect3 * effect4 / 4096;

	return productOne * productTwo / 4096;
}

int AIController::AICalculateMoveTypeEffectiveness(const pokemonMove& currentMove, const BattlePokemon& target)
{
	size_t moveType = static_cast<size_t>(currentMove.GetMoveTypeEnum());
	size_t defensiveTypeOne = static_cast<size_t>(target.GetTypeOneEnum());
	size_t defensiveTypeTwo = static_cast<size_t>(target.GetTypeTwoEnum());

	uint16_t effect1 = typeChart[moveType][defensiveTypeOne];
	uint16_t effect2 = (defensiveTypeTwo == 18) ? 4096 : typeChart[moveType][defensiveTypeTwo];

	if (effect1 == 0 || effect2 == 0)
	{
		return 0;
	}

	int product = static_cast<int>(effect1 * effect2);
	return (product / 4096);
}

int AIController::AICalculateDamage(const pokemonMove& currentMove, const Player& targetPlayer, const BattlePokemon& source, const BattlePokemon& target)
{
	int baseDamage{ 0 };

	int effectiveness = AICalculateMoveTypeEffectiveness(currentMove, target);

	if (effectiveness <= 0)
	{
		return 0;
	}

	if ((currentMove.GetMoveEffectEnum() == MoveEffect::OHKO) && effectiveness != 0)
	{
		baseDamage = target.GetCurrentHP();
		return baseDamage;
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

			return { 2 + stage, 2 };
		};

	// START: Calculate total attack and defense values of attacker and defender
	bool isPhysical{ currentMove.GetCategoryEnum() == Category::Physical };

	int baseSourceAttack{ isPhysical ? source.GetAttack() : source.GetSpecialAttack() };
	int baseTargetDefense{ isPhysical ? target.GetDefense() : target.GetSpecialDefense() };

	int sourceStage{ isPhysical ? source.GetAttackStage() : source.GetSpecialAttackStage() };
	int targetStage{ isPhysical ? target.GetDefenseStage() : target.GetSpecialDefenseStage() };

	auto [atkNumerator, atkDenominator] = GetStageRatio(sourceStage);
	int sourceAttack{ baseSourceAttack * atkNumerator / atkDenominator };

	auto [defNumerator, defDenominator] = GetStageRatio(targetStage);
	int targetDefense{ baseTargetDefense * defNumerator / defDenominator };
	// END:

	int currentMovePower{ currentMove.GetPower() };

	auto CalculateLowKickPower = [](const BattlePokemon& target)
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
		};

	if (currentMove.GetMoveEffectEnum() == MoveEffect::LowKick)
	{
		currentMovePower = CalculateLowKickPower(target);
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

	if (targetPlayer.HasReflect() && isPhysical)
	{
		other = (other * 2048 + 2048) / 4096;
	}

	if (targetPlayer.HasLightScreen() && !isPhysical)
	{
		other = (other * 2048 + 2048) / 4096;
	}

	int finalDamage = interimDamage * other / 4096;

	if (effectiveness != 0)
	{
		finalDamage = std::max(1, finalDamage);
	}

	return finalDamage;
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