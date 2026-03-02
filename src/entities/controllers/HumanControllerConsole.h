#pragma once

#include "IPlayerController.h"

struct pokemonMove;

class HumanControllerConsole : public IPlayerController
{
public:

	PlayerDecisionOutcome ChooseAction(Player&, Player&, BattlePokemon&, BattlePokemon&, RandomEngine&) override;
	BattlePokemon* PromptForSwitch(Player&, Player&, BattlePokemon&, BattlePokemon&) override;

private:
	pokemonMove* FightAction(Player&, BattlePokemon&);
	BattlePokemon* SwitchAction(Player&, BattlePokemon&);
	BattleAction ForfeitAction(Player&);
};