#pragma once

#include "Player.h"

struct BattleContext;
class IAIStrategy;

class AIPlayer : public Player
{
public:
	explicit AIPlayer(std::string_view, IAIStrategy&);
	void ChooseAction(BattlePokemon*, BattleContext&);

	bool IsHuman() const override;
	BattlePokemon::pokemonMove* ChooseMove(BattlePokemon&);
	bool WantsToSwitch(BattlePokemon&);
	void ChooseSwitch(BattlePokemon&);

private:
	IAIStrategy& m_aiStrategy;
		
};