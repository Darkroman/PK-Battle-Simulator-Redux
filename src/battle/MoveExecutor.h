#pragma once

#include "../moves/MoveRoutineDeps.h"

class MoveExecutor
{
public:
    MoveExecutor(
        BattleContext& context,
        BattleCalculations& calculations,
        StatusEffectProcessor& statusProcessor,
        IMoveResultsUI& resultsUI,
        RandomEngine& rng,
        SwitchExecutor& switchExecutor
    );

    void ExecuteMove();

private:
    MoveRoutineDeps m_deps;
};