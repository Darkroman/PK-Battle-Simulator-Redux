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

	if ((ctx.defendingPokemon->HasSubstitute() && !ctx.currentMove->mp_move->CanBypassSubstitute()) || ctx.defendingPokemon->GetCurrentHP() <= 0 || ctx.defendingPokemon->GetStatus() != Status::Normal || ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
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

	case MoveEffect::BypassSubSleep:
		return std::make_unique<BypassSubSleep>();

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

	case MoveEffect::SleepMove:
		return std::make_unique<SleepMove>();

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

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void NormalHit::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	if (ctx.flags.hit)
	{
		deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

		deps.resultsUI.DisplayCritTextDialog();
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
	}
	else
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void IncreasedCritical::DoMove(MoveRoutineDeps& deps) {
	auto& ctx = deps.context;

	int oldCritStage = ctx.attackingPokemon->GetCriticalHitStage();
	int newCritStage = oldCritStage + 1;

	ctx.attackingPokemon->SetCriticalHitStage(newCritStage);

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	if (ctx.flags.hit)
	{
		deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

		deps.resultsUI.DisplayCritTextDialog();
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
	}

	else
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetCriticalHitStage(oldCritStage);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void MultiAttack::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

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

	if (ctx.flags.hit)
	{
		int timesHit = 0;
		while (turnCount > 0 && ctx.defendingPokemon->GetCurrentHP() > 0)
		{
			deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

			if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
			{
				deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
				return;
			}

			deps.resultsUI.DisplayCritTextDialog();
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
			deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

			deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);

			++timesHit;
			--turnCount;
		}

		deps.resultsUI.DisplayMultiAttackMsg(timesHit);

		if (ctx.defendingPokemon->IsBiding())
		{
			ctx.defendingPokemon->AddBideDamage(static_cast<int>(ctx.damageTaken));
		}
	}

	else
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void BurnHit::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	if (ctx.flags.hit)
	{
		deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

		deps.resultsUI.DisplayCritTextDialog();
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

		InflictNVStatus(Status::Burned, ctx.currentMove->mp_move->GetEffectChance(), deps);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
	}
	else
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void FreezeHit::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	if (ctx.flags.hit)
	{
		deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

		deps.resultsUI.DisplayCritTextDialog();
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

		InflictNVStatus(Status::Frozen, ctx.currentMove->mp_move->GetEffectChance(), deps);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
	}
	else
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void ParalyzeHit::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	if (ctx.flags.hit)
	{
		deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

		deps.resultsUI.DisplayCritTextDialog();
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

		InflictNVStatus(Status::Paralyzed, ctx.currentMove->mp_move->GetEffectChance(), deps);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
	}
	else
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void OHKO::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	if (ctx.flags.hit)
	{
		// OHKO specific logic done in CalculateDamage()
		deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
	}
	else
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void RazorWind::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	if (ctx.attackingPokemon->IsCharging())
	{
		ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

		if (ctx.flags.hit)
		{
			deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

			deps.resultsUI.DisplayCritTextDialog();
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
			deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

			deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		}
		else
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		}

		deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

		ctx.attackingPokemon->SetCharging(false);
		ctx.attackingPlayer->SetCanSwitch(true);
		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
		ctx.currentMove->m_currentPP -= 1;
	}
	else
	{
		deps.resultsUI.DisplayRazorWindChargeMsg();
		ctx.attackingPokemon->SetCharging(true);
		ctx.attackingPlayer->SetCanSwitch(false);
	}
}

void AttackUp2::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

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

	ctx.currentMove->m_currentPP -= 1;
	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void Gust::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	if (ctx.flags.hit)
	{
		// Power modifier for when defending Pokemon is SemiInvulnerableFly is in CalculateDamage()
		deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

		deps.resultsUI.DisplayCritTextDialog();
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
	}
	else
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void MakeEnemySwitch::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.currentMove->m_currentPP -= 1;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	// Conditions where forced switch fails
	if (ctx.defendingPlayer->GetPokemonCount() == 1 || !ctx.defendingPlayer->IsFirst() || ctx.defendingPokemon->IsSemiInvulnerable())
	{
		deps.resultsUI.DisplayFailedTextDialog();
		return;
	}

	ctx.flags.hit = true;

	std::deque<BattlePokemon*> enemyPokemonList{};
	int numberOfPokemon = 0;

	if (ctx.flags.hit)
	{
		for (size_t i = 1; i <= 6; ++i)
		{
			BattlePokemon* beltMon = ctx.defendingPlayer->GetBelt(i);
			if (beltMon && beltMon != ctx.defendingPokemon && beltMon->GetCurrentHP() > 0)
			{
				enemyPokemonList.push_back(beltMon);
				++numberOfPokemon;
			}
		}

		std::uniform_int_distribution<size_t> randomModDistributor(1, numberOfPokemon);
		size_t randomMod = randomModDistributor(deps.rng.GetGenerator());

		// Reset stats for the Pokémon being forced out
		ctx.defendingPokemon->ResetStatsOnSwitch();

		if (ctx.attackingPokemon->IsBound())
		{
			ctx.attackingPokemon->SetBound(false);
			ctx.attackingPokemon->ResetBoundCounter();
			ctx.attackingPokemon->SetBoundTurnCount(0);
		}

		if (ctx.defendingPokemon->HasSubstitute())
		{
			ctx.defendingPokemon->SetSubstitute(false);
			ctx.defendingPokemon->SetSubstituteHP(0);
		}

		if (ctx.defendingPlayer == ctx.playerOne)
		{
			deps.context.playerOneCurrentPokemon = enemyPokemonList.at(randomMod - 1);
		}
		else
		{
			deps.context.playerTwoCurrentPokemon = enemyPokemonList.at(randomMod - 1);
		}

		deps.resultsUI.DisplayEnemySwitchMsg(*enemyPokemonList.at(randomMod - 1));
	}
	else
	{
		deps.resultsUI.DisplayFailedTextDialog();
	}

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void Fly::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	if (ctx.attackingPokemon->IsCharging())
	{
		ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

		if (ctx.flags.hit)
		{
			deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

			deps.resultsUI.DisplayCritTextDialog();
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
			deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

			deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		}
		else
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		}

		deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

		ctx.attackingPokemon->SetCharging(false);
		ctx.attackingPokemon->SetSemiInvulnerableFly(false);
		ctx.attackingPlayer->SetCanSwitch(true);
		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
		ctx.currentMove->m_currentPP -= 1;
	}
	else
	{
		deps.resultsUI.DisplayFlyChargeMsg();

		ctx.attackingPokemon->SetCharging(true);
		ctx.attackingPokemon->SetSemiInvulnerableFly(true);
		ctx.attackingPlayer->SetCanSwitch(false);
	}
}

void Bound::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	bool isGhost = ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Ghost || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Ghost;

	if (ctx.flags.hit)
	{
		deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

		deps.resultsUI.DisplayCritTextDialog();
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

		if (!ctx.defendingPokemon->IsBound() && ctx.flags.currentEffectiveness != BattleStateFlags::Effectiveness::No && !ctx.defendingPokemon->HasSubstitute())
		{
			ctx.defendingPokemon->SetBound(true);

			if (!isGhost)
			{
				ctx.defendingPlayer->SetCanSwitch(false);
			}

			std::uniform_int_distribution<int> randomModDistributor(4, 5);
			int randomMod = randomModDistributor(deps.rng.GetGenerator());
			ctx.defendingPokemon->SetBoundTurnCount(randomMod);
			ctx.defendingPokemon->ResetBoundCounter();
			ctx.defendingPokemon->SetBoundMoveName(ctx.currentMove->mp_move->GetMoveIndex());

			deps.resultsUI.BoundMoveText(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon, ctx.currentMove);
		}

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
	}
	else
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void Stomp::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.flags.hit = false;
	if (ctx.defendingPokemon->HasUsedMinimize() && !ctx.defendingPokemon->IsSemiInvulnerable())
	{
		ctx.flags.hit = true;
	}
	else
	{
		ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	}

	if (ctx.flags.hit)
	{
		// Damage multiplier for when defending Pokemon has minimized is in CalculateDamage()
		deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

		deps.resultsUI.DisplayCritTextDialog();
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

		std::uniform_int_distribution<int> randomModDistributor(1, 100);
		int randomMod = randomModDistributor(deps.rng.GetGenerator());

		if ((!ctx.defendingPokemon->HasSubstitute() || ctx.currentMove->mp_move->CanBypassSubstitute()) &&
			ctx.defendingPokemon->GetCurrentHP() != 0 &&
		    !ctx.defendingPlayer->IsFirst() && 
			ctx.flags.currentEffectiveness != BattleStateFlags::Effectiveness::No &&
			randomMod <= ctx.currentMove->mp_move->GetEffectChance())
		{
			ctx.defendingPokemon->SetIsFlinched(true);
		}

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
	}
	else
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void DoubleHit::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	if (ctx.flags.hit)
	{
		int turnCount = 2;
		int timesHit = 0;

		while (turnCount != 0 && ctx.defendingPokemon->GetCurrentHP() > 0)
		{
			deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

			if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
			{
				deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
				return;
			}

			deps.resultsUI.DisplayCritTextDialog();
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
			deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

			deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);

			++timesHit;
			--turnCount;
		}

		deps.resultsUI.DisplayMultiAttackMsg(timesHit);

		if (ctx.defendingPokemon->IsBiding())
		{
			ctx.defendingPokemon->AddBideDamage(static_cast<int>(ctx.damageTaken));
		}
	}
	else
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void JumpKick::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	if (ctx.flags.hit)
	{
		deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

		deps.resultsUI.DisplayCritTextDialog();
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
	}

	else
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);

		double crashDamage = std::floor(ctx.attackingPokemon->GetMaxHP() / 2);
		ctx.attackingPokemon->DamageCurrentHP(static_cast<int>(crashDamage));

		deps.resultsUI.DisplayJumpKickCrashMsg();
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void FlinchHit::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	if (ctx.flags.hit)
	{
		deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

		deps.resultsUI.DisplayCritTextDialog();
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

		std::uniform_int_distribution<int> randomModDistributor(1, 100);
		int randomMod = randomModDistributor(deps.rng.GetGenerator());

		if ((!ctx.defendingPokemon->HasSubstitute() || ctx.currentMove->mp_move->CanBypassSubstitute()) &&
			ctx.defendingPokemon->GetCurrentHP() != 0 &&
			!ctx.defendingPlayer->IsFirst() &&
			ctx.flags.currentEffectiveness != BattleStateFlags::Effectiveness::No &&
			randomMod <= ctx.currentMove->mp_move->GetEffectChance())
		{
			ctx.defendingPokemon->SetIsFlinched(true);
		}

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
	}
	else
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void AccuracyDown::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (ctx.defendingPlayer->HasMist() && ctx.flags.hit)
	{
		deps.statusUI.DisplayProtectedByMistMsg();

		ctx.currentMove->m_currentPP -= 1;

		deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		return;
	}

	if (ctx.flags.hit && ctx.defendingPokemon->HasSubstitute() && !ctx.currentMove->mp_move->CanBypassSubstitute())
	{
		deps.resultsUI.DisplayFailedTextDialog();
	}
	else if (ctx.flags.hit)
	{
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
	else
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void BodySlam::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.flags.hit = false;
	if (ctx.defendingPokemon->HasUsedMinimize() && !ctx.defendingPokemon->IsSemiInvulnerable())
	{
		ctx.flags.hit = true;
	}
	else
	{
		ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
	}

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	if (ctx.flags.hit)
	{
		// Damage multiplier for when defending Pokemon has minimized is in CalculateDamage()
		deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
		deps.resultsUI.DisplayCritTextDialog();
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		InflictNVStatus(Status::Paralyzed, ctx.currentMove->mp_move->GetEffectChance(), deps);
		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
	}
	else
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void RecoilQuarter::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	double recoilDamage = 0.0;

	if (ctx.flags.hit && !ctx.defendingPokemon->HasSubstitute())
	{
		double targetHPBegin = ctx.defendingPokemon->GetCurrentHP();

		deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
		deps.resultsUI.DisplayCritTextDialog();
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

		double targetHPEnd = ctx.defendingPokemon->GetCurrentHP();

		deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

		recoilDamage = std::floor((targetHPBegin - targetHPEnd) / 4);
		ctx.attackingPokemon->DamageCurrentHP(static_cast<int>(recoilDamage));

		if (ctx.flags.currentEffectiveness != BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayRecoilMsg();
			deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);
		}
	}
	else if (ctx.flags.hit && ctx.defendingPokemon->HasSubstitute())
	{
		double targetHPBegin = ctx.defendingPokemon->GetSubstituteHP();

		deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
		deps.resultsUI.DisplayCritTextDialog();
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);

		double targetHPEnd = ctx.defendingPokemon->GetSubstituteHP();

		deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

		recoilDamage = std::floor((targetHPBegin - targetHPEnd) / 4);
		ctx.attackingPokemon->DamageCurrentHP(static_cast<int>(recoilDamage));

		if (ctx.flags.currentEffectiveness != BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayRecoilMsg();
			deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);
		}
	}
	else
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;
	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void Thrash::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	if (!ctx.currentMove->b_isDisabled)
	{
		ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

		if (ctx.flags.hit)
		{
			deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
			deps.resultsUI.DisplayCritTextDialog();
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
			deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
			deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);

			if (!ctx.attackingPokemon->IsThrashing())
			{
				ctx.attackingPokemon->SetThrashing(true);
				ctx.attackingPlayer->SetCanSwitch(false);

				std::uniform_int_distribution<int> randomModDistributor(2, 3);
				int randomMod(randomModDistributor(deps.rng.GetGenerator()));
				ctx.attackingPokemon->SetThrashTurnCount(randomMod);
				ctx.attackingPokemon->ResetThrashCounter();

				ctx.currentMove->m_currentPP -= 1;
			}
		}
		else
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		}
	}
	else
	{
		deps.statusUI.DisplayThrashDisabledMsg();
	}

	ctx.attackingPokemon->IncrementThrashCounter();

	if (ctx.attackingPokemon->GetThrashCounter() == ctx.attackingPokemon->GetThrashTurnCount() && !ctx.attackingPokemon->IsConfused())
	{
		deps.statusProcessor.ThrashConfuse();
	}

	if ((ctx.attackingPokemon->GetThrashCounter() == ctx.attackingPokemon->GetThrashTurnCount() ||
		ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No ||
		!ctx.flags.hit ||
		ctx.currentMove->b_isDisabled) && ctx.attackingPokemon->IsThrashing())
	{
		deps.statusProcessor.ThrashStop();
		deps.statusProcessor.ThrashReset();
	}

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void RecoilThird::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	double recoilDamage{ 0 };

	if (ctx.flags.hit && !ctx.defendingPokemon->HasSubstitute())
	{
		double targetHPBegin = ctx.defendingPokemon->GetCurrentHP();

		deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
		deps.resultsUI.DisplayCritTextDialog();
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		double targetHPEnd = ctx.defendingPokemon->GetCurrentHP();

		deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

		recoilDamage = floor((targetHPBegin - targetHPEnd) / 3);
		ctx.attackingPokemon->DamageCurrentHP(static_cast<int>(recoilDamage));

		if (ctx.flags.currentEffectiveness != BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayRecoilMsg();

			deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);
		}
	}
	else if (ctx.flags.hit && ctx.defendingPokemon->HasSubstitute())
	{
		double targetHPBegin = ctx.defendingPokemon->GetSubstituteHP();

		deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
		deps.resultsUI.DisplayCritTextDialog();
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		double targetHPEnd = ctx.defendingPokemon->GetSubstituteHP();

		deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

		recoilDamage = floor((targetHPBegin - targetHPEnd) / 3);
		ctx.attackingPokemon->DamageCurrentHP(static_cast<int>(recoilDamage));

		if (ctx.flags.currentEffectiveness != BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayRecoilMsg();

			deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);
		}
	}
	else
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void DefenseDown::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (ctx.defendingPlayer->HasMist() && ctx.flags.hit)
	{
		deps.statusUI.DisplayProtectedByMistMsg();

		ctx.currentMove->m_currentPP -= 1;

		deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		return;
	}

	if (ctx.flags.hit && ctx.defendingPokemon->HasSubstitute() && !ctx.currentMove->mp_move->CanBypassSubstitute())
	{
		deps.resultsUI.DisplayFailedTextDialog();
	}
	else if (ctx.flags.hit)
	{
		if (ctx.defendingPokemon->GetDefenseStage() > -6)
		{
			int lowerDefense{ ctx.defendingPokemon->GetDefenseStage() - 1 };
			ctx.defendingPokemon->SetDefenseStage(lowerDefense);

			deps.statusUI.DisplayStatLowered1Msg("defense");
		}
		else
		{
			deps.statusUI.DisplayStatLoweredFailMsg("defense");
		}
	}
	else
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void PoisonHit::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	if (ctx.flags.hit)
	{
		deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
		deps.resultsUI.DisplayCritTextDialog();
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		InflictNVStatus(Status::Poisoned, ctx.currentMove->mp_move->GetEffectChance(), deps);
		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
	}
	else
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void Twineedle::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	if (ctx.flags.hit)
	{
		int turnCount = 2;
		int timesHit{ 0 };

		while (turnCount != 0 && ctx.defendingPokemon->GetCurrentHP() > 0)
		{
			deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

			if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
			{
				deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
				return;
			}

			deps.resultsUI.DisplayCritTextDialog();
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
			deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
			InflictNVStatus(Status::Poisoned, ctx.currentMove->mp_move->GetEffectChance(), deps);
			deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);

			++timesHit;
			--turnCount;
		}

		deps.resultsUI.DisplayMultiAttackMsg(timesHit);
	}
	else
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void AttackDown::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (ctx.flags.hit && ctx.defendingPlayer->HasMist())
	{
		deps.statusUI.DisplayProtectedByMistMsg();

		ctx.currentMove->m_currentPP -= 1;

		deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
		return;
	}

	if (ctx.flags.hit && ctx.defendingPokemon->HasSubstitute() && !ctx.currentMove->mp_move->CanBypassSubstitute())
	{
		deps.resultsUI.DisplayFailedTextDialog();
	}
	else if (ctx.flags.hit)
	{
		if (ctx.defendingPokemon->GetAttackStage() > -6)
		{
			int lowered = ctx.defendingPokemon->GetAttackStage() - 1;
			ctx.defendingPokemon->SetAttackStage(lowered);

			deps.statusUI.DisplayStatLowered1Msg("attack");
		}
		else
		{
			deps.statusUI.DisplayStatLoweredFailMsg("attack");
		}
	}
	else
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void BypassSubSleep::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (ctx.flags.hit)
	{
		if (ctx.defendingPokemon->GetStatus() == Status::Normal)
		{
			deps.statusUI.DisplayFellAsleepMsg();

			ctx.defendingPokemon->ChangeStatus(Status::Sleeping);

			std::uniform_int_distribution<int> randomModDistributor(1, 3);
			int randomMod(randomModDistributor(deps.rng.GetGenerator()));
			ctx.defendingPokemon->SetSleepTurnCount(randomMod);
			ctx.defendingPokemon->ResetSleepCounter();
		}
		else if (ctx.defendingPokemon->GetStatus() == Status::Sleeping)
		{
			deps.statusUI.DisplayAlreadyAsleepMsg();
		}
		else
		{
			deps.resultsUI.DisplayFailedTextDialog();
		}
	}
	else
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void Confuse::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (ctx.flags.hit)
	{
		if (!ctx.defendingPokemon->IsConfused() && (!ctx.defendingPokemon->HasSubstitute() || ctx.currentMove->mp_move->CanBypassSubstitute()))
		{
			deps.statusUI.DisplayBecameConfuseMsg();

			ctx.defendingPokemon->SetConfusedStatus(true);

			std::uniform_int_distribution<int> randomModDistributor(2, 4);
			int randomMod(randomModDistributor(deps.rng.GetGenerator()));
			ctx.defendingPokemon->SetConfusedTurnCount(randomMod);
			ctx.defendingPokemon->ResetConfusedCounter();
		}
		else
		{
			deps.resultsUI.DisplayFailedTextDialog();
		}
	}
	else
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void SonicBoom::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);
	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	double sonicBoomDamage = 20;

	if (ctx.flags.hit)
	{
		if (ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Ghost || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Ghost)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
			sonicBoomDamage = 0;
		}

		if (ctx.defendingPokemon->HasSubstitute())
		{
			if (sonicBoomDamage > ctx.defendingPokemon->GetSubstituteHP())
			{
				sonicBoomDamage = ctx.defendingPokemon->GetSubstituteHP();
			}

			ctx.defendingPokemon->DamageSubstitute(static_cast<int>(sonicBoomDamage));
			deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
			deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		}
		else
		{
			if (sonicBoomDamage > ctx.defendingPokemon->GetCurrentHP())
			{
				sonicBoomDamage = ctx.defendingPokemon->GetCurrentHP();
			}

			ctx.defendingPokemon->DamageCurrentHP(static_cast<int>(sonicBoomDamage));
		}

	}
	else
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
	}

	ctx.damageTaken = sonicBoomDamage;

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void Disable::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	if (ctx.flags.hit)
	{
		if (ctx.defendingPokemon->GetLastUsedMove() == nullptr)
		{
			deps.resultsUI.DisplayFailedTextDialog();
		}
		else if (ctx.defendingPokemon->MoveIsDisabled() ||
			ctx.defendingPokemon->GetLastUsedMove()->m_currentPP == 0 ||
			ctx.defendingPokemon->GetLastUsedMove()->mp_move->GetMoveEffectEnum() == MoveEffect::Struggle)
		{
			deps.resultsUI.DisplayFailedTextDialog();
		}
		else
		{
			ctx.defendingPokemon->SetDisabledStatus(true);
			ctx.defendingPokemon->ResetDisabledCounter();

			deps.statusUI.DisplayMoveDisabledMsg();
		}
	}
	else
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void SpecialDefenseDownHit::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	if (ctx.flags.hit)
	{
		deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
		deps.resultsUI.DisplayCritTextDialog();
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

		std::uniform_int_distribution<int> randomModDistributor(1, 100);
		int randomNumber{ randomModDistributor(deps.rng.GetGenerator()) };

		if (randomNumber <= ctx.currentMove->mp_move->GetEffectChance() &&
			!ctx.defendingPlayer->HasMist() &&
			(!ctx.defendingPokemon->HasSubstitute() || ctx.currentMove->mp_move->CanBypassSubstitute()) &&
			ctx.defendingPokemon->GetCurrentHP() != 0)
		{
			if (ctx.defendingPokemon->GetSpecialDefenseStage() > -6)
			{
				int lowerSpecialDefense = ctx.defendingPokemon->GetSpecialDefenseStage() - 1;
				ctx.defendingPokemon->SetSpecialDefenseStage(lowerSpecialDefense);

				deps.statusUI.DisplayStatLowered1Msg("special defense");
			}
		}

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
	}
	else
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void Mist::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	if (!ctx.attackingPlayer->HasMist())
	{
		deps.statusUI.DisplayMistMsg();
		ctx.attackingPlayer->SetMist(true);
	}
	else
	{
		deps.resultsUI.DisplayFailedTextDialog();
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void ConfuseHit::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	if (ctx.flags.hit)
	{
		deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
		deps.resultsUI.DisplayCritTextDialog();
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

		if (!ctx.defendingPokemon->IsConfused())
		{
			std::uniform_int_distribution<int> randomModDistributor(1, 100);
			int randomNumber{ randomModDistributor(deps.rng.GetGenerator()) };

			if (randomNumber <= ctx.currentMove->mp_move->GetEffectChance() && 
				(!ctx.defendingPokemon->HasSubstitute() || ctx.currentMove->mp_move->CanBypassSubstitute()) &&
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
	else
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void SpeedDownHit::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	if (ctx.flags.hit)
	{
		deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
		deps.resultsUI.DisplayCritTextDialog();
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

		std::uniform_int_distribution<int> randomModDistributor(1, 100);
		int randomNumber{ randomModDistributor(deps.rng.GetGenerator()) };

		if (randomNumber <= ctx.currentMove->mp_move->GetEffectChance() &&
			!ctx.defendingPlayer->HasMist() &&
			(!ctx.defendingPokemon->HasSubstitute() || ctx.currentMove->mp_move->CanBypassSubstitute()) &&
			ctx.defendingPokemon->GetCurrentHP() != 0)
		{
			if (ctx.defendingPokemon->GetSpeedStage() > -6)
			{
				int lowerSpeed{ ctx.defendingPokemon->GetSpeedStage() - 1 };
				ctx.defendingPokemon->SetSpeedStage(lowerSpeed);

				deps.statusUI.DisplayStatLowered1Msg("speed");
			}
		}
		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
	}
	else
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void AttackDownHit::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	if (ctx.flags.hit)
	{
		deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
		deps.resultsUI.DisplayCritTextDialog();
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

		std::uniform_int_distribution<int> randomModDistributor(1, 100);
		int randomNumber{ randomModDistributor(deps.rng.GetGenerator()) };

		if (randomNumber <= ctx.currentMove->mp_move->GetEffectChance() &&
			!ctx.defendingPlayer->HasMist() &&
			(!ctx.defendingPokemon->HasSubstitute() || ctx.currentMove->mp_move->CanBypassSubstitute()) &&
			ctx.defendingPokemon->GetCurrentHP() != 0)
		{
			if (ctx.defendingPokemon->GetAttackStage() > -6)
			{
				int lowerAttack{ ctx.defendingPokemon->GetAttackStage() - 1 };
				ctx.defendingPokemon->SetAttackStage(lowerAttack);

				deps.statusUI.DisplayStatLowered1Msg("attack");
			}
		}
		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
	}
	else
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void RechargeAttack::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	if (!ctx.attackingPokemon->IsRecharging())
	{
		ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

		if (ctx.flags.hit)
		{
			deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
			deps.resultsUI.DisplayCritTextDialog();
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
			deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
			deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);

			if (ctx.flags.currentEffectiveness != BattleStateFlags::Effectiveness::No)
			{
				ctx.attackingPokemon->SetRecharging(true);
				ctx.attackingPlayer->SetCanSwitch(false);
			}
		}
		else
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		}

		deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
		ctx.currentMove->m_currentPP -= 1;
	}
	else
	{
		deps.resultsUI.DisplayRechargeMsg();
		ctx.attackingPokemon->SetRecharging(false);
		ctx.attackingPlayer->SetCanSwitch(true);
	}
}

void LowKick::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	if (ctx.flags.hit)
	{
		deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
		deps.resultsUI.DisplayCritTextDialog();
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
	}
	else
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void Counter::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	double counterDamage = ctx.damageTaken * 2;

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	if (ctx.flags.hit)
	{
		if (counterDamage < 1 || ctx.attackingPlayer->IsFirst())
		{
			deps.resultsUI.DisplayFailedTextDialog();
		}
		else
		{
			const auto& lastMove = ctx.defendingPokemon->GetLastUsedMove();
			const auto category = lastMove ? lastMove->mp_move->GetCategoryEnum() : Category::Physical;

			if ((ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No && category != Category::Physical) ||
				(ctx.flags.currentEffectiveness != BattleStateFlags::Effectiveness::No && category != Category::Physical))
			{
				deps.resultsUI.DisplayFailedTextDialog();
				counterDamage = 0;
			}
			else if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No && category == Category::Physical)
			{
				deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
				counterDamage = 0;
			}

			if (ctx.defendingPokemon->HasSubstitute())
			{
				if (counterDamage > ctx.defendingPokemon->GetSubstituteHP())
					counterDamage = ctx.defendingPokemon->GetSubstituteHP();

				ctx.defendingPokemon->DamageSubstitute(static_cast<int>(counterDamage));
				deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
				deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
			}
			else
			{
				if (counterDamage > ctx.defendingPokemon->GetCurrentHP())
					counterDamage = ctx.defendingPokemon->GetCurrentHP();

				ctx.defendingPokemon->DamageCurrentHP(static_cast<int>(counterDamage));
			}

		}
	}
	else
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
	}

	ctx.damageTaken = counterDamage;

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void SeismicToss::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	double seismicTossDamage = ctx.attackingPokemon->GetLevel();

	if (ctx.flags.hit)
	{
		if (ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Ghost || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Ghost)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
			seismicTossDamage = 0;
		}

		if (ctx.defendingPokemon->HasSubstitute())
		{
			if (seismicTossDamage > ctx.defendingPokemon->GetSubstituteHP())
			{
				seismicTossDamage = ctx.defendingPokemon->GetSubstituteHP();
			}
			ctx.defendingPokemon->DamageSubstitute(static_cast<int>(seismicTossDamage));
			deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
			deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		}
		else
		{
			if (seismicTossDamage > ctx.defendingPokemon->GetCurrentHP())
			{
				seismicTossDamage = ctx.defendingPokemon->GetCurrentHP();
			}
			ctx.defendingPokemon->DamageCurrentHP(static_cast<int>(seismicTossDamage));
		}

	}
	else
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
	}

	ctx.damageTaken = seismicTossDamage;

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void Leech::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	if (ctx.flags.hit)
	{
		deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
		deps.resultsUI.DisplayCritTextDialog();
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);

		double leechedHealth{};

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
			leechedHealth = floor(ctx.damageTaken / 2);
		}

		if (leechedHealth > (ctx.attackingPokemon->GetMaxHP() - ctx.attackingPokemon->GetCurrentHP()))
		{
			leechedHealth = (ctx.attackingPokemon->GetMaxHP() - ctx.attackingPokemon->GetCurrentHP());
		}

		ctx.attackingPokemon->HealCurrentHP(static_cast<int>(leechedHealth));

		deps.resultsUI.DisplayEnergyDrainedMsg();
	}
	else
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void LeechSeed::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	if (ctx.flags.hit)
	{
		if (ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Grass || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Grass)
		{
			deps.statusUI.DisplayDoesntAffectMsg();
		}
		else if (ctx.defendingPokemon->IsSeeded() || ctx.defendingPokemon->HasSubstitute())
		{
			deps.resultsUI.DisplayFailedTextDialog();
		}
		else
		{
			ctx.defendingPokemon->SetSeededStatus(true);

			deps.statusUI.DisplaySeededMsg();
		}
	}
	else
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void Growth::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

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

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void SolarBeam::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	if (ctx.attackingPokemon->IsCharging())
	{
		ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

		if (ctx.flags.hit)
		{
			deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
			deps.resultsUI.DisplayCritTextDialog();
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
			deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
			deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		}
		else
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		}

		deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

		ctx.attackingPokemon->SetCharging(false);
		ctx.attackingPlayer->SetCanSwitch(true);
		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
	}
	else
	{
		deps.resultsUI.DisplaySolarBeamChargeMsg();
		ctx.attackingPokemon->SetCharging(true);
		ctx.attackingPlayer->SetCanSwitch(false);
		ctx.currentMove->m_currentPP -= 1;
	}
}

void PoisonPowder::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	bool isImmune = ((ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Grass || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Grass) ||
		(ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Poison || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Poison) ||
		(ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Steel || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Steel));

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	}
	else if (isImmune)
	{
		deps.statusUI.DisplayDoesntAffectMsg();
	}
	else if (ctx.defendingPokemon->GetStatus() == Status::Poisoned)
	{
		deps.statusUI.DisplayAlreadyPoisonedMsg();
	}
	else if (ctx.defendingPokemon->GetStatus() != Status::Normal || (ctx.defendingPokemon->HasSubstitute() && !ctx.currentMove->mp_move->CanBypassSubstitute()))
	{
		deps.resultsUI.DisplayFailedTextDialog();
	}
	else
	{
		InflictNVStatus(Status::Poisoned, 100, deps);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void StunSpore::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	bool isImmune = ((ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Grass || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Grass) ||
		(ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Electric || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Electric));

	if (ctx.flags.hit)
	{
		if ((ctx.defendingPokemon->GetStatus() != Status::Normal && isImmune) ||
			(ctx.defendingPokemon->GetStatus() == Status::Normal && isImmune))
		{
			deps.statusUI.DisplayDoesntAffectMsg();
		}
		else if (ctx.defendingPokemon->GetStatus() == Status::Paralyzed)
		{
			deps.statusUI.DisplayAlreadyParalyzedMsg();
		}
		else if ((ctx.defendingPokemon->GetStatus() != Status::Paralyzed && ctx.defendingPokemon->GetStatus() != Status::Normal) || ctx.defendingPokemon->HasSubstitute())
		{
			deps.resultsUI.DisplayFailedTextDialog();
		}
		else if (ctx.defendingPokemon->GetStatus() == Status::Normal && !isImmune)
		{
			InflictNVStatus(Status::Paralyzed, 100, deps);
		}
	}
	else
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void SleepPowder::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	bool isImmune = (ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Grass || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Grass);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	}
	else if (isImmune)
	{
		deps.statusUI.DisplayDoesntAffectMsg();
	}
	else if (ctx.defendingPokemon->GetStatus() == Status::Sleeping)
	{
		deps.statusUI.DisplayAlreadyAsleepMsg();
	}
	else if (ctx.defendingPokemon->GetStatus() != Status::Normal || (ctx.defendingPokemon->HasSubstitute() && ctx.currentMove->mp_move->CanBypassSubstitute()))
	{
		deps.resultsUI.DisplayFailedTextDialog();
	}
	else
	{
		InflictNVStatus(Status::Sleeping, 100, deps);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void SpeedDown2::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (ctx.defendingPlayer->HasMist() && ctx.flags.hit)
	{
		deps.statusUI.DisplayProtectedByMistMsg();

		ctx.currentMove->m_currentPP -= 1;

		deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		return;
	}

	if (ctx.flags.hit && ctx.defendingPokemon->HasSubstitute() && !ctx.currentMove->mp_move->CanBypassSubstitute())
	{
		deps.resultsUI.DisplayFailedTextDialog();
	}
	else if (ctx.flags.hit)
	{
		if (ctx.defendingPokemon->GetSpeedStage() == -5)
		{
			int lowerSpeed{ ctx.defendingPokemon->GetSpeedStage() - 1 };
			ctx.defendingPokemon->SetSpeedStage(lowerSpeed);

			deps.statusUI.DisplayStatLowered1Msg("speed");
		}
		else if (ctx.defendingPokemon->GetSpeedStage() > -5)
		{
			int lowerSpeed{ ctx.defendingPokemon->GetSpeedStage() - 2 };
			ctx.defendingPokemon->SetSpeedStage(lowerSpeed);

			deps.statusUI.DisplayStatLowered2Msg("speed");
		}
		else if (ctx.defendingPokemon->GetSpeedStage() < -5)
		{
			deps.statusUI.DisplayStatLoweredFailMsg("speed");
		}
	}
	else
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void DragonRage::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	int dragonRageDamage{ 40 };

	if (ctx.flags.hit)
	{
		if (ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Fairy || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Fairy)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
			dragonRageDamage = 0;
		}

		if (ctx.defendingPokemon->HasSubstitute())
		{
			if (dragonRageDamage > ctx.defendingPokemon->GetSubstituteHP())
			{
				dragonRageDamage = ctx.defendingPokemon->GetSubstituteHP();
			}
			ctx.defendingPokemon->DamageSubstitute(dragonRageDamage);
			deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
			deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		}
		else
		{
			if (dragonRageDamage > ctx.defendingPokemon->GetCurrentHP())
			{
				dragonRageDamage = ctx.defendingPokemon->GetCurrentHP();
			}
			ctx.defendingPokemon->DamageCurrentHP(dragonRageDamage);
		}

	}
	else
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
	}

	ctx.damageTaken = dragonRageDamage;

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void Paralyze::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);
	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	bool isElectricType = (ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Electric ||
		ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Electric);

	bool isImmune = (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No || isElectricType);

	// Special case: Glare affects Ghosts despite being Normal-type
	if (ctx.currentMove->mp_move->GetName() == "Glare")
	{
		bool isGhostType = (ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Ghost ||
			ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Ghost);

		if (isGhostType && !isElectricType)
			isImmune = false;
	}

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	}
	else if (isImmune)
	{
		deps.statusUI.DisplayDoesntAffectMsg();
	}
	else if (ctx.defendingPokemon->GetStatus() == Status::Paralyzed)
	{
		deps.statusUI.DisplayAlreadyParalyzedMsg();
	}
	else if (ctx.defendingPokemon->GetStatus() != Status::Normal || (ctx.defendingPokemon->HasSubstitute() && !ctx.currentMove->mp_move->CanBypassSubstitute()))
	{
		deps.resultsUI.DisplayFailedTextDialog();
	}
	else
	{
		InflictNVStatus(Status::Paralyzed, 100, deps);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void Earthquake::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (ctx.flags.hit)
	{
		// Damage multiplier for when defending Pokemon is SemiInvulnerableDig is in CalculateDamage()
		deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
		deps.resultsUI.DisplayCritTextDialog();
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
	}
	else
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void Dig::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	if (ctx.attackingPokemon->IsCharging())
	{
		ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

		if (ctx.flags.hit)
		{
			deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
			deps.resultsUI.DisplayCritTextDialog();
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
			deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
			deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		}
		else
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		}

		deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

		ctx.attackingPokemon->SetCharging(false);
		ctx.attackingPokemon->SetSemiInvulnerableDig(false);
		ctx.attackingPlayer->SetCanSwitch(true);
		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
		ctx.currentMove->m_currentPP -= 1;
	}
	else
	{
		deps.resultsUI.DisplayDigChargeMsg();
		ctx.attackingPokemon->SetCharging(true);
		ctx.attackingPokemon->SetSemiInvulnerableDig(true);
		ctx.attackingPlayer->SetCanSwitch(false);
	}
}

void Toxic::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	bool isImmune = ((ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Grass || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Grass) ||
		(ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Poison || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Poison) ||
		(ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Steel || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Steel));

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	}
	else if (isImmune)
	{
		deps.statusUI.DisplayDoesntAffectMsg();
	}
	else if (ctx.defendingPokemon->GetStatus() == Status::Poisoned)
	{
		deps.statusUI.DisplayAlreadyPoisonedMsg();
	}
	else if (ctx.defendingPokemon->GetStatus() != Status::Normal || (ctx.defendingPokemon->HasSubstitute() && ctx.currentMove->mp_move->CanBypassSubstitute()))
	{
		deps.resultsUI.DisplayFailedTextDialog();
	}
	else
	{
		InflictNVStatus(Status::Badly_Poisoned, 100, deps);
	}
	
	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void SleepMove::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (!ctx.flags.hit)
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	}
	else if (ctx.defendingPokemon->GetStatus() == Status::Sleeping)
	{
		deps.statusUI.DisplayAlreadyAsleepMsg();
	}
	else if (ctx.defendingPokemon->GetStatus() != Status::Normal || (ctx.defendingPokemon->HasSubstitute() && !ctx.currentMove->mp_move->CanBypassSubstitute()))
	{
		deps.resultsUI.DisplayFailedTextDialog();
	}
	else
	{
		InflictNVStatus(Status::Sleeping, 100, deps);
	}
	
	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void AttackUp::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

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

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void SpeedUp2::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

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
	else if (speedStage < 5)
	{
		speedStage += 2;
		ctx.attackingPokemon->SetSpeedStage(speedStage);
		deps.statusUI.DisplayStatRaised2Msg("speed");
	}

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void Rage::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (ctx.flags.hit)
	{
		deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
		deps.resultsUI.DisplayCritTextDialog();
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		ctx.attackingPokemon->SetRaging(true);
	}

	else
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void Teleport::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.currentMove->m_currentPP -= 1;

	if (ctx.attackingPlayer->GetPokemonCount() == 1)
	{
		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);
		deps.resultsUI.DisplayFailedTextDialog();
		return;
	}

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	deps.battleUI.SwitchPokemonOption(ctx.attackingPlayer, ctx.attackingPokemon);

	deps.switchExecutor.ExecuteSwitch(ctx.attackingPlayer, ctx.attackingPokemon);

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void NightShade::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	int nightShadeDamage = ctx.attackingPokemon->GetLevel();

	if (ctx.flags.hit)
	{
		if (ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Normal || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Normal)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
			nightShadeDamage = 0;
		}

		if (ctx.defendingPokemon->HasSubstitute())
		{
			if (nightShadeDamage > ctx.defendingPokemon->GetSubstituteHP())
			{
				nightShadeDamage = ctx.defendingPokemon->GetSubstituteHP();
			}
			ctx.defendingPokemon->DamageSubstitute(nightShadeDamage);
			deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
			deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		}
		else
		{
			if (nightShadeDamage > ctx.defendingPokemon->GetCurrentHP())
			{
				nightShadeDamage = ctx.defendingPokemon->GetCurrentHP();
			}
			ctx.defendingPokemon->DamageCurrentHP(nightShadeDamage);
		}

	}
	else
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
	}

	ctx.damageTaken = nightShadeDamage;

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void Mimic::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.currentMove->m_currentPP -= 1;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	if (ctx.defendingPokemon->GetLastUsedMove() == nullptr)
	{
		deps.resultsUI.DisplayFailedTextDialog();
		return;
	}

	if (ctx.defendingPokemon->GetLastUsedMove()->mp_move->GetName() == "Transform" ||
		ctx.defendingPokemon->GetLastUsedMove()->mp_move->GetName() == "Struggle" ||
		ctx.defendingPokemon->GetLastUsedMove()->mp_move->GetName() == "Metronome")
	{
		deps.resultsUI.DisplayFailedTextDialog();
		return;
	}

	if (ctx.defendingPokemon->IsSemiInvulnerable())
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		return;
	}

	ctx.attackingPokemon->SetUsedMimic(true);
	ctx.attackingPokemon->SetMimicPP(ctx.currentMove->m_currentPP);

	deps.statusUI.DisplayLearnedMimicMoveMsg();

	ctx.currentMove->mp_move = Database::GetInstance().GetPointerToMovedexNumber(
		ctx.defendingPokemon->GetLastUsedMove()->mp_move->GetMoveIndex() - 1);
	ctx.currentMove->m_currentPP = ctx.defendingPokemon->GetLastUsedMove()->mp_move->GetPP();
	ctx.currentMove->m_maxPP = ctx.defendingPokemon->GetLastUsedMove()->mp_move->GetPP();
	ctx.currentMove->b_isMimicked = true;
}

void DefenseDown2::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	if (ctx.defendingPlayer->HasMist() && ctx.flags.hit)
	{
		deps.statusUI.DisplayProtectedByMistMsg();

		ctx.currentMove->m_currentPP -= 1;

		deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		return;
	}

	if (ctx.flags.hit && ctx.defendingPokemon->HasSubstitute() && !ctx.currentMove->mp_move->CanBypassSubstitute())
	{
		deps.resultsUI.DisplayFailedTextDialog();
	}
	else if (ctx.flags.hit)
	{
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
		else if (ctx.defendingPokemon->GetDefenseStage() > -5)
		{
			int lowerDefense = ctx.defendingPokemon->GetDefenseStage() - 2;
			ctx.defendingPokemon->SetDefenseStage(lowerDefense);

			deps.statusUI.DisplayStatLowered2Msg("defense");
		}
	}
	else
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void EvasionUp::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

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

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void HealHalfHP::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	if (ctx.attackingPokemon->GetCurrentHP() < ctx.attackingPokemon->GetMaxHP())
	{
		double healAmount = std::ceil(static_cast<double>(ctx.attackingPokemon->GetMaxHP()) / 2.0);

		ctx.attackingPokemon->HealCurrentHP(static_cast<int>(healAmount));

		deps.resultsUI.DisplayRecoveredHPRestoredMsg(healAmount);
	}

	else
	{
		deps.resultsUI.DisplayHPFullMsg();
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void DefenseUp::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

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

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void Minimize::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

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
	else if (evasionStage < 5)
	{
		evasionStage += 2;
		ctx.attackingPokemon->SetEvasionStage(evasionStage);
		deps.statusUI.DisplayStatRaised2Msg("evasiveness");
		ctx.attackingPokemon->SetUsedMinimize(true);
	}

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void DefenseUp2::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	int defenseStage = ctx.attackingPokemon->GetDefenseStage();

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
	else if (defenseStage < 5)
	{
		defenseStage += 2;
		ctx.attackingPokemon->SetDefenseStage(defenseStage);
		deps.statusUI.DisplayStatRaised2Msg("defense");
	}

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void LightScreen::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	if (ctx.attackingPlayer->HasLightScreen())
	{
		deps.resultsUI.DisplayFailedTextDialog();
	}
	else
	{
		ctx.attackingPlayer->SetLightScreen(true);
		deps.statusUI.DisplayLightScreenMsg();
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void Haze::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

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

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void Reflect::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	if (ctx.attackingPlayer->HasReflect())
	{
		deps.resultsUI.DisplayFailedTextDialog();
	}
	else
	{
		ctx.attackingPlayer->SetReflect(true);
		deps.statusUI.DisplayReflectMsg();
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void FocusEnergy::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

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

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void Bide::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	if (!ctx.currentMove->b_isDisabled)
	{
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
			ctx.flags.hit = !ctx.defendingPokemon->IsSemiInvulnerable();
			
			deps.statusUI.DisplayBideUnleashedMsg();

			int bideDamage = ctx.attackingPokemon->GetBideDamage() * 2;

			if (ctx.flags.hit)
			{
				if (ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Ghost ||
					ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Ghost)
				{
					deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
					bideDamage = 0;
				}

				if (ctx.defendingPokemon->HasSubstitute())
				{
					if (bideDamage >= ctx.defendingPokemon->GetSubstituteHP())
					{
						bideDamage = ctx.defendingPokemon->GetSubstituteHP();
					}

					ctx.defendingPokemon->DamageSubstitute(bideDamage);
					deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
					deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
					ctx.damageTaken = bideDamage;
				}
				else
				{
					if (bideDamage >= ctx.defendingPokemon->GetCurrentHP())
					{
						bideDamage = ctx.defendingPokemon->GetCurrentHP();
					}

					ctx.defendingPokemon->DamageCurrentHP(bideDamage);
					ctx.damageTaken = bideDamage;
				}

			}
			else
			{
				deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
			}

			deps.statusProcessor.BideStop();
			deps.statusProcessor.BideReset();
		}
		else
		{
			ctx.attackingPokemon->IncrementBideCounter();
			deps.statusUI.DisplayBideStoringEnergyMsg();
		}
	}
	else
	{
		deps.statusUI.DisplayBideDisabledMsg();

		deps.statusProcessor.BideStop();
		deps.statusProcessor.BideReset();
	}

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void Metronome::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	size_t randomMod{ 67 };

	while (randomMod == 67 || randomMod == 101 || randomMod == 117 || randomMod == 118 || randomMod == 143)
	{
		std::uniform_int_distribution<size_t> randomModDistributor(0, 163);
		randomMod = randomModDistributor(deps.rng.GetGenerator());
	}

	Move* selectedMove = Database::GetInstance().GetPointerToMovedexNumber(randomMod);

	ctx.attackingPokemon->metronomeMove.mp_move = selectedMove;
	ctx.attackingPokemon->metronomeMove.m_currentPP = 1;
	ctx.attackingPokemon->metronomeMove.m_maxPP = 1;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	deps.statusUI.DisplayMetronomeMsg(selectedMove);

	ctx.currentMove->m_currentPP -= 1;

	{
		std::unique_ptr<IMoveRoutine> moveRoutine = MoveRoutineFactory::Call(selectedMove->GetMoveEffectEnum());
		moveRoutine->DoMove(deps);
	}

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void MirrorMove::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	if (ctx.defendingPokemon->GetLastUsedMove() == nullptr)
	{
		deps.resultsUI.DisplayFailedTextDialog();
		ctx.currentMove->m_currentPP -= 1;
		return;
	}

	BattlePokemon::pokemonMove* targetLastUsedMove = ctx.defendingPokemon->GetLastUsedMove();

	if (targetLastUsedMove->mp_move->IsAffectedByMirrorMove())
	{
		Move* selectedMove = Database::GetInstance().GetPointerToMovedexNumber(targetLastUsedMove->mp_move->GetMoveIndex() - 1);

		ctx.attackingPokemon->mirrorMove.mp_move = selectedMove;
		ctx.attackingPokemon->mirrorMove.m_currentPP = 1;
		ctx.attackingPokemon->mirrorMove.m_maxPP = 1;

		{
			std::unique_ptr<IMoveRoutine> moveRoutine = MoveRoutineFactory::Call(selectedMove->GetMoveEffectEnum());
			moveRoutine->DoMove(deps);
		}
	}
	else
	{
		deps.resultsUI.DisplayFailedTextDialog();
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void Explosion::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	if (ctx.flags.hit)
	{
		deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
		deps.resultsUI.DisplayCritTextDialog();
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
	}
	else
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
	}

	ctx.attackingPokemon->DamageCurrentHP(ctx.attackingPokemon->GetCurrentHP());

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void Swift::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	ctx.flags.hit = true;

	if (ctx.defendingPokemon->IsSemiInvulnerable())
	{
		ctx.flags.hit = false;
	}

	if (ctx.flags.hit)
	{
		deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
		deps.resultsUI.DisplayCritTextDialog();
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
	}
	else
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void SkullBash::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	if (ctx.attackingPokemon->IsCharging())
	{
		ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

		if (ctx.flags.hit)
		{
			deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
			deps.resultsUI.DisplayCritTextDialog();
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
			deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
			deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		}
		else
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		}

		deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

		ctx.attackingPokemon->SetCharging(false);
		ctx.attackingPlayer->SetCanSwitch(true);

		ctx.currentMove->m_currentPP -= 1;

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
	}
	else
	{
		deps.resultsUI.DisplaySkullBashChargeMsg();

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

		ctx.attackingPokemon->SetCharging(true);
		ctx.attackingPlayer->SetCanSwitch(false);
	}
}

void SpecialDefenseUp2::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

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
	else if (specialDefenseStage < 5)
	{
		specialDefenseStage += 2;
		ctx.attackingPokemon->SetSpecialDefenseStage(specialDefenseStage);
		deps.statusUI.DisplayStatRaised2Msg("special defense");
	}

	ctx.currentMove->m_currentPP -= 1;

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void DreamEater::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	if (ctx.defendingPokemon->GetStatus() != Status::Sleeping)
	{
		deps.statusUI.DisplayDoesntAffectMsg();
		ctx.currentMove->m_currentPP -= 1;
		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
		return;
	}

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	if (ctx.flags.hit)
	{
		deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
		deps.resultsUI.DisplayCritTextDialog();
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);

		double leechedHealth{};

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
			leechedHealth = floor(ctx.damageTaken / 2);
		}

		if (leechedHealth > (ctx.attackingPokemon->GetMaxHP() - ctx.attackingPokemon->GetCurrentHP()))
		{
			leechedHealth = (ctx.attackingPokemon->GetMaxHP() - ctx.attackingPokemon->GetCurrentHP());
		}

		ctx.attackingPokemon->HealCurrentHP(static_cast<int>(leechedHealth));

		deps.resultsUI.DisplayEnergyDrainedMsg();
	}
	else
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void PoisonGas::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	bool isImmune = ((ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Poison || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Poison) ||
		(ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Steel || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Steel));

	if (ctx.flags.hit)
	{
		if ((ctx.defendingPokemon->GetStatus() != Status::Normal && isImmune) ||
			(ctx.defendingPokemon->GetStatus() == Status::Normal && isImmune))
		{
			deps.statusUI.DisplayDoesntAffectMsg();
		}
		else if (ctx.defendingPokemon->GetStatus() == Status::Poisoned)
		{
			deps.statusUI.DisplayAlreadyPoisonedMsg();
		}
		else if (ctx.defendingPokemon->GetStatus() != Status::Poisoned && ctx.defendingPokemon->GetStatus() != Status::Normal)
		{
			deps.resultsUI.DisplayFailedTextDialog();
		}
		else if (ctx.defendingPokemon->GetStatus() == Status::Normal && !isImmune)
		{
			InflictNVStatus(Status::Poisoned, 100, deps);
		}
		else if (ctx.defendingPokemon->HasSubstitute())
		{
			deps.resultsUI.DisplayFailedTextDialog();
		}
	}
	else
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void SkyAttack::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	if (ctx.attackingPokemon->IsCharging())
	{
		ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

		if (ctx.flags.hit)
		{
			deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
			deps.resultsUI.DisplayCritTextDialog();
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
			deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

			std::uniform_int_distribution<int> randomModDistributor(1, 100);
			int randomMod = randomModDistributor(deps.rng.GetGenerator());

			if (randomMod <= 30 &&
				!ctx.defendingPlayer->IsFirst() &&
				ctx.flags.currentEffectiveness != BattleStateFlags::Effectiveness::No &&
				!ctx.defendingPokemon->HasSubstitute() &&
				ctx.defendingPokemon->GetCurrentHP() != 0)
			{
				ctx.defendingPokemon->SetIsFlinched(true);
			}

			deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		}
		else
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
		}

		deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

		ctx.attackingPokemon->SetCharging(false);
		ctx.attackingPlayer->SetCanSwitch(true);

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
		ctx.currentMove->m_currentPP -= 1;
	}
	else
	{
		deps.resultsUI.DisplaySkyAttackChargeMsg();

		ctx.attackingPokemon->SetCharging(true);
		ctx.attackingPlayer->SetCanSwitch(false);
	}
}

void Transform::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	if (ctx.attackingPokemon->IsTransformed() || ctx.defendingPokemon->IsTransformed() || ctx.defendingPokemon->HasSubstitute())
	{
		deps.resultsUI.DisplayFailedTextDialog();
		return;
	}

	ctx.flags.hit = true;

	if (ctx.defendingPokemon->IsSemiInvulnerable())
	{
		ctx.flags.hit = false;
	}

	if (ctx.flags.hit)
	{
		ctx.attackingPokemon->SetTransformation(ctx.defendingPokemon);
		deps.statusUI.DisplayTransformMsg();
	}
	else
	{
		deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void Psywave::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	double psywaveDamage = 0.0;

	if (ctx.flags.hit)
	{
		if (ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Dark || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Dark)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
			psywaveDamage = 0;
		}
		else
		{
			std::uniform_int_distribution<int> rngDist(0, 100);
			int randomNumber{ rngDist(deps.rng.GetGenerator()) };

			psywaveDamage = floor(ctx.attackingPokemon->GetLevel() * (randomNumber + 50) / 100);

			if (psywaveDamage == 0)
			{
				psywaveDamage = 1;
			}

			if (ctx.defendingPokemon->HasSubstitute())
			{
				ctx.defendingPokemon->DamageSubstitute(static_cast<int>(psywaveDamage));
				deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
				deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
			}
			else
			{
				ctx.defendingPokemon->DamageCurrentHP(static_cast<int>(psywaveDamage));
			}

		}
	}
	else
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
	}

	ctx.damageTaken = static_cast<int>(psywaveDamage);

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void Splash::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	deps.resultsUI.DisplaySplashMsg();

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void Rest::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

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

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void Conversion::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	if (ctx.attackingPokemon->GetTypeOneEnum() == ctx.currentMove->mp_move->GetMoveTypeEnum() ||
		ctx.attackingPokemon->GetTypeTwoEnum() == ctx.currentMove->mp_move->GetMoveTypeEnum() ||
		ctx.attackingPokemon->IsConverted())
	{
		deps.resultsUI.DisplayFailedTextDialog();
	}
	else
	{
		ctx.attackingPokemon->SetConversion(ctx.currentMove);

		deps.statusUI.DisplayConversionMsg();
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void TriAttack::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.flags.hit = deps.calculations.CalculateHitChance(ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	if (ctx.flags.hit)
	{
		deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
		deps.resultsUI.DisplayCritTextDialog();
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);

		std::uniform_int_distribution<int> randomModDistributor(1, 100);
		int randomNumber{ randomModDistributor(deps.rng.GetGenerator()) };

		if (randomNumber <= ctx.currentMove->mp_move->GetEffectChance()	 && ctx.flags.currentEffectiveness != BattleStateFlags::Effectiveness::No &&
			!ctx.defendingPokemon->HasSubstitute() && ctx.defendingPokemon->GetCurrentHP() != 0)
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

			deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		}
	}
	else
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void SuperFang::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.calculations.CalculateTypeEffectiveness(ctx.currentMove, ctx.defendingPokemon);

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	double superFangDamage{};

	if (ctx.defendingPokemon->HasSubstitute())
	{
		superFangDamage = floor(ctx.defendingPokemon->GetSubstituteHP() / 2);
	}
	else
	{
		superFangDamage = floor(ctx.defendingPokemon->GetCurrentHP() / 2);
	}

	if (superFangDamage <= 0)
	{
		superFangDamage = 1;
	}

	if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
	{
		deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		superFangDamage = 0;
	}

	if (ctx.defendingPokemon->HasSubstitute())
	{
		ctx.defendingPokemon->DamageSubstitute(static_cast<int>(superFangDamage));
		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
	}
	else
	{
		ctx.defendingPokemon->DamageCurrentHP(static_cast<int>(superFangDamage));
	}

	ctx.damageTaken = superFangDamage;

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void Substitute::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	if (ctx.attackingPokemon->HasSubstitute())
	{
		deps.statusUI.DisplayAlreadyHasSubstituteMsg();

		ctx.currentMove->m_currentPP -= 1;

		deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		return;
	}

	double substituteHP = floor(ctx.attackingPokemon->GetMaxHP() / 4);

	if (substituteHP >= ctx.attackingPokemon->GetCurrentHP())
	{
		deps.statusUI.DisplayNotEnoughHPSubstituteMsg();
	}
	else
	{
		deps.statusUI.DisplayPutInSubstituteMsg();
		ctx.attackingPokemon->SetSubstitute(true);
		ctx.attackingPokemon->SetSubstituteHP(static_cast<int>(substituteHP));
		ctx.attackingPokemon->DamageCurrentHP(static_cast<int>(substituteHP));
		ctx.attackingPokemon->SetBound(false);
		ctx.attackingPokemon->ResetBoundCounter();
		ctx.attackingPokemon->SetBoundTurnCount(0);
	}

	ctx.currentMove->m_currentPP -= 1;

	deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}

void Struggle::DoMove(MoveRoutineDeps& deps)
{
	auto& ctx = deps.context;

	ctx.flags.hit = true;

	if (ctx.defendingPokemon->IsSemiInvulnerable())
	{
		ctx.flags.hit = false;
	}

	deps.statusUI.DisplayNoMovesLeftStruggleMsg();

	deps.resultsUI.UsedTextDialog(ctx.attackingPlayer, ctx.currentMove, ctx.attackingPokemon);

	double recoilDamage{ 0 };

	if (ctx.flags.hit)
	{
		deps.calculations.CalculateDamage(ctx.defendingPlayer, ctx.currentMove, ctx.attackingPokemon, ctx.defendingPokemon);
		deps.resultsUI.DisplayCritTextDialog();
		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);

		deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);

		recoilDamage = round(ctx.attackingPokemon->GetMaxHP() / 4);
		ctx.attackingPokemon->DamageCurrentHP(static_cast<int>(recoilDamage));

		if (ctx.flags.currentEffectiveness != BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayRecoilMsg();

			deps.statusProcessor.CheckFaintCondition(ctx.attackingPlayer, ctx.defendingPlayer, ctx.attackingPokemon, ctx.defendingPokemon);
		}
	}
	else
	{
		deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer, ctx.attackingPokemon);
	}

	ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);
}