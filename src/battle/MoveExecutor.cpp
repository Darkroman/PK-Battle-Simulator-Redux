#include "MoveExecutor.h"

#include "BattleContext.h"

MoveExecutor::MoveExecutor(
    BattleContext& context,
    BattleCalculations& calculations,
    StatusEffectProcessor& statusProcessor,
    IMoveResultsUI& resultsUI,
    IBattleMenuUI& battleMenuUI,
    IStatusEffectUI& statusEffectUI,
    RandomEngine& rng,
    SwitchExecutor& switchExecutor
)
    : m_deps{
        context,
        calculations,
        statusProcessor,
        resultsUI,
        battleMenuUI,
        statusEffectUI,
        rng,
        switchExecutor
    }
{}

void MoveExecutor::ExecuteMove()
{
    ExecuteMoveRoutine::Execute(m_deps.context.currentMove->GetMoveEffectEnum(), m_deps);
}