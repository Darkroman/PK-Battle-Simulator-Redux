#pragma once

#include "../../battle/BattleCalculations.h"
#include "../../battle/BattleContext.h"

struct IMoveResultsUI
{
	virtual void UsedTextDialog(Player*, BattlePokemon::pokemonMove*, BattlePokemon*) = 0;

	virtual void DisplayCritTextDialog() = 0;

	virtual void DisplayEffectivenessTextDialog(Player*, BattlePokemon*) = 0;

	virtual void DisplayAccuracyLoweredTextDialog(Player*, BattlePokemon*) = 0;

	virtual void DisplayAttackMissedTextDialog(Player*, BattlePokemon*) = 0;

	virtual void DisplayAttackAvoidedTextDialog(Player*, BattlePokemon*) = 0;

	virtual void DisplayFailedTextDialog() = 0;

	virtual void DisplaySubstituteDamageTextDialog(Player*, BattlePokemon*) = 0;

	virtual void BoundMoveText(Player*, Player*, BattlePokemon*, BattlePokemon*, BattlePokemon::pokemonMove*) = 0;
};