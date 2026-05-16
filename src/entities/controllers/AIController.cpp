#include <utility>
#include <algorithm>

#include "AIController.h"

#include "../../battle/BattleContext.h"
#include "../../battle/Typechart.h"
#include "../Player.h"
#include "../../moves/MoveEffectEnums.h"
#include "../../data/StringToTypes.h"
#include "../../data/Pokemon.h"
#include "../PlayerDecisionOutcome.h"
#include "move scoring/AIMoveScoring.h"
#include "switch logic/AISwitchLogic.h"

AIController::AIController(Difficulty difficulty)
	: m_difficulty(difficulty)
 {}

std::unique_ptr<IPlayerController> AIController::clone() const
{
	return std::make_unique<AIController>(*this);
}

PlayerDecisionOutcome AIController::ChooseAction(Player& player, const Player& targetPlayer, BattlePokemon& selfMon, const BattlePokemon& targetMon, RandomEngine& rng)
{
	PlayerDecisionOutcome decision{};

	if (player.GetAIController().GetDifficulty() >= Difficulty::Medium)
	{
		if (AISwitchLogic::WantsToSwitch(player, targetPlayer, selfMon, targetMon))
		{
			decision.action = BattleAction::SwitchPokemon;
			decision.chosenPokemon = SwitchAction(player, targetPlayer, selfMon, targetMon);

			if (decision.chosenPokemon != nullptr)
			{
				return decision;
			}
		}
	}

	decision.chosenMove = FightAction(player, targetPlayer, selfMon, targetMon, rng);

	if (decision.chosenMove == &GetStruggle())
	{
		decision.action = BattleAction::Struggle;
	}
	else
	{
		decision.action = BattleAction::Fight;
	}

	return decision;
}

BattlePokemon* AIController::PromptForSwitch(Player& player, const Player& targetPlayer, const BattlePokemon& selfMon, const BattlePokemon& targetMon)
{
	BattlePokemon* selectedPokemon = SwitchActionPostKO(player, targetPlayer, selfMon, targetMon);
	return selectedPokemon;
}

Difficulty AIController::GetDifficulty() const
{
	return m_difficulty;
}

pokemonMove* AIController::FightAction(const Player& player, const Player& targetPlayer, BattlePokemon& selfMon, const BattlePokemon& targetMon, RandomEngine& rng)
{
	if (selfMon.WillPerformStruggle())
	{
		return &GetStruggle();
	}

	pokemonMove* selectedMove = AIMoveScoring::GetWinningMove(player, targetPlayer, selfMon, targetMon, rng);

    return selectedMove;
}

BattlePokemon* AIController::SwitchAction(Player& player, const Player& targetPlayer, const BattlePokemon& selfMon, const BattlePokemon& targetMon)
{
	BattlePokemon* selectedPokemon = AISwitchLogic::ChooseSwitch(player, targetPlayer, selfMon, targetMon);

	return selectedPokemon;
}

BattlePokemon* AIController::SwitchActionPostKO(Player& player, const Player& targetPlayer, const BattlePokemon& selfMon, const BattlePokemon& targetMon)
{
	BattlePokemon* selectedPokemon = AISwitchLogic::ChoosePostKOSwitch(player, targetPlayer, selfMon, targetMon);

	return selectedPokemon;
}

BattleAction AIController::ForfeitAction(const Player&)
{
	return BattleAction::Forfeit;
}

void AIController::OnBattleStart(const Player& self, BattleContext& context)
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
}

void AIController::OnActivePokemonChanged(const BattleContext& context)
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

void AIController::GetOpponentParty(const Player& opponent)
{
    for (size_t i = 0; i < memory.opponentMemory.size(); ++i)
    {
		memory.opponentMemory[i].pokemon = &(opponent.GetBelt(i + 1));
    }
}

std::array<const pokemonMove*, 4> AIController::GetObservedMoves() const
{
	const auto* activePokemon = memory.slotOfActivePokemon;

	if (activePokemon == nullptr || activePokemon->pokemon->IsFainted())
	{
		return std::array<const pokemonMove*, 4>{};
	}

	return activePokemon->observedMoves.moves;
}

void AIController::UpdateObservedMoves(const pokemonMove& currentMove)
{
	auto* activePokemon = memory.slotOfActivePokemon;

	if (activePokemon == nullptr || activePokemon->pokemon->IsFainted())
	{
		return;
	}

	memory.activeOpponent.opponentLastUsedMove = &currentMove;

	auto& observed = activePokemon->observedMoves;

	for (size_t moveSlot = 0; moveSlot < observed.moves.size(); ++moveSlot)
	{
		if (observed.moves[moveSlot] != nullptr && observed.moves[moveSlot]->GetName() == currentMove.GetName())
		{
			return;
		}

		if (observed.moves[moveSlot] == nullptr)
		{
			observed.moves[moveSlot] = &currentMove;
			observed.revealed[moveSlot] = true;
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

void AIController::UpdateOpponentActivePokemon(const BattlePokemon& activeOpponentMon)
{
	memory.activeOpponent.opponentActivePokemon = &activeOpponentMon;
	memory.slotOfActivePokemon = FindActivePokemonSlot();
}

void AIController::OnMoveResolved(const BattleContext& context)
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

	bool amIDefender = (memory.selfPlayer == context.defendingPlayer);

	const pokemonMove& moveUsed = *context.currentMove;

	if (amIDefender)
	{
		UpdateObservedMoves(moveUsed);
	}
}

int AIController::AICalculatePokemonTypeEffectiveness(const BattlePokemon& source, const BattlePokemon& target)
{
	size_t atk1 = static_cast<size_t>(source.GetTypeOneEnum());
	size_t atk2 = static_cast<size_t>(source.GetTypeTwoEnum());
	size_t def1 = static_cast<size_t>(target.GetTypeOneEnum());
	size_t def2 = static_cast<size_t>(target.GetTypeTwoEnum());

	bool atk2Exists = (atk2 != 18);

	int score1 = typeChart[atk1][def1];
	if (def2 != 18)
		score1 = (score1 * typeChart[atk1][def2]) / 4096;

	int score2 = 0;
	if (atk2Exists)
	{
		score2 = typeChart[atk2][def1];
		if (def2 != 18)
			score2 = (score2 * typeChart[atk2][def2]) / 4096;
	}

	return std::max(score1, score2);
}

int AIController::AICalculateMoveTypeEffectiveness(const pokemonMove& currentMove, const BattlePokemon& target)
{
	if (currentMove.GetCategoryEnum() == Category::Status)
	{
		return 0;
	}

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

PersistentMemory* AIController::FindActivePokemonSlot()
{
	auto it = std::find_if(
		memory.opponentMemory.begin(),
		memory.opponentMemory.end(),
		[this](const PersistentMemory& mem)
		{
			return memory.activeOpponent.opponentActivePokemon == mem.pokemon;
		});

	return (it == memory.opponentMemory.end()) ? nullptr : &(*it);
}
/*
const PersistentMemory* AIController::FindActivePokemonSlot() const
{
	auto it = std::find_if(
		memory.opponentMemory.begin(),
		memory.opponentMemory.end(),
		[this](const PersistentMemory& mem)
		{
			return memory.activeOpponent.opponentActivePokemon == mem.pokemon;
		});

	return (it == memory.opponentMemory.end()) ? nullptr : &(*it);
}
*/