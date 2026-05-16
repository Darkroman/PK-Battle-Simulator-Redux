#pragma once

#include <memory>

#include "../PlayerDecisionOutcome.h"
#include "../../entities/pokemonMove.h"

struct BattleContext;
class Player;
class BattlePokemon;
class BattleCalculations;
class RandomEngine;

class IPlayerController
{
public:
	virtual std::unique_ptr<IPlayerController> clone() const = 0;

	virtual ~IPlayerController() = default;
	virtual PlayerDecisionOutcome ChooseAction(Player&, const Player&, BattlePokemon&, const BattlePokemon&, RandomEngine&) = 0;
	virtual BattlePokemon* PromptForSwitch(Player&, const Player&, const BattlePokemon&, const BattlePokemon&) = 0;

protected:
	static pokemonMove& GetStruggle()
	{
		static pokemonMove struggleInstance{ 164 };
		return struggleInstance;
	}
};