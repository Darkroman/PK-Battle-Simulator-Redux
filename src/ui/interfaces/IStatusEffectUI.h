#pragma once

#include <string_view>

class Player;
class BattlePokemon;
class Move;

class IStatusEffectUI
{
public:
	virtual ~IStatusEffectUI() = default;

	// Status changes
	virtual void DisplayNVStatusMsg(std::string_view status) = 0;
	virtual void DisplayFellAsleepMsg() = 0;
	virtual void DisplayWokenUpMsg() = 0;
	virtual void DisplayIsAsleepMsg() = 0;
	virtual void DisplayFrozenSolidMsg() = 0;
	virtual void DisplayThawedMsg() = 0;
	virtual void DisplayBecameConfuseMsg() = 0;
	virtual void DisplayNoLongerConfusedMsg() = 0;
	virtual void DisplayIsConfusedMsg() = 0;
	virtual void DisplayHurtItselfConfuseMsg() = 0;
	virtual void DisplayCantMoveParalysisMsg() = 0;
	virtual void DisplayFlinchMsg() = 0;
	virtual void DisplayAlreadyPoisonedMsg() = 0;
	virtual void DisplayAlreadyParalyzedMsg() = 0;
	virtual void DisplayAlreadyAsleepMsg() = 0;
	virtual void DisplayDoesntAffectMsg() = 0;

	// Stat changes
	virtual void DisplayStatRaised2Msg(std::string_view statName) = 0;
	virtual void DisplayStatRaised1Msg(std::string_view statName) = 0;
	virtual void DisplayStatRaiseFailMsg(std::string_view statName) = 0;
	virtual void DisplayStatLowered2Msg(std::string_view statName) = 0;
	virtual void DisplayStatLowered1Msg(std::string_view statName) = 0;
	virtual void DisplayStatLoweredFailMsg(std::string_view statName) = 0;
	virtual void DisplayFocusEnergyMsg() = 0;

	// Barriers / field effects
	virtual void DisplayProtectedByMistMsg() = 0;
	virtual void DisplayMistMsg() = 0;
	virtual void DisplayNoLongerProtectedMist(Player*) = 0;
	virtual void DisplayLightScreenMsg() = 0;
	virtual void DisplayHazeMsg() = 0;
	virtual void DisplayReflectMsg() = 0;
	virtual void DisplayFieldEffectFadedMsg(Player* player, std::string_view effect) = 0;

	// Post-turn effects
	virtual void DisplayLeechSeedSappedMsg(Player* player, BattlePokemon* pokemon) = 0;
	virtual void DisplayDamagedByStatusPostTurn(std::string_view statusName, Player* player, BattlePokemon* pokemon) = 0;
	virtual void DisplayHurtByBoundMsg(Player* player, BattlePokemon* pokemon) = 0;
	virtual void DisplayFreedFromBoundMsg(Player* player, BattlePokemon* pokemon) = 0;

	// Confusion / rage / thrash
	virtual void DisplayThrashDisabledMsg() = 0;
	virtual void DisplayThrashConfusionMsg() = 0;
	virtual void DisplayRageStartedMsg() = 0;

	// Disable
	virtual void DisplayMoveDisabledMsg() = 0;
	virtual void DisplayMoveIsDisabledMsg() = 0;
	virtual void DisplayMoveNoLongerDisabledMsg(Player* player, BattlePokemon* pokemon) = 0;

	// Substitute
	virtual void DisplayAlreadyHasSubstituteMsg() = 0;
	virtual void DisplayNotEnoughHPSubstituteMsg() = 0;
	virtual void DisplayPutInSubstituteMsg() = 0;
	virtual void DisplaySubstituteFadedMsg() = 0;

	// Other effects
	virtual void DisplaySeededMsg() = 0;
	virtual void DisplayLearnedMimicMoveMsg() = 0;
	virtual void DisplayTransformMsg() = 0;
	virtual void DisplayMetronomeMsg(Move*) = 0;
	virtual void DisplayConversionMsg() = 0;

	// Bide
	virtual void DisplayBideUnleashedMsg() = 0;
	virtual void DisplayBideStoringEnergyMsg() = 0;
	virtual void DisplayBideDisabledMsg() = 0;

	// Struggle
	virtual void DisplayNoMovesLeftStruggleMsg() = 0;

	// Faint
	virtual void DisplayFaintedMsg(Player& player, BattlePokemon& pokemon) = 0;
};
