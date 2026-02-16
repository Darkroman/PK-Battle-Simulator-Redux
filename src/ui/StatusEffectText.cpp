#include <iostream>

#include "interfaces/ITextSink.h"
#include "StatusEffectText.h"
#include "../battle/BattleContext.h"

// Status changes
void StatusEffectText::DisplayFellAsleepMsg(std::string_view defendingPlayerName, std::string_view defendingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {} fell asleep!", defendingPlayerName, defendingPokemonName));
}

void StatusEffectText::DisplayWokenUpMsg(std::string_view attackingPokemonName) const
{
	m_sink.Emit(std::format("{} has woken up!", attackingPokemonName));
}

void StatusEffectText::DisplayIsAsleepMsg(std::string_view attackingPokemonName) const
{
	m_sink.Emit(std::format("{} is sleeping.", attackingPokemonName));
}

void StatusEffectText::DisplayFrozenSolidMsg(std::string_view attackingPokemonName) const
{
	m_sink.Emit(std::format("{} is frozen solid!", attackingPokemonName));
}

void StatusEffectText::DisplayThawedMsg(std::string_view attackingPokemonName) const
{
	m_sink.Emit(std::format("{} thawed out!", attackingPokemonName));
}

void StatusEffectText::DisplayNoLongerConfusedMsg(std::string_view attackingPlayerName, std::string_view attackingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {} is no longer confused!", attackingPlayerName, attackingPokemonName));
}

void StatusEffectText::DisplayIsConfusedMsg(std::string_view attackingPlayerName, std::string_view attackingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {} is confused!", attackingPlayerName, attackingPokemonName));
}

void StatusEffectText::DisplayHurtItselfConfuseMsg() const
{
	m_sink.Emit(std::format("It hurt itself in its confusion!"));
}

void StatusEffectText::DisplayCantMoveParalysisMsg(std::string_view attackingPokemonName) const
{
	m_sink.Emit(std::format("{} couldn't move because it's paralyzed!", attackingPokemonName));
}

void StatusEffectText::DisplayFlinchMsg(std::string_view attackingPlayerName, std::string_view attackingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {} flinched and couldn't move!", attackingPlayerName, attackingPokemonName));
}

// Barriers / field effects
void StatusEffectText::DisplayNoLongerProtectedMist(std::string_view playerName) const
{
	m_sink.Emit(std::format("{}'s team is no longer protected by mist!", playerName));
}

void StatusEffectText::DisplayFieldEffectFadedMsg(std::string_view playerName, std::string_view effect) const
{
	if (effect == "Light Screen" || effect == "light screen" || effect == "Reflect" || effect == "reflect")
	{
		m_sink.Emit(std::format("{}'s team's {} wore off!", playerName, effect));
	}

	else if (effect == "Mist" || effect == "mist")
	{
		m_sink.Emit(std::format("{}'s team is no longer protected by {}!", playerName, effect));
		
	}
}

// Post-turn effects
void StatusEffectText::DisplayLeechSeedSappedMsg(std::string_view playerName, std::string_view pokemonName) const
{
	m_sink.Emit(std::format("{}'s {}'s health is sapped by Leech Seed.", playerName, pokemonName));
}

void StatusEffectText::DisplayDamagedByStatusPostTurn(std::string_view status, std::string_view playerName, std::string_view pokemonName) const
{
	m_sink.Emit(std::format("{}'s {} was damaged by {}.", playerName, pokemonName, status));
}

void StatusEffectText::DisplayHurtByBoundMsg(std::string_view playerName, std::string_view pokemonName, std::string_view boundMoveName) const
{
	m_sink.Emit(std::format("{}'s {} was hurt by {}!", playerName, pokemonName, boundMoveName));
}

void StatusEffectText::DisplayFreedFromBoundMsg(std::string_view playerName, std::string_view pokemonName, std::string_view boundMoveName) const
{
	m_sink.Emit(std::format("{}'s {} was freed from {}!", playerName, pokemonName, boundMoveName));
}

// Thrash
void StatusEffectText::DisplayThrashConfusionMsg(std::string_view attackingPlayerName, std::string_view attackingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {} became confused due to fatigue!", attackingPlayerName, attackingPokemonName));
}

// Rage
void StatusEffectText::DisplayRageStartedMsg(std::string_view attackingPlayerName, std::string_view attackingPokemonName) const
{
	m_sink.Emit(std::format("(Rage started on {}'s {})", attackingPlayerName, attackingPokemonName));
}

void StatusEffectText::DisplayStatRaised1Msg(std::string_view statName, std::string_view defendingPlayerName, std::string_view defendingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {}'s {} rose!", defendingPlayerName, defendingPokemonName, statName));
}

void StatusEffectText::DisplayStatRaiseFailMsg(std::string_view statName, std::string_view defendingPlayerName, std::string_view defendingPokemonName) const
{
	m_sink.Emit(std::format("{}'s {} can't raise its {} any higher!", defendingPlayerName, defendingPokemonName, statName));
}

// Disable
void StatusEffectText::DisplayMoveIsDisabledMsg(std::string_view attackingPlayerName, std::string_view attackingPokemonName, std::string_view moveName) const
{
	m_sink.Emit(std::format("{}'s {}'s {} is disabled!", attackingPlayerName, attackingPokemonName, moveName));
}

void StatusEffectText::DisplayMoveNoLongerDisabledMsg(std::string_view attackingPlayerName, std::string_view attackingPokemonName, std::string_view disabledMoveName) const
{
	m_sink.Emit(std::format("{}'s {}'s {} is no longer disabled!", attackingPlayerName, attackingPokemonName, disabledMoveName));
}

// Faint
void StatusEffectText::DisplayFaintedMsg(std::string_view playerName, std::string_view pokemonName) const
{
	m_sink.Emit(std::format("{}'s {} has fainted!", playerName, pokemonName));
}

void StatusEffectText::NewLine() const
{
	std::cout << '\n';
}