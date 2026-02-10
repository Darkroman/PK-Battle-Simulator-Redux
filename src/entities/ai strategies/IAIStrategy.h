#pragma once

class Move;
class Player;
class AIPlayer;
class IAIStrategy 
{

public:
	virtual ~IAIStrategy() = default;

	virtual BattlePokemon::pokemonMove* ChooseMove(AIPlayer&, BattlePokemon&) = 0;
	virtual bool WantsToSwitch(Player&, BattlePokemon&) = 0;
	virtual void ChooseSwitch(Player&, BattlePokemon&) = 0;
};