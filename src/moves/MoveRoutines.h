// MoveRoutines.h
#pragma once
#include <array>
#include "MoveEffectEnums.h"

struct MoveRoutineDeps;

namespace MoveRoutines
{
    void Noop(MoveRoutineDeps& deps);
    void NormalHit(MoveRoutineDeps& deps);
    void IncreasedCritical(MoveRoutineDeps& deps);
    void MultiHit(MoveRoutineDeps& deps);
    void BurnHit(MoveRoutineDeps& deps);
    void FreezeHit(MoveRoutineDeps& deps);
    void ParalyzeHit(MoveRoutineDeps& deps);
    void OHKO(MoveRoutineDeps& deps);
    void RazorWind(MoveRoutineDeps& deps);
    void AttackUp2(MoveRoutineDeps& deps);
    void Gust(MoveRoutineDeps& deps);
    void ForceSwitch(MoveRoutineDeps& deps);
    void Fly(MoveRoutineDeps& deps);
    void PartialTrap(MoveRoutineDeps& deps);
    void Stomp(MoveRoutineDeps& deps);
    void DoubleHit(MoveRoutineDeps& deps);
    void JumpKick(MoveRoutineDeps& deps);
    void FlinchHit(MoveRoutineDeps& deps);
    void AccuracyDown(MoveRoutineDeps& deps);
    void BodySlam(MoveRoutineDeps& deps);
    void RecoilQuarter(MoveRoutineDeps& deps);
    void Rampage(MoveRoutineDeps& deps);
    void RecoilThird(MoveRoutineDeps& deps);
    void DefenseDown(MoveRoutineDeps& deps);
    void PoisonHit(MoveRoutineDeps& deps);
    void Twineedle(MoveRoutineDeps& deps);
    void AttackDown(MoveRoutineDeps& deps);
    void SleepMove(MoveRoutineDeps& deps);
    void Confuse(MoveRoutineDeps& deps);
    void SonicBoom(MoveRoutineDeps& deps);
    void Disable(MoveRoutineDeps& deps);
    void SpecialDefenseDownHit(MoveRoutineDeps& deps);
    void Mist(MoveRoutineDeps& deps);
    void ConfuseHit(MoveRoutineDeps& deps);
    void SpeedDownHit(MoveRoutineDeps& deps);
    void AttackDownHit(MoveRoutineDeps& deps);
    void RechargeAttack(MoveRoutineDeps& deps);
    void LowKick(MoveRoutineDeps& deps);
    void Counter(MoveRoutineDeps& deps);
    void SeismicToss(MoveRoutineDeps& deps);
    void Leech(MoveRoutineDeps& deps);
    void LeechSeed(MoveRoutineDeps& deps);
    void Growth(MoveRoutineDeps& deps);
    void SolarBeam(MoveRoutineDeps& deps);
    void PoisonPowder(MoveRoutineDeps& deps);
    void StunSpore(MoveRoutineDeps& deps);
    void SleepPowder(MoveRoutineDeps& deps);
    void SpeedDown2(MoveRoutineDeps& deps);
    void DragonRage(MoveRoutineDeps& deps);
    void Paralyze(MoveRoutineDeps& deps);
    void Earthquake(MoveRoutineDeps& deps);
    void Dig(MoveRoutineDeps& deps);
    void Toxic(MoveRoutineDeps& deps);
    void AttackUp(MoveRoutineDeps& deps);
    void SpeedUp2(MoveRoutineDeps& deps);
    void Rage(MoveRoutineDeps& deps);
    void Teleport(MoveRoutineDeps& deps);
    void NightShade(MoveRoutineDeps& deps);
    void Mimic(MoveRoutineDeps& deps);
    void DefenseDown2(MoveRoutineDeps& deps);
    void EvasionUp(MoveRoutineDeps& deps);
    void HealHalfHP(MoveRoutineDeps& deps);
    void DefenseUp(MoveRoutineDeps& deps);
    void Minimize(MoveRoutineDeps& deps);
    void DefenseUp2(MoveRoutineDeps& deps);
    void LightScreen(MoveRoutineDeps& deps);
    void Haze(MoveRoutineDeps& deps);
    void Reflect(MoveRoutineDeps& deps);
    void FocusEnergy(MoveRoutineDeps& deps);
    void Bide(MoveRoutineDeps& deps);
    void Metronome(MoveRoutineDeps& deps);
    void MirrorMove(MoveRoutineDeps& deps);
    void Explosion(MoveRoutineDeps& deps);
    void AlwaysHit(MoveRoutineDeps& deps);
    void SkullBash(MoveRoutineDeps& deps);
    void SpecialDefenseUp2(MoveRoutineDeps& deps);
    void DreamEater(MoveRoutineDeps& deps);
    void PoisonGas(MoveRoutineDeps& deps);
    void SkyAttack(MoveRoutineDeps& deps);
    void Transform(MoveRoutineDeps& deps);
    void Psywave(MoveRoutineDeps& deps);
    void Splash(MoveRoutineDeps& deps);
    void Rest(MoveRoutineDeps& deps);
    void Conversion(MoveRoutineDeps& deps);
    void TriAttack(MoveRoutineDeps& deps);
    void SuperFang(MoveRoutineDeps& deps);
    void Substitute(MoveRoutineDeps& deps);
    void Struggle(MoveRoutineDeps& deps);

    using RoutineFn = void(*)(MoveRoutineDeps&);

    void Execute(MoveEffect ID, MoveRoutineDeps& deps);
}