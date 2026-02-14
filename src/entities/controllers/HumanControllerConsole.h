#pragma once

#include "IPlayerController.h"

class HumanControllerConsole : public IPlayerController
{
public:

	PlayerDecisionOutcome ChooseAction(Player&, BattlePokemon&) override;
	BattlePokemon* PromptForSwitch(Player&, BattlePokemon&) override;

private:
	BattlePokemon::pokemonMove* FightAction(Player&, BattlePokemon&);
	BattlePokemon* SwitchAction(Player&, BattlePokemon&);
	BattleAction ForfeitAction(Player&);
};