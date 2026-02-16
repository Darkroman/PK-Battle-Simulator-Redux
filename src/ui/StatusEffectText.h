#pragma once

#include "../ui/interfaces/IStatusEffectUI.h"

class ITextSink;

class StatusEffectText : public IStatusEffectUI
{
public:
	StatusEffectText(ITextSink& sink)
		: m_sink(sink) {}

	// Status changes
	void DisplayFellAsleepMsg(std::string_view, std::string_view) const override;
	void DisplayWokenUpMsg(std::string_view) const override;
	void DisplayIsAsleepMsg(std::string_view) const override;
	void DisplayFrozenSolidMsg(std::string_view) const override;
	void DisplayThawedMsg(std::string_view) const override;
	void DisplayNoLongerConfusedMsg(std::string_view, std::string_view) const override;
	void DisplayIsConfusedMsg(std::string_view, std::string_view) const override;
	void DisplayHurtItselfConfuseMsg() const override;
	void DisplayCantMoveParalysisMsg(std::string_view) const override;
	void DisplayFlinchMsg(std::string_view, std::string_view) const override;

	// Barriers / field effects
	void DisplayNoLongerProtectedMist(std::string_view) const override;
	void DisplayFieldEffectFadedMsg(std::string_view, std::string_view) const override;

	// Post-turn effects
	void DisplayLeechSeedSappedMsg(std::string_view, std::string_view) const override;
	void DisplayDamagedByStatusPostTurn(std::string_view, std::string_view, std::string_view) const override;
	void DisplayHurtByBoundMsg(std::string_view, std::string_view, std::string_view) const override;
	void DisplayFreedFromBoundMsg(std::string_view, std::string_view, std::string_view) const override;

	// Thrash
	void DisplayThrashConfusionMsg(std::string_view, std::string_view) const override;

	// Rage
	void DisplayRageStartedMsg(std::string_view, std::string_view) const override;
	void DisplayStatRaised1Msg(std::string_view, std::string_view, std::string_view) const override;
	void DisplayStatRaiseFailMsg(std::string_view, std::string_view, std::string_view) const override;

	// Disable
	void DisplayMoveIsDisabledMsg(std::string_view, std::string_view, std::string_view) const override;
	void DisplayMoveNoLongerDisabledMsg(std::string_view, std::string_view, std::string_view) const override;

	// Faint
	void DisplayFaintedMsg(std::string_view, std::string_view) const override;

	void NewLine() const override;

private:
	ITextSink& m_sink;
};
