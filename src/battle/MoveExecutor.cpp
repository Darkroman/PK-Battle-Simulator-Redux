#include "MoveExecutor.h"

#include "BattleContext.h"
#include "../moves/MoveEffectEnums.h"
#include "../data/Move.h"

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
    auto effectEnum = m_deps.context.currentMove->mp_move->GetMoveEffectEnum();
    auto moveRoutine = MoveRoutineFactory::Call(effectEnum);
    moveRoutine->DoMove(m_deps);
}