#include "MoveRoutines.h"

#include "MoveHelpers.h"
#include "MoveRoutineDeps.h"
#include "../data/Database.h"
#include "../battle/BattleCalculations.h"
#include "../battle/StatusEffectProcessor.h"
#include "../ui/interfaces/IMoveResultsUI.h"
#include "../battle/RandomEngine.h"
#include "../battle/SwitchExecutor.h"
#include "../data/Move.h"
#include "../data/StringToTypes.h"
#include "../entities/controllers/AIController.h"
#include "../entities/Player.h"

namespace MoveRoutines
{
	static inline constexpr std::array<RoutineFn, static_cast<size_t>(MoveEffect::COUNT)> JumpTable = []() {
		std::array<RoutineFn, static_cast<size_t>(MoveEffect::COUNT)> table{};

		table.fill(Noop);

		table[static_cast<size_t>(MoveEffect::NormalHit)] = NormalHit;
		table[static_cast<size_t>(MoveEffect::IncreasedCritical)] = IncreasedCritical;
		table[static_cast<size_t>(MoveEffect::MultiHit)] = MultiHit;
		table[static_cast<size_t>(MoveEffect::BurnHit)] = BurnHit;
		table[static_cast<size_t>(MoveEffect::FreezeHit)] = FreezeHit;
		table[static_cast<size_t>(MoveEffect::ParalyzeHit)] = ParalyzeHit;
		table[static_cast<size_t>(MoveEffect::OHKO)] = OHKO;
		table[static_cast<size_t>(MoveEffect::RazorWind)] = RazorWind;
		table[static_cast<size_t>(MoveEffect::AttackUp2)] = AttackUp2;
		table[static_cast<size_t>(MoveEffect::Gust)] = Gust;
		table[static_cast<size_t>(MoveEffect::ForceSwitch)] = ForceSwitch;
		table[static_cast<size_t>(MoveEffect::Fly)] = Fly;
		table[static_cast<size_t>(MoveEffect::PartialTrap)] = PartialTrap;
		table[static_cast<size_t>(MoveEffect::Stomp)] = Stomp;
		table[static_cast<size_t>(MoveEffect::DoubleHit)] = DoubleHit;
		table[static_cast<size_t>(MoveEffect::JumpKick)] = JumpKick;
		table[static_cast<size_t>(MoveEffect::FlinchHit)] = FlinchHit;
		table[static_cast<size_t>(MoveEffect::AccuracyDown)] = AccuracyDown;
		table[static_cast<size_t>(MoveEffect::BodySlam)] = BodySlam;
		table[static_cast<size_t>(MoveEffect::RecoilQuarter)] = RecoilQuarter;
		table[static_cast<size_t>(MoveEffect::Rampage)] = Rampage;
		table[static_cast<size_t>(MoveEffect::RecoilThird)] = RecoilThird;
		table[static_cast<size_t>(MoveEffect::DefenseDown)] = DefenseDown;
		table[static_cast<size_t>(MoveEffect::PoisonHit)] = PoisonHit;
		table[static_cast<size_t>(MoveEffect::Twineedle)] = Twineedle;
		table[static_cast<size_t>(MoveEffect::AttackDown)] = AttackDown;
		table[static_cast<size_t>(MoveEffect::SleepMove)] = SleepMove;
		table[static_cast<size_t>(MoveEffect::Confuse)] = Confuse;
		table[static_cast<size_t>(MoveEffect::SonicBoom)] = SonicBoom;
		table[static_cast<size_t>(MoveEffect::Disable)] = Disable;
		table[static_cast<size_t>(MoveEffect::SpecialDefenseDownHit)] = SpecialDefenseDownHit;
		table[static_cast<size_t>(MoveEffect::Mist)] = Mist;
		table[static_cast<size_t>(MoveEffect::ConfuseHit)] = ConfuseHit;
		table[static_cast<size_t>(MoveEffect::SpeedDownHit)] = SpeedDownHit;
		table[static_cast<size_t>(MoveEffect::AttackDownHit)] = AttackDownHit;
		table[static_cast<size_t>(MoveEffect::RechargeAttack)] = RechargeAttack;
		table[static_cast<size_t>(MoveEffect::LowKick)] = LowKick;
		table[static_cast<size_t>(MoveEffect::Counter)] = Counter;
		table[static_cast<size_t>(MoveEffect::SeismicToss)] = SeismicToss;
		table[static_cast<size_t>(MoveEffect::Leech)] = Leech;
		table[static_cast<size_t>(MoveEffect::LeechSeed)] = LeechSeed;
		table[static_cast<size_t>(MoveEffect::Growth)] = Growth;
		table[static_cast<size_t>(MoveEffect::SolarBeam)] = SolarBeam;
		table[static_cast<size_t>(MoveEffect::PoisonPowder)] = PoisonPowder;
		table[static_cast<size_t>(MoveEffect::StunSpore)] = StunSpore;
		table[static_cast<size_t>(MoveEffect::SleepPowder)] = SleepPowder;
		table[static_cast<size_t>(MoveEffect::SpeedDown2)] = SpeedDown2;
		table[static_cast<size_t>(MoveEffect::DragonRage)] = DragonRage;
		table[static_cast<size_t>(MoveEffect::Paralyze)] = Paralyze;
		table[static_cast<size_t>(MoveEffect::Earthquake)] = Earthquake;
		table[static_cast<size_t>(MoveEffect::Dig)] = Dig;
		table[static_cast<size_t>(MoveEffect::Toxic)] = Toxic;
		table[static_cast<size_t>(MoveEffect::AttackUp)] = AttackUp;
		table[static_cast<size_t>(MoveEffect::SpeedUp2)] = SpeedUp2;
		table[static_cast<size_t>(MoveEffect::Rage)] = Rage;
		table[static_cast<size_t>(MoveEffect::Teleport)] = Teleport;
		table[static_cast<size_t>(MoveEffect::NightShade)] = NightShade;
		table[static_cast<size_t>(MoveEffect::Mimic)] = Mimic;
		table[static_cast<size_t>(MoveEffect::DefenseDown2)] = DefenseDown2;
		table[static_cast<size_t>(MoveEffect::EvasionUp)] = EvasionUp;
		table[static_cast<size_t>(MoveEffect::HealHalfHP)] = HealHalfHP;
		table[static_cast<size_t>(MoveEffect::DefenseUp)] = DefenseUp;
		table[static_cast<size_t>(MoveEffect::Minimize)] = Minimize;
		table[static_cast<size_t>(MoveEffect::DefenseUp2)] = DefenseUp2;
		table[static_cast<size_t>(MoveEffect::LightScreen)] = LightScreen;
		table[static_cast<size_t>(MoveEffect::Haze)] = Haze;
		table[static_cast<size_t>(MoveEffect::Reflect)] = Reflect;
		table[static_cast<size_t>(MoveEffect::FocusEnergy)] = FocusEnergy;
		table[static_cast<size_t>(MoveEffect::Bide)] = Bide;
		table[static_cast<size_t>(MoveEffect::Metronome)] = Metronome;
		table[static_cast<size_t>(MoveEffect::MirrorMove)] = MirrorMove;
		table[static_cast<size_t>(MoveEffect::Explosion)] = Explosion;
		table[static_cast<size_t>(MoveEffect::AlwaysHit)] = AlwaysHit;
		table[static_cast<size_t>(MoveEffect::SkullBash)] = SkullBash;
		table[static_cast<size_t>(MoveEffect::SpecialDefenseUp2)] = SpecialDefenseUp2;
		table[static_cast<size_t>(MoveEffect::DreamEater)] = DreamEater;
		table[static_cast<size_t>(MoveEffect::PoisonGas)] = PoisonGas;
		table[static_cast<size_t>(MoveEffect::SkyAttack)] = SkyAttack;
		table[static_cast<size_t>(MoveEffect::Transform)] = Transform;
		table[static_cast<size_t>(MoveEffect::Psywave)] = Psywave;
		table[static_cast<size_t>(MoveEffect::Splash)] = Splash;
		table[static_cast<size_t>(MoveEffect::Rest)] = Rest;
		table[static_cast<size_t>(MoveEffect::Conversion)] = Conversion;
		table[static_cast<size_t>(MoveEffect::TriAttack)] = TriAttack;
		table[static_cast<size_t>(MoveEffect::SuperFang)] = SuperFang;
		table[static_cast<size_t>(MoveEffect::Substitute)] = Substitute;
		table[static_cast<size_t>(MoveEffect::Struggle)] = Struggle;

		return table;
		}();

	void Execute(MoveEffect ID, MoveRoutineDeps& deps)
	{
		JumpTable[static_cast<size_t>(ID)](deps);
	}

	void Noop(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());
		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.resultsUI.DisplayNoopMsg();
	}

	void NormalHit(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
			return;
		}

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		DamageRoutine(deps);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);
	}

	void IncreasedCritical(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
			return;
		}

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		size_t oldCritStage = ctx.attackingPokemon->GetCriticalHitStage();
		size_t newCritStage = oldCritStage + 1;

		ctx.attackingPokemon->SetCriticalHitStage(newCritStage);

		DamageRoutine(deps);

		ctx.attackingPokemon->SetCriticalHitStage(oldCritStage);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);
	}

	void MultiHit(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		ctx.currentMove->DeductPP();

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
			return;
		}

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		std::uniform_int_distribution<int> randomModDistributor(1, 100);
		int randomNumber = randomModDistributor(deps.rng.GetGenerator());

		int turnCount{};

		if (randomNumber <= 35)
			turnCount = 2;
		else if (randomNumber <= 70)
			turnCount = 3;
		else if (randomNumber <= 85)
			turnCount = 4;
		else
			turnCount = 5;

		MultiStrikeRoutine(deps, turnCount);

		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);
	}

	void BurnHit(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
			return;
		}

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		DamageRoutine(deps);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);

		InflictNVStatus(Status::Burned, ctx.currentMove->GetEffectChance(), deps);
	}

	void FreezeHit(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
			return;
		}

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		DamageRoutine(deps);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);

		InflictNVStatus(Status::Frozen, ctx.currentMove->GetEffectChance(), deps);
	}

	void ParalyzeHit(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
			return;
		}

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		DamageRoutine(deps);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);

		InflictNVStatus(Status::Paralyzed, ctx.currentMove->GetEffectChance(), deps);
	}

	void OHKO(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
			return;
		}

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		// OHKO specific logic done in CalculateDamage()
		DamageRoutine(deps);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);
	}

	void RazorWind(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		if (HandleCharging(deps, &IMoveResultsUI::DisplayRazorWindChargeMsg))
		{
			return;
		}

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
			return;
		}

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		size_t oldCritStage = ctx.attackingPokemon->GetCriticalHitStage();
		size_t newCritStage = oldCritStage + 1;

		ctx.attackingPokemon->SetCriticalHitStage(newCritStage);

		DamageRoutine(deps);

		ctx.attackingPokemon->SetCriticalHitStage(oldCritStage);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);
	}

	void AttackUp2(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		StageUpRoutine(deps, 2, "attack", [](BattlePokemon& p) { return p.GetAttackStage(); }, [](BattlePokemon& p, size_t val) { p.SetAttackStage(val); });
	}

	void Gust(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
			return;
		}

		// Specific accuracy check for gust vs SemiInvulnerableFly done in CalculateHitChance()
		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		if (ctx.defendingPokemon->IsSemiInvulnerableFromFly())
		{
			ctx.initialPowerMultiplier = 20;
		}

		DamageRoutine(deps);
	}

	void ForceSwitch(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		if (ctx.defendingPlayer->GetPokemonLeft() == 1 || !ctx.defendingPlayer->IsFirst())
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

		std::vector<BattlePokemon*> enemyPokemonList{};

		for (auto& candidateMon : ctx.defendingPlayer->GetBeltArray())
		{
			if (candidateMon.HasPokemon() && &candidateMon != ctx.defendingPokemon && !candidateMon.IsFainted())
			{
				enemyPokemonList.emplace_back(&candidateMon);
			}
		}

		std::uniform_int_distribution<size_t> randomModDistributor(0, enemyPokemonList.size() - 1);
		BattlePokemon* newMon = enemyPokemonList[randomModDistributor(deps.rng.GetGenerator())];

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
		}
		else
		{
			ctx.playerTwoCurrentPokemon = newMon;
		}

		for (auto player : ctx.vec_aiPlayers)
		{
			player->GetAIController().OnActivePokemonChanged(ctx);
		}

		deps.resultsUI.DisplayEnemySwitchMsg(newMon->GetNameView());
	}

	void Fly(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		ChargingMoveHooks hooks;
		hooks.preCharge = [](MoveRoutineDeps& deps) { deps.context.attackingPokemon->SetSemiInvulnerableFly(true); };
		hooks.postCharge = [](MoveRoutineDeps& deps) { deps.context.attackingPokemon->SetSemiInvulnerableFly(false); };

		if (HandleCharging(deps, &IMoveResultsUI::DisplayFlyChargeMsg, hooks))
		{
			return;
		}

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
			return;
		}

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		DamageRoutine(deps);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);
	}

	void PartialTrap(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
			return;
		}

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		DamageRoutine(deps);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.defendingPokemon->IsFainted() && !ctx.defendingPokemon->IsBound() && ctx.flags.currentEffectiveness != BattleStateFlags::Effectiveness::No && !ctx.flags.hitSubstitute)
		{
			bool isGhost = ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Ghost || ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Ghost;

			ctx.defendingPokemon->SetBound(true);

			if (!isGhost)
			{
				ctx.defendingPlayer->SetCanSwitch(false);
			}

			std::uniform_int_distribution<unsigned int> randomModDistributor(4, 5);
			unsigned int randomMod = randomModDistributor(deps.rng.GetGenerator());
			ctx.defendingPokemon->SetBoundTurnCount(randomMod);
			ctx.defendingPokemon->ResetBoundCounter();
			ctx.defendingPokemon->SetBoundMoveName(ctx.currentMove->GetMoveIndex());

			deps.resultsUI.BoundMoveText(ctx.attackingPlayer->GetPlayerNameView(), ctx.defendingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.defendingPokemon->GetNameView(), ctx.currentMove->GetMoveIndex());
		}
	}

	void Stomp(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

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
			ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);
		}

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		// Damage multiplier for when defending Pokemon has minimized is in CalculateDamage()
		DamageRoutine(deps);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);

		FlinchRoutine(deps);
	}

	void DoubleHit(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
			return;
		}

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		int turnCount = 2;

		MultiStrikeRoutine(deps, turnCount);

		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);
	}

	void JumpKick(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
			return;
		}

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());

			int crashDamage = ctx.attackingPokemon->GetMaxHP() / 2;
			ctx.attackingPokemon->DamageCurrentHP(crashDamage);

			deps.resultsUI.DisplayJumpKickCrashMsg(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		DamageRoutine(deps);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);
	}

	void FlinchHit(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
			return;
		}

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		DamageRoutine(deps);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);

		FlinchRoutine(deps);
	}

	void AccuracyDown(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

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

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
			return;
		}

		StageDownRoutine(deps, 1, "accuracy", [](BattlePokemon& p) { return p.GetAccuracyStage(); }, [](BattlePokemon& p, size_t val) { p.SetAccuracyStage(val); });
	}

	void BodySlam(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

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
			ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);
		}

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		// Damage multiplier for when defending Pokemon has minimized is in CalculateDamage()
		DamageRoutine(deps);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);

		InflictNVStatus(Status::Paralyzed, ctx.currentMove->GetEffectChance(), deps);
	}

	void RecoilQuarter(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
			return;
		}

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		bool hitSubstitute = ctx.defendingPokemon->HasSubstitute() && !ctx.currentMove->CanBypassSubstitute();

		int targetHPBegin = hitSubstitute ? ctx.defendingPokemon->GetSubstituteHP() : ctx.defendingPokemon->GetCurrentHP();

		DamageRoutine(deps);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);

		int targetHPEnd = hitSubstitute ? ctx.defendingPokemon->GetSubstituteHP() : ctx.defendingPokemon->GetCurrentHP();

		RecoilRoutine(deps, 4, targetHPBegin, targetHPEnd);
	}

	void Rampage(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		if (!ctx.attackingPokemon->IsThrashing())
		{
			ctx.currentMove->DeductPP();

			ctx.attackingPokemon->SetThrashing(true);
			ctx.attackingPlayer->SetCanSwitch(false);

			std::uniform_int_distribution<unsigned int> randomModDistributor(1, 2);
			unsigned int randomMod(randomModDistributor(deps.rng.GetGenerator()));
			ctx.attackingPokemon->SetThrashTurnCount(randomMod);
			ctx.attackingPokemon->ResetThrashCounter();
		}

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
		}

		if (ctx.flags.currentEffectiveness != BattleStateFlags::Effectiveness::No)
		{
			ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);
		}

		if (!ctx.flags.hit && ctx.flags.currentEffectiveness != BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		}

		if (ctx.flags.hit && ctx.flags.currentEffectiveness != BattleStateFlags::Effectiveness::No)
		{
			DamageRoutine(deps);

			deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
			deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);
		}

		bool reachedEnd = ctx.attackingPokemon->GetThrashCounter() >= ctx.attackingPokemon->GetThrashTurnCount();
		bool moveFailed = !ctx.flags.hit || ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No;

		if (moveFailed || reachedEnd)
		{
			if (reachedEnd && !ctx.attackingPokemon->IsConfused())
			{
				deps.statusProcessor.ThrashConfuse();
			}

			deps.statusProcessor.ThrashStop();
			deps.statusProcessor.ThrashReset();
		}
	}

	void RecoilThird(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

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

		DamageRoutine(deps);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);

		int targetHPEnd = hitSubstitute ? ctx.defendingPokemon->GetSubstituteHP() : ctx.defendingPokemon->GetCurrentHP();

		RecoilRoutine(deps, 3, targetHPBegin, targetHPEnd);
	}

	void DefenseDown(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

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

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
			return;
		}

		StageDownRoutine(deps, 1, "defense", [](BattlePokemon& p) { return p.GetDefenseStage(); }, [](BattlePokemon& p, size_t val) { p.SetDefenseStage(val); });
	}

	void PoisonHit(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
			return;
		}

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		DamageRoutine(deps);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);

		InflictNVStatus(Status::Poisoned, ctx.currentMove->GetEffectChance(), deps);
	}

	void Twineedle(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
			return;
		}

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		int turnCount = 2;

		MultiStrikeRoutine(deps, turnCount);

		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);
	}

	void AttackDown(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

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

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
			return;
		}

		StageDownRoutine(deps, 1, "attack", [](BattlePokemon& p) { return p.GetAttackStage(); }, [](BattlePokemon& p, size_t val) { p.SetAttackStage(val); });
	}

	void SleepMove(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

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

	void Confuse(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

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

		std::uniform_int_distribution<unsigned int> randomModDistributor(1, 4);
		unsigned int randomMod(randomModDistributor(deps.rng.GetGenerator()));
		ctx.defendingPokemon->SetConfusedTurnCount(randomMod);
		ctx.defendingPokemon->ResetConfusedCounter();

		deps.resultsUI.DisplayBecameConfuseMsg(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
	}

	void SonicBoom(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
			return;
		}

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		const int baseDamage = 20;

		FixedDamageRoutine(deps, baseDamage);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);
	}

	void Disable(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

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

	void SpecialDefenseDownHit(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
			return;
		}

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		DamageRoutine(deps);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);

		StageDownDamageRoutine(deps, 1, "special defense", [](BattlePokemon& p) { return p.GetSpecialDefenseStage(); }, [](BattlePokemon& p, size_t val) { p.SetSpecialDefenseStage(val); });
	}

	void Mist(MoveRoutineDeps& deps)
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

	void ConfuseHit(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
			return;
		}

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		}

		DamageRoutine(deps);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.defendingPokemon->IsFainted() && !ctx.defendingPokemon->IsConfused() && !ctx.flags.hitSubstitute)
		{
			std::uniform_int_distribution<int> randomModDistributor(1, 100);
			int randomNumber{ randomModDistributor(deps.rng.GetGenerator()) };

			if (randomNumber <= ctx.currentMove->GetEffectChance())
			{
				deps.resultsUI.DisplayBecameConfuseMsg(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());

				ctx.defendingPokemon->SetConfusedStatus(true);

				std::uniform_int_distribution<unsigned int> randomModDistributor(1, 4);
				unsigned int randomMod(randomModDistributor(deps.rng.GetGenerator()));
				ctx.defendingPokemon->SetConfusedTurnCount(randomMod);
				ctx.defendingPokemon->ResetConfusedCounter();
			}
		}
	}

	void SpeedDownHit(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
			return;
		}

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		DamageRoutine(deps);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);

		StageDownDamageRoutine(deps, 1, "speed", [](BattlePokemon& p) { return p.GetSpeedStage(); }, [](BattlePokemon& p, size_t val) { p.SetSpeedStage(val); });
	}

	void AttackDownHit(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
			return;
		}

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		DamageRoutine(deps);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);

		StageDownDamageRoutine(deps, 1, "attack", [](BattlePokemon& p) { return p.GetAttackStage(); }, [](BattlePokemon& p, size_t val) { p.SetAttackStage(val); });
	}

	void RechargeAttack(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
			return;
		}

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		DamageRoutine(deps);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (ctx.damageTaken > 0)
		{
			ctx.attackingPokemon->SetRecharging(true);
			ctx.attackingPlayer->SetCanSwitch(false);
		}
	}

	void LowKick(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
			return;
		}

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		// Low Kick power calculated in CalculateDamage()
		DamageRoutine(deps);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);
	}

	void Counter(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		ctx.currentMove->DeductPP();

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		int counterDamage = ctx.damageTaken * 2;

		const unsigned int lastDamage = ctx.damageTaken;

		ctx.damageTaken = 0;

		if (ctx.attackingPlayer->IsFirst())
		{
			deps.resultsUI.DisplayFailedTextDialog();
			return;
		}

		const auto* lastMove = ctx.defendingPokemon->GetLastUsedMove();
		const bool lastWasPhysical = lastMove && lastMove->GetCategoryEnum() == Category::Physical;

		if (!lastWasPhysical || lastDamage <= 0 || ctx.flags.hitSubstitute)
		{
			deps.resultsUI.DisplayFailedTextDialog();
			return;
		}

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
			return;
		}

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		FixedDamageRoutine(deps, counterDamage);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);
	}

	void SeismicToss(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
			return;
		}

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		const int baseDamage = ctx.defendingPokemon->GetLevel();

		FixedDamageRoutine(deps, baseDamage);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);
	}

	void Leech(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
			return;
		}

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		DamageRoutine(deps);

		unsigned int leechedHealth{ ctx.damageTaken / 2 };

		unsigned int finalLeech = std::min(std::max((unsigned int)1, leechedHealth), ctx.attackingPokemon->GetMaxHP() - ctx.attackingPokemon->GetCurrentHP());

		ctx.attackingPokemon->HealCurrentHP(finalLeech);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);

		if (finalLeech > 0)
		{
			deps.resultsUI.DisplayEnergyDrainedMsg(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		}

		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);
	}

	void LeechSeed(MoveRoutineDeps& deps)
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

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

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

	void Growth(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		StageUpRoutine(deps, 1, "attack", [](BattlePokemon& p) { return p.GetAttackStage(); }, [](BattlePokemon& p, size_t val) { p.SetAttackStage(val); });
		StageUpRoutine(deps, 1, "special attack", [](BattlePokemon& p) { return p.GetSpecialAttackStage(); }, [](BattlePokemon& p, size_t val) { p.SetSpecialAttackStage(val); });
	}

	void SolarBeam(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		if (HandleCharging(deps, &IMoveResultsUI::DisplaySolarBeamChargeMsg))
		{
			return;
		}

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
			return;
		}

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		DamageRoutine(deps);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);
	}

	void PoisonPowder(MoveRoutineDeps& deps)
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

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

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

	void StunSpore(MoveRoutineDeps& deps)
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

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

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

		if ((ctx.defendingPokemon->GetStatus() != Status::Normal) || (ctx.defendingPokemon->HasSubstitute() && !ctx.currentMove->CanBypassSubstitute()))
		{
			deps.resultsUI.DisplayFailedTextDialog();
			return;
		}

		InflictNVStatus(Status::Paralyzed, 100, deps);
	}

	void SleepPowder(MoveRoutineDeps& deps)
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

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

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

		if ((ctx.defendingPokemon->GetStatus() != Status::Normal) || (ctx.defendingPokemon->HasSubstitute() && !ctx.currentMove->CanBypassSubstitute()))
		{
			deps.resultsUI.DisplayFailedTextDialog();
			return;
		}

		InflictNVStatus(Status::Sleeping, 100, deps);
	}

	void SpeedDown2(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

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

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
			return;
		}

		StageDownRoutine(deps, 2, "speed", [](BattlePokemon& p) { return p.GetSpeedStage(); }, [](BattlePokemon& p, size_t val) { p.SetSpeedStage(val); });
	}

	void DragonRage(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
			return;
		}

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		const int baseDamage = 40;

		FixedDamageRoutine(deps, baseDamage);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);
	}

	void Paralyze(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		bool isElectricType = (ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Electric ||
			ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Electric);

		bool electricVsGround = ctx.currentMove->GetMoveTypeEnum() == PokemonType::Electric && ctx.defendingPokemon->GetTypeOneEnum() == PokemonType::Ground ||
			ctx.defendingPokemon->GetTypeTwoEnum() == PokemonType::Ground;

		bool isImmune = (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No || isElectricType || electricVsGround);

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

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

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

	void Earthquake(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
			return;
		}

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		// Damage multiplier for when defending Pokemon is SemiInvulnerableDig is in CalculateDamage()
		DamageRoutine(deps);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);
	}

	void Dig(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		ChargingMoveHooks hooks;
		hooks.preCharge = [](MoveRoutineDeps& deps) { deps.context.attackingPokemon->SetSemiInvulnerableDig(true); };
		hooks.postCharge = [](MoveRoutineDeps& deps) { deps.context.attackingPokemon->SetSemiInvulnerableDig(false); };

		if (HandleCharging(deps, &IMoveResultsUI::DisplayDigChargeMsg, hooks))
		{
			return;
		}

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
			return;
		}

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		DamageRoutine(deps);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);
	}

	void Toxic(MoveRoutineDeps& deps)
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

		if (ctx.attackingPokemon->GetTypeOneEnum() == PokemonType::Poison || ctx.attackingPokemon->GetTypeTwoEnum() == PokemonType::Poison)
		{
			ctx.flags.hit = true;
		}
		else
		{
			ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);
		}

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

	void AttackUp(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		StageUpRoutine(deps, 1, "attack", [](BattlePokemon& p) { return p.GetAttackStage(); }, [](BattlePokemon& p, size_t val) { p.SetAttackStage(val); });
	}

	void SpeedUp2(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		StageUpRoutine(deps, 2, "speed", [](BattlePokemon& p) { return p.GetSpeedStage(); }, [](BattlePokemon& p, size_t val) { p.SetSpeedStage(val); });
	}

	void Rage(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
			return;
		}

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		DamageRoutine(deps);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);

		ctx.attackingPokemon->SetRaging(true);
	}

	void Teleport(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		if (ctx.attackingPlayer->GetPokemonLeft() == 1)
		{
			deps.resultsUI.DisplayFailedTextDialog();
			return;
		}

		BattlePokemon* newPokemon = ctx.attackingPlayer->GetController().PromptForSwitch(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);
		ctx.attackingPlayer->SetPokemonToSwitchTo(newPokemon);

		deps.switchExecutor.ExecuteSwitch(*ctx.attackingPlayer, ctx.attackingPokemon);
	}

	void NightShade(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
			return;
		}

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		const int baseDamage = ctx.defendingPokemon->GetLevel();

		FixedDamageRoutine(deps, baseDamage);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);
	}

	void Mimic(MoveRoutineDeps& deps)
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

	void DefenseDown2(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

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

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackAvoidedTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
			return;
		}

		StageDownRoutine(deps, 2, "defense", [](BattlePokemon& p) { return p.GetDefenseStage(); }, [](BattlePokemon& p, size_t val) { p.SetDefenseStage(val); });
	}

	void EvasionUp(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		StageUpRoutine(deps, 1, "evasion", [](BattlePokemon& p) { return p.GetEvasionStage(); }, [](BattlePokemon& p, size_t val) { p.SetEvasionStage(val); });
	}

	void HealHalfHP(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		if (ctx.attackingPokemon->GetCurrentHP() < ctx.attackingPokemon->GetMaxHP())
		{
			unsigned int healAmount = (ctx.attackingPokemon->GetMaxHP() + 1) / 2;

			ctx.attackingPokemon->HealCurrentHP(healAmount);

			deps.resultsUI.DisplayRecoveredHPRestoredMsg(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		}

		else
		{
			deps.resultsUI.DisplayHPFullMsg(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		}
	}

	void DefenseUp(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		StageUpRoutine(deps, 1, "defense", [](BattlePokemon& p) { return p.GetDefenseStage(); }, [](BattlePokemon& p, size_t val) { p.SetDefenseStage(val); });
	}

	void Minimize(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		StageUpRoutine(deps, 2, "evasion", [](BattlePokemon& p) { return p.GetEvasionStage(); }, [](BattlePokemon& p, size_t val) { p.SetEvasionStage(val); });
	}

	void DefenseUp2(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		StageUpRoutine(deps, 2, "defense", [](BattlePokemon& p) { return p.GetDefenseStage(); }, [](BattlePokemon& p, size_t val) { p.SetDefenseStage(val); });

	}

	void LightScreen(MoveRoutineDeps& deps)
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

	void Haze(MoveRoutineDeps& deps)
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

	void Reflect(MoveRoutineDeps& deps)
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

	void FocusEnergy(MoveRoutineDeps& deps)
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

	void Bide(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		if (!ctx.attackingPokemon->IsBiding())
		{
			ctx.currentMove->DeductPP();

			deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

			ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

			ctx.attackingPokemon->SetBide(true);
			ctx.attackingPlayer->SetCanSwitch(false);

			ctx.attackingPokemon->SetBideTurnCount(2);
			ctx.attackingPokemon->ResetBideCounter();
		}

		bool isUnleashing = ctx.attackingPokemon->GetBideCounter() >= ctx.attackingPokemon->GetBideTurnCount();

		if (isUnleashing)
		{
			deps.resultsUI.DisplayBideUnleashedMsg(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());

			deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

			if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
			{
				deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
			}

			if (ctx.flags.currentEffectiveness != BattleStateFlags::Effectiveness::No)
			{
				ctx.flags.hit = !ctx.defendingPokemon->IsSemiInvulnerable();
			}

			if (ctx.flags.currentEffectiveness != BattleStateFlags::Effectiveness::No && !ctx.flags.hit)
			{
				deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			}
		}

		if (isUnleashing && ctx.flags.hit && ctx.flags.currentEffectiveness != BattleStateFlags::Effectiveness::No)
		{
			int bideDamage = ctx.attackingPokemon->GetBideDamage() * 2;

			if (bideDamage <= 0)
			{
				deps.resultsUI.DisplayFailedTextDialog();
			}
			else
			{
				FixedDamageRoutine(deps, bideDamage);
			}

			deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
			deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);
		}

		if (isUnleashing)
		{
			deps.statusProcessor.BideStop();
			deps.statusProcessor.BideReset();
		}
		else
		{
			deps.resultsUI.DisplayBideStoringEnergyMsg(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		}
	}

	void Metronome(MoveRoutineDeps& deps)
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

		const Move* selectedMove = Database::GetInstance().GetPointerToMovedexNumber(randomMod);

		deps.resultsUI.DisplayMetronomeMsg(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), selectedMove->GetName());

		ctx.attackingPokemon->SetMetronome(*selectedMove);

		{
			Execute(deps.context.currentMove->GetMoveEffectEnum(), deps);
		}
	}

	void MirrorMove(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		pokemonMove* targetLastUsedMove = ctx.defendingPokemon->GetLastUsedMove();

		if (!targetLastUsedMove || !targetLastUsedMove->IsAffectedByMirrorMove())
		{
			deps.resultsUI.DisplayFailedTextDialog();
			return;
		}

		const Move* selectedMove = Database::GetInstance().GetPointerToMovedexNumber(targetLastUsedMove->GetMoveIndex() - 1);

		ctx.attackingPokemon->SetMirrorMove(*selectedMove);

		{
			Execute(deps.context.currentMove->GetMoveEffectEnum(), deps);
		}
	}

	void Explosion(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		ctx.attackingPokemon->DamageCurrentHP(ctx.attackingPokemon->GetCurrentHP());

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));

			deps.statusProcessor.CheckFaintCondition(*ctx.defendingPlayer, *ctx.attackingPlayer, *ctx.defendingPokemon, *ctx.attackingPokemon);

			return;
		}

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());

			deps.statusProcessor.CheckFaintCondition(*ctx.defendingPlayer, *ctx.attackingPlayer, *ctx.defendingPokemon, *ctx.attackingPokemon);

			return;
		}

		deps.statusProcessor.CheckFaintCondition(*ctx.defendingPlayer, *ctx.attackingPlayer, *ctx.defendingPokemon, *ctx.attackingPokemon);

		DamageRoutine(deps);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);
	}

	void AlwaysHit(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

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

		DamageRoutine(deps);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);
	}

	void SkullBash(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		ChargingMoveHooks hooks;
		hooks.stageUp = &StageUpRoutine;
		hooks.getStage = [](BattlePokemon& p) { return p.GetDefenseStage(); };
		hooks.setStage = [](BattlePokemon& p, size_t val) { p.SetDefenseStage(val); };
		hooks.stageIncreaseAmount = 1;
		hooks.stageName = "defense";

		if (HandleCharging(deps, &IMoveResultsUI::DisplaySkullBashChargeMsg, hooks))
		{
			return;
		}

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
			return;
		}

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		DamageRoutine(deps);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);
	}

	void SpecialDefenseUp2(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		StageUpRoutine(deps, 2, "special defense", [](BattlePokemon& p) { return p.GetSpecialDefenseStage(); }, [](BattlePokemon& p, size_t val) { p.SetSpecialDefenseStage(val); });
	}

	void DreamEater(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.defendingPokemon->GetStatus() != Status::Sleeping || ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayDoesntAffectMsg(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
			return;
		}

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		DamageRoutine(deps);

		unsigned int leechedHealth{ ctx.damageTaken / 2 };

		unsigned int finalLeech = std::min(std::max((unsigned int)1, leechedHealth), ctx.attackingPokemon->GetMaxHP() - ctx.attackingPokemon->GetCurrentHP());

		ctx.attackingPokemon->HealCurrentHP(finalLeech);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);

		if (finalLeech > 0)
		{
			deps.resultsUI.DisplayEnergyDrainedMsg(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView());
		}

		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);
	}

	void PoisonGas(MoveRoutineDeps& deps)
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

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

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

	void SkyAttack(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		if (HandleCharging(deps, &IMoveResultsUI::DisplaySkyAttackChargeMsg))
		{
			return;
		}

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
			return;
		}

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		size_t oldCritStage = ctx.attackingPokemon->GetCriticalHitStage();
		size_t newCritStage = oldCritStage + 1;

		ctx.attackingPokemon->SetCriticalHitStage(newCritStage);

		DamageRoutine(deps);

		ctx.attackingPokemon->SetCriticalHitStage(oldCritStage);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);

		FlinchRoutine(deps);
	}

	void Transform(MoveRoutineDeps& deps)
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

	void Psywave(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
			return;
		}

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		unsigned int psywaveDamage = 0;

		std::uniform_int_distribution<unsigned int> rngDist(0, 100);
		unsigned int randomNumber{ rngDist(deps.rng.GetGenerator()) };

		psywaveDamage = std::max((unsigned int)1, ctx.attackingPokemon->GetLevel() * (randomNumber + 50) / 100);

		FixedDamageRoutine(deps, psywaveDamage);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);
	}

	void Splash(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.resultsUI.DisplaySplashMsg();
	}

	void Rest(MoveRoutineDeps& deps)
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

			unsigned int healAmount = ctx.attackingPokemon->GetMaxHP() - ctx.attackingPokemon->GetCurrentHP();
			ctx.attackingPokemon->HealCurrentHP(healAmount);
		}
		else if (ctx.attackingPokemon->GetCurrentHP() >= ctx.attackingPokemon->GetMaxHP())
		{
			deps.resultsUI.DisplayHPFullMsg(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		}
	}

	void Conversion(MoveRoutineDeps& deps)
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

	void TriAttack(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
			return;
		}

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		DamageRoutine(deps);

		std::uniform_int_distribution<int> randomModDistributor(1, 100);
		int randomNumber{ randomModDistributor(deps.rng.GetGenerator()) };

		if (randomNumber <= ctx.currentMove->GetEffectChance() &&
			(!ctx.defendingPokemon->HasSubstitute() || ctx.currentMove->CanBypassSubstitute()) &&
			ctx.defendingPokemon->GetCurrentHP() != 0)
		{
			std::uniform_int_distribution<unsigned int> statusDist(1, 3);
			unsigned int randomStatus{ statusDist(deps.rng.GetGenerator()) };

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

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);
	}

	void SuperFang(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.currentMove->DeductPP();

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

		if (ctx.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No)
		{
			deps.resultsUI.DisplayEffectivenessTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ToEffectivenessText(ctx.flags.currentEffectiveness));
			return;
		}

		ctx.flags.hit = deps.calculations.CalculateHitChance(*ctx.currentMove, *ctx.attackingPokemon, *ctx.defendingPokemon);

		if (!ctx.flags.hit)
		{
			deps.resultsUI.DisplayAttackMissedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
			return;
		}

		bool hasSubstitute = ctx.defendingPokemon->HasSubstitute() && !ctx.currentMove->CanBypassSubstitute();

		int hpSource = hasSubstitute ? ctx.defendingPokemon->GetSubstituteHP() : ctx.defendingPokemon->GetCurrentHP();

		int finalDamage = std::max(1, hpSource / 2);

		deps.calculations.ApplyDamage(*ctx.currentMove, *ctx.defendingPokemon, finalDamage);
		deps.resultsUI.DisplaySubstituteDamageTextDialog(ctx.defendingPlayer->GetPlayerNameView(), ctx.defendingPokemon->GetNameView(), ctx.defendingPokemon->GetSubstituteHP(), ctx.defendingPokemon->HasSubstitute(), ctx.flags.hitSubstitute);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);
	}

	void Substitute(MoveRoutineDeps& deps)
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

		unsigned int substituteHP = ctx.attackingPokemon->GetMaxHP() / 4;

		if (substituteHP >= ctx.attackingPokemon->GetCurrentHP())
		{
			deps.resultsUI.DisplayNotEnoughHPSubstituteMsg();
			return;
		}

		unsigned int hpCost = substituteHP;

		deps.resultsUI.DisplayPutInSubstituteMsg(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());
		ctx.attackingPokemon->SetSubstitute(true);
		ctx.attackingPokemon->SetSubstituteHP(hpCost);
		ctx.attackingPokemon->DamageCurrentHP(hpCost);
		ctx.attackingPokemon->SetBound(false);
		ctx.attackingPokemon->ResetBoundCounter();
		ctx.attackingPokemon->SetBoundTurnCount(0);
	}

	void Struggle(MoveRoutineDeps& deps)
	{
		auto& ctx = deps.context;

		deps.resultsUI.DisplayNoMovesLeftStruggleMsg(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());

		deps.resultsUI.UsedTextDialog(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView(), ctx.currentMove->GetName());

		ctx.attackingPokemon->SetLastUsedMove(ctx.currentMove);

		deps.calculations.CalculateTypeEffectiveness(ctx, *ctx.currentMove, *ctx.defendingPokemon);

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

		DamageRoutine(deps);

		deps.statusProcessor.CheckSubstituteCondition(ctx.defendingPlayer, ctx.defendingPokemon);
		deps.statusProcessor.CheckFaintCondition(*ctx.attackingPlayer, *ctx.defendingPlayer, *ctx.attackingPokemon, *ctx.defendingPokemon);

		int recoilDamage = (ctx.attackingPokemon->GetMaxHP() + 2) / 4;

		int finalDamage = std::max(1, recoilDamage);

		ctx.attackingPokemon->DamageCurrentHP(finalDamage);

		deps.resultsUI.DisplayRecoilMsg(ctx.attackingPlayer->GetPlayerNameView(), ctx.attackingPokemon->GetNameView());

		deps.statusProcessor.CheckFaintCondition(*ctx.defendingPlayer, *ctx.attackingPlayer, *ctx.defendingPokemon, *ctx.attackingPokemon);
	}
}