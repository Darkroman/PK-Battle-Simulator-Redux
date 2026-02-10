#pragma once

#include <memory>

#include "../moves/MoveEffectEnums.h"

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
	RandomEngine& rng;
	SwitchExecutor& switchExecutor;

	MoveRoutineDeps
	(BattleContext& ctx,
	BattleCalculations& calc,
	StatusEffectProcessor& statusProc,
	IMoveResultsUI& results,
	IBattleMenuUI& BUI,
	RandomEngine& randomEngine,
	SwitchExecutor& switchExec)
	
	: context(ctx)
	, calculations(calc)
	, statusProcessor(statusProc)
	, resultsUI(results)
	, battleUI(BUI)
	, rng(randomEngine)
	, switchExecutor(switchExec)
	{}
};

void InflictNVStatus(Status, int, MoveRoutineDeps&);

void BasicDamageRoutine(MoveRoutineDeps&);

class Noop
{
public:
	void DoMove(MoveRoutineDeps&);
};

class NormalHit
{
public:
	void DoMove(MoveRoutineDeps&);
};

class IncreasedCritical
{
public:
	void DoMove(MoveRoutineDeps&);
};

class MultiHit
{
public:
	void DoMove(MoveRoutineDeps&);
};

class BurnHit
{
public:
	void DoMove(MoveRoutineDeps&);
};

class FreezeHit
{
public:
	void DoMove(MoveRoutineDeps&);
};

class ParalyzeHit
{
public:
	void DoMove(MoveRoutineDeps&);
};

class OHKO
{
public:
	void DoMove(MoveRoutineDeps&);
};

class RazorWind
{
public:
	void DoMove(MoveRoutineDeps&);
};

class AttackUp2
{
public:
	void DoMove(MoveRoutineDeps&);
};

class Gust
{
public:
	void DoMove(MoveRoutineDeps&);
};

class ForceSwitch
{
public:
	void DoMove(MoveRoutineDeps&);
};

class Fly
{
public:
	void DoMove(MoveRoutineDeps&);
};

class PartialTrap
{
public:
	void DoMove(MoveRoutineDeps&);
};

class Stomp
{
public:
	void DoMove(MoveRoutineDeps&);
};

class DoubleHit
{
public:
	void DoMove(MoveRoutineDeps&);
};

class JumpKick
{
public:
	void DoMove(MoveRoutineDeps&);
};

class FlinchHit
{
public:
	void DoMove(MoveRoutineDeps&);
};

class AccuracyDown
{
public:
	void DoMove(MoveRoutineDeps&);
};

class BodySlam
{
public:
	void DoMove(MoveRoutineDeps&);
};

class RecoilQuarter
{
public:
	void DoMove(MoveRoutineDeps&);
};

class Rampage
{
public:
	void DoMove(MoveRoutineDeps&);
};

class RecoilThird
{
public:
	void DoMove(MoveRoutineDeps&);
};

class DefenseDown
{
public:
	void DoMove(MoveRoutineDeps&);
};

class PoisonHit
{
public:
	void DoMove(MoveRoutineDeps&);
};

class Twineedle
{
public:
	void DoMove(MoveRoutineDeps&);
};

class AttackDown
{
public:
	void DoMove(MoveRoutineDeps&);
};

class SleepMove
{
public:
	void DoMove(MoveRoutineDeps&);
};

class Confuse
{
public:
	void DoMove(MoveRoutineDeps&);
};

class SonicBoom
{
public:
	void DoMove(MoveRoutineDeps&);
};

class Disable
{
public:
	void DoMove(MoveRoutineDeps&);
};

class SpecialDefenseDownHit
{
public:
	void DoMove(MoveRoutineDeps&);
};

class Mist
{
public:
	void DoMove(MoveRoutineDeps&);
};

class ConfuseHit
{
public:
	void DoMove(MoveRoutineDeps&);
};

class SpeedDownHit
{
public:
	void DoMove(MoveRoutineDeps&);
};

class AttackDownHit
{
public:
	void DoMove(MoveRoutineDeps&);
};

class RechargeAttack
{
public:
	void DoMove(MoveRoutineDeps&);
};

class LowKick
{
public:
	void DoMove(MoveRoutineDeps&);
};

class Counter
{
public:
	void DoMove(MoveRoutineDeps&);
};

class SeismicToss
{
public:
	void DoMove(MoveRoutineDeps&);
};

class Leech
{
public:
	void DoMove(MoveRoutineDeps&);
};

class LeechSeed
{
public:
	void DoMove(MoveRoutineDeps&);
};

class Growth
{
public:
	void DoMove(MoveRoutineDeps&);
};

class SolarBeam
{
public:
	void DoMove(MoveRoutineDeps&);
};

class PoisonPowder
{
public:
	void DoMove(MoveRoutineDeps&);
};

class StunSpore
{
public:
	void DoMove(MoveRoutineDeps&);
};

class SleepPowder
{
public:
	void DoMove(MoveRoutineDeps&);
};

class SpeedDown2
{
public:
	void DoMove(MoveRoutineDeps&);
};

class DragonRage
{
public:
	void DoMove(MoveRoutineDeps&);
};

class Paralyze
{
public:
	void DoMove(MoveRoutineDeps&);
};

class Earthquake
{
public:
	void DoMove(MoveRoutineDeps&);
};

class Dig
{
public:
	void DoMove(MoveRoutineDeps&);
};

class Toxic
{
public:
	void DoMove(MoveRoutineDeps&);
};

class AttackUp
{
public:
	void DoMove(MoveRoutineDeps&);
};

class SpeedUp2
{
public:
	void DoMove(MoveRoutineDeps&);
};

class Rage
{
public:
	void DoMove(MoveRoutineDeps&);
};

class Teleport
{
public:
	void DoMove(MoveRoutineDeps&);
};

class NightShade
{
public:
	void DoMove(MoveRoutineDeps&);
};

class Mimic
{
public:
	void DoMove(MoveRoutineDeps&);
};

class DefenseDown2
{
public:
	void DoMove(MoveRoutineDeps&);
};

class EvasionUp
{
public:
	void DoMove(MoveRoutineDeps&);
};

class HealHalfHP
{
public:
	void DoMove(MoveRoutineDeps&);
};

class DefenseUp
{
public:
	void DoMove(MoveRoutineDeps&);
};

class Minimize
{
public:
	void DoMove(MoveRoutineDeps&);
};

class DefenseUp2
{
public:
	void DoMove(MoveRoutineDeps&);
};

class LightScreen
{
public:
	void DoMove(MoveRoutineDeps&);
};

class Haze
{
public:
	void DoMove(MoveRoutineDeps&);
};

class Reflect
{
public:
	void DoMove(MoveRoutineDeps&);
};

class FocusEnergy
{
public:
	void DoMove(MoveRoutineDeps&);
};

class Bide
{
public:
	void DoMove(MoveRoutineDeps&);
};

class Metronome
{
public:
	void DoMove(MoveRoutineDeps&);
};

class MirrorMove
{
public:
	void DoMove(MoveRoutineDeps&);
};

class Explosion
{
public:
	void DoMove(MoveRoutineDeps&);
};

class AlwaysHit
{
public:
	void DoMove(MoveRoutineDeps&);
};

class SkullBash
{
public:
	void DoMove(MoveRoutineDeps&);
};

class SpecialDefenseUp2
{
public:
	void DoMove(MoveRoutineDeps&);
};

class DreamEater
{
public:
	void DoMove(MoveRoutineDeps&);
};

class PoisonGas
{
public:
	void DoMove(MoveRoutineDeps&);
};

class SkyAttack
{
public:
	void DoMove(MoveRoutineDeps&);
};

class Transform
{
public:
	void DoMove(MoveRoutineDeps&);
};

class Psywave
{
public:
	void DoMove(MoveRoutineDeps&);
};

class Splash
{
public:
	void DoMove(MoveRoutineDeps&);
};

class Rest
{
public:
	void DoMove(MoveRoutineDeps&);
};

class Conversion
{
public:
	void DoMove(MoveRoutineDeps&);
};

class TriAttack
{
public:
	void DoMove(MoveRoutineDeps&);
};

class SuperFang
{
public:
	void DoMove(MoveRoutineDeps&);
};

class Substitute
{
public:
	void DoMove(MoveRoutineDeps&);
};

class Struggle
{
public:
	void DoMove(MoveRoutineDeps&);
};

using MoveRoutineTypes = std::variant<
	Noop,
	NormalHit,
	IncreasedCritical,
	MultiHit,
	BurnHit,
	FreezeHit,
	ParalyzeHit,
	OHKO,
	RazorWind,
	AttackUp2,
	Gust,
	ForceSwitch,
	Fly,
	PartialTrap,
	Stomp,
	DoubleHit,
	JumpKick,
	FlinchHit,
	AccuracyDown,
	BodySlam,
	RecoilQuarter,
	Rampage,
	RecoilThird,
	DefenseDown,
	PoisonHit,
	Twineedle,
	AttackDown,
	SleepMove,
	Confuse,
	SonicBoom,
	Disable,
	SpecialDefenseDownHit,
	Mist,
	ConfuseHit,
	SpeedDownHit,
	AttackDownHit,
	RechargeAttack,
	LowKick,
	Counter,
	SeismicToss,
	Leech,
	LeechSeed,
	Growth,
	SolarBeam,
	PoisonPowder,
	StunSpore,
	SleepPowder,
	SpeedDown2,
	DragonRage,
	Paralyze,
	Earthquake,
	Dig,
	Toxic,
	AttackUp,
	SpeedUp2,
	Rage,
	Teleport,
	NightShade,
	Mimic,
	DefenseDown2,
	EvasionUp,
	HealHalfHP,
	DefenseUp,
	Minimize,
	DefenseUp2,
	LightScreen,
	Haze,
	Reflect,
	FocusEnergy,
	Bide,
	Metronome,
	MirrorMove,
	Explosion,
	AlwaysHit,
	SkullBash,
	SpecialDefenseUp2,
	DreamEater,
	PoisonGas,
	SkyAttack,
	Transform,
	Psywave,
	Splash,
	Rest,
	Conversion,
	TriAttack,
	SuperFang,
	Substitute,
	Struggle
> ;

void Execute(MoveEffect, MoveRoutineDeps&);

class MoveRoutineFactory
{
public:
	static MoveRoutineTypes Make(MoveEffect ID);
};