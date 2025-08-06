#pragma once

#include "Player.h"

class HumanPlayer : public Player
{
public:
	explicit HumanPlayer(std::string_view);

	bool IsHuman() const override;

};