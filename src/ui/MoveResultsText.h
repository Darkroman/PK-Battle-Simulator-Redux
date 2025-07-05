#pragma once

#include "interfaces/IMoveResultsUI.h"

struct MoveResultsText : public IMoveResultsUI
{
	MoveResultsText(BattleContext&);

	void UsedTextDialog(Player*, BattlePokemon::pokemonMove*, BattlePokemon*);

	void DisplayCritTextDialog();

	void DisplayEffectivenessTextDialog(Player*, BattlePokemon*);

	void DisplayAccuracyLoweredTextDialog(Player*, BattlePokemon*);

	void DisplayAttackMissedTextDialog(Player*, BattlePokemon*);

	void DisplayAttackAvoidedTextDialog(Player*, BattlePokemon*);

	void DisplayFailedTextDialog();

	void DisplaySubstituteDamageTextDialog(Player*, BattlePokemon*);

	void BoundMoveText(Player*, Player*, BattlePokemon*, BattlePokemon*, BattlePokemon::pokemonMove*);

	BattleContext& context;
};