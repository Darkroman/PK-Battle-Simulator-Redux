#pragma once

#include "interfaces/IMoveResultsUI.h"

struct BattleContext;

class MoveResultsText : public IMoveResultsUI
{
public:
	MoveResultsText(BattleContext&);

	void UsedTextDialog(Player*, BattlePokemon::pokemonMove*, BattlePokemon*) override;

	void DisplayCritTextDialog() override;

	void DisplayEffectivenessTextDialog(Player*, BattlePokemon*) override;

	void DisplayAttackMissedTextDialog(Player*, BattlePokemon*) override;

	void DisplayAttackAvoidedTextDialog(Player*, BattlePokemon*) override;

	void DisplayFailedTextDialog() override;

	void DisplaySubstituteDamageTextDialog(Player*, BattlePokemon*) override;

	void BoundMoveText(Player*, Player*, BattlePokemon*, BattlePokemon*, BattlePokemon::pokemonMove*) override;

	void DisplayNoopMsg() override;

	void DisplayDirectDamageInflictedMsg(double) override;

	void DisplayMultiAttackMsg(int) override;

	void DisplayRazorWindChargeMsg() override;

	void DisplayEnemySwitchMsg(BattlePokemon&) override;

	void DisplayFlyChargeMsg() override;

	void DisplayJumpKickCrashMsg() override;

	void DisplayRecoilMsg() override;

	void DisplayRechargeMsg() override;

	void DisplayEnergyDrainedMsg() override;

	void DisplaySolarBeamChargeMsg() override;

	void DisplayDigChargeMsg() override;

	void DisplayRecoveredHPRestoredMsg(double) override;

	void DisplayHPFullMsg() override;

	void DisplaySkullBashChargeMsg() override;

	void DisplaySkyAttackChargeMsg() override;

	void DisplaySplashMsg() override;

	void DisplayRestMsg() override;

private:
	BattleContext& m_context;
};