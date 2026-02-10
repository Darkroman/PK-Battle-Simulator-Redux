#pragma once

#include <string_view>

class IStatusEffectUI
{
public:
	virtual ~IStatusEffectUI() = default;

	// Status changes
	virtual void DisplayFellAsleepMsg(std::string_view, std::string_view) const = 0;
	virtual void DisplayWokenUpMsg(std::string_view) const = 0;
	virtual void DisplayIsAsleepMsg(std::string_view) const = 0;
	virtual void DisplayFrozenSolidMsg(std::string_view) const = 0;
	virtual void DisplayThawedMsg(std::string_view) const = 0;
	virtual void DisplayNoLongerConfusedMsg(std::string_view, std::string_view) const = 0;
	virtual void DisplayIsConfusedMsg(std::string_view, std::string_view) const = 0;
	virtual void DisplayHurtItselfConfuseMsg() const = 0;
	virtual void DisplayCantMoveParalysisMsg(std::string_view) const = 0;
	virtual void DisplayFlinchMsg(std::string_view, std::string_view) const = 0;

	// Barriers / field effects
	virtual void DisplayNoLongerProtectedMist(std::string_view) const = 0;
	virtual void DisplayFieldEffectFadedMsg(std::string_view, std::string_view) const = 0;

	// Post-turn effects
	virtual void DisplayLeechSeedSappedMsg(std::string_view, std::string_view) const = 0;
	virtual void DisplayDamagedByStatusPostTurn(std::string_view, std::string_view, std::string_view) const = 0;
	virtual void DisplayHurtByBoundMsg(std::string_view, std::string_view, std::string_view) const = 0;
	virtual void DisplayFreedFromBoundMsg(std::string_view, std::string_view, std::string_view) const = 0;

	// Thrash
	virtual void DisplayThrashConfusionMsg(std::string_view, std::string_view) const = 0;

	// Rage
	virtual void DisplayRageStartedMsg(std::string_view, std::string_view) const = 0;
	virtual void DisplayStatRaised1Msg(std::string_view, std::string_view, std::string_view) const = 0;
	virtual void DisplayStatRaiseFailMsg(std::string_view, std::string_view, std::string_view) const = 0;

	// Disable
	virtual void DisplayMoveIsDisabledMsg(std::string_view, std::string_view, std::string_view) const = 0;
	virtual void DisplayMoveNoLongerDisabledMsg(std::string_view, std::string_view, std::string_view) const = 0;

	// Faint
	virtual void DisplayFaintedMsg(std::string_view, std::string_view) const = 0;
};
