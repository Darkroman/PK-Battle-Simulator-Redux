#include <deque>

#include "../data/Database.h"
#include "../battle/BattleContext.h"
#include "../battle/BattleCalculations.h"
#include "../battle/StatusEffectProcessor.h"
#include "../ui/interfaces/IMoveResultsUI.h"
#include "../ui/interfaces/IBattleMenuUI.h"
#include "../ui/interfaces/IStatusEffectUI.h"
#include "../battle/RandomEngine.h"
#include "../battle/SwitchExecutor.h"
#include "../data/Move.h"
#include "../data/StringToTypes.h"
#include "../moves/MoveEffectEnums.h"

#include "MoveRoutines.h"

void IMoveRoutine::InflictNVStatus(Status status, int chance, MoveRoutineDeps& deps)
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

	deps.statusUI.DisplayNVStatusMsg(statusMessage);
}

void IMoveRoutine::BasicDamageRoutine(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog();
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
}

std::unique_ptr<IMoveRoutine> MoveRoutineFactory::Call(MoveEffect ID)
{
	switch (ID)
	{
	case MoveEffect::NormalHit:
		return std::make_unique<NormalHit>();

	case MoveEffect::IncreasedCritical:
		return std::make_unique<IncreasedCritical>();

	case MoveEffect::MultiAttack:
		return std::make_unique<MultiAttack>();

	case MoveEffect::BurnHit:
		return std::make_unique<BurnHit>();

	case MoveEffect::FreezeHit:
		return std::make_unique<FreezeHit>();

	case MoveEffect::ParalyzeHit:
		return std::make_unique<ParalyzeHit>();

	case MoveEffect::OHKO:
		return std::make_unique<OHKO>();

	case MoveEffect::RazorWind:
		return std::make_unique<RazorWind>();

	case MoveEffect::AttackUp2:
		return std::make_unique<AttackUp2>();

	case MoveEffect::Gust:
		return std::make_unique<Gust>();

	case MoveEffect::MakeEnemySwitch:
		return std::make_unique<MakeEnemySwitch>();

	case MoveEffect::Fly:
		return std::make_unique<Fly>();

	case MoveEffect::Bound:
		return std::make_unique<Bound>();

	case MoveEffect::Stomp:
		return std::make_unique<Stomp>();

	case MoveEffect::DoubleHit:
		return std::make_unique<DoubleHit>();

	case MoveEffect::JumpKick:
		return std::make_unique<JumpKick>();

	case MoveEffect::FlinchHit:
		return std::make_unique<FlinchHit>();

	case MoveEffect::AccuracyDown:
		return std::make_unique<AccuracyDown>();

	case MoveEffect::BodySlam:
		return std::make_unique<BodySlam>();

	case MoveEffect::RecoilQuarter:
		return std::make_unique<RecoilQuarter>();

	case MoveEffect::Thrash:
		return std::make_unique<Thrash>();

	case MoveEffect::RecoilThird:
		return std::make_unique<RecoilThird>();

	case MoveEffect::DefenseDown:
		return std::make_unique<DefenseDown>();

	case MoveEffect::PoisonHit:
		return std::make_unique<PoisonHit>();

	case MoveEffect::Twineedle:
		return std::make_unique<Twineedle>();

	case MoveEffect::AttackDown:
		return std::make_unique<AttackDown>();

	case MoveEffect::SleepMove:
		return std::make_unique<SleepMove>();

	case MoveEffect::Confuse:
		return std::make_unique<Confuse>();

	case MoveEffect::SonicBoom:
		return std::make_unique<SonicBoom>();

	case MoveEffect::Disable:
		return std::make_unique<Disable>();

	case MoveEffect::SpecialDefenseDownHit:
		return std::make_unique<SpecialDefenseDownHit>();

	case MoveEffect::Mist:
		return std::make_unique<Mist>();

	case MoveEffect::ConfuseHit:
		return std::make_unique<ConfuseHit>();

	case MoveEffect::SpeedDownHit:
		return std::make_unique<SpeedDownHit>();

	case MoveEffect::AttackDownHit:
		return std::make_unique<AttackDownHit>();

	case MoveEffect::RechargeAttack:
		return std::make_unique<RechargeAttack>();

	case MoveEffect::LowKick:
		return std::make_unique<LowKick>();

	case MoveEffect::Counter:
		return std::make_unique<Counter>();

	case MoveEffect::SeismicToss:
		return std::make_unique<SeismicToss>();

	case MoveEffect::Leech:
		return std::make_unique<Leech>();

	case MoveEffect::LeechSeed:
		return std::make_unique<LeechSeed>();

	case MoveEffect::Growth:
		return std::make_unique<Growth>();

	case MoveEffect::SolarBeam:
		return std::make_unique<SolarBeam>();

	case MoveEffect::PoisonPowder:
		return std::make_unique<PoisonPowder>();

	case MoveEffect::StunSpore:
		return std::make_unique<StunSpore>();

	case MoveEffect::SleepPowder:
		return std::make_unique<SleepPowder>();

	case MoveEffect::SpeedDown2:
		return std::make_unique<SpeedDown2>();

	case MoveEffect::DragonRage:
		return std::make_unique<DragonRage>();

	case MoveEffect::Paralyze:
		return std::make_unique<Paralyze>();

	case MoveEffect::Earthquake:
		return std::make_unique<Earthquake>();

	case MoveEffect::Dig:
		return std::make_unique<Dig>();

	case MoveEffect::Toxic:
		return std::make_unique<Toxic>();

	case MoveEffect::AttackUp:
		return std::make_unique<AttackUp>();

	case MoveEffect::SpeedUp2:
		return std::make_unique<SpeedUp2>();

	case MoveEffect::Rage:
		return std::make_unique<Rage>();

	case MoveEffect::Teleport:
		return std::make_unique<Teleport>();

	case MoveEffect::NightShade:
		return std::make_unique<NightShade>();

	case MoveEffect::Mimic:
		return std::make_unique<Mimic>();

	case MoveEffect::DefenseDown2:
		return std::make_unique<DefenseDown2>();

	case MoveEffect::EvasionUp:
		return std::make_unique<EvasionUp>();

	case MoveEffect::HealHalfHP:
		return std::make_unique<HealHalfHP>();

	case MoveEffect::DefenseUp:
		return std::make_unique<DefenseUp>();

	case MoveEffect::Minimize:
		return std::make_unique<Minimize>();

	case MoveEffect::DefenseUp2:
		return std::make_unique<DefenseUp2>();

	case MoveEffect::LightScreen:
		return std::make_unique<LightScreen>();

	case MoveEffect::Haze:
		return std::make_unique<Haze>();

	case MoveEffect::Reflect:
		return std::make_unique<Reflect>();

	case MoveEffect::FocusEnergy:
		return std::make_unique<FocusEnergy>();

	case MoveEffect::Bide:
		return std::make_unique<Bide>();

	case MoveEffect::Metronome:
		return std::make_unique<Metronome>();

	case MoveEffect::MirrorMove:
		return std::make_unique<MirrorMove>();

	case MoveEffect::Explosion:
		return std::make_unique<Explosion>();

	case MoveEffect::Swift:
		return std::make_unique<Swift>();

	case MoveEffect::SkullBash:
		return std::make_unique<SkullBash>();

	case MoveEffect::SpecialDefenseUp2:
		return std::make_unique<SpecialDefenseUp2>();

	case MoveEffect::DreamEater:
		return std::make_unique<DreamEater>();

	case MoveEffect::PoisonGas:
		return std::make_unique<PoisonGas>();

	case MoveEffect::SkyAttack:
		return std::make_unique<SkyAttack>();

	case MoveEffect::Transform:
		return std::make_unique<Transform>();

	case MoveEffect::Psywave:
		return std::make_unique<Psywave>();

	case MoveEffect::Splash:
		return std::make_unique<Splash>();

	case MoveEffect::Rest:
		return std::make_unique<Rest>();

	case MoveEffect::Conversion:
		return std::make_unique<Conversion>();

	case MoveEffect::TriAttack:
		return std::make_unique<TriAttack>();

	case MoveEffect::SuperFang:
		return std::make_unique<SuperFang>();

	case MoveEffect::Substitute:
		return std::make_unique<Substitute>();

	case MoveEffect::Struggle:
		return std::make_unique<Struggle>();

	default:
		return std::make_unique<Noop>();

	}
	return std::make_unique<Noop>();
}

void Noop::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.resultsUI.DisplayNoopMsg();
}

void NormalHit::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog();
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void IncreasedCritical::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		return;
	}

	int oldCritStage = ctx.attackingPokemon->GetCriticalHitStage();
	int newCritStage = oldCritStage + 1;

	ctx.attackingPokemon->SetCriticalHitStage(newCritStage);

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog();
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

	ctx.attackingPokemon->SetCriticalHitStage(oldCritStage);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void MultiAttack::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
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
		deps.resultsUI.DisplayCritTextDialog();
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);

		++timesHit;
		--turnCount;
	}

	if (timesHit > 1)
	{
		deps.resultsUI.DisplayMultiAttackMsg(timesHit);
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

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog();
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	InflictNVStatus(Status::Burned, ctx.currentMove->GetEffectChance(), deps);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void FreezeHit::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog();
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	InflictNVStatus(Status::Frozen, ctx.currentMove->GetEffectChance(), deps);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void ParalyzeHit::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog();
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	InflictNVStatus(Status::Paralyzed, ctx.currentMove->GetEffectChance(), deps);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void OHKO::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		return;
	}

	// OHKO specific logic done in CalculateDamage()
	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayCritTextDialog();
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void RazorWind::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	if (!ctx.attackingPokemon->IsCharging())
	{
		deps.resultsUI.DisplayRazorWindChargeMsg();
		ctx.attackingPokemon->SetCharging(true);
		ctx.attackingPlayer->SetCanSwitch(false);
		return;
	}

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	ctx.attackingPokemon->SetCharging(false);
	ctx.attackingPlayer->SetCanSwitch(true);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog();
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void AttackUp2::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	int attackStage = ctx.attackingPokemon->GetAttackStage();

	if (attackStage >= 6)
	{
		deps.statusUI.DisplayStatRaiseFailMsg("attack");
	}
	else if (attackStage == 5)
	{
		ctx.attackingPokemon->SetAttackStage(6);
		deps.statusUI.DisplayStatRaised1Msg("attack");
	}
	else // attackStage < 5
	{
		ctx.attackingPokemon->SetAttackStage(attackStage + 2);
		deps.statusUI.DisplayStatRaised2Msg("attack");
	}
}

void Gust::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}
	
	// Specific accuracy check for gust vs SemiInvulnerableFly done in CalculateHitChance()
	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		return;
	}

	if (ctx.defendingPokemon->IsSemiInvulnerableFromFly())
	{
		ctx.initialPowerMultiplier = 20;
	}
	
	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog();
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void MakeEnemySwitch::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

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
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
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
		deps.context.playerOneCurrentPokemon = newMon;
	}
	else
	{
		deps.context.playerTwoCurrentPokemon = newMon;
	}

	deps.resultsUI.DisplayEnemySwitchMsg(*newMon);
}

void Fly::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	if (!ctx.attackingPokemon->IsCharging())
	{
		deps.resultsUI.DisplayFlyChargeMsg();
		ctx.attackingPokemon->SetCharging(true);
		ctx.attackingPokemon->SetSemiInvulnerableFly(true);
		ctx.attackingPlayer->SetCanSwitch(false);
		return;
	}

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	ctx.attackingPokemon->SetCharging(false);
	ctx.attackingPokemon->SetSemiInvulnerableFly(false);
	ctx.attackingPlayer->SetCanSwitch(true);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog();
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void Bound::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog();
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

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

		deps.resultsUI.BoundMoveText(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon, ctx.currentMove);
	}

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void Stomp::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
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
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		return;
	}

	// Damage multiplier for when defending Pokemon has minimized is in CalculateDamage()
	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog();
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

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

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
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
		deps.resultsUI.DisplayCritTextDialog();
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);

		++timesHit;
		--turnCount;
	}

	if (timesHit > 1)
	{
		deps.resultsUI.DisplayMultiAttackMsg(timesHit);
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

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);

		int crashDamage = ctx.attackingPokemon->GetMaxHP() / 2;
		ctx.attackingPokemon->DamageCurrentHP(crashDamage);

		deps.resultsUI.DisplayJumpKickCrashMsg();
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog();
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void FlinchHit::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog();
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

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

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	if (ctx.flags.hit && ctx.defendingPokemon->HasSubstitute() && !ctx.currentMove->CanBypassSubstitute())
	{
		deps.resultsUI.DisplayFailedTextDialog();
		return;
	}

	if (ctx.defendingPlayer->HasMist() && ctx.flags.hit)
	{
		deps.statusUI.DisplayProtectedByMistMsg();
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	if (ctx.defendingPokemon->GetAccuracyStage() > -6)
	{
		int lowerAccuracy = ctx.defendingPokemon->GetAccuracyStage() - 1;
		ctx.defendingPokemon->SetAccuracyStage(lowerAccuracy);

		deps.statusUI.DisplayStatLowered1Msg("accuracy");
	}
	else
	{
		deps.statusUI.DisplayStatLoweredFailMsg("accuracy");
	}
}

void BodySlam::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
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
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		return;
	}

	// Damage multiplier for when defending Pokemon has minimized is in CalculateDamage()
	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog();
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	InflictNVStatus(Status::Paralyzed, ctx.currentMove->GetEffectChance(), deps);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void RecoilQuarter::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		return;
	}

	bool hitSubstitute = ctx.defendingPokemon->HasSubstitute() && !ctx.currentMove->CanBypassSubstitute();

	int targetHPBegin = hitSubstitute ? ctx.defendingPokemon->GetSubstituteHP() : ctx.defendingPokemon->GetCurrentHP();
	
	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog();
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);

	int targetHPEnd = hitSubstitute ? ctx.defendingPokemon->GetSubstituteHP() : ctx.defendingPokemon->GetCurrentHP();

	int recoilDamage = (targetHPBegin - targetHPEnd) / 4;

	ctx.attackingPokemon->DamageCurrentHP(recoilDamage);

	deps.resultsUI.DisplayRecoilMsg();

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);
}

void Thrash::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	auto endRampage = [&]() {
		deps.statusProcessor.ThrashStop();
		deps.statusProcessor.ThrashReset();
	};

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	if (ctx.attackingPokemon->IsThrashing())
	{
		ctx.attackingPokemon->IncrementThrashCounter();
	}

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

		if (ctx.attackingPokemon->GetThrashCounter() == ctx.attackingPokemon->GetThrashTurnCount() && !ctx.attackingPokemon->IsConfused())
		{
			deps.statusProcessor.ThrashConfuse();
		}

		endRampage;

		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit || ctx.currentMove->b_isDisabled)
	{
		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		}
		else
		{
			deps.statusUI.DisplayThrashDisabledMsg();
		}

		if (ctx.attackingPokemon->GetThrashCounter() == ctx.attackingPokemon->GetThrashTurnCount() && !ctx.attackingPokemon->IsConfused())
		{
			deps.statusProcessor.ThrashConfuse();
		}

		endRampage;

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

		ctx.currentMove->m_currentPP -= 1;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog();
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);

	if (ctx.attackingPokemon->GetThrashCounter() == ctx.attackingPokemon->GetThrashTurnCount() && !ctx.attackingPokemon->IsConfused())
	{
		deps.statusProcessor.ThrashConfuse();

		endRampage;
	}

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);
}

void RecoilThird::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		return;
	}

	bool hitSubstitute = ctx.defendingPokemon->HasSubstitute() && !ctx.currentMove->CanBypassSubstitute();

	int targetHPBegin = hitSubstitute ? ctx.defendingPokemon->GetSubstituteHP() : ctx.defendingPokemon->GetCurrentHP();

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog();
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);

	int targetHPEnd = hitSubstitute ? ctx.defendingPokemon->GetSubstituteHP() : ctx.defendingPokemon->GetCurrentHP();

	int recoilDamage = (targetHPBegin - targetHPEnd) / 3;

	ctx.attackingPokemon->DamageCurrentHP(recoilDamage);

	deps.resultsUI.DisplayRecoilMsg();

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);
}

void DefenseDown::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	if (ctx.flags.hit && ctx.defendingPokemon->HasSubstitute() && !ctx.currentMove->CanBypassSubstitute())
	{
		deps.resultsUI.DisplayFailedTextDialog();
		return;
	}

	if (ctx.defendingPlayer->HasMist() && ctx.flags.hit)
	{
		deps.statusUI.DisplayProtectedByMistMsg();
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	if (ctx.defendingPokemon->GetDefenseStage() > -6)
	{
		int lowerDefense = ctx.defendingPokemon->GetDefenseStage() - 1;
		ctx.defendingPokemon->SetDefenseStage(lowerDefense);

		deps.statusUI.DisplayStatLowered1Msg("defense");
	}
	else
	{
		deps.statusUI.DisplayStatLoweredFailMsg("defense");
	}
}

void PoisonHit::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog();
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	InflictNVStatus(Status::Poisoned, ctx.currentMove->GetEffectChance(), deps);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void Twineedle::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
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
		deps.resultsUI.DisplayCritTextDialog();
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

		InflictNVStatus(Status::Poisoned, ctx.currentMove->GetEffectChance(), deps);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);

		++timesHit;
		--turnCount;
	}

	if (timesHit > 1)
	{
		deps.resultsUI.DisplayMultiAttackMsg(timesHit);
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

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	if (ctx.flags.hit && ctx.defendingPokemon->HasSubstitute() && !ctx.currentMove->CanBypassSubstitute())
	{
		deps.resultsUI.DisplayFailedTextDialog();
		return;
	}

	if (ctx.defendingPlayer->HasMist() && ctx.flags.hit)
	{
		deps.statusUI.DisplayProtectedByMistMsg();
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	if (ctx.defendingPokemon->GetAttackStage() > -6)
	{
		int lowerAttack = ctx.defendingPokemon->GetAttackStage() - 1;
		ctx.defendingPokemon->SetAttackStage(lowerAttack);

		deps.statusUI.DisplayStatLowered1Msg("attack");
	}
	else
	{
		deps.statusUI.DisplayStatLoweredFailMsg("attack");
	}
}

void SleepMove::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	if (ctx.defendingPokemon->GetStatus() == Status::Sleeping)
	{
		deps.statusUI.DisplayAlreadyAsleepMsg();
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

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
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

	deps.statusUI.DisplayBecameConfuseMsg();
}

void SonicBoom::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		return;
	}

	const int baseDamage = 20;

	bool hasSubstitute = ctx.defendingPokemon->HasSubstitute();

	int maxDamage = hasSubstitute ? ctx.defendingPokemon->GetSubstituteHP() : ctx.defendingPokemon->GetCurrentHP();

	int finalDamage = std::min(baseDamage, maxDamage);

	if (hasSubstitute)
	{
		ctx.defendingPokemon->DamageSubstitute(finalDamage);

		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
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

void Disable::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
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

	deps.statusUI.DisplayMoveDisabledMsg();
	
}

void SpecialDefenseDownHit::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		return;
	}
		
	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog();
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

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

			deps.statusUI.DisplayStatLowered1Msg("special defense");
		}
	}

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void Mist::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	if (!ctx.attackingPlayer->HasMist())
	{
		deps.statusUI.DisplayMistMsg();
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

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog();
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.defendingPokemon->IsFainted() && !ctx.defendingPokemon->IsConfused())
	{
		std::uniform_int_distribution<int> randomModDistributor(1, 100);
		int randomNumber{ randomModDistributor(deps.rng.GetGenerator()) };

		if (randomNumber <= ctx.currentMove->GetEffectChance() && 
			(!ctx.defendingPokemon->HasSubstitute() || ctx.currentMove->CanBypassSubstitute()) &&
			ctx.defendingPokemon->GetCurrentHP() != 0)
		{
			deps.statusUI.DisplayBecameConfuseMsg();

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

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		return;
	}
		
	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog();
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

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

			deps.statusUI.DisplayStatLowered1Msg("speed");
		}
	}

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void AttackDownHit::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog();
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

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

			deps.statusUI.DisplayStatLowered1Msg("attack");
		}
	}

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void RechargeAttack::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	if (ctx.attackingPokemon->IsRecharging())
	{
		deps.resultsUI.DisplayRechargeMsg();
		ctx.attackingPokemon->SetRecharging(false);
		ctx.attackingPlayer->SetCanSwitch(true);
		return;
	}

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog();
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

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

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		return;
	}

	// Low Kick power calculated in CalculateDamage()
	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog();
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);
}

void Counter::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

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
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		return;
	}

	int counterDamage = ctx.damageTaken * 2;

	bool hasSubstitute = ctx.defendingPokemon->HasSubstitute();

	int maxDamage = hasSubstitute ? ctx.defendingPokemon->GetSubstituteHP() : ctx.defendingPokemon->GetCurrentHP();

	int finalDamage = std::min(counterDamage, maxDamage);

	if (hasSubstitute)
	{
		ctx.defendingPokemon->DamageSubstitute(finalDamage);

		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
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

void SeismicToss::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	const int baseDamage = ctx.defendingPokemon->GetLevel();

	bool hasSubstitute = ctx.defendingPokemon->HasSubstitute();

	int maxDamage = hasSubstitute ? ctx.defendingPokemon->GetSubstituteHP() : ctx.defendingPokemon->GetCurrentHP();

	int finalDamage = std::min(baseDamage, maxDamage);

	if (hasSubstitute)
	{
		ctx.defendingPokemon->DamageSubstitute(finalDamage);

		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
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

void Leech::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		return;
	}
	
	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog();
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

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
		deps.resultsUI.DisplayEnergyDrainedMsg();
	}

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);
}

void LeechSeed::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	bool isImmune = ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Grass || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Grass;

	if (isImmune)
	{
		deps.statusUI.DisplayDoesntAffectMsg();
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	if (ctx.defendingPokemon->IsSeeded() || ctx.defendingPokemon->HasSubstitute())
	{
		deps.resultsUI.DisplayFailedTextDialog();
		return;
	}

	ctx.defendingPokemon->SetSeededStatus(true);
	
	deps.statusUI.DisplaySeededMsg();
}

void Growth::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	int attackStage = ctx.attackingPokemon->GetAttackStage();
	int specialAttackStage = ctx.attackingPokemon->GetSpecialAttackStage();

	if (attackStage >= 6)
	{
		deps.statusUI.DisplayStatRaiseFailMsg("attack");
	}
	else
	{
		++attackStage;
		ctx.attackingPokemon->SetAttackStage(attackStage);
		deps.statusUI.DisplayStatRaised1Msg("attack");
	}

	if (specialAttackStage >= 6)
	{
		deps.statusUI.DisplayStatRaiseFailMsg("special attack");
	}
	else
	{
		++specialAttackStage;
		ctx.attackingPokemon->SetSpecialAttackStage(specialAttackStage);
		deps.statusUI.DisplayStatRaised1Msg("special attack");
	}
}

void SolarBeam::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	if (!ctx.attackingPokemon->IsCharging())
	{
		deps.resultsUI.DisplaySolarBeamChargeMsg();
		ctx.attackingPokemon->SetCharging(true);
		ctx.attackingPlayer->SetCanSwitch(false);
		return;
	}

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	ctx.attackingPokemon->SetCharging(false);
	ctx.attackingPlayer->SetCanSwitch(true);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog();
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void PoisonPowder::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	bool isImmune = ((ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Grass || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Grass) ||
		(ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Poison || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Poison) ||
		(ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Steel || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Steel));

	if (isImmune)
	{
		deps.statusUI.DisplayDoesntAffectMsg();
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	if (ctx.defendingPokemon->GetStatus() == Status::Poisoned || ctx.defendingPokemon->GetStatus() == Status::Badly_Poisoned)
	{
		deps.statusUI.DisplayAlreadyPoisonedMsg();
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

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	bool isImmune = ((ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Grass || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Grass) ||
		(ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Electric || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Electric));

	if (isImmune)
	{
		deps.statusUI.DisplayDoesntAffectMsg();
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	if (ctx.defendingPokemon->GetStatus() == Status::Paralyzed)
	{
		deps.statusUI.DisplayAlreadyParalyzedMsg();
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

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	bool isImmune = (ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Grass || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Grass);

	if (isImmune)
	{
		deps.statusUI.DisplayDoesntAffectMsg();
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	if (ctx.defendingPokemon->GetStatus() == Status::Sleeping)
	{
		deps.statusUI.DisplayAlreadyAsleepMsg();
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

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	if (ctx.flags.hit && ctx.defendingPokemon->HasSubstitute() && !ctx.currentMove->CanBypassSubstitute())
	{
		deps.resultsUI.DisplayFailedTextDialog();
		return;
	}

	if (ctx.defendingPlayer->HasMist() && ctx.flags.hit)
	{
		deps.statusUI.DisplayProtectedByMistMsg();
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	if (ctx.defendingPokemon->GetSpeedStage() < -5)
	{
		deps.statusUI.DisplayStatLoweredFailMsg("speed");
	}
	else if (ctx.defendingPokemon->GetSpeedStage() == -5)
	{
		int lowerSpeed{ ctx.defendingPokemon->GetSpeedStage() - 1 };
		ctx.defendingPokemon->SetSpeedStage(lowerSpeed);

		deps.statusUI.DisplayStatLowered1Msg("speed");
	}
	else // GetSpeedStage() > -5
	{
		int lowerSpeed{ ctx.defendingPokemon->GetSpeedStage() - 2 };
		ctx.defendingPokemon->SetSpeedStage(lowerSpeed);

		deps.statusUI.DisplayStatLowered2Msg("speed");
	}
}

void DragonRage::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		return;
	}

	const int baseDamage = 40;

	bool hasSubstitute = ctx.defendingPokemon->HasSubstitute();

	int maxDamage = hasSubstitute ? ctx.defendingPokemon->GetSubstituteHP() : ctx.defendingPokemon->GetCurrentHP();

	int finalDamage = std::min(baseDamage, maxDamage);

	if (hasSubstitute)
	{
		ctx.defendingPokemon->DamageSubstitute(finalDamage);

		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
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

void Paralyze::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

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
		deps.statusUI.DisplayDoesntAffectMsg();
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	if (ctx.defendingPokemon->GetStatus() == Status::Paralyzed)
	{
		deps.statusUI.DisplayAlreadyParalyzedMsg();
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

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		return;
	}

	// Damage multiplier for when defending Pokemon is SemiInvulnerableDig is in CalculateDamage()
	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog();
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void Dig::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	if (!ctx.attackingPokemon->IsCharging())
	{
		deps.resultsUI.DisplayDigChargeMsg();
		ctx.attackingPokemon->SetCharging(true);
		ctx.attackingPokemon->SetSemiInvulnerableDig(true);
		ctx.attackingPlayer->SetCanSwitch(false);
		return;
	}

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	ctx.attackingPokemon->SetCharging(false);
	ctx.attackingPokemon->SetSemiInvulnerableDig(false);
	ctx.attackingPlayer->SetCanSwitch(true);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog();
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void Toxic::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	bool isImmune = ((ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Poison || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Poison) ||
		(ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Steel || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Steel));

	if (isImmune)
	{
		deps.statusUI.DisplayDoesntAffectMsg();
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	if (ctx.defendingPokemon->GetStatus() == Status::Poisoned || ctx.defendingPokemon->GetStatus() == Status::Badly_Poisoned)
	{
		deps.statusUI.DisplayAlreadyPoisonedMsg();
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

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	int attackStage = ctx.attackingPokemon->GetAttackStage();

	if (attackStage >= 6)
	{
		deps.statusUI.DisplayStatRaiseFailMsg("attack");
	}
	else
	{
		++attackStage;
		ctx.attackingPokemon->SetAttackStage(attackStage);
		deps.statusUI.DisplayStatRaised1Msg("attack");
	}
}

void SpeedUp2::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	int speedStage = ctx.attackingPokemon->GetSpeedStage();

	if (speedStage >= 6)
	{
		deps.statusUI.DisplayStatRaiseFailMsg("speed");
	}
	else if (speedStage == 5)
	{
		++speedStage;
		ctx.attackingPokemon->SetSpeedStage(speedStage);
		deps.statusUI.DisplayStatRaised1Msg("speed");
	}
	else // speedStage < 5
	{
		speedStage += 2;
		ctx.attackingPokemon->SetSpeedStage(speedStage);
		deps.statusUI.DisplayStatRaised2Msg("speed");
	}
}

void Rage::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog();
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
	ctx.attackingPokemon->SetRaging(true);
}

void Teleport::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	if (ctx.attackingPlayer->GetPokemonCount() == 1)
	{
		deps.resultsUI.DisplayFailedTextDialog();
		return;
	}

	deps.battleUI.SwitchPokemonOption(ctx.attackingPlayer, ctx.attackingPokemon);

	deps.switchExecutor.ExecuteSwitch(ctx.attackingPlayer, ctx.attackingPokemon);
}

void NightShade::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		return;
	}

	const int baseDamage = ctx.defendingPokemon->GetLevel();

	bool hasSubstitute = ctx.defendingPokemon->HasSubstitute();

	int maxDamage = hasSubstitute ? ctx.defendingPokemon->GetSubstituteHP() : ctx.defendingPokemon->GetCurrentHP();

	int finalDamage = std::min(baseDamage, maxDamage);

	if (hasSubstitute)
	{
		ctx.defendingPokemon->DamageSubstitute(finalDamage);

		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
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

void Mimic::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

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
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	ctx.attackingPokemon->SetUsedMimic(true);
	ctx.attackingPokemon->SetMimicPP(ctx.currentMove->m_currentPP);

	deps.statusUI.DisplayLearnedMimicMoveMsg();

	ctx.currentMove->SetMovePointer(Database::GetInstance().GetPointerToMovedexNumber(
		ctx.defendingPokemon->GetLastUsedMove()->GetMoveIndex() - 1));
	ctx.currentMove->m_currentPP = ctx.defendingPokemon->GetLastUsedMove()->GetPP();
	ctx.currentMove->m_maxPP = ctx.defendingPokemon->GetLastUsedMove()->GetPP();
	ctx.currentMove->b_isMimicked = true;
}

void DefenseDown2::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	if (ctx.flags.hit && ctx.defendingPokemon->HasSubstitute() && !ctx.currentMove->CanBypassSubstitute())
	{
		deps.resultsUI.DisplayFailedTextDialog();
		return;
	}

	if (ctx.defendingPlayer->HasMist() && ctx.flags.hit)
	{
		deps.statusUI.DisplayProtectedByMistMsg();
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	if (ctx.defendingPokemon->GetDefenseStage() <= -6)
	{
		deps.statusUI.DisplayStatLoweredFailMsg("defense");
	}
	else if (ctx.defendingPokemon->GetDefenseStage() == -5)
	{
		int lowerDefense = ctx.defendingPokemon->GetDefenseStage() - 1;
		ctx.defendingPokemon->SetDefenseStage(lowerDefense);

		deps.statusUI.DisplayStatLowered1Msg("defense");
	}
	else // GetDefenseStage() > -5
	{
		int lowerDefense = ctx.defendingPokemon->GetDefenseStage() - 2;
		ctx.defendingPokemon->SetDefenseStage(lowerDefense);

		deps.statusUI.DisplayStatLowered2Msg("defense");
	}
}

void EvasionUp::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	int evasionStage = ctx.attackingPokemon->GetEvasionStage();

	if (evasionStage >= 6)
	{
		deps.statusUI.DisplayStatRaiseFailMsg("evasiveness");
	}
	else
	{
		++evasionStage;
		ctx.attackingPokemon->SetEvasionStage(evasionStage);
		deps.statusUI.DisplayStatRaised1Msg("evasiveness");
	}
}

void HealHalfHP::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	if (ctx.attackingPokemon->GetCurrentHP() < ctx.attackingPokemon->GetMaxHP())
	{
		int healAmount = (ctx.attackingPokemon->GetMaxHP() + 1) / 2;

		ctx.attackingPokemon->HealCurrentHP(healAmount);

		deps.resultsUI.DisplayRecoveredHPRestoredMsg(healAmount);
	}

	else
	{
		deps.resultsUI.DisplayHPFullMsg();
	}
}

void DefenseUp::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	int defenseStage = ctx.attackingPokemon->GetDefenseStage();

	if (defenseStage >= 6)
	{
		deps.statusUI.DisplayStatRaiseFailMsg("defense");
	}
	else
	{
		++defenseStage;
		ctx.attackingPokemon->SetDefenseStage(defenseStage);
		deps.statusUI.DisplayStatRaised1Msg("defense");
	}
}

void Minimize::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	int evasionStage = ctx.attackingPokemon->GetEvasionStage();

	if (evasionStage >= 6)
	{
		deps.statusUI.DisplayStatRaiseFailMsg("evasiveness");
	}
	else if (evasionStage == 5)
	{
		++evasionStage;
		ctx.attackingPokemon->SetEvasionStage(evasionStage);
		deps.statusUI.DisplayStatRaised1Msg("evasiveness");
		ctx.attackingPokemon->SetUsedMinimize(true);
	}
	else // evasionStage < 5
	{
		evasionStage += 2;
		ctx.attackingPokemon->SetEvasionStage(evasionStage);
		deps.statusUI.DisplayStatRaised2Msg("evasiveness");
		ctx.attackingPokemon->SetUsedMinimize(true);
	}
}

void DefenseUp2::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	int defenseStage = ctx.attackingPokemon->GetDefenseStage();

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	if (defenseStage >= 6)
	{
		deps.statusUI.DisplayStatRaiseFailMsg("defense");
	}
	else if (defenseStage == 5)
	{
		++defenseStage;
		ctx.attackingPokemon->SetDefenseStage(defenseStage);
		deps.statusUI.DisplayStatRaised1Msg("defense");
	}
	else // defenseStage < 5
	{
		defenseStage += 2;
		ctx.attackingPokemon->SetDefenseStage(defenseStage);
		deps.statusUI.DisplayStatRaised2Msg("defense");
	}
}

void LightScreen::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	if (ctx.attackingPlayer->HasLightScreen())
	{
		deps.resultsUI.DisplayFailedTextDialog();
	}
	else
	{
		ctx.attackingPlayer->SetLightScreen(true);
		deps.statusUI.DisplayLightScreenMsg();
	}
}

void Haze::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

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

	deps.statusUI.DisplayHazeMsg();
}

void Reflect::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	if (ctx.attackingPlayer->HasReflect())
	{
		deps.resultsUI.DisplayFailedTextDialog();
	}
	else
	{
		ctx.attackingPlayer->SetReflect(true);
		deps.statusUI.DisplayReflectMsg();
	}
}

void FocusEnergy::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	if (ctx.attackingPokemon->HasFocusEnergy())
	{
		deps.resultsUI.DisplayFailedTextDialog();
	}
	else
	{
		ctx.attackingPokemon->SetFocusEnergy(true);
		ctx.attackingPokemon->SetCriticalHitStage(2);
		deps.statusUI.DisplayFocusEnergyMsg();
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
		deps.statusUI.DisplayBideDisabledMsg();

		endBide;

		return;
	}

	if (ctx.attackingPokemon->IsBiding())
	{
		ctx.attackingPokemon->IncrementBideCounter();
		deps.statusUI.DisplayBideStoringEnergyMsg();
		
		return;
	}

	if (!ctx.attackingPokemon->IsBiding())
	{
		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

		ctx.attackingPokemon->SetBide(true);
		ctx.attackingPlayer->SetCanSwitch(false);

		ctx.attackingPokemon->SetBideTurnCount(2);
		ctx.attackingPokemon->ResetBideCounter();

		ctx.currentMove->m_currentPP -= 1;
	}

	if (ctx.attackingPokemon->GetBideCounter() >= ctx.attackingPokemon->GetBideTurnCount())
	{		
		deps.statusUI.DisplayBideUnleashedMsg();

		if (ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Ghost ||
			ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Ghost)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

			endBide;

			return;
		}

		ctx.flags.hit = !ctx.defendingPokemon->IsSemiInvulnerable();

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);

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

			deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
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

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	size_t randomMod{ 67 };
	while (randomMod == 67 || randomMod == 101 || randomMod == 117 || randomMod == 118 || randomMod == 143)
	{
		std::uniform_int_distribution<size_t> randomModDistributor(0, 163);
		randomMod = randomModDistributor(deps.rng.GetGenerator());
	}

	Move* selectedMove = Database::GetInstance().GetPointerToMovedexNumber(randomMod);

	deps.statusUI.DisplayMetronomeMsg(selectedMove);

	ctx.attackingPokemon->metronomeMove.SetMovePointer(selectedMove);
	ctx.attackingPokemon->metronomeMove.m_currentPP = 1;
	ctx.attackingPokemon->metronomeMove.m_maxPP = 1;

	{
		std::unique_ptr<IMoveRoutine> moveRoutine = MoveRoutineFactory::Call(selectedMove->GetMoveEffectEnum());
		moveRoutine->DoMove(deps);
	}
}

void MirrorMove::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

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
		std::unique_ptr<IMoveRoutine> moveRoutine = MoveRoutineFactory::Call(selectedMove->GetMoveEffectEnum());
		moveRoutine->DoMove(deps);
	}
}

void Explosion::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);

		ctx.attackingPokemon->DamageCurrentHP(ctx.attackingPokemon->GetCurrentHP());

		deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog();
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);

	ctx.attackingPokemon->DamageCurrentHP(ctx.attackingPokemon->GetCurrentHP());

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);
}

void Swift::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	ctx.flags.hit = !ctx.defendingPokemon->IsSemiInvulnerable();

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog();
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void SkullBash::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	if (!ctx.attackingPokemon->IsCharging())
	{
		deps.resultsUI.DisplaySkullBashChargeMsg();
		ctx.attackingPokemon->SetCharging(true);
		ctx.attackingPlayer->SetCanSwitch(false);

		int defenseStage = ctx.attackingPokemon->GetDefenseStage();

		if (defenseStage >= 6)
		{
			deps.statusUI.DisplayStatRaiseFailMsg("defense");
		}
		else
		{
			++defenseStage;
			ctx.attackingPokemon->SetDefenseStage(defenseStage);
			deps.statusUI.DisplayStatRaised1Msg("defense");
		}

		return;
	}

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	ctx.attackingPokemon->SetCharging(false);
	ctx.attackingPlayer->SetCanSwitch(true);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog();
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
}

void SpecialDefenseUp2::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	int specialDefenseStage = ctx.attackingPokemon->GetSpecialDefenseStage();

	if (specialDefenseStage >= 6)
	{
		deps.statusUI.DisplayStatRaiseFailMsg("special defense");
	}
	else if (specialDefenseStage == 5)
	{
		++specialDefenseStage;
		ctx.attackingPokemon->SetSpecialDefenseStage(specialDefenseStage);
		deps.statusUI.DisplayStatRaised1Msg("special defense");
	}
	else // specialDefenseStage < 5
	{
		specialDefenseStage += 2;
		ctx.attackingPokemon->SetSpecialDefenseStage(specialDefenseStage);
		deps.statusUI.DisplayStatRaised2Msg("special defense");
	}
}

void DreamEater::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.defendingPokemon->GetStatus() != Status::Sleeping || ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.statusUI.DisplayDoesntAffectMsg();
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog();
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

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
		deps.resultsUI.DisplayEnergyDrainedMsg();
	}

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);
}

void PoisonGas::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	bool isImmune = ((ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Poison || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Poison) ||
		(ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Steel || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Steel));

	if (isImmune)
	{
		deps.statusUI.DisplayDoesntAffectMsg();
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	if (ctx.defendingPokemon->GetStatus() == Status::Poisoned || ctx.defendingPokemon->GetStatus() == Status::Badly_Poisoned)
	{
		deps.statusUI.DisplayAlreadyPoisonedMsg();
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
		deps.resultsUI.DisplaySkullBashChargeMsg();
		ctx.attackingPokemon->SetCharging(true);
		ctx.attackingPlayer->SetCanSwitch(false);
		return;
	}

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	ctx.attackingPokemon->SetCharging(false);
	ctx.attackingPlayer->SetCanSwitch(true);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		return;
	}

	int oldCritStage = ctx.attackingPokemon->GetCriticalHitStage();
	int newCritStage = oldCritStage + 1;

	ctx.attackingPokemon->SetCriticalHitStage(newCritStage);

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog();
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

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

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	if (ctx.attackingPokemon->IsTransformed() || ctx.defendingPokemon->IsTransformed() || ctx.defendingPokemon->HasSubstitute())
	{
		deps.resultsUI.DisplayFailedTextDialog();
		return;
	}

	ctx.flags.hit = !ctx.defendingPokemon->IsSemiInvulnerable();

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	ctx.attackingPokemon->SetTransformation(ctx.defendingPokemon);
	deps.statusUI.DisplayTransformMsg();
}

void Psywave::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
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

	if (ctx.defendingPokemon->HasSubstitute())
	{
		ctx.defendingPokemon->DamageSubstitute(psywaveDamage);

		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		ctx.flags.hitSubstitute = true;
	}
	else
	{
		ctx.defendingPokemon->DamageCurrentHP(psywaveDamage);
		ctx.flags.hitSubstitute = false;
	}

	ctx.damageTaken = psywaveDamage;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);
}

void Splash::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.resultsUI.DisplaySplashMsg();
}

void Rest::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	if (ctx.attackingPokemon->GetStatus() != Status::Sleeping &&
		ctx.attackingPokemon->GetCurrentHP() < ctx.attackingPokemon->GetMaxHP())
	{
		deps.resultsUI.DisplayRestMsg();

		ctx.attackingPokemon->ChangeStatus(Status::Sleeping);

		ctx.attackingPokemon->SetSleepTurnCount(2);
		ctx.attackingPokemon->ResetSleepCounter();

		int healAmount = ctx.attackingPokemon->GetMaxHP() - ctx.attackingPokemon->GetCurrentHP();
		ctx.attackingPokemon->HealCurrentHP(healAmount);
	}
	else if (ctx.attackingPokemon->GetCurrentHP() >= ctx.attackingPokemon->GetMaxHP())
	{
		deps.resultsUI.DisplayHPFullMsg();
	}
}

void Conversion::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	if (ctx.attackingPokemon->GetTypeOneEnum() == ctx.currentMove->GetMoveTypeEnum() ||
		ctx.attackingPokemon->GetTypeTwoEnum() == ctx.currentMove->GetMoveTypeEnum() ||
		ctx.attackingPokemon->IsConverted())
	{
		deps.resultsUI.DisplayFailedTextDialog();
	}
	else
	{
		ctx.attackingPokemon->SetConversion(ctx.currentMove);

		deps.statusUI.DisplayConversionMsg();
	}

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void TriAttack::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog();
	deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

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

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		return;
	}

	bool hasSubstitute = ctx.defendingPokemon->HasSubstitute();

	int hpSource = hasSubstitute ? ctx.defendingPokemon->GetSubstituteHP() : ctx.defendingPokemon->GetCurrentHP();

	int finalDamage = std::max(1, hpSource / 2);

	if (hasSubstitute)
	{
		ctx.defendingPokemon->DamageSubstitute(finalDamage);

		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
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

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	if (ctx.attackingPokemon->HasSubstitute())
	{
		deps.statusUI.DisplayAlreadyHasSubstituteMsg();
		return;
	}

	int substituteHP = ctx.attackingPokemon->GetMaxHP() / 4;

	if (substituteHP >= ctx.attackingPokemon->GetCurrentHP())
	{
		deps.statusUI.DisplayNotEnoughHPSubstituteMsg();
		return;
	}

	int hpCost = substituteHP;

	deps.statusUI.DisplayPutInSubstituteMsg();
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

	deps.statusUI.DisplayNoMovesLeftStruggleMsg();

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	ctx.flags.hit = !ctx.defendingPokemon->IsSemiInvulnerable();

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		return;
	}

	int damage = deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	deps.calculations.ApplyDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon, damage);
	deps.resultsUI.DisplayDirectDamageInflictedMsg(damage);
	deps.resultsUI.DisplayCritTextDialog();
	deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);

	int recoilDamage = (ctx.attackingPokemon->GetMaxHP() + 2) / 4;

	ctx.attackingPokemon->DamageCurrentHP(recoilDamage);

	deps.resultsUI.DisplayRecoilMsg();

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

}
