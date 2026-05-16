#pragma once

#include "IPlayerController.h"

struct pokemonMove;

class HumanControllerConsole : public IPlayerController
{
public:
	HumanControllerConsole() = default;
	HumanControllerConsole(const HumanControllerConsole& other) = default;

	std::unique_ptr<IPlayerController> clone() const override;

	PlayerDecisionOutcome ChooseAction(Player&, const Player&, BattlePokemon&, const BattlePokemon&, RandomEngine&) override;
	BattlePokemon* PromptForSwitch(Player&, const Player&, const BattlePokemon&, const BattlePokemon&) override;

private:
	pokemonMove* FightAction(const Player&, BattlePokemon&, const BattlePokemon&);
	BattlePokemon* SwitchAction(Player&, const BattlePokemon&);
	BattleAction ForfeitAction(const Player&);
};