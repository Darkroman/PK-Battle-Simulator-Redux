#pragma once

#include <memory>

struct BattleContext;
class BattleCalculations;
class BattleStatusManager;
class TurnProcessor;
class IMoveResultsUI;
class IBattleMenuUI;
class IStatusEffectUI;
class RandomEngine;

enum class Status;
enum class MoveEffect;

struct MoveEffectsDependencies {
	BattleContext& context;
	BattleCalculations& calculations;
	BattleStatusManager& statusManager;
	TurnProcessor& turnProcessor;
	IMoveResultsUI& resultsUI;
	IBattleMenuUI& battleUI;
	IStatusEffectUI& statusUI;
	RandomEngine& rng;

	MoveEffectsDependencies
	(BattleContext& ctx,
	BattleCalculations& calc,
	BattleStatusManager& statusMgr,
	TurnProcessor& turnProc,
	IMoveResultsUI& results,
	IBattleMenuUI& BUI,
	IStatusEffectUI& statusUI,
	RandomEngine& randomEngine)
	
	: context(ctx)
	, calculations(calc)
	, statusManager(statusMgr)
	, turnProcessor(turnProc)
	, resultsUI(results)
	, battleUI(BUI)
	, statusUI(statusUI)
	, rng(randomEngine)
	{}
};

class IMoveEffects
{
public:
	virtual void DoMove(MoveEffectsDependencies&) = 0;
	virtual ~IMoveEffects() = default;

	void InflictNVStatus(Status, int, MoveEffectsDependencies&);
};

class MoveEffectsFactory
{
public:
	static std::unique_ptr<IMoveEffects> Call(MoveEffect);
};

class Noop : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class NormalHit : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class IncreasedCritical : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class MultiAttack : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class BurnHit : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class FreezeHit : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class ParalyzeHit10 : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class OHKO : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class RazorWind : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class AttackUp2 : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class Gust : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class MakeEnemySwitch : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class Fly : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class Bound : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class Stomp : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class DoubleHit : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class JumpKick : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class FlinchHit : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class AccuracyDown : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class BodySlam : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class RecoilQuarter : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class Thrash : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class RecoilThird : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class DefenseDown : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class PoisonHit : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class Twineedle : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class AttackDown : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class BypassSubSleep : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class Confuse : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class SonicBoom : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class Disable : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class SpecialDefenseDownHit : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class Mist : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class ConfuseHit : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class SpeedDownHit : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class AttackDownHit : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class RechargeAttack : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class LowKick : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class Counter : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class SeismicToss : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class Leech : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class LeechSeed : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class Growth : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class SolarBeam : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class PoisonPowder : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class StunSpore : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class SleepPowder : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class SpeedDown2 : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class DragonRage : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class Paralyze : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class ParalyzeHit30 : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class Earthquake : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class Dig : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class Toxic : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class SleepMove : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class AttackUp : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class SpeedUp2 : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class Rage : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class Teleport : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class NightShade : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class Mimic : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class DefenseDown2 : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class EvasionUp : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class HealHalfHP : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class DefenseUp : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class Minimize : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class DefenseUp2 : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class LightScreen : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class Haze : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class Reflect : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class FocusEnergy : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class Bide : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class Metronome : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class MirrorMove : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class Explosion : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class PoisonHit40 : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class FlinchHit10 : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class FlinchHit20 : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class Swift : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class SkullBash : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class SpecialDefenseUp2 : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class DreamEater : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class PoisonGas : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class SkyAttack : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class Transform : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class ConfuseHit20 : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class Psywave : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class Splash : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class Rest : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class Conversion : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class TriAttack : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class SuperFang : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class Substitute : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};

class Struggle : public IMoveEffects
{
public:
	void DoMove(MoveEffectsDependencies&) override;
};
