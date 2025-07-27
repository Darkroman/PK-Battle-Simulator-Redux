#pragma once

#include <memory>

struct BattleContext;
class BattleCalculations;
class StatusEffectProcessor;
class TurnProcessor;
class IMoveResultsUI;
class IBattleMenuUI;
class IStatusEffectUI;
class RandomEngine;
class SwitchExecutor;

enum class Status;
enum class MoveEffect;

struct MoveRoutineDeps {
	BattleContext& context;
	BattleCalculations& calculations;
	StatusEffectProcessor& statusProcessor;
	IMoveResultsUI& resultsUI;
	IBattleMenuUI& battleUI;
	IStatusEffectUI& statusUI;
	RandomEngine& rng;
	SwitchExecutor& switchExecutor;

	MoveRoutineDeps
	(BattleContext& ctx,
	BattleCalculations& calc,
	StatusEffectProcessor& statusProc,
	IMoveResultsUI& results,
	IBattleMenuUI& BUI,
	IStatusEffectUI& statusUI,
	RandomEngine& randomEngine,
	SwitchExecutor& switchExec)
	
	: context(ctx)
	, calculations(calc)
	, statusProcessor(statusProc)
	, resultsUI(results)
	, battleUI(BUI)
	, statusUI(statusUI)
	, rng(randomEngine)
	, switchExecutor(switchExec)
	{}
};

class IMoveRoutine
{
public:
	virtual void DoMove(MoveRoutineDeps&) = 0;
	virtual ~IMoveRoutine() = default;

	void InflictNVStatus(Status, int, MoveRoutineDeps&);
};

class MoveRoutineFactory
{
public:
	static std::unique_ptr<IMoveRoutine> Call(MoveEffect);
};

class Noop : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class NormalHit : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class IncreasedCritical : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class MultiAttack : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class BurnHit : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class FreezeHit : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class ParalyzeHit : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class OHKO : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class RazorWind : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class AttackUp2 : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class Gust : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class MakeEnemySwitch : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class Fly : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class Bound : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class Stomp : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class DoubleHit : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class JumpKick : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class FlinchHit : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class AccuracyDown : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class BodySlam : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class RecoilQuarter : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class Thrash : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class RecoilThird : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class DefenseDown : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class PoisonHit : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class Twineedle : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class AttackDown : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class BypassSubSleep : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class Confuse : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class SonicBoom : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class Disable : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class SpecialDefenseDownHit : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class Mist : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class ConfuseHit : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class SpeedDownHit : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class AttackDownHit : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class RechargeAttack : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class LowKick : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class Counter : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class SeismicToss : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class Leech : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class LeechSeed : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class Growth : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class SolarBeam : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class PoisonPowder : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class StunSpore : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class SleepPowder : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class SpeedDown2 : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class DragonRage : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class Paralyze : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class Earthquake : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class Dig : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class Toxic : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class SleepMove : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class AttackUp : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class SpeedUp2 : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class Rage : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class Teleport : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class NightShade : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class Mimic : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class DefenseDown2 : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class EvasionUp : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class HealHalfHP : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class DefenseUp : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class Minimize : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class DefenseUp2 : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class LightScreen : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class Haze : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class Reflect : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class FocusEnergy : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class Bide : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class Metronome : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class MirrorMove : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class Explosion : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class Swift : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class SkullBash : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class SpecialDefenseUp2 : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class DreamEater : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class PoisonGas : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class SkyAttack : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class Transform : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class Psywave : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class Splash : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class Rest : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class Conversion : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class TriAttack : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class SuperFang : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class Substitute : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};

class Struggle : public IMoveRoutine
{
public:
	void DoMove(MoveRoutineDeps&) override;
};