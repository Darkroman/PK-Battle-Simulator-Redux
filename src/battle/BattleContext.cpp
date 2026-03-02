#include "BattleContext.h"

BattleContext::BattleContext(std::vector<std::unique_ptr<Player>>& vec_players)
	: playerOne(vec_players[0].get()),
	playerTwo(vec_players[1].get())
{

}