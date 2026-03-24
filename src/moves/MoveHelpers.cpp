#include "MoveRoutineDeps.h"
#include "MoveEffectEnums.h"
#include "../entities/Player.h"
#include "../data/StringToTypes.h"
#include "../battle/RandomEngine.h"
#include "../ui/interfaces/IMoveResultsUI.h"
#include "../battle/BattleCalculations.h"
#include "../battle/StatusEffectProcessor.h"

#include "MoveHelpers.h"

EffectivenessText ToEffectivenessText(BattleStateFlags::Effectiveness e)
{
	using E = BattleStateFlags::Effectiveness;

	switch (e)
	{
	case E::Less:  return EffectivenessText::Less;
	case E::Super: return EffectivenessText::Super;
	case E::No:	   return EffectivenessText::No;
	case E::OHKO:  return EffectivenessText::OHKO;
	default:	   return EffectivenessText::Normal;
	}
}

void InflictNVStatus(Status status, int chance, MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	if (ctx.flags.hitSubstitute || ctx.defendingPokemon->GetCurrentHP() <= 0 || ctx.defendingPokemon->GetStatus() != Status::Normal || ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		return;
	}

	if (status == Status::Burned && (ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Fire || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Fire))
	{
		return;
	}

	else if (status == Status::Frozen && (ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Ice || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Ice))
	{
		return;
	}

	else if (status == Status::Paralyzed && (ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Electric || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Electric))
	{
		return;
	}

	else if ((status == Status::Poisoned || status == Status::Badly_Poisoned) && ((ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Poison || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Poison)
		|| (ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Steel || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Steel)))
	{
		return;
	}

	std::uniform_int_distribution<int> randomModDistributor(1, 100);
	int randomNumber{ randomModDistributor(deps.rng.GetGenerator()) };

	if (randomNumber > chance)
	{
		return;
	}

	std::string statusMessage = ctx.defendingPlayer->GetPlayerName() + "'s " + ctx.defendingPokemon->GetName() + " ";

	if (status == Status::Burned)
		statusMessage += "was burned!";
	else if (status == Status::Frozen)
		statusMessage += "was frozen solid!";
	else if (status == Status::Paralyzed)
		statusMessage += "is paralyzed! It may be unable to move!";
	else if (status == Status::Poisoned)
		statusMessage += "was poisoned!";
	else if (status == Status::Badly_Poisoned)
		statusMessage += "was badly poisoned!";
	else if (status == Status::Sleeping && ctx.defendingPokemon->GetStatus() == Status::Normal)
		statusMessage += "fell asleep!";


	ctx.defendingPokemon->ChangeStatus(status);
	if (status == Status::Badly_Poisoned)
	{
		ctx.defendingPokemon->ResetBadlyPoisonCounter();
	}

	if (status == Status::Sleeping)
	{
		std::uniform_int_distribution<int> randomModDistributor(1, 3);
		int randomMod(randomModDistributor(deps.rng.GetGenerator()));
		ctx.defendingPokemon->SetSleepTurnCount(randomMod);
		ctx.defendingPokemon->ResetSleepCounter();
	}

	deps.resultsUI.DisplayNVStatusMsg(statusMessage);
}

void DamageRoutine(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;
	auto& calc = deps.calculations;
	auto& resultsUI = deps.resultsUI;
	auto& statusProcessor = deps.statusProcessor;

	int damage = calc.CalculateDamage(ctx, *ctx.defendingPlayer, *ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);
	calc.ApplyDamage(*ctx.defendingPlayer, *ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon, damage);
	resultsUI.DisplayDirectDamageInflictedMsg(damage);
	resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
	resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
	resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);
}

void MultiStrikeRoutine(MoveRoutineDeps& deps, int turnCount)
{
	auto& ctx = deps.context;

	int timesHit{};

	int totalDamage{};
	for (int i = 0; i < turnCount; ++i)
	{
		int damage = deps.calculations.CalculateDamage(ctx, *ctx.defendingPlayer, *ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);
		deps.calculations.ApplyDamage(*ctx.defendingPlayer, *ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon, damage);
		totalDamage += damage;
		deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);

		if (ctx.currentMove->GetMoveEffectEnum() == MoveEffect::Twineedle)
		{
			InflictNVStatus(Status::Poisoned, ctx.currentMove->GetEffectChance(), deps);
		}

		++timesHit;

		if (ctx.defendingPokemon->GetCurrentHP() <= 0)
		{
			break;
		}
	}

	if (timesHit > 1)
	{
		deps.resultsUI.DisplayMultiAttackMsg(ctx.defendingPokemon->GetNameView(), timesHit);
	}

	deps.resultsUI.DisplayDirectDamageInflictedMsg(totalDamage);
}

void FixedDamageRoutine(MoveRoutineDeps& deps, int fixedDamage)
{
	auto& ctx = deps.context;

	bool hasSubstitute = ctx.defendingPokemon->HasSubstitute() && !ctx.currentMove->CanBypassSubstitute();

	int maxDamage = hasSubstitute ? ctx.defendingPokemon->GetSubstituteHP() : ctx.defendingPokemon->GetCurrentHP();

	int finalDamage = std::min(fixedDamage, maxDamage);

	deps.calculations.ApplyDamage(*ctx.defendingPlayer, *ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon, finalDamage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(fixedDamage);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);
}

void FlinchRoutine(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	if (!ctx.defendingPokemon->IsFainted() && !ctx.flags.hitSubstitute && !ctx.defendingPlayer->IsFirst())
	{
		std::uniform_int_distribution<int> randomModDistributor(1, 100);
		int randomMod = randomModDistributor(deps.rng.GetGenerator());

		if (randomMod <= ctx.currentMove->GetEffectChance())
		{
			ctx.defendingPokemon->SetIsFlinched(true);
		}
	}
}

bool HandleCharging(MoveRoutineDeps& deps, ChargeMsgMemFn chargeMsg, const ChargingMoveHooks& hooks)
{
	auto& ctx = deps.context;
	auto* atkPlayer = ctx.attackingPlayer;
	auto* atkPkmn = ctx.attackingPokemon;

	if (!atkPkmn->IsCharging())
	{
		(deps.resultsUI.*chargeMsg)(atkPlayer->GetPlayerNameView(), atkPkmn->GetPokemonNameView());

		if (hooks.preCharge)
		{
			hooks.preCharge(deps);
		}

		if (hooks.stageUp)
		{
			int amount = hooks.stageIncreaseAmount;
			std::string_view stageName = hooks.stageName;

			hooks.stageUp(deps, amount, stageName, hooks.getStage, hooks.setStage);
		}

		atkPkmn->SetCharging(true);
		ctx.attackingPlayer->SetCanSwitch(false);

		return true;
	}

	atkPkmn->SetCharging(false);
	atkPlayer->SetCanSwitch(true);

	if (hooks.postCharge)
	{
		hooks.postCharge(deps);
	}

	return false;
}

void StageUpRoutine(MoveRoutineDeps& deps, int amount, std::string_view stageName, GetStageFn getStage, SetStageFn setStage)
{
	auto& ctx = deps.context;
	auto& atkPlayer = *ctx.attackingPlayer;
	auto& atkPkmn = *ctx.attackingPokemon;
	auto playerName = atkPlayer.GetPlayerNameView();
	auto pokemonName = atkPkmn.GetNameView();

	int stage{ getStage(atkPkmn) };

	int rise{ std::min(amount, 6 - stage) };

	if (rise <= 0)
	{
		deps.resultsUI.DisplayStatRaiseFailMsg(stageName, playerName, pokemonName);
		return;
	}

	setStage(atkPkmn, stage + rise);
	DisplayStatChange(deps.resultsUI, rise, true, stageName, playerName, pokemonName);
}

void StageDownRoutine(MoveRoutineDeps& deps, int amount, std::string_view stageName, GetStageFn getStage, SetStageFn setStage)
{
	auto& ctx = deps.context;
	auto& defPlayer = *ctx.defendingPlayer;
	auto& defPkmn = *ctx.defendingPokemon;
	auto playerName = defPlayer.GetPlayerNameView();
	auto pokemonName = defPkmn.GetNameView();

	int stage{ getStage(defPkmn) };

	int drop{ std::min(amount, stage + 6) };

	if (drop <= 0)
	{
		deps.resultsUI.DisplayStatLoweredFailMsg(stageName, playerName, pokemonName);
		return;
	}

	setStage(defPkmn, stage - drop);

	DisplayStatChange(deps.resultsUI, drop, false, stageName, playerName, pokemonName);
}

void StageDownDamageRoutine(MoveRoutineDeps& deps, int amount, std::string_view stageName, GetStageFn getStage, SetStageFn setStage)
{
	auto& ctx = deps.context;
	auto& defPlayer = *ctx.defendingPlayer;
	auto& defPkmn = *ctx.defendingPokemon;
	auto playerName = defPlayer.GetPlayerNameView();
	auto pokemonName = defPkmn.GetNameView();

	if (defPkmn.IsFainted() || ctx.flags.hitSubstitute || defPlayer.HasMist())
	{
		return;
	}

	int stage = getStage(defPkmn);

	int drop{ std::min(amount, stage + 6) };

	if (drop <= 0)
	{
		return;
	}

	std::uniform_int_distribution<int> randomModDistributor(1, 100);
	int roll{ randomModDistributor(deps.rng.GetGenerator()) };

	if (roll <= ctx.currentMove->GetEffectChance())
	{
		setStage(defPkmn, stage - drop);

		DisplayStatChange(deps.resultsUI, drop, false, stageName, playerName, pokemonName);
	}
}

void DisplayStatChange(IMoveResultsUI& ui, int amount, bool isUp, std::string_view stageName, std::string_view playerName, std::string_view pokemonName)
{
	if (amount == 1)
	{
		isUp ? ui.DisplayStatRaised1Msg(stageName, playerName, pokemonName)
			: ui.DisplayStatLowered1Msg(stageName, playerName, pokemonName);
	}
	else if (amount == 2)
	{
		isUp ? ui.DisplayStatRaised2Msg(stageName, playerName, pokemonName)
			: ui.DisplayStatLowered2Msg(stageName, playerName, pokemonName);
	}
}