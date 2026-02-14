#include "MoveExecutor.h"

#include "BattleContext.h"

MoveExecutor::MoveExecutor(
    BattleContext& context,
    BattleCalculations& calculations,
    StatusEffectProcessor& statusProcessor,
    IMoveResultsUI& resultsUI,
    RandomEngine& rng,
    SwitchExecutor& switchExecutor
)
    : m_deps{
        context,
        calculations,
        statusProcessor,
        resultsUI,
        rng,
        switchExecutor
    }
{}

void MoveExecutor::ExecuteMove()
{
    Execute(m_deps.context.currentMove->GetMoveEffectEnum(), m_deps);
}