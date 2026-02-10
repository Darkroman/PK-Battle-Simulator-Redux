#include <deque>

#include "../data/Database.h"
#include "../battle/BattleContext.h"
#include "../battle/BattleCalculations.h"
#include "../battle/StatusEffectProcessor.h"
#include "../ui/interfaces/IMoveResultsUI.h"
#include "../ui/interfaces/IBattleMenuUI.h"
#include "../battle/RandomEngine.h"
#include "../battle/SwitchExecutor.h"
#include "../data/Move.h"
#include "../data/StringToTypes.h"
#include "../ui/EffectivenessText.h"

#include "MoveRoutines.h"

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

	if ((ctx.defendingPokemon->HasSubstitute() && !ctx.currentMove->CanBypassSubstitute()) || ctx.defendingPokemon->GetCurrentHP() <= 0 || ctx.defendingPokemon->GetStatus() != Status::Normal || ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
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

void BasicDamageRoutine(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);
}

void Execute(MoveEffect ID, MoveRoutineDeps& deps)
{
	auto routine{ MoveRoutineFactory::Make(ID) };
	std::visit([&](auto&& r) {r.DoMove(deps); }, routine);
}

MoveRoutineTypes MoveRoutineFactory::Make(MoveEffect ID)
{
	switch (ID)
	{
	case MoveEffect::NormalHit:             return NormalHit{};
	case MoveEffect::IncreasedCritical:     return IncreasedCritical{};
	case MoveEffect::MultiHit:              return MultiHit{};
	case MoveEffect::BurnHit:               return BurnHit{};
	case MoveEffect::FreezeHit:             return FreezeHit{};
	case MoveEffect::ParalyzeHit:           return ParalyzeHit{};
	case MoveEffect::OHKO:                  return OHKO{};
	case MoveEffect::RazorWind:             return RazorWind{};
	case MoveEffect::AttackUp2:             return AttackUp2{};
	case MoveEffect::Gust:                  return Gust{};
	case MoveEffect::ForceSwitch:           return ForceSwitch{};
	case MoveEffect::Fly:                   return Fly{};
	case MoveEffect::PartialTrap:           return PartialTrap{};
	case MoveEffect::Stomp:                 return Stomp{};
	case MoveEffect::DoubleHit:             return DoubleHit{};
	case MoveEffect::JumpKick:              return JumpKick{};
	case MoveEffect::FlinchHit:             return FlinchHit{};
	case MoveEffect::AccuracyDown:          return AccuracyDown{};
	case MoveEffect::BodySlam:              return BodySlam{};
	case MoveEffect::RecoilQuarter:         return RecoilQuarter{};
	case MoveEffect::Rampage:               return Rampage{};
	case MoveEffect::RecoilThird:           return RecoilThird{};
	case MoveEffect::DefenseDown:           return DefenseDown{};
	case MoveEffect::PoisonHit:             return PoisonHit{};
	case MoveEffect::Twineedle:             return Twineedle{};
	case MoveEffect::AttackDown:            return AttackDown{};
	case MoveEffect::SleepMove:             return SleepMove{};
	case MoveEffect::Confuse:               return Confuse{};
	case MoveEffect::SonicBoom:             return SonicBoom{};
	case MoveEffect::Disable:               return Disable{};
	case MoveEffect::SpecialDefenseDownHit: return SpecialDefenseDownHit{};
	case MoveEffect::Mist:                  return Mist{};
	case MoveEffect::ConfuseHit:            return ConfuseHit{};
	case MoveEffect::SpeedDownHit:          return SpeedDownHit{};
	case MoveEffect::AttackDownHit:         return AttackDownHit{};
	case MoveEffect::RechargeAttack:        return RechargeAttack{};
	case MoveEffect::LowKick:               return LowKick{};
	case MoveEffect::Counter:               return Counter{};
	case MoveEffect::SeismicToss:           return SeismicToss{};
	case MoveEffect::Leech:                 return Leech{};
	case MoveEffect::LeechSeed:             return LeechSeed{};
	case MoveEffect::Growth:                return Growth{};
	case MoveEffect::SolarBeam:             return SolarBeam{};
	case MoveEffect::PoisonPowder:          return PoisonPowder{};
	case MoveEffect::StunSpore:             return StunSpore{};
	case MoveEffect::SleepPowder:           return SleepPowder{};
	case MoveEffect::SpeedDown2:            return SpeedDown2{};
	case MoveEffect::DragonRage:            return DragonRage{};
	case MoveEffect::Paralyze:              return Paralyze{};
	case MoveEffect::Earthquake:            return Earthquake{};
	case MoveEffect::Dig:                   return Dig{};
	case MoveEffect::Toxic:                 return Toxic{};
	case MoveEffect::AttackUp:              return AttackUp{};
	case MoveEffect::SpeedUp2:              return SpeedUp2{};
	case MoveEffect::Rage:                  return Rage{};
	case MoveEffect::Teleport:              return Teleport{};
	case MoveEffect::NightShade:            return NightShade{};
	case MoveEffect::Mimic:                 return Mimic{};
	case MoveEffect::DefenseDown2:          return DefenseDown2{};
	case MoveEffect::EvasionUp:             return EvasionUp{};
	case MoveEffect::HealHalfHP:            return HealHalfHP{};
	case MoveEffect::DefenseUp:             return DefenseUp{};
	case MoveEffect::Minimize:              return Minimize{};
	case MoveEffect::DefenseUp2:            return DefenseUp2{};
	case MoveEffect::LightScreen:           return LightScreen{};
	case MoveEffect::Haze:                  return Haze{};
	case MoveEffect::Reflect:               return Reflect{};
	case MoveEffect::FocusEnergy:           return FocusEnergy{};
	case MoveEffect::Bide:                  return Bide{};
	case MoveEffect::Metronome:             return Metronome{};
	case MoveEffect::MirrorMove:            return MirrorMove{};
	case MoveEffect::Explosion:             return Explosion{};
	case MoveEffect::AlwaysHit:             return AlwaysHit{};
	case MoveEffect::SkullBash:             return SkullBash{};
	case MoveEffect::SpecialDefenseUp2:     return SpecialDefenseUp2{};
	case MoveEffect::DreamEater:            return DreamEater{};
	case MoveEffect::PoisonGas:             return PoisonGas{};
	case MoveEffect::SkyAttack:             return SkyAttack{};
	case MoveEffect::Transform:             return Transform{};
	case MoveEffect::Psywave:               return Psywave{};
	case MoveEffect::Splash:                return Splash{};
	case MoveEffect::Rest:                  return Rest{};
	case MoveEffect::Conversion:            return Conversion{};
	case MoveEffect::TriAttack:             return TriAttack{};
	case MoveEffect::SuperFang:             return SuperFang{};
	case MoveEffect::Substitute:            return Substitute{};
	case MoveEffect::Struggle:              return Struggle{};

	default:								return Noop{};
	}
}

void Noop::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());
	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.resultsUI.DisplayNoopMsg();
}

void NormalHit::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void IncreasedCritical::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	int oldCritStage = ctx.attackingPokemon->GetCriticalHitStage();
	int newCritStage = oldCritStage + 1;

	ctx.attackingPokemon->SetCriticalHitStage(newCritStage);

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	ctx.attackingPokemon->SetCriticalHitStage(oldCritStage);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void MultiHit::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	std::uniform_int_distribution<int> randomModDistributor(1, 100);
	int randomNumber = randomModDistributor(deps.rng.GetGenerator());

	int turnCount = 0;

	if (randomNumber <= 35)
		turnCount = 2;
	else if (randomNumber <= 70)
		turnCount = 3;
	else if (randomNumber <= 85)
		turnCount = 4;
	else
		turnCount = 5;

	int timesHit = 0;

	int totalDamage{};
	while (turnCount > 0 && ctx.defendingPokemon->GetCurrentHP() > 0)
	{
		int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
		deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
		totalDamage += damage;
		deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);

		++timesHit;
		--turnCount;
	}

	if (timesHit > 1)
	{
		deps.resultsUI.DisplayMultiAttackMsg(ctx.defendingPokemon->GetNameView(), timesHit);
	}
	
	deps.resultsUI.DisplayDirectDamageInflictedMsg(totalDamage);

	if (ctx.defendingPokemon->IsBiding() && ctx.defendingPokemon->GetCurrentHP() != 0 && !ctx.flags.hitSubstitute)
	{
		ctx.defendingPokemon->AddBideDamage(ctx.damageTaken);
	}

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void BurnHit::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	InflictNVStatus(Status::Burned, ctx.currentMove->GetEffectChance(), deps);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void FreezeHit::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	InflictNVStatus(Status::Frozen, ctx.currentMove->GetEffectChance(), deps);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void ParalyzeHit::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	InflictNVStatus(Status::Paralyzed, ctx.currentMove->GetEffectChance(), deps);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void OHKO::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	// OHKO specific logic done in CalculateDamage()
	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void RazorWind::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	if (!ctx.attackingPokemon->IsCharging())
	{
		deps.resultsUI.DisplayRazorWindChargeMsg(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		ctx.attackingPokemon->SetCharging(true);
		ctx.attackingPlayer->SetCanSwitch(false);
		return;
	}

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	ctx.attackingPokemon->SetCharging(false);
	ctx.attackingPlayer->SetCanSwitch(true);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void AttackUp2::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	int attackStage = ctx.attackingPokemon->GetAttackStage();

	if (attackStage >= 6)
	{
		deps.resultsUI.DisplayStatRaiseFailMsg("attack", ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
	}
	else if (attackStage == 5)
	{
		ctx.attackingPokemon->SetAttackStage(6);
		deps.resultsUI.DisplayStatRaised1Msg("attack", ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
	}
	else // attackStage < 5
	{
		ctx.attackingPokemon->SetAttackStage(attackStage + 2);
		deps.resultsUI.DisplayStatRaised2Msg("attack", ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
	}
}

void Gust::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}
	
	// Specific accuracy check for gust vs SemiInvulnerableFly done in CalculateHitChance()
	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	if (ctx.defendingPokemon->IsSemiInvulnerableFromFly())
	{
		ctx.initialPowerMultiplier = 20;
	}
	
	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void ForceSwitch::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	// Conditions where forced switch fails
	if (ctx.defendingPlayer->GetPokemonCount() == 1 || !ctx.defendingPlayer->IsFirst())
	{
		deps.resultsUI.DisplayFailedTextDialog();
		return;
	}

	ctx.flags.hit = !ctx.defendingPokemon->IsSemiInvulnerable();

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	std::deque<BattlePokemon*> enemyPokemonList{};

	for (size_t i = 1; i <= 6; ++i)
	{
		BattlePokemon* candidateMon = ctx.defendingPlayer->GetBelt(i);
		if (candidateMon && candidateMon != ctx.defendingPokemon && candidateMon->GetCurrentHP() > 0)
		{
			enemyPokemonList.push_back(candidateMon);
		}
	}

	std::uniform_int_distribution<size_t> randomModDistributor(0, enemyPokemonList.size() - 1);
	BattlePokemon* newMon = enemyPokemonList.at(randomModDistributor(deps.rng.GetGenerator()));

	// Reset stats for the Pokémon being forced out
	ctx.defendingPokemon->ResetStatsOnSwitch();

	if (ctx.attackingPokemon->IsBound())
	{
		ctx.attackingPokemon->SetBound(false);
		ctx.attackingPokemon->ResetBoundCounter();
		ctx.attackingPokemon->SetBoundTurnCount(0);
	}

	if (ctx.defendingPlayer == ctx.playerOne)
	{
		ctx.playerOneCurrentPokemon = newMon;
		if (ctx.aiPlayerTwo)
		{
			ctx.aiPlayerTwo->UpdateActivePokemon(ctx.playerOneCurrentPokemon);
		}
	}
	else
	{
		ctx.playerTwoCurrentPokemon = newMon;
		if (ctx.aiPlayerOne)
		{
			ctx.aiPlayerOne->UpdateActivePokemon(ctx.playerTwoCurrentPokemon);
		}
	}

	deps.resultsUI.DisplayEnemySwitchMsg(newMon->GetNameView());
}

void Fly::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	if (!ctx.attackingPokemon->IsCharging())
	{
		deps.resultsUI.DisplayFlyChargeMsg(ctx.attackingPokemon->GetNameView());
		ctx.attackingPokemon->SetCharging(true);
		ctx.attackingPokemon->SetSemiInvulnerableFly(true);
		ctx.attackingPlayer->SetCanSwitch(false);
		return;
	}

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	ctx.attackingPokemon->SetCharging(false);
	ctx.attackingPokemon->SetSemiInvulnerableFly(false);
	ctx.attackingPlayer->SetCanSwitch(true);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void PartialTrap::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.defendingPokemon->IsFainted() && !ctx.defendingPokemon->IsBound() && ctx.flags.currentEffectiveness != BattleStateFlags::Effectiveness::No && !ctx.defendingPokemon->HasSubstitute())
	{
		bool isGhost = ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Ghost || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Ghost;

		ctx.defendingPokemon->SetBound(true);

		if (!isGhost)
		{
			ctx.defendingPlayer->SetCanSwitch(false);
		}

		std::uniform_int_distribution<int> randomModDistributor(4, 5);
		int randomMod = randomModDistributor(deps.rng.GetGenerator());
		ctx.defendingPokemon->SetBoundTurnCount(randomMod);
		ctx.defendingPokemon->ResetBoundCounter();
		ctx.defendingPokemon->SetBoundMoveName(ctx.currentMove->GetMoveIndex());

		deps.resultsUI.BoundMoveText(ctx.attackingPlayer->GetPlayerNameView(), ctx.defendingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.defendingPokemon->GetNameView(), ctx.currentMove->GetMoveIndex());
	}

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void Stomp::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	if (ctx.defendingPokemon->HasUsedMinimize() && !ctx.defendingPokemon->IsSemiInvulnerable())
	{
		ctx.flags.hit = true;
	}
	else if (ctx.defendingPokemon->IsSemiInvulnerable())
	{
		ctx.flags.hit = false;
	}
	else
	{
		ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	}

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	// Damage multiplier for when defending Pokemon has minimized is in CalculateDamage()
	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.defendingPokemon->IsFainted())
	{
		std::uniform_int_distribution<int> randomModDistributor(1, 100);
		int randomMod = randomModDistributor(deps.rng.GetGenerator());

		if ((!ctx.defendingPokemon->HasSubstitute() || ctx.currentMove->CanBypassSubstitute()) &&
			!ctx.defendingPlayer->IsFirst() &&
			randomMod <= ctx.currentMove->GetEffectChance())
		{
			ctx.defendingPokemon->SetIsFlinched(true);
		}
	}

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void DoubleHit::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	int turnCount = 2;
	int timesHit = 0;

	int totalDamage{};
	while (turnCount != 0 && ctx.defendingPokemon->GetCurrentHP() > 0)
	{
		int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
		deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
		totalDamage += damage;
		deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);

		++timesHit;
		--turnCount;
	}

	if (timesHit > 1)
	{
		deps.resultsUI.DisplayMultiAttackMsg(ctx.defendingPokemon->GetNameView(), timesHit);
	}

	deps.resultsUI.DisplayDirectDamageInflictedMsg(totalDamage);

	if (ctx.defendingPokemon->IsBiding() && ctx.defendingPokemon->GetCurrentHP() != 0 && !ctx.flags.hitSubstitute)
	{
		ctx.defendingPokemon->AddBideDamage(ctx.damageTaken);
	}

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);
}

void JumpKick::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());

		int crashDamage = ctx.attackingPokemon->GetMaxHP() / 2;
		ctx.attackingPokemon->DamageCurrentHP(crashDamage);

		deps.resultsUI.DisplayJumpKickCrashMsg(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void FlinchHit::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.defendingPokemon->IsFainted())
	{
		std::uniform_int_distribution<int> randomModDistributor(1, 100);
		int randomMod = randomModDistributor(deps.rng.GetGenerator());

		if ((!ctx.defendingPokemon->HasSubstitute() || ctx.currentMove->CanBypassSubstitute()) &&
			!ctx.defendingPlayer->IsFirst() &&
			randomMod <= ctx.currentMove->GetEffectChance())
		{
			ctx.defendingPokemon->SetIsFlinched(true);
		}
	}

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void AccuracyDown::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	if (ctx.flags.hit && ctx.defendingPokemon->HasSubstitute() && !ctx.currentMove->CanBypassSubstitute())
	{
		deps.resultsUI.DisplayFailedTextDialog();
		return;
	}

	if (ctx.defendingPlayer->HasMist() && ctx.flags.hit)
	{
		deps.resultsUI.DisplayProtectedByMistMsg(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		return;
	}

	if (ctx.defendingPokemon->GetAccuracyStage() > -6)
	{
		int lowerAccuracy = ctx.defendingPokemon->GetAccuracyStage() - 1;
		ctx.defendingPokemon->SetAccuracyStage(lowerAccuracy);

		deps.resultsUI.DisplayStatLowered1Msg("accuracy", ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
	}
	else
	{
		deps.resultsUI.DisplayStatLoweredFailMsg("accuracy", ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
	}
}

void BodySlam::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	if (ctx.defendingPokemon->HasUsedMinimize() && !ctx.defendingPokemon->IsSemiInvulnerable())
	{
		ctx.flags.hit = true;
	}
	else if (ctx.defendingPokemon->IsSemiInvulnerable())
	{
		ctx.flags.hit = false;
	}
	else
	{
		ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	}

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	// Damage multiplier for when defending Pokemon has minimized is in CalculateDamage()
	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	InflictNVStatus(Status::Paralyzed, ctx.currentMove->GetEffectChance(), deps);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void RecoilQuarter::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	bool hitSubstitute = ctx.defendingPokemon->HasSubstitute() && !ctx.currentMove->CanBypassSubstitute();

	int targetHPBegin = hitSubstitute ? ctx.defendingPokemon->GetSubstituteHP() : ctx.defendingPokemon->GetCurrentHP();
	
	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);

	int targetHPEnd = hitSubstitute ? ctx.defendingPokemon->GetSubstituteHP() : ctx.defendingPokemon->GetCurrentHP();

	int recoilDamage = (targetHPBegin - targetHPEnd) / 4;

	ctx.attackingPokemon->DamageCurrentHP(recoilDamage);

	deps.resultsUI.DisplayRecoilMsg(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);
}

void Rampage::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	auto endRampage = [&]() {
		deps.statusProcessor.ThrashStop();
		deps.statusProcessor.ThrashReset();
	};

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	if (ctx.attackingPokemon->IsThrashing())
	{
		ctx.attackingPokemon->IncrementThrashCounter();
	}

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));

		if (ctx.attackingPokemon->GetThrashCounter() == ctx.attackingPokemon->GetThrashTurnCount() && !ctx.attackingPokemon->IsConfused())
		{
			deps.statusProcessor.ThrashConfuse();
		}

		endRampage();

		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit || ctx.currentMove->b_isDisabled)
	{
		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		}
		else
		{
			deps.resultsUI.DisplayThrashDisabledMsg(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		}

		if (ctx.attackingPokemon->GetThrashCounter() == ctx.attackingPokemon->GetThrashTurnCount() && !ctx.attackingPokemon->IsConfused())
		{
			deps.statusProcessor.ThrashConfuse();
		}

		endRampage();

		return;
	}

	if (!ctx.attackingPokemon->IsThrashing())
	{
		ctx.attackingPokemon->SetThrashing(true);
		ctx.attackingPlayer->SetCanSwitch(false);

		std::uniform_int_distribution<int> randomModDistributor(1, 2);
		int randomMod(randomModDistributor(deps.rng.GetGenerator()));
		ctx.attackingPokemon->SetThrashTurnCount(randomMod);
		ctx.attackingPokemon->ResetThrashCounter();

		ctx.currentMove->DeductPP();
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);

	if (ctx.attackingPokemon->GetThrashCounter() == ctx.attackingPokemon->GetThrashTurnCount() && !ctx.attackingPokemon->IsConfused())
	{
		deps.statusProcessor.ThrashConfuse();

		endRampage();
	}

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);
}

void RecoilThird::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	bool hitSubstitute = ctx.defendingPokemon->HasSubstitute() && !ctx.currentMove->CanBypassSubstitute();

	int targetHPBegin = hitSubstitute ? ctx.defendingPokemon->GetSubstituteHP() : ctx.defendingPokemon->GetCurrentHP();

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);

	int targetHPEnd = hitSubstitute ? ctx.defendingPokemon->GetSubstituteHP() : ctx.defendingPokemon->GetCurrentHP();

	int recoilDamage = (targetHPBegin - targetHPEnd) / 3;

	ctx.attackingPokemon->DamageCurrentHP(recoilDamage);

	deps.resultsUI.DisplayRecoilMsg(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);
}

void DefenseDown::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	if (ctx.flags.hit && ctx.defendingPokemon->HasSubstitute() && !ctx.currentMove->CanBypassSubstitute())
	{
		deps.resultsUI.DisplayFailedTextDialog();
		return;
	}

	if (ctx.defendingPlayer->HasMist() && ctx.flags.hit)
	{
		deps.resultsUI.DisplayProtectedByMistMsg(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		return;
	}

	if (ctx.defendingPokemon->GetDefenseStage() > -6)
	{
		int lowerDefense = ctx.defendingPokemon->GetDefenseStage() - 1;
		ctx.defendingPokemon->SetDefenseStage(lowerDefense);

		deps.resultsUI.DisplayStatLowered1Msg("defense", ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
	}
	else
	{
		deps.resultsUI.DisplayStatLoweredFailMsg("defense", ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
	}
}

void PoisonHit::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	InflictNVStatus(Status::Poisoned, ctx.currentMove->GetEffectChance(), deps);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void Twineedle::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	int turnCount = 2;
	int timesHit = 0;

	int totalDamage{};
	while (turnCount != 0 && ctx.defendingPokemon->GetCurrentHP() > 0)
	{
		int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
		deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
		totalDamage += damage;
		deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

		InflictNVStatus(Status::Poisoned, ctx.currentMove->GetEffectChance(), deps);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);

		++timesHit;
		--turnCount;
	}

	if (timesHit > 1)
	{
		deps.resultsUI.DisplayMultiAttackMsg(ctx.defendingPokemon->GetNameView(), timesHit);
	}

	deps.resultsUI.DisplayDirectDamageInflictedMsg(totalDamage);

	if (ctx.defendingPokemon->IsBiding() && ctx.defendingPokemon->GetCurrentHP() != 0)
	{
		ctx.defendingPokemon->AddBideDamage(ctx.damageTaken);
	}

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);
}

void AttackDown::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	if (ctx.flags.hit && ctx.defendingPokemon->HasSubstitute() && !ctx.currentMove->CanBypassSubstitute())
	{
		deps.resultsUI.DisplayFailedTextDialog();
		return;
	}

	if (ctx.defendingPlayer->HasMist() && ctx.flags.hit)
	{
		deps.resultsUI.DisplayProtectedByMistMsg(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		return;
	}

	if (ctx.defendingPokemon->GetAttackStage() > -6)
	{
		int lowerAttack = ctx.defendingPokemon->GetAttackStage() - 1;
		ctx.defendingPokemon->SetAttackStage(lowerAttack);

		deps.resultsUI.DisplayStatLowered1Msg("attack", ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
	}
	else
	{
		deps.resultsUI.DisplayStatLoweredFailMsg("attack", ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
	}
}

void SleepMove::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		return;
	}

	if (ctx.defendingPokemon->GetStatus() == Status::Sleeping)
	{
		deps.resultsUI.DisplayAlreadyAsleepMsg(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		return;
	}

	if (ctx.defendingPokemon->GetStatus() != Status::Normal || (ctx.defendingPokemon->HasSubstitute() && !ctx.currentMove->CanBypassSubstitute()))
	{
		deps.resultsUI.DisplayFailedTextDialog();
		return;
	}

	InflictNVStatus(Status::Sleeping, 100, deps);
}

void Confuse::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		return;
	}

	if (ctx.defendingPokemon->IsConfused() || (ctx.defendingPokemon->HasSubstitute() && !ctx.currentMove->CanBypassSubstitute()))
	{
		deps.resultsUI.DisplayFailedTextDialog();
		return;
	}

	ctx.defendingPokemon->SetConfusedStatus(true);

	std::uniform_int_distribution<int> randomModDistributor(2, 4);
	int randomMod(randomModDistributor(deps.rng.GetGenerator()));
	ctx.defendingPokemon->SetConfusedTurnCount(randomMod);
	ctx.defendingPokemon->ResetConfusedCounter();

	deps.resultsUI.DisplayBecameConfuseMsg(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
}

void SonicBoom::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	const int baseDamage = 20;

	bool hasSubstitute = ctx.defendingPokemon->HasSubstitute();

	int maxDamage = hasSubstitute ? ctx.defendingPokemon->GetSubstituteHP() : ctx.defendingPokemon->GetCurrentHP();

	int finalDamage = std::min(baseDamage, maxDamage);

	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, finalDamage);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void Disable::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		return;
	}

	auto* lastUsed = ctx.defendingPokemon->GetLastUsedMove();

	if (!lastUsed ||
		ctx.defendingPokemon->MoveIsDisabled() ||
		ctx.defendingPokemon->GetLastUsedMove()->m_currentPP == 0 ||
		ctx.defendingPokemon->GetLastUsedMove()->GetMoveEffectEnum() == MoveEffect::Struggle)
	{
		deps.resultsUI.DisplayFailedTextDialog();
		return;
	}

	ctx.defendingPokemon->SetDisabledStatus(true);
	ctx.defendingPokemon->ResetDisabledCounter();

	deps.resultsUI.DisplayMoveDisabledMsg(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetLastUsedMove()->GetName());
	
}

void SpecialDefenseDownHit::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}
		
	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.defendingPokemon->IsFainted() && ctx.defendingPokemon->GetSpecialDefenseStage() > -6)
	{
		std::uniform_int_distribution<int> randomModDistributor(1, 100);
		int randomNumber{ randomModDistributor(deps.rng.GetGenerator()) };

		if (randomNumber <= ctx.currentMove->GetEffectChance() &&
			!ctx.defendingPlayer->HasMist() &&
			(!ctx.defendingPokemon->HasSubstitute() || ctx.currentMove->CanBypassSubstitute()))
		{
			int lowerSpecialDefense = ctx.defendingPokemon->GetSpecialDefenseStage() - 1;
			ctx.defendingPokemon->SetSpecialDefenseStage(lowerSpecialDefense);

			deps.resultsUI.DisplayStatLowered1Msg("special defense", ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		}
	}

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void Mist::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	if (!ctx.attackingPlayer->HasMist())
	{
		deps.resultsUI.DisplayMistMsg(ctx.attackingPlayer->GetPlayerNameView());
		ctx.attackingPlayer->SetMist(true);
	}
	else
	{
		deps.resultsUI.DisplayFailedTextDialog();
	}
}

void ConfuseHit::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.defendingPokemon->IsFainted() && !ctx.defendingPokemon->IsConfused())
	{
		std::uniform_int_distribution<int> randomModDistributor(1, 100);
		int randomNumber{ randomModDistributor(deps.rng.GetGenerator()) };

		if (randomNumber <= ctx.currentMove->GetEffectChance() && 
			(!ctx.defendingPokemon->HasSubstitute() || ctx.currentMove->CanBypassSubstitute()) &&
			ctx.defendingPokemon->GetCurrentHP() != 0)
		{
			deps.resultsUI.DisplayBecameConfuseMsg(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());

			ctx.defendingPokemon->SetConfusedStatus(true);

			std::uniform_int_distribution<int> randomModDistributor(2, 4);
			int randomMod(randomModDistributor(deps.rng.GetGenerator()));
			ctx.defendingPokemon->SetConfusedTurnCount(randomMod);
			ctx.defendingPokemon->ResetConfusedCounter();
		}
	}

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void SpeedDownHit::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}
		
	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.defendingPokemon->IsFainted() && ctx.defendingPokemon->GetSpeedStage() > -6)
	{
		std::uniform_int_distribution<int> randomModDistributor(1, 100);
		int randomNumber{ randomModDistributor(deps.rng.GetGenerator()) };

		if (randomNumber <= ctx.currentMove->GetEffectChance() &&
			!ctx.defendingPlayer->HasMist() &&
			(!ctx.defendingPokemon->HasSubstitute() || ctx.currentMove->CanBypassSubstitute()))
		{
			int lowerSpeed = ctx.defendingPokemon->GetSpeedStage() - 1;
			ctx.defendingPokemon->SetSpeedStage(lowerSpeed);

			deps.resultsUI.DisplayStatLowered1Msg("speed", ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		}
	}

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void AttackDownHit::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.defendingPokemon->IsFainted() && ctx.defendingPokemon->GetAttackStage() > -6)
	{
		std::uniform_int_distribution<int> randomModDistributor(1, 100);
		int randomNumber{ randomModDistributor(deps.rng.GetGenerator()) };

		if (randomNumber <= ctx.currentMove->GetEffectChance() &&
			!ctx.defendingPlayer->HasMist() &&
			(!ctx.defendingPokemon->HasSubstitute() || ctx.currentMove->CanBypassSubstitute()))
		{
			int lowerAttack = ctx.defendingPokemon->GetAttackStage() - 1;
			ctx.defendingPokemon->SetAttackStage(lowerAttack);

			deps.resultsUI.DisplayStatLowered1Msg("attack", ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		}
	}

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void RechargeAttack::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	if (ctx.attackingPokemon->IsRecharging())
	{
		deps.resultsUI.DisplayRechargeMsg(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		ctx.attackingPokemon->SetRecharging(false);
		ctx.attackingPlayer->SetCanSwitch(true);
		return;
	}

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	if (ctx.damageTaken > 0)
	{
		ctx.attackingPokemon->SetRecharging(true);
		ctx.attackingPlayer->SetCanSwitch(false);
	}

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void LowKick::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	// Low Kick power calculated in CalculateDamage()
	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);
}

void Counter::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	int counterDamage = ctx.damageTaken * 2;

	ctx.damageTaken = 0;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	if (ctx.attackingPlayer->IsFirst())
	{
		deps.resultsUI.DisplayFailedTextDialog();
		return;
	}

	const auto* lastMove = ctx.defendingPokemon->GetLastUsedMove();
	const bool lastWasPhysical = lastMove && lastMove->GetCategoryEnum() == Category::Physical;
	const int lastDamage = ctx.damageTaken;

	if (!lastWasPhysical || lastDamage <= 0 || ctx.flags.hitSubstitute)
	{
		deps.resultsUI.DisplayFailedTextDialog();
		return;
	}

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	bool hasSubstitute = ctx.defendingPokemon->HasSubstitute();

	int maxDamage = hasSubstitute ? ctx.defendingPokemon->GetSubstituteHP() : ctx.defendingPokemon->GetCurrentHP();

	int finalDamage = std::min(counterDamage, maxDamage);

	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, finalDamage);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void SeismicToss::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	const int baseDamage = ctx.defendingPokemon->GetLevel();

	bool hasSubstitute = ctx.defendingPokemon->HasSubstitute();

	int maxDamage = hasSubstitute ? ctx.defendingPokemon->GetSubstituteHP() : ctx.defendingPokemon->GetCurrentHP();

	int finalDamage = std::min(baseDamage, maxDamage);

	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, finalDamage);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void Leech::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}
	
	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	int leechedHealth{};

	if (ctx.damageTaken <= 0)
	{
		leechedHealth = 0;
	}
	else if (ctx.damageTaken == 1)
	{
		leechedHealth = 1;
	}
	else
	{
		leechedHealth = ctx.damageTaken / 2;
	}

	if (leechedHealth > (ctx.attackingPokemon->GetMaxHP() - ctx.attackingPokemon->GetCurrentHP()))
	{
		leechedHealth = (ctx.attackingPokemon->GetMaxHP() - ctx.attackingPokemon->GetCurrentHP());
	}

	ctx.attackingPokemon->HealCurrentHP(leechedHealth);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
	
	if (!ctx.defendingPokemon->HasSubstitute() && leechedHealth > 0)
	{
		deps.resultsUI.DisplayEnergyDrainedMsg(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
	}

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);
}

void LeechSeed::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	bool isImmune = ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Grass || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Grass;

	if (isImmune)
	{
		deps.resultsUI.DisplayDoesntAffectMsg(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		return;
	}

	if (ctx.defendingPokemon->IsSeeded() || ctx.defendingPokemon->HasSubstitute())
	{
		deps.resultsUI.DisplayFailedTextDialog();
		return;
	}

	ctx.defendingPokemon->SetSeededStatus(true);
	
	deps.resultsUI.DisplaySeededMsg(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
}

void Growth::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	int attackStage = ctx.attackingPokemon->GetAttackStage();
	int specialAttackStage = ctx.attackingPokemon->GetSpecialAttackStage();

	if (attackStage >= 6)
	{
		deps.resultsUI.DisplayStatRaiseFailMsg("attack", ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
	}
	else
	{
		++attackStage;
		ctx.attackingPokemon->SetAttackStage(attackStage);
		deps.resultsUI.DisplayStatRaised1Msg("attack", ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
	}

	if (specialAttackStage >= 6)
	{
		deps.resultsUI.DisplayStatRaiseFailMsg("special attack", ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
	}
	else
	{
		++specialAttackStage;
		ctx.attackingPokemon->SetSpecialAttackStage(specialAttackStage);
		deps.resultsUI.DisplayStatRaised1Msg("special attack", ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
	}
}

void SolarBeam::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	if (!ctx.attackingPokemon->IsCharging())
	{
		deps.resultsUI.DisplaySolarBeamChargeMsg(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		ctx.attackingPokemon->SetCharging(true);
		ctx.attackingPlayer->SetCanSwitch(false);
		return;
	}

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	ctx.attackingPokemon->SetCharging(false);
	ctx.attackingPlayer->SetCanSwitch(true);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void PoisonPowder::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	bool isImmune = ((ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Grass || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Grass) ||
		(ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Poison || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Poison) ||
		(ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Steel || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Steel));

	if (isImmune)
	{
		deps.resultsUI.DisplayDoesntAffectMsg(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		return;
	}

	if (ctx.defendingPokemon->GetStatus() == Status::Poisoned || ctx.defendingPokemon->GetStatus() == Status::Badly_Poisoned)
	{
		deps.resultsUI.DisplayAlreadyPoisonedMsg(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		return;
	}

	if (ctx.defendingPokemon->GetStatus() != Status::Normal || (ctx.defendingPokemon->HasSubstitute() && !ctx.currentMove->CanBypassSubstitute()))
	{
		deps.resultsUI.DisplayFailedTextDialog();
		return;
	}

	InflictNVStatus(Status::Poisoned, 100, deps);
}

void StunSpore::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	bool isImmune = ((ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Grass || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Grass) ||
		(ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Electric || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Electric));

	if (isImmune)
	{
		deps.resultsUI.DisplayDoesntAffectMsg(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		return;
	}

	if (ctx.defendingPokemon->GetStatus() == Status::Paralyzed)
	{
		deps.resultsUI.DisplayAlreadyParalyzedMsg(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		return;
	}

	if ((ctx.defendingPokemon->GetStatus() != Status::Paralyzed && ctx.defendingPokemon->GetStatus() != Status::Normal) || ctx.defendingPokemon->HasSubstitute())
	{
		deps.resultsUI.DisplayFailedTextDialog();
		return;
	}

	InflictNVStatus(Status::Paralyzed, 100, deps);
}

void SleepPowder::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	bool isImmune = (ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Grass || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Grass);

	if (isImmune)
	{
		deps.resultsUI.DisplayDoesntAffectMsg(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		return;
	}

	if (ctx.defendingPokemon->GetStatus() == Status::Sleeping)
	{
		deps.resultsUI.DisplayAlreadyAsleepMsg(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		return;
	}

	if ((ctx.defendingPokemon->GetStatus() != Status::Sleeping && ctx.defendingPokemon->GetStatus() != Status::Normal) || ctx.defendingPokemon->HasSubstitute())
	{
		deps.resultsUI.DisplayFailedTextDialog();
		return;
	}

	InflictNVStatus(Status::Sleeping, 100, deps);
}

void SpeedDown2::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	if (ctx.flags.hit && ctx.defendingPokemon->HasSubstitute() && !ctx.currentMove->CanBypassSubstitute())
	{
		deps.resultsUI.DisplayFailedTextDialog();
		return;
	}

	if (ctx.defendingPlayer->HasMist() && ctx.flags.hit)
	{
		deps.resultsUI.DisplayProtectedByMistMsg(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		return;
	}

	if (ctx.defendingPokemon->GetSpeedStage() < -5)
	{
		deps.resultsUI.DisplayStatLoweredFailMsg("speed", ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
	}
	else if (ctx.defendingPokemon->GetSpeedStage() == -5)
	{
		int lowerSpeed{ ctx.defendingPokemon->GetSpeedStage() - 1 };
		ctx.defendingPokemon->SetSpeedStage(lowerSpeed);

		deps.resultsUI.DisplayStatLowered1Msg("speed", ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
	}
	else // GetSpeedStage() > -5
	{
		int lowerSpeed{ ctx.defendingPokemon->GetSpeedStage() - 2 };
		ctx.defendingPokemon->SetSpeedStage(lowerSpeed);

		deps.resultsUI.DisplayStatLowered2Msg("speed", ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
	}
}

void DragonRage::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	const int baseDamage = 40;

	bool hasSubstitute = ctx.defendingPokemon->HasSubstitute();

	int maxDamage = hasSubstitute ? ctx.defendingPokemon->GetSubstituteHP() : ctx.defendingPokemon->GetCurrentHP();

	int finalDamage = std::min(baseDamage, maxDamage);

	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, finalDamage);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void Paralyze::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	bool isElectricType = (ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Electric ||
		ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Electric);

	bool isImmune = (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No || isElectricType);

	// Special case: Glare affects Ghosts despite being Normal-type
	if (ctx.currentMove->GetName() == "Glare")
	{
		bool isGhostType = (ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Ghost ||
			ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Ghost);

		if (isGhostType && !isElectricType)
			isImmune = false;
	}

	if (isImmune)
	{
		deps.resultsUI.DisplayDoesntAffectMsg(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		return;
	}

	if (ctx.defendingPokemon->GetStatus() == Status::Paralyzed)
	{
		deps.resultsUI.DisplayAlreadyParalyzedMsg(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		return;
	}

	if (ctx.defendingPokemon->GetStatus() != Status::Normal || (ctx.defendingPokemon->HasSubstitute() && !ctx.currentMove->CanBypassSubstitute()))
	{
		deps.resultsUI.DisplayFailedTextDialog();
		return;
	}

	InflictNVStatus(Status::Paralyzed, 100, deps);
}

void Earthquake::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	// Damage multiplier for when defending Pokemon is SemiInvulnerableDig is in CalculateDamage()
	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void Dig::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	if (!ctx.attackingPokemon->IsCharging())
	{
		deps.resultsUI.DisplayDigChargeMsg(ctx.attackingPokemon->GetNameView());
		ctx.attackingPokemon->SetCharging(true);
		ctx.attackingPokemon->SetSemiInvulnerableDig(true);
		ctx.attackingPlayer->SetCanSwitch(false);
		return;
	}

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	ctx.attackingPokemon->SetCharging(false);
	ctx.attackingPokemon->SetSemiInvulnerableDig(false);
	ctx.attackingPlayer->SetCanSwitch(true);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void Toxic::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	bool isImmune = ((ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Poison || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Poison) ||
		(ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Steel || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Steel));

	if (isImmune)
	{
		deps.resultsUI.DisplayDoesntAffectMsg(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		return;
	}

	if (ctx.defendingPokemon->GetStatus() == Status::Poisoned || ctx.defendingPokemon->GetStatus() == Status::Badly_Poisoned)
	{
		deps.resultsUI.DisplayAlreadyPoisonedMsg(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		return;
	}

	if (ctx.defendingPokemon->GetStatus() != Status::Normal || (ctx.defendingPokemon->HasSubstitute() && !ctx.currentMove->CanBypassSubstitute()))
	{
		deps.resultsUI.DisplayFailedTextDialog();
		return;
	}

	InflictNVStatus(Status::Badly_Poisoned, 100, deps);
}

void AttackUp::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	int attackStage = ctx.attackingPokemon->GetAttackStage();

	if (attackStage >= 6)
	{
		deps.resultsUI.DisplayStatRaiseFailMsg("attack", ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
	}
	else
	{
		++attackStage;
		ctx.attackingPokemon->SetAttackStage(attackStage);
		deps.resultsUI.DisplayStatRaised1Msg("attack", ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
	}
}

void SpeedUp2::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	int speedStage = ctx.attackingPokemon->GetSpeedStage();

	if (speedStage >= 6)
	{
		deps.resultsUI.DisplayStatRaiseFailMsg("speed", ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
	}
	else if (speedStage == 5)
	{
		++speedStage;
		ctx.attackingPokemon->SetSpeedStage(speedStage);
		deps.resultsUI.DisplayStatRaised1Msg("speed", ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
	}
	else // speedStage < 5
	{
		speedStage += 2;
		ctx.attackingPokemon->SetSpeedStage(speedStage);
		deps.resultsUI.DisplayStatRaised2Msg("speed", ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
	}
}

void Rage::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
	ctx.attackingPokemon->SetRaging(true);
}

void Teleport::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	if (ctx.attackingPlayer->GetPokemonCount() == 1 || !ctx.attackingPlayer->IsHuman())
	{
		deps.resultsUI.DisplayFailedTextDialog();
		return;
	}

	deps.battleUI.SwitchPokemonOption(*ctx.attackingPlayer, *ctx.attackingPokemon);
	ctx.attackingPlayer->SetPokemonToSwitchTo(deps.battleUI.GetChosenPokemon());

	deps.switchExecutor.ExecuteSwitch(ctx.attackingPlayer, ctx.attackingPokemon);
}

void NightShade::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	const int baseDamage = ctx.defendingPokemon->GetLevel();

	bool hasSubstitute = ctx.defendingPokemon->HasSubstitute();

	int maxDamage = hasSubstitute ? ctx.defendingPokemon->GetSubstituteHP() : ctx.defendingPokemon->GetCurrentHP();

	int finalDamage = std::min(baseDamage, maxDamage);

	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, finalDamage);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void Mimic::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	if (ctx.defendingPokemon->GetLastUsedMove() == nullptr)
	{
		deps.resultsUI.DisplayFailedTextDialog();
		return;
	}

	if (ctx.defendingPokemon->GetLastUsedMove()->GetName() == "Transform" ||
		ctx.defendingPokemon->GetLastUsedMove()->GetName() == "Struggle" ||
		ctx.defendingPokemon->GetLastUsedMove()->GetName() == "Metronome")
	{
		deps.resultsUI.DisplayFailedTextDialog();
		return;
	}

	ctx.flags.hit = !ctx.defendingPokemon->IsSemiInvulnerable();

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		return;
	}

	ctx.attackingPokemon->SetUsedMimic(true);
	ctx.attackingPokemon->SetMimicPP(ctx.currentMove->m_currentPP);

	deps.resultsUI.DisplayLearnedMimicMoveMsg(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.defendingPokemon->GetLastUsedMove()->GetName());

	ctx.currentMove->SetMovePointer(Database::GetInstance().GetPointerToMovedexNumber(
		ctx.defendingPokemon->GetLastUsedMove()->GetMoveIndex() - 1));
	ctx.currentMove->m_currentPP = ctx.defendingPokemon->GetLastUsedMove()->GetPP();
	ctx.currentMove->m_maxPP = ctx.defendingPokemon->GetLastUsedMove()->GetPP();
	ctx.currentMove->b_isMimicked = true;
}

void DefenseDown2::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	if (ctx.flags.hit && ctx.defendingPokemon->HasSubstitute() && !ctx.currentMove->CanBypassSubstitute())
	{
		deps.resultsUI.DisplayFailedTextDialog();
		return;
	}

	if (ctx.defendingPlayer->HasMist() && ctx.flags.hit)
	{
		deps.resultsUI.DisplayProtectedByMistMsg(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		return;
	}

	if (ctx.defendingPokemon->GetDefenseStage() <= -6)
	{
		deps.resultsUI.DisplayStatLoweredFailMsg("defense", ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
	}
	else if (ctx.defendingPokemon->GetDefenseStage() == -5)
	{
		int lowerDefense = ctx.defendingPokemon->GetDefenseStage() - 1;
		ctx.defendingPokemon->SetDefenseStage(lowerDefense);

		deps.resultsUI.DisplayStatLowered1Msg("defense", ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
	}
	else // GetDefenseStage() > -5
	{
		int lowerDefense = ctx.defendingPokemon->GetDefenseStage() - 2;
		ctx.defendingPokemon->SetDefenseStage(lowerDefense);

		deps.resultsUI.DisplayStatLowered2Msg("defense", ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
	}
}

void EvasionUp::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	int evasionStage = ctx.attackingPokemon->GetEvasionStage();

	if (evasionStage >= 6)
	{
		deps.resultsUI.DisplayStatRaiseFailMsg("evasiveness", ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
	}
	else
	{
		++evasionStage;
		ctx.attackingPokemon->SetEvasionStage(evasionStage);
		deps.resultsUI.DisplayStatRaised1Msg("evasiveness", ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
	}
}

void HealHalfHP::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	if (ctx.attackingPokemon->GetCurrentHP() < ctx.attackingPokemon->GetMaxHP())
	{
		int healAmount = (ctx.attackingPokemon->GetMaxHP() + 1) / 2;

		ctx.attackingPokemon->HealCurrentHP(healAmount);

		deps.resultsUI.DisplayRecoveredHPRestoredMsg(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
	}

	else
	{
		deps.resultsUI.DisplayHPFullMsg(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
	}
}

void DefenseUp::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	int defenseStage = ctx.attackingPokemon->GetDefenseStage();

	if (defenseStage >= 6)
	{
		deps.resultsUI.DisplayStatRaiseFailMsg("defense", ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
	}
	else
	{
		++defenseStage;
		ctx.attackingPokemon->SetDefenseStage(defenseStage);
		deps.resultsUI.DisplayStatRaised1Msg("defense", ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
	}
}

void Minimize::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	int evasionStage = ctx.attackingPokemon->GetEvasionStage();

	if (evasionStage >= 6)
	{
		deps.resultsUI.DisplayStatRaiseFailMsg("evasiveness", ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
	}
	else if (evasionStage == 5)
	{
		++evasionStage;
		ctx.attackingPokemon->SetEvasionStage(evasionStage);
		deps.resultsUI.DisplayStatRaised1Msg("evasiveness", ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		ctx.attackingPokemon->SetUsedMinimize(true);
	}
	else // evasionStage < 5
	{
		evasionStage += 2;
		ctx.attackingPokemon->SetEvasionStage(evasionStage);
		deps.resultsUI.DisplayStatRaised2Msg("evasiveness", ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		ctx.attackingPokemon->SetUsedMinimize(true);
	}
}

void DefenseUp2::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	int defenseStage = ctx.attackingPokemon->GetDefenseStage();

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	if (defenseStage >= 6)
	{
		deps.resultsUI.DisplayStatRaiseFailMsg("defense", ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
	}
	else if (defenseStage == 5)
	{
		++defenseStage;
		ctx.attackingPokemon->SetDefenseStage(defenseStage);
		deps.resultsUI.DisplayStatRaised1Msg("defense", ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
	}
	else // defenseStage < 5
	{
		defenseStage += 2;
		ctx.attackingPokemon->SetDefenseStage(defenseStage);
		deps.resultsUI.DisplayStatRaised2Msg("defense", ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
	}
}

void LightScreen::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	if (ctx.attackingPlayer->HasLightScreen())
	{
		deps.resultsUI.DisplayFailedTextDialog();
	}
	else
	{
		ctx.attackingPlayer->SetLightScreen(true);
		deps.resultsUI.DisplayLightScreenMsg(ctx.attackingPlayer->GetPlayerNameView());
	}
}

void Haze::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	ctx.attackingPokemon->SetAttackStage(0);
	ctx.attackingPokemon->SetDefenseStage(0);
	ctx.attackingPokemon->SetSpecialAttackStage(0);
	ctx.attackingPokemon->SetSpecialDefenseStage(0);
	ctx.attackingPokemon->SetSpeedStage(0);
	ctx.attackingPokemon->SetEvasionStage(0);
	ctx.attackingPokemon->SetAccuracyStage(0);

	ctx.defendingPokemon->SetAttackStage(0);
	ctx.defendingPokemon->SetDefenseStage(0);
	ctx.defendingPokemon->SetSpecialAttackStage(0);
	ctx.defendingPokemon->SetSpecialDefenseStage(0);
	ctx.defendingPokemon->SetSpeedStage(0);
	ctx.defendingPokemon->SetEvasionStage(0);
	ctx.defendingPokemon->SetAccuracyStage(0);

	deps.resultsUI.DisplayHazeMsg();
}

void Reflect::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	if (ctx.attackingPlayer->HasReflect())
	{
		deps.resultsUI.DisplayFailedTextDialog();
	}
	else
	{
		ctx.attackingPlayer->SetReflect(true);
		deps.resultsUI.DisplayReflectMsg(ctx.attackingPlayer->GetPlayerNameView());
	}
}

void FocusEnergy::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	if (ctx.attackingPokemon->HasFocusEnergy())
	{
		deps.resultsUI.DisplayFailedTextDialog();
	}
	else
	{
		ctx.attackingPokemon->SetFocusEnergy(true);
		ctx.attackingPokemon->SetCriticalHitStage(2);
		deps.resultsUI.DisplayFocusEnergyMsg(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
	}
}

void Bide::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	auto endBide = [&]() {
		deps.statusProcessor.BideStop();
		deps.statusProcessor.BideReset();
		};

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	if (ctx.currentMove->b_isDisabled)
	{
		deps.resultsUI.DisplayBideDisabledMsg(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());

		endBide;

		return;
	}

	if (ctx.attackingPokemon->IsBiding())
	{
		ctx.attackingPokemon->IncrementBideCounter();
		deps.resultsUI.DisplayBideStoringEnergyMsg(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		
		return;
	}

	if (!ctx.attackingPokemon->IsBiding())
	{
		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.attackingPokemon->SetBide(true);
		ctx.attackingPlayer->SetCanSwitch(false);

		ctx.attackingPokemon->SetBideTurnCount(2);
		ctx.attackingPokemon->ResetBideCounter();

		ctx.currentMove->DeductPP();
	}

	if (ctx.attackingPokemon->GetBideCounter() >= ctx.attackingPokemon->GetBideTurnCount())
	{		
		deps.resultsUI.DisplayBideUnleashedMsg(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());

		if (ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Ghost ||
			ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Ghost)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));

			endBide;

			return;
		}

		ctx.flags.hit = !ctx.defendingPokemon->IsSemiInvulnerable();

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());

			endBide;

			return;
		}

		int bideDamage = ctx.attackingPokemon->GetBideDamage() * 2;

		bool hasSubstitute = ctx.defendingPokemon->HasSubstitute();

		int maxDamage = hasSubstitute ? ctx.defendingPokemon->GetSubstituteHP() : ctx.defendingPokemon->GetCurrentHP();

		int finalDamage = std::min(bideDamage, maxDamage);

		if (hasSubstitute)
		{
			ctx.defendingPokemon->DamageSubstitute(finalDamage);

			deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);
			deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
			ctx.flags.hitSubstitute = true;
		}

		else
		{
			ctx.defendingPokemon->DamageCurrentHP(finalDamage);
			ctx.flags.hitSubstitute = false;
		}

		ctx.damageTaken = finalDamage;

		deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

		endBide;
	}
}

void Metronome::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	size_t randomMod{ 67 };
	while (randomMod == 67 || randomMod == 101 || randomMod == 117 || randomMod == 118 || randomMod == 143)
	{
		std::uniform_int_distribution<size_t> randomModDistributor(0, 163);
		randomMod = randomModDistributor(deps.rng.GetGenerator());
	}

	Move* selectedMove = Database::GetInstance().GetPointerToMovedexNumber(randomMod);

	deps.resultsUI.DisplayMetronomeMsg(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), selectedMove->GetName());

	ctx.attackingPokemon->metronomeMove.SetMovePointer(selectedMove);
	ctx.attackingPokemon->metronomeMove.m_currentPP = 1;
	ctx.attackingPokemon->metronomeMove.m_maxPP = 1;

	{
		Execute(deps.context.currentMove->GetMoveEffectEnum(), deps);
	}
}

void MirrorMove::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	BattlePokemon::pokemonMove* targetLastUsedMove = ctx.defendingPokemon->GetLastUsedMove();

	if (!targetLastUsedMove || !targetLastUsedMove->IsAffectedByMirrorMove())
	{
		deps.resultsUI.DisplayFailedTextDialog();
		return;
	}

	Move* selectedMove = Database::GetInstance().GetPointerToMovedexNumber(targetLastUsedMove->GetMoveIndex() - 1);

	ctx.attackingPokemon->mirrorMove.SetMovePointer(selectedMove);
	ctx.attackingPokemon->mirrorMove.m_currentPP = 1;
	ctx.attackingPokemon->mirrorMove.m_maxPP = 1;

	{
		Execute(deps.context.currentMove->GetMoveEffectEnum(), deps);
	}
}

void Explosion::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));

		ctx.attackingPokemon->DamageCurrentHP(ctx.attackingPokemon->GetCurrentHP());

		deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());

		ctx.attackingPokemon->DamageCurrentHP(ctx.attackingPokemon->GetCurrentHP());

		deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);

	ctx.attackingPokemon->DamageCurrentHP(ctx.attackingPokemon->GetCurrentHP());

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);
}

void AlwaysHit::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	ctx.flags.hit = !ctx.defendingPokemon->IsSemiInvulnerable();

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void SkullBash::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	if (!ctx.attackingPokemon->IsCharging())
	{
		deps.resultsUI.DisplaySkullBashChargeMsg(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		ctx.attackingPokemon->SetCharging(true);
		ctx.attackingPlayer->SetCanSwitch(false);

		int defenseStage = ctx.attackingPokemon->GetDefenseStage();

		if (defenseStage >= 6)
		{
			deps.resultsUI.DisplayStatRaiseFailMsg("defense", ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		}
		else
		{
			++defenseStage;
			ctx.attackingPokemon->SetDefenseStage(defenseStage);
			deps.resultsUI.DisplayStatRaised1Msg("defense", ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		}

		return;
	}

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	ctx.attackingPokemon->SetCharging(false);
	ctx.attackingPlayer->SetCanSwitch(true);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void SpecialDefenseUp2::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	int specialDefenseStage = ctx.attackingPokemon->GetSpecialDefenseStage();

	if (specialDefenseStage >= 6)
	{
		deps.resultsUI.DisplayStatRaiseFailMsg("special defense", ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
	}
	else if (specialDefenseStage == 5)
	{
		++specialDefenseStage;
		ctx.attackingPokemon->SetSpecialDefenseStage(specialDefenseStage);
		deps.resultsUI.DisplayStatRaised1Msg("special defense", ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
	}
	else // specialDefenseStage < 5
	{
		specialDefenseStage += 2;
		ctx.attackingPokemon->SetSpecialDefenseStage(specialDefenseStage);
		deps.resultsUI.DisplayStatRaised2Msg("special defense", ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
	}
}

void DreamEater::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.defendingPokemon->GetStatus() != Status::Sleeping || ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayDoesntAffectMsg(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	int leechedHealth{};

	if (ctx.damageTaken <= 0)
	{
		leechedHealth = 0;
	}
	else if (ctx.damageTaken == 1)
	{
		leechedHealth = 1;
	}
	else
	{
		leechedHealth = ctx.damageTaken / 2;
	}

	if (leechedHealth > (ctx.attackingPokemon->GetMaxHP() - ctx.attackingPokemon->GetCurrentHP()))
	{
		leechedHealth = (ctx.attackingPokemon->GetMaxHP() - ctx.attackingPokemon->GetCurrentHP());
	}

	ctx.attackingPokemon->HealCurrentHP(leechedHealth);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);

	if (leechedHealth > 0)
	{
		deps.resultsUI.DisplayEnergyDrainedMsg(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
	}

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);
}

void PoisonGas::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	bool isImmune = ((ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Poison || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Poison) ||
		(ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Steel || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Steel));

	if (isImmune)
	{
		deps.resultsUI.DisplayDoesntAffectMsg(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		return;
	}

	if (ctx.defendingPokemon->GetStatus() == Status::Poisoned || ctx.defendingPokemon->GetStatus() == Status::Badly_Poisoned)
	{
		deps.resultsUI.DisplayAlreadyPoisonedMsg(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		return;
	}

	if ((ctx.defendingPokemon->GetStatus() != Status::Poisoned && ctx.defendingPokemon->GetStatus() != Status::Normal) || ctx.defendingPokemon->HasSubstitute())
	{
		deps.resultsUI.DisplayFailedTextDialog();
		return;
	}

	InflictNVStatus(Status::Poisoned, 100, deps);
}

void SkyAttack::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	if (!ctx.attackingPokemon->IsCharging())
	{
		deps.resultsUI.DisplaySkyAttackChargeMsg(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		ctx.attackingPokemon->SetCharging(true);
		ctx.attackingPlayer->SetCanSwitch(false);
		return;
	}

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	ctx.attackingPokemon->SetCharging(false);
	ctx.attackingPlayer->SetCanSwitch(true);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	int oldCritStage = ctx.attackingPokemon->GetCriticalHitStage();
	int newCritStage = oldCritStage + 1;

	ctx.attackingPokemon->SetCriticalHitStage(newCritStage);

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	ctx.attackingPokemon->SetCriticalHitStage(oldCritStage);

	std::uniform_int_distribution<int> randomModDistributor(1, 100);
	int randomMod = randomModDistributor(deps.rng.GetGenerator());

	if ((!ctx.defendingPokemon->HasSubstitute() || ctx.currentMove->CanBypassSubstitute()) &&
		ctx.defendingPokemon->GetCurrentHP() != 0 &&
		!ctx.defendingPlayer->IsFirst() &&
		randomMod <= ctx.currentMove->GetEffectChance())
	{
		ctx.defendingPokemon->SetIsFlinched(true);
	}

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void Transform::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	if (ctx.attackingPokemon->IsTransformed() || ctx.defendingPokemon->IsTransformed() || ctx.defendingPokemon->HasSubstitute())
	{
		deps.resultsUI.DisplayFailedTextDialog();
		return;
	}

	ctx.flags.hit = !ctx.defendingPokemon->IsSemiInvulnerable();

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		return;
	}

	ctx.attackingPokemon->SetTransformation(ctx.defendingPokemon);
	deps.resultsUI.DisplayTransformMsg(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.defendingPokemon->GetNameView());
}

void Psywave::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	int psywaveDamage = 0;

	std::uniform_int_distribution<int> rngDist(0, 100);
	int randomNumber{ rngDist(deps.rng.GetGenerator()) };

	psywaveDamage = ctx.attackingPokemon->GetLevel() * (randomNumber + 50) / 100;

	if (psywaveDamage == 0)
	{
		psywaveDamage = 1;
	}

	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, psywaveDamage);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void Splash::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.resultsUI.DisplaySplashMsg();
}

void Rest::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	if (ctx.attackingPokemon->GetStatus() != Status::Sleeping &&
		ctx.attackingPokemon->GetCurrentHP() < ctx.attackingPokemon->GetMaxHP())
	{
		deps.resultsUI.DisplayRestMsg(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());

		ctx.attackingPokemon->ChangeStatus(Status::Sleeping);

		ctx.attackingPokemon->SetSleepTurnCount(2);
		ctx.attackingPokemon->ResetSleepCounter();

		int healAmount = ctx.attackingPokemon->GetMaxHP() - ctx.attackingPokemon->GetCurrentHP();
		ctx.attackingPokemon->HealCurrentHP(healAmount);
	}
	else if (ctx.attackingPokemon->GetCurrentHP() >= ctx.attackingPokemon->GetMaxHP())
	{
		deps.resultsUI.DisplayHPFullMsg(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
	}
}

void Conversion::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	if (ctx.attackingPokemon->GetTypeOneEnum() == ctx.currentMove->GetMoveTypeEnum() ||
		ctx.attackingPokemon->GetTypeTwoEnum() == ctx.currentMove->GetMoveTypeEnum() ||
		ctx.attackingPokemon->IsConverted())
	{
		deps.resultsUI.DisplayFailedTextDialog();
	}
	else
	{
		ctx.attackingPokemon->SetConversion(ctx.currentMove);

		deps.resultsUI.DisplayConversionMsg(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.attackingPokemon->GetTypeOne());
	}

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void TriAttack::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	std::uniform_int_distribution<int> randomModDistributor(1, 100);
	int randomNumber{ randomModDistributor(deps.rng.GetGenerator()) };

	if (randomNumber <= ctx.currentMove->GetEffectChance() &&
		(!ctx.defendingPokemon->HasSubstitute() || ctx.currentMove->CanBypassSubstitute()) &&
		ctx.defendingPokemon->GetCurrentHP() != 0)
	{
		std::uniform_int_distribution<int> statusDist(1, 3);
		int randomStatus{ statusDist(deps.rng.GetGenerator()) };

		switch (randomStatus)
		{
		case 1:
			InflictNVStatus(Status::Burned, 100, deps);
			break;

		case 2:
			InflictNVStatus(Status::Frozen, 100, deps);
			break;

		case 3:
			InflictNVStatus(Status::Paralyzed, 100, deps);
			break;

		default:
			break;
		}
	}

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void SuperFang::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	bool hasSubstitute = ctx.defendingPokemon->HasSubstitute();

	int hpSource = hasSubstitute ? ctx.defendingPokemon->GetSubstituteHP() : ctx.defendingPokemon->GetCurrentHP();

	int finalDamage = std::max(1, hpSource / 2);

	if (hasSubstitute)
	{
		ctx.defendingPokemon->DamageSubstitute(finalDamage);

		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);
		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		ctx.flags.hitSubstitute = true;
	}
	else
	{
		ctx.defendingPokemon->DamageCurrentHP(finalDamage);
		ctx.flags.hitSubstitute = false;
	}

	ctx.damageTaken = finalDamage;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);
}

void Substitute::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.currentMove->DeductPP();

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	if (ctx.attackingPokemon->HasSubstitute())
	{
		deps.resultsUI.DisplayAlreadyHasSubstituteMsg(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	int substituteHP = ctx.attackingPokemon->GetMaxHP() / 4;

	if (substituteHP >= ctx.attackingPokemon->GetCurrentHP())
	{
		deps.resultsUI.DisplayNotEnoughHPSubstituteMsg();
		return;
	}

	int hpCost = substituteHP;

	deps.resultsUI.DisplayPutInSubstituteMsg(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
	ctx.attackingPokemon->SetSubstitute(true);
	ctx.attackingPokemon->SetSubstituteHP(hpCost);
	ctx.attackingPokemon->DamageCurrentHP(hpCost);
	ctx.attackingPokemon->SetBound(false);
	ctx.attackingPokemon->ResetBoundCounter();
	ctx.attackingPokemon->SetBoundTurnCount(0);
}

void Struggle::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.DisplayNoMovesLeftStruggleMsg(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		return;
	}

	ctx.flags.hit = !ctx.defendingPokemon->IsSemiInvulnerable();

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog(ctx.flags.isCriticalHit);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);

	int recoilDamage = (ctx.attackingPokemon->GetMaxHP() + 2) / 4;

	ctx.attackingPokemon->DamageCurrentHP(recoilDamage);

	deps.resultsUI.DisplayRecoilMsg(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);
}