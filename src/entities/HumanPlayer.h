#pragma once

#include "Player.h"

class HumanPlayer : public Player
{
public:
	explicit HumanPlayer(std::string_view);

	const bool IsHuman() const;

};