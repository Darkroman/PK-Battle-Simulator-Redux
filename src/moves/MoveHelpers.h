#pragma once

#include <string_view>

#include "../entities/BattlePokemon.h"
#include "../battle/BattleContext.h"

class IMoveResultsUI;
struct MoveRoutineDeps;

EffectivenessText ToEffectivenessText(BattleStateFlags::Effectiveness);

void InflictNVStatus(Status, int, MoveRoutineDeps&);

void DamageRoutine(MoveRoutineDeps&);

void MultiStrikeRoutine(MoveRoutineDeps&, int);

void FixedDamageRoutine(MoveRoutineDeps&, int);

void FlinchRoutine(MoveRoutineDeps&);

using GetStageFn = int (*)(BattlePokemon&);
using SetStageFn = void (*)(BattlePokemon&, int);

void StageUpRoutine(MoveRoutineDeps& deps, int amount, std::string_view stageName, GetStageFn getStage, SetStageFn setStage);
void StageDownRoutine(MoveRoutineDeps& deps, int amount, std::string_view stageName, GetStageFn getStage, SetStageFn setStage);
void StageDownDamageRoutine(MoveRoutineDeps& deps, int amount, std::string_view stageName, GetStageFn getStage, SetStageFn setStage);
void DisplayStatChange(IMoveResultsUI& ui, int amount, bool isUp, std::string_view stageName, std::string_view playerName, std::string_view pokemonName);

using ChargeMsgMemFn = void (IMoveResultsUI::*)(std::string_view, std::string_view) const;
using ChargeHookFn = void (*)(MoveRoutineDeps& deps);
using ChargeStageUpFn = void (*)(MoveRoutineDeps& deps, int amount, std::string_view stageName, GetStageFn getStage, SetStageFn setStage);

struct ChargingMoveHooks
{
    ChargeHookFn preCharge{ nullptr };
    ChargeHookFn postCharge{ nullptr };
    ChargeStageUpFn stageUp{ nullptr };
    GetStageFn getStage{ nullptr };
    SetStageFn setStage{ nullptr };
    int stageIncreaseAmount{};
    std::string_view stageName{};
};

bool HandleCharging(MoveRoutineDeps&, ChargeMsgMemFn, const ChargingMoveHooks& hooks = {});