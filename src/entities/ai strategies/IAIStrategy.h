#pragma once

class Move;
class BattlePokemon;
struct BattlePokemon::pokemonMove;
class Player;

class IAIStrategy 
{

public:
	virtual ~IAIStrategy() = default;

	virtual BattlePokemon::pokemonMove* ChooseMove(Player&, BattlePokemon&) = 0;
	virtual bool WantsToSwitch(Player&, BattlePokemon&) = 0;
	virtual void ChooseSwitch(Player&, BattlePokemon&) = 0;
};