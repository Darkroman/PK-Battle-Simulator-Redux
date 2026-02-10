#include <format>

#include "MoveResultsText.h"
#include "interfaces/ITextSink.h"

void MoveResultsText::UsedTextDialog(std::string_view attackingPlayerName, std::string_view attackingPokemonName, std::string_view currentMoveName) const
{
	m_sink.Emit(std::format("{}'s {} used {}", attackingPlayerName, attackingPokemonName, currentMoveName));
}

void MoveResultsText::DisplayCritTextDialog(bool isCriticalHit) const
{
	if (isCriticalHit)
	{
		m_sink.Emit(std::format("Critical hit!"));
	}
}

void MoveResultsText::DisplayEffectivenessTextDialog(std::string_view defendingPlayerName, std::string_view defendingPokemonName, EffectivenessText e) const
{
	using E = EffectivenessText;

	switch (e)
	{
		case E::Less:
			m_sink.Emit(std::format("It was not very effective..."));
			break;

		case E::Super:
			m_sink.Emit(std::format("It was super effective!"));
			break;

		case E::No:
			m_sink.Emit(std::format("It doesn't affect {}'s {}...", defendingPlayerName, defendingPokemonName));
			break;

		case E::OHKO:
			m_sink.Emit(std::format("It was a one-hit KO!"));
			break;

		default:
			return;
	}
}

void MoveResultsText::DisplayAttackMissedTextDialog(std::string_view attackingPlayerName, std::string_view attackingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {}'s attack missed!", attackingPlayerName, attackingPokemonName));
}

void MoveResultsText::DisplayAttackAvoidedTextDialog(std::string_view defendingPlayerName, std::string_view defendingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {} avoided the attack!", defendingPlayerName, defendingPokemonName));
}

void MoveResultsText::DisplayFailedTextDialog() const
{
	m_sink.Emit(std::format("But it failed!"));
}

void MoveResultsText::DisplaySubstituteDamageTextDialog(std::string_view defendingPlayerName, std::string_view defendingPokemonName, int substituteHP, bool hasSubstitute, bool hitSubstitute) const
{
	if (hitSubstitute && hasSubstitute)
	{
		if (substituteHP > 0)
		{
			m_sink.Emit(std::format("The substitute took damage for {}'s {}!", defendingPlayerName, defendingPokemonName));
		}
		else
		{
			m_sink.Emit(std::format("{}'s {}'s substitute faded!", defendingPlayerName, defendingPokemonName));
		}
	}
}

void MoveResultsText::BoundMoveText(std::string_view attackingPlayerName, std::string_view defendingPlayerName, std::string_view attackingPokemonName, std::string_view defendingPokemonName, size_t moveIndex) const
{
	switch (moveIndex)
	{
		case 20:
			m_sink.Emit(std::format("{}'s {} was squeezed by {}'s {}!", defendingPlayerName, defendingPokemonName, attackingPlayerName, attackingPokemonName));
			break;

		case 35:
			m_sink.Emit(std::format("{}'s {} was wrapped by {}'s {}!", defendingPlayerName, defendingPokemonName, attackingPlayerName, attackingPokemonName));
			break;

		case 83:
			m_sink.Emit(std::format("{}'s {} became trapped in the fiery vortex!", defendingPlayerName, defendingPokemonName));
			break;

		case 128:
			m_sink.Emit(std::format("{}'s {} clamped down on {}'s {}!", attackingPlayerName, attackingPokemonName, defendingPlayerName, defendingPokemonName));
			break;

		default:
			m_sink.Emit(std::format("Text not found for move index number {}", moveIndex));
	}
}

void MoveResultsText::DisplayNoopMsg() const
{
	m_sink.Emit(std::format("This isn't implemented yet!"));
}

void MoveResultsText::DisplayDirectDamageInflictedMsg(int damage) const
{
	if (damage > 0)
	{
		m_sink.Emit(std::format("{} damage inflicted.", damage));
	}
}

void MoveResultsText::DisplayMultiAttackMsg(std::string_view defendingPokemonName, int timesHit) const
{
	m_sink.Emit(std::format("{} was hit {} times!", defendingPokemonName, timesHit));
}

void MoveResultsText::DisplayRazorWindChargeMsg(std::string_view attackingPlayerName, std::string_view attackingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {} made a whirlwind!", attackingPlayerName, attackingPokemonName));
}

void MoveResultsText::DisplayEnemySwitchMsg(std::string_view attackingPokemonName) const
{
	m_sink.Emit(std::format("{} was dragged out!", attackingPokemonName));
}

void MoveResultsText::DisplayFlyChargeMsg(std::string_view attackingPokemonName) const
{
	m_sink.Emit(std::format("{} flew up high in the sky!", attackingPokemonName));
}

void MoveResultsText::DisplayJumpKickCrashMsg(std::string_view attackingPlayerName, std::string_view attackingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {} kept going and crashed!", attackingPlayerName, attackingPokemonName));
}

void MoveResultsText::DisplayRecoilMsg(std::string_view attackingPlayerName, std::string_view attackingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {} was damaged by the recoil!", attackingPlayerName, attackingPokemonName));
}

void MoveResultsText::DisplayRechargeMsg(std::string_view attackingPlayerName, std::string_view attackingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {} must recharge!", attackingPlayerName, attackingPokemonName));
}

void MoveResultsText::DisplayEnergyDrainedMsg(std::string_view defendingPlayerName, std::string_view defendingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {} had its energy drained!", defendingPlayerName, defendingPokemonName));
}

void MoveResultsText::DisplaySolarBeamChargeMsg(std::string_view attackingPlayerName, std::string_view attackingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {} absorbed light!", attackingPlayerName, attackingPokemonName));
}

void MoveResultsText::DisplayDigChargeMsg(std::string_view attackingPokemonName) const
{
	m_sink.Emit(std::format("{} burrowed its way under the ground!", attackingPokemonName));
}

void MoveResultsText::DisplayRecoveredHPRestoredMsg(std::string_view attackingPlayerName, std::string_view attackingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {} had its HP restored!", attackingPlayerName, attackingPokemonName));
}

void MoveResultsText::DisplayHPFullMsg(std::string_view attackingPlayerName, std::string_view attackingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {}'s HP is full!", attackingPlayerName, attackingPokemonName));
}

void MoveResultsText::DisplaySkullBashChargeMsg(std::string_view attackingPlayerName, std::string_view attackingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {} tucked in its head!", attackingPlayerName, attackingPokemonName));
}

void MoveResultsText::DisplaySkyAttackChargeMsg(std::string_view attackingPlayerName, std::string_view attackingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {} became cloaked in a harsh light!", attackingPlayerName, attackingPokemonName));
}

void MoveResultsText::DisplaySplashMsg() const
{
	m_sink.Emit(std::format("But nothing happened!"));
}

void MoveResultsText::DisplayRestMsg(std::string_view attackingPlayerName, std::string_view attackingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {} slept and became healthy!", attackingPlayerName, attackingPokemonName));
}

void MoveResultsText::DisplayNVStatusMsg(std::string_view statusMessage) const
{
	m_sink.Emit(std::format("{}", statusMessage));
}

void MoveResultsText::DisplayBecameConfuseMsg(std::string_view defendingPlayerName, std::string_view defendingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {} became confused!", defendingPlayerName, defendingPokemonName));
}

void MoveResultsText::DisplayAlreadyPoisonedMsg(std::string_view defendingPlayerName, std::string_view defendingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {} is already poisoned!", defendingPlayerName, defendingPokemonName));
}

void MoveResultsText::DisplayAlreadyParalyzedMsg(std::string_view defendingPlayerName, std::string_view defendingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {} is already paralyzed!", defendingPlayerName, defendingPokemonName));
}

void MoveResultsText::DisplayAlreadyAsleepMsg(std::string_view defendingPlayerName, std::string_view defendingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {} is already asleep!", defendingPlayerName, defendingPokemonName));
}

void MoveResultsText::DisplayDoesntAffectMsg(std::string_view defendingPlayerName, std::string_view defendingPokemonName) const
{
	m_sink.Emit(std::format("It doesn't affect {}'s {}...", defendingPlayerName, defendingPokemonName));
}

void MoveResultsText::DisplayStatRaised2Msg(std::string_view statName, std::string_view attackingPlayerName, std::string_view attackingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {}'s {} rose sharply!", attackingPlayerName, attackingPokemonName, statName));
}

void MoveResultsText::DisplayStatRaised1Msg(std::string_view statName, std::string_view attackingPlayerName, std::string_view attackingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {}'s {} rose!", attackingPlayerName, attackingPokemonName, statName));
}

void MoveResultsText::DisplayStatRaiseFailMsg(std::string_view statName, std::string_view attackingPlayerName, std::string_view attackingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {} can't raise its {} any higher!", attackingPlayerName, attackingPokemonName, statName));
}

void MoveResultsText::DisplayStatLowered2Msg(std::string_view statName, std::string_view defendingPlayerName, std::string_view defendingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {}'s {} fell harshly!", defendingPlayerName, defendingPokemonName, statName));
}

void MoveResultsText::DisplayStatLowered1Msg(std::string_view statName, std::string_view defendingPlayerName, std::string_view defendingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {}'s {} fell!", defendingPlayerName, defendingPokemonName, statName));
}

void MoveResultsText::DisplayStatLoweredFailMsg(std::string_view statName, std::string_view defendingPlayerName, std::string_view defendingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {}'s {} won't go any lower!", defendingPlayerName, defendingPokemonName, statName));
}

void MoveResultsText::DisplayFocusEnergyMsg(std::string_view attackingPlayerName, std::string_view attackingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {} is getting pumped!", attackingPlayerName, attackingPokemonName));
}

void MoveResultsText::DisplayProtectedByMistMsg(std::string_view defendingPlayerName, std::string_view defendingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {} is protected by the mist!", defendingPlayerName, defendingPokemonName));
}

void MoveResultsText::DisplayMistMsg(std::string_view attackingPlayerName) const
{
	m_sink.Emit(std::format("{}'s team became shrouded in mist!", attackingPlayerName));
}

void MoveResultsText::DisplayLightScreenMsg(std::string_view attackingPlayerName) const
{
	m_sink.Emit(std::format("Light screen made {}'s team stronger against special moves!", attackingPlayerName));
}

void MoveResultsText::DisplayHazeMsg() const
{
	m_sink.Emit(std::format("All stat changes were eliminated!"));
}

void MoveResultsText::DisplayReflectMsg(std::string_view attackingPlayerName) const
{
	m_sink.Emit(std::format("Reflect made {}'s team stronger against physical moves!", attackingPlayerName));
}

void MoveResultsText::DisplayThrashDisabledMsg(std::string_view attackingPlayerName, std::string_view attackingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {}'s Thrash is disabled!", attackingPlayerName, attackingPokemonName));
}

void MoveResultsText::DisplayMoveDisabledMsg(std::string_view defendingPlayerName, std::string_view defendingPokemonName, std::string_view moveName) const
{
	m_sink.Emit(std::format("{}'s {}'s {} was disabled!", defendingPlayerName, defendingPokemonName, moveName));
}

void MoveResultsText::DisplayAlreadyHasSubstituteMsg(std::string_view attackingPlayerName, std::string_view attackingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {} already has a substitute!", attackingPlayerName, attackingPokemonName));
}

void MoveResultsText::DisplayNotEnoughHPSubstituteMsg() const
{
	m_sink.Emit(std::format("But it does not have enough HP left to make a substitute!"));
}

void MoveResultsText::DisplayPutInSubstituteMsg(std::string_view attackingPlayerName, std::string_view attackingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {} put in a substitute!", attackingPlayerName, attackingPokemonName));
}

void MoveResultsText::DisplaySeededMsg(std::string_view defendingPlayerName, std::string_view defendingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {} was seeded!", defendingPlayerName, defendingPokemonName));
}

void MoveResultsText::DisplayLearnedMimicMoveMsg(std::string_view attackingPlayerName, std::string_view attackingPokemonName, std::string_view lastUsedMoveName) const
{
	m_sink.Emit(std::format("{}'s {} learned {}!", attackingPlayerName, attackingPokemonName, lastUsedMoveName));
}

void MoveResultsText::DisplayTransformMsg(std::string_view attackingPlayerName, std::string_view attackingPokemonName, std::string_view defendingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {} transformed into {}!", attackingPlayerName, attackingPokemonName, defendingPokemonName));
}

void MoveResultsText::DisplayMetronomeMsg(std::string_view attackingPlayerName, std::string_view attackingPokemonName, std::string_view moveName) const
{
	m_sink.Emit(std::format("Waggling its finger let {}'s {} use {}", attackingPlayerName, attackingPokemonName, moveName));
}

void MoveResultsText::DisplayConversionMsg(std::string_view attackingPlayerName, std::string_view attackingPokemonName, std::string_view attackingPokemonNewType) const
{
	m_sink.Emit(std::format("{}'s {}'s type changed to {}!", attackingPlayerName, attackingPokemonName, attackingPokemonNewType));
}

void MoveResultsText::DisplayBideUnleashedMsg(std::string_view attackingPlayerName, std::string_view attackingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {} unleashed its energy!", attackingPlayerName, attackingPokemonName));
}

void MoveResultsText::DisplayBideStoringEnergyMsg(std::string_view attackingPlayerName, std::string_view attackingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {} is storing energy!", attackingPlayerName, attackingPokemonName));
}

void MoveResultsText::DisplayBideDisabledMsg(std::string_view attackingPlayerName, std::string_view attackingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {}'s Bide is disabled!", attackingPlayerName, attackingPokemonName));
}

void MoveResultsText::DisplayNoMovesLeftStruggleMsg(std::string_view attackingPlayerName, std::string_view attackingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {} has no moves left!", attackingPlayerName, attackingPokemonName));
}