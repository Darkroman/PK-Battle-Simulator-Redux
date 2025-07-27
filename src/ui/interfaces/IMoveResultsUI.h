#pragma once

#include "../../battle/BattleCalculations.h"

class IMoveResultsUI
{
public:
	virtual ~IMoveResultsUI() = default;

	virtual void UsedTextDialog(Player*, BattlePokemon::pokemonMove*, BattlePokemon*) = 0;

	virtual void DisplayCritTextDialog() = 0;

	virtual void DisplayEffectivenessTextDialog(Player*, BattlePokemon*) = 0;

	virtual void DisplayAttackMissedTextDialog(Player*, BattlePokemon*) = 0;

	virtual void DisplayAttackAvoidedTextDialog(Player*, BattlePokemon*) = 0;

	virtual void DisplayFailedTextDialog() = 0;

	virtual void DisplaySubstituteDamageTextDialog(Player*, BattlePokemon*) = 0;

	virtual void BoundMoveText(Player*, Player*, BattlePokemon*, BattlePokemon*, BattlePokemon::pokemonMove*) = 0;

	virtual void DisplayNoopMsg() = 0;

	virtual void DisplayMultiAttackMsg(int) = 0;

	virtual void DisplayRazorWindChargeMsg() = 0;

	virtual void DisplayEnemySwitchMsg(BattlePokemon&) = 0;

	virtual void DisplayFlyChargeMsg() = 0;

	virtual void DisplayJumpKickCrashMsg() = 0;

	virtual void DisplayRecoilMsg() = 0;

	virtual void DisplayDirectDamageInflictedMsg(double) = 0;

	virtual void DisplayRechargeMsg() = 0;

	virtual void DisplayEnergyDrainedMsg() = 0;

	virtual void DisplaySolarBeamChargeMsg() = 0;

	virtual void DisplayDigChargeMsg() = 0;

	virtual void DisplayRecoveredHPRestoredMsg(double) = 0;

	virtual void DisplayHPFullMsg() = 0;

	virtual void DisplaySkullBashChargeMsg() = 0;

	virtual void DisplaySkyAttackChargeMsg() = 0;

	virtual void DisplaySplashMsg() = 0;

	virtual void DisplayRestMsg() = 0;
};