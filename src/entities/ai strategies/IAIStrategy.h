#pragma once

#include "../BattlePokemon.h"

class Move;
class Player;
class AIPlayer;


class IAIStrategy 
{

public:
	virtual ~IAIStrategy() = default;

	virtual BattlePokemon::pokemonMove& ChooseMove(Player&, BattlePokemon&) = 0;
	virtual bool WantsToSwitch(Player&, BattlePokemon&) = 0;
	virtual BattlePokemon* ChooseSwitch(Player&, BattlePokemon&) = 0;
};