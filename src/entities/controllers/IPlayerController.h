#pragma once

#include "../../battle/BattleAction.h"
#include "../BattlePokemon.h"
#include "../PlayerDecisionOutcome.h"

class Player;
class BattlePokemon;
struct BattleContext;

class IPlayerController
{
public:

	virtual ~IPlayerController() = default;
	virtual PlayerDecisionOutcome ChooseAction(Player&, BattlePokemon&) = 0;
	virtual BattlePokemon* PromptForSwitch(Player&, BattlePokemon&) = 0;
};