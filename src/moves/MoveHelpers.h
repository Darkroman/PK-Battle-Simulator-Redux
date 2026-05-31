#pragma once

#include <string_view>

#include "../ui/EffectivenessText.h"
#include "../entities/BattlePokemon.h"
#include "../battle/BattleContext.h"

class IMoveResultsUI;
struct MoveRoutineDeps;

EffectivenessText ToEffectivenessText(BattleStateFlags::Effectiveness);

void InflictNVStatus(Status, int effectChance, MoveRoutineDeps&);

void DamageRoutine(MoveRoutineDeps&);

void MultiStrikeRoutine(MoveRoutineDeps&, int turnCount);

void FixedDamageRoutine(MoveRoutineDeps&, unsigned int baseDamage);

void FlinchRoutine(MoveRoutineDeps&);

void RecoilRoutine(MoveRoutineDeps& deps, unsigned int recoilDivisor, unsigned int targetHPBegin, unsigned int targetHPEnd);

using GetStageFn = size_t (*)(BattlePokemon&);
using SetStageFn = void (*)(BattlePokemon&, size_t);

void StageUpRoutine(MoveRoutineDeps& deps, size_t amount, std::string_view stageName, GetStageFn getStage, SetStageFn setStage);
void StageDownRoutine(MoveRoutineDeps& deps, size_t amount, std::string_view stageName, GetStageFn getStage, SetStageFn setStage);
void StageDownDamageRoutine(MoveRoutineDeps& deps, size_t amount, std::string_view stageName, GetStageFn getStage, SetStageFn setStage);
void DisplayStatChange(IMoveResultsUI& ui, size_t amount, bool isUp, std::string_view stageName, std::string_view playerName, std::string_view pokemonName);

using ChargeMsgMemFn = void (IMoveResultsUI::*)(std::string_view, std::string_view) const;
using ChargeHookFn = void (*)(MoveRoutineDeps& deps);
using ChargeStageUpFn = void (*)(MoveRoutineDeps& deps, size_t amount, std::string_view stageName, GetStageFn getStage, SetStageFn setStage);

struct ChargingMoveHooks
{
    ChargeHookFn preCharge{ nullptr };
    ChargeHookFn postCharge{ nullptr };
    ChargeStageUpFn stageUp{ nullptr };
    GetStageFn getStage{ nullptr };
    SetStageFn setStage{ nullptr };
    size_t stageIncreaseAmount{};
    std::string_view stageName{};
};

bool HandleCharging(MoveRoutineDeps&, ChargeMsgMemFn, const ChargingMoveHooks& hooks = {});