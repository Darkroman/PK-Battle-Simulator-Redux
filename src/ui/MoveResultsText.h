#pragma once

#include "interfaces/IMoveResultsUI.h"

class ITextSink;

class MoveResultsText : public IMoveResultsUI
{
public:
	MoveResultsText(ITextSink& sink)
		: m_sink(sink) {}

	void UsedTextDialog(std::string_view, std::string_view, std::string_view) const override;

	void DisplayCritTextDialog(bool) const override;

	void DisplayEffectivenessTextDialog(std::string_view, std::string_view, EffectivenessText) const override;

	void DisplayAttackMissedTextDialog(std::string_view, std::string_view) const override;

	void DisplayAttackAvoidedTextDialog(std::string_view, std::string_view) const override;

	void DisplayFailedTextDialog() const override;

	void DisplaySubstituteDamageTextDialog(std::string_view, std::string_view, int, bool, bool) const override;

	void BoundMoveText(std::string_view, std::string_view, std::string_view, std::string_view, size_t) const override;

	void DisplayNoopMsg() const override;

	void DisplayDirectDamageInflictedMsg(int) const override;

	void DisplayMultiAttackMsg(std::string_view, int) const override;

	void DisplayRazorWindChargeMsg(std::string_view, std::string_view) const override;

	void DisplayEnemySwitchMsg(std::string_view) const override;

	void DisplayFlyChargeMsg(std::string_view) const override;

	void DisplayJumpKickCrashMsg(std::string_view, std::string_view) const override;

	void DisplayRecoilMsg(std::string_view, std::string_view) const override;

	void DisplayRechargeMsg(std::string_view, std::string_view) const override;

	void DisplayEnergyDrainedMsg(std::string_view, std::string_view) const override;

	void DisplaySolarBeamChargeMsg(std::string_view, std::string_view) const override;

	void DisplayDigChargeMsg(std::string_view) const override;

	void DisplayRecoveredHPRestoredMsg(std::string_view, std::string_view) const override;

	void DisplayHPFullMsg(std::string_view, std::string_view) const override;

	void DisplaySkullBashChargeMsg(std::string_view, std::string_view) const override;

	void DisplaySkyAttackChargeMsg(std::string_view, std::string_view) const override;

	void DisplaySplashMsg() const override;

	void DisplayRestMsg(std::string_view, std::string_view) const override;

	void DisplayNVStatusMsg(std::string_view) const override;

	void DisplayBecameConfuseMsg(std::string_view, std::string_view) const override;

	void DisplayAlreadyPoisonedMsg(std::string_view, std::string_view) const override;

	void DisplayAlreadyParalyzedMsg(std::string_view, std::string_view) const override;

	void DisplayAlreadyAsleepMsg(std::string_view, std::string_view) const override;

	void DisplayDoesntAffectMsg(std::string_view, std::string_view) const override;

	void DisplayStatRaised2Msg(std::string_view, std::string_view, std::string_view) const override;

	void DisplayStatRaised1Msg(std::string_view, std::string_view, std::string_view) const override;

	void DisplayStatRaiseFailMsg(std::string_view, std::string_view, std::string_view) const override;

	void DisplayStatLowered2Msg(std::string_view, std::string_view, std::string_view) const override;

	void DisplayStatLowered1Msg(std::string_view, std::string_view, std::string_view) const override;

	void DisplayStatLoweredFailMsg(std::string_view, std::string_view, std::string_view) const override;

	void DisplayFocusEnergyMsg(std::string_view, std::string_view) const override;

	void DisplayProtectedByMistMsg(std::string_view, std::string_view) const override;

	void DisplayMistMsg(std::string_view) const override;

	void DisplayLightScreenMsg(std::string_view) const override;

	void DisplayHazeMsg() const override;

	void DisplayReflectMsg(std::string_view) const override;

	void DisplayThrashDisabledMsg(std::string_view, std::string_view) const override;

	void DisplayMoveDisabledMsg(std::string_view, std::string_view, std::string_view) const override;

	void DisplayAlreadyHasSubstituteMsg(std::string_view, std::string_view) const override;

	void DisplayNotEnoughHPSubstituteMsg() const override;

	void DisplayPutInSubstituteMsg(std::string_view, std::string_view) const override;

	void DisplaySeededMsg(std::string_view, std::string_view) const override;

	void DisplayLearnedMimicMoveMsg(std::string_view, std::string_view, std::string_view) const override;

	void DisplayTransformMsg(std::string_view, std::string_view, std::string_view) const override;

	void DisplayMetronomeMsg(std::string_view, std::string_view, std::string_view) const override;

	void DisplayConversionMsg(std::string_view, std::string_view, std::string_view) const override;

	void DisplayBideUnleashedMsg(std::string_view, std::string_view) const override;

	void DisplayBideStoringEnergyMsg(std::string_view, std::string_view) const override;

	void DisplayBideDisabledMsg(std::string_view, std::string_view) const override;

	void DisplayNoMovesLeftStruggleMsg(std::string_view, std::string_view) const override;

private:
	ITextSink& m_sink;

};