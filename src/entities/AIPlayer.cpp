#include "../battle/BattleContext.h"
#include "AIPlayer.h"
#include "ai strategies/IAIStrategy.h"

AIPlayer::AIPlayer(std::string_view name, IAIStrategy& strat) :
	Player(name), m_aiStrategy(strat) {}

bool AIPlayer::IsHuman() const
{
	return false;
}

void AIPlayer::ChooseAction(BattlePokemon* selfMon, BattleContext& context)
{
    if (m_aiStrategy.WantsToSwitch(*this, *selfMon))
    {
        m_aiStrategy.ChooseSwitch(*this, *selfMon);
    }
    else
    {
        context.selectedMove = m_aiStrategy.ChooseMove(*this, *selfMon);
    }
}

BattlePokemon::pokemonMove* AIPlayer::ChooseMove(BattlePokemon& selfMon)
{
    return m_aiStrategy.ChooseMove(*this, selfMon);
}

bool AIPlayer::WantsToSwitch(BattlePokemon& selfMon)
{
    return m_aiStrategy.WantsToSwitch(*this, selfMon);
}

void AIPlayer::ChooseSwitch(BattlePokemon& selfMon)
{
    m_aiStrategy.ChooseSwitch(*this, selfMon);
}