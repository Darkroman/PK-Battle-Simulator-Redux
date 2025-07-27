#pragma once

#include "../moves/MoveRoutines.h"

class MoveExecutor
{
public:
    MoveExecutor(
        BattleContext& context,
        BattleCalculations& calculations,
        StatusEffectProcessor& statusProcessor,
        IMoveResultsUI& resultsUI,
        IBattleMenuUI& battleMenuUI,
        IStatusEffectUI& statusEffectUI,
        RandomEngine& rng,
        SwitchExecutor& switchExecutor
    );

    void ExecuteMove();

private:
    MoveRoutineDeps m_deps;
};