#include "HumanPlayer.h"

HumanPlayer::HumanPlayer(std::string_view name) :
    Player(name) {}

const bool HumanPlayer::IsHuman() const
{
	return true;
}