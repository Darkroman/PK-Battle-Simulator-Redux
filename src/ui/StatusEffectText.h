#pragma once

#include "../ui/interfaces/IStatusEffectUI.h"

struct BattleContext;

class StatusEffectText : public IStatusEffectUI
{
public:
	StatusEffectText(BattleContext&);

	// Status changes
	void DisplayNVStatusMsg(std::string_view) override;
	void DisplayFellAsleepMsg() override;
	void DisplayWokenUpMsg() override;
	void DisplayIsAsleepMsg() override;
	void DisplayFrozenSolidMsg() override;
	void DisplayThawedMsg() override;
	void DisplayBecameConfuseMsg() override;
	void DisplayNoLongerConfusedMsg() override;
	void DisplayIsConfusedMsg() override;
	void DisplayHurtItselfConfuseMsg() override;
	void DisplayCantMoveParalysisMsg() override;
	void DisplayFlinchMsg() override;
	void DisplayAlreadyPoisonedMsg() override;
	void DisplayAlreadyParalyzedMsg() override;
	void DisplayAlreadyAsleepMsg() override;
	void DisplayDoesntAffectMsg() override;

	// Stat changes
	void DisplayStatRaised2Msg(std::string_view) override;
	void DisplayStatRaised1Msg(std::string_view) override;
	void DisplayStatRaiseFailMsg(std::string_view) override;
	void DisplayStatLowered2Msg(std::string_view) override;
	void DisplayStatLowered1Msg(std::string_view) override;
	void DisplayStatLoweredFailMsg(std::string_view) override;
	void DisplayFocusEnergyMsg() override;

	// Barriers / field effects
	void DisplayProtectedByMistMsg() override;
	void DisplayMistMsg() override;
	void DisplayNoLongerProtectedMist(Player*) override;
	void DisplayLightScreenMsg() override;
	void DisplayHazeMsg() override;
	void DisplayReflectMsg() override;
	void DisplayFieldEffectFadedMsg(Player*, std::string_view) override;

	// Post-turn effects
	void DisplayLeechSeedSappedMsg(Player*, BattlePokemon*) override;
	void DisplayDamagedByStatusPostTurn(std::string_view, Player*, BattlePokemon*) override;
	void DisplayHurtByBoundMsg(Player*, BattlePokemon*) override;
	void DisplayFreedFromBoundMsg(Player*, BattlePokemon*) override;

	// Rage / thrash
	void DisplayThrashDisabledMsg() override;
	void DisplayThrashConfusionMsg() override;
	void DisplayRageStartedMsg() override;

	// Disable
	void DisplayMoveDisabledMsg() override;
	void DisplayMoveIsDisabledMsg() override;
	void DisplayMoveNoLongerDisabledMsg(Player*, BattlePokemon*) override;

	// Substitute
	void DisplayAlreadyHasSubstituteMsg() override;
	void DisplayNotEnoughHPSubstituteMsg() override;
	void DisplayPutInSubstituteMsg() override;
	void DisplaySubstituteFadedMsg() override;

	// Other effects
	void DisplaySeededMsg() override;
	void DisplayLearnedMimicMoveMsg() override;
	void DisplayTransformMsg() override;
	void DisplayMetronomeMsg(Move*) override;
	void DisplayConversionMsg() override;

	// Bide
	void DisplayBideUnleashedMsg() override;
	void DisplayBideStoringEnergyMsg() override;
	void DisplayBideDisabledMsg() override;

	// Struggle
	void DisplayNoMovesLeftStruggleMsg() override;

	// Faint
	void DisplayFaintedMsg(Player&, BattlePokemon&) override;

private:
	BattleContext& m_context;
};
