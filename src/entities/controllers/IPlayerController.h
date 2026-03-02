#pragma once

#include "../../battle/BattleAction.h"
#include "../PlayerDecisionOutcome.h"

struct BattleContext;
class Player;
class BattlePokemon;
class BattleCalculations;
class RandomEngine;

class IPlayerController
{
public:

	virtual ~IPlayerController() = default;
	virtual PlayerDecisionOutcome ChooseAction(Player&, Player&, BattlePokemon&, BattlePokemon&, RandomEngine&) = 0;
	virtual BattlePokemon* PromptForSwitch(Player&, Player&, BattlePokemon&, BattlePokemon&) = 0;
};