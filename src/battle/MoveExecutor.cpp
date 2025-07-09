#include "MoveExecutor.h"

#include "BattleContext.h"

MoveExecutor::MoveExecutor(
    BattleContext& context,
    BattleCalculations& calculations,
    BattleStatusManager& statusManager,
    IMoveResultsUI& resultsUI,
    IBattleMenuUI& battleMenuUI,
    IStatusEffectUI& statusEffectUI,
    RandomEngine& rng,
    TurnUtils& turnUtils
)
    : m_deps{
        context,
        calculations,
        statusManager,
        resultsUI,
        battleMenuUI,
        statusEffectUI,
        rng,
        turnUtils
    }
{}

void MoveExecutor::ExecuteMove()
{
    auto effectEnum = m_deps.context.currentMove->mp_move->GetMoveEffectEnum();
    auto moveEffect = MoveEffectsFactory::Call(effectEnum);
    moveEffect->DoMove(m_deps);
}
