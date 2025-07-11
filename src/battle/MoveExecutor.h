#pragma once

#include "../moves/MoveEffects.h"

class MoveExecutor
{
public:
    MoveExecutor(
        BattleContext& context,
        BattleCalculations& calculations,
        BattleStatusManager& statusManager,
        IMoveResultsUI& resultsUI,
        IBattleMenuUI& battleMenuUI,
        IStatusEffectUI& statusEffectUI,
        RandomEngine& rng,
        TurnUtils& turnUtils
    );

    void ExecuteMove();

private:
    MoveEffectsDependencies m_deps;
};