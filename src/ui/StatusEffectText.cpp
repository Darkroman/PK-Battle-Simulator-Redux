#include <iostream>

#include "StatusEffectText.h"
#include "../entities/Player.h"
#include "../entities/BattlePokemon.h"
#include "../battle/BattleContext.h"
#include "../data/Move.h"

StatusEffectText::StatusEffectText(BattleContext& context) :
	m_context(context) {}

// Status changes
void StatusEffectText::DisplayNVStatusMsg(std::string_view statusMessage)
{
	std::cout << statusMessage << '\n';
}

void StatusEffectText::DisplayFellAsleepMsg()
{
	std::cout << m_context.defendingPlayer->GetPlayerNameView() << "'s " << m_context.defendingPokemon->GetNameView() << " fell asleep!\n";
}

void StatusEffectText::DisplayWokenUpMsg() 
{
	std::cout << m_context.attackingPokemon->GetNameView() << " has woken up!\n";
}

void StatusEffectText::DisplayIsAsleepMsg()
{
	std::cout << m_context.attackingPokemon->GetNameView() << " is sleeping.\n";
}

void StatusEffectText::DisplayFrozenSolidMsg()
{
	std::cout << m_context.attackingPokemon->GetNameView() << " is frozen solid!\n";
}

void StatusEffectText::DisplayThawedMsg() 
{
	std::cout << m_context.attackingPokemon->GetNameView() << " thawed out!\n";
}

void StatusEffectText::DisplayBecameConfuseMsg() 
{
	std::cout << m_context.defendingPlayer->GetPlayerNameView() << "'s " << m_context.defendingPokemon->GetNameView() << " became confused!\n";
}

void StatusEffectText::DisplayNoLongerConfusedMsg() 
{
	std::cout << m_context.attackingPlayer->GetPlayerNameView() << "'s " << m_context.attackingPokemon->GetNameView() << " is no longer confused!\n";
}

void StatusEffectText::DisplayIsConfusedMsg() 
{
	std::cout << m_context.attackingPlayer->GetPlayerNameView() << "'s " << m_context.attackingPokemon->GetNameView() << " is confused!\n";
}

void StatusEffectText::DisplayHurtItselfConfuseMsg()
{
	std::cout << "It hurt itself in its confusion!\n";
}

void StatusEffectText::DisplayCantMoveParalysisMsg() 
{
	std::cout << m_context.attackingPokemon->GetNameView() << " couldn't move because it's paralyzed!\n";
}

void StatusEffectText::DisplayFlinchMsg() 
{
	std::cout << m_context.attackingPlayer->GetPlayerNameView() << "'s " << m_context.attackingPokemon->GetNameView() << " flinched and couldn't move!\n";
}

void StatusEffectText::DisplayAlreadyPoisonedMsg() 
{
	std::cout << m_context.defendingPlayer->GetPlayerNameView() << "'s " << m_context.defendingPokemon->GetNameView() << " is already poisoned!\n";
}

void StatusEffectText::DisplayAlreadyParalyzedMsg() 
{
	std::cout << m_context.defendingPlayer->GetPlayerNameView() << "'s " << m_context.defendingPokemon->GetNameView() << " is already paralyzed!\n";
}

void StatusEffectText::DisplayAlreadyAsleepMsg() 
{
	std::cout << m_context.defendingPlayer->GetPlayerNameView() << "'s " << m_context.defendingPokemon->GetNameView() << " is already asleep!\n";
}

void StatusEffectText::DisplayDoesntAffectMsg()
{
	std::cout << "It doesn't affect " << m_context.defendingPlayer->GetPlayerNameView() << "'s " << m_context.defendingPokemon->GetNameView() << "...\n";
}

// Stat changes
void StatusEffectText::DisplayStatRaised2Msg(std::string_view statName)
{
	std::cout << m_context.attackingPlayer->GetPlayerNameView() << "'s " << m_context.attackingPokemon->GetNameView() << "'s " << statName << " rose sharply!\n";
}

void StatusEffectText::DisplayStatRaised1Msg(std::string_view statName)
{
	std::cout << m_context.attackingPlayer->GetPlayerNameView() << "'s " << m_context.attackingPokemon->GetNameView() << "'s " << statName << " rose!\n";
}

void StatusEffectText::DisplayStatRaiseFailMsg(std::string_view statName)
{
	std::cout << m_context.attackingPlayer->GetPlayerNameView() << "'s " << m_context.attackingPokemon->GetNameView() << " can't raise its " << statName << " any higher!\n";
}

void StatusEffectText::DisplayStatLowered2Msg(std::string_view statName)
{
	std::cout << m_context.defendingPlayer->GetPlayerNameView() << "'s " << m_context.defendingPokemon->GetNameView() << "'s " << statName << " fell harshly!\n";
}

void StatusEffectText::DisplayStatLowered1Msg(std::string_view statName)
{
	std::cout << m_context.defendingPlayer->GetPlayerNameView() << "'s " << m_context.defendingPokemon->GetNameView() << "'s " << statName << " fell!\n";
}

void StatusEffectText::DisplayStatLoweredFailMsg(std::string_view statName)
{
	std::cout << m_context.defendingPlayer->GetPlayerNameView() << "'s " << m_context.defendingPokemon->GetNameView() << "'s " << statName << " won't go any lower!\n";
}

void StatusEffectText::DisplayFocusEnergyMsg()
{
	std::cout << m_context.attackingPlayer->GetPlayerNameView() << "'s " << m_context.attackingPokemon->GetNameView()
		<< " is getting pumped!\n";
}

// Barriers / field effects
void StatusEffectText::DisplayProtectedByMistMsg()
{
	std::cout << m_context.defendingPlayer->GetPlayerNameView() << "'s " << m_context.defendingPokemon->GetNameView() << " is protected by the mist!\n";
}

void StatusEffectText::DisplayMistMsg() 
{
	std::cout << m_context.attackingPlayer->GetPlayerNameView() << "'s team became shrouded in mist!\n";
}

void StatusEffectText::DisplayNoLongerProtectedMist(Player* player)
{
	std::cout << player->GetPlayerNameView() << "'s team is no longer protected by mist!\n";
}

void StatusEffectText::DisplayLightScreenMsg() 
{
	std::cout << "Light screen made " << m_context.attackingPlayer->GetPlayerNameView()
		<< "'s team stronger against special moves!\n";
}

void StatusEffectText::DisplayHazeMsg() 
{
	std::cout << "All stat changes were eliminated!\n";
}

void StatusEffectText::DisplayReflectMsg() 
{
	std::cout << "Reflect made " << m_context.attackingPlayer->GetPlayerNameView()
		      << "'s team stronger against physical moves!\n";
}

// Post-turn effects
void StatusEffectText::DisplayFieldEffectFadedMsg(Player* player, std::string_view effect) 
{
	if (effect == "Light Screen" || effect == "light screen" || effect == "Reflect" || effect == "reflect")
	{
		std::cout << player->GetPlayerNameView() << "'s team's " << effect << " wore off!\n";
	}

	else if (effect == "Mist" || effect == "mist")
	{
		std::cout << player->GetPlayerNameView() << "'s team is no longer protected by " << effect << "!\n";
	}
}

void StatusEffectText::DisplayLeechSeedSappedMsg(Player* player, BattlePokemon* pokemon) 
{
	std::cout << player->GetPlayerNameView() << "'s " << pokemon->GetNameView() << "'s health is sapped by Leech Seed.\n";
}

void StatusEffectText::DisplayDamagedByStatusPostTurn(std::string_view status, Player* player, BattlePokemon* pokemon) 
{
	std::cout << player->GetPlayerNameView() << "'s " << pokemon->GetNameView() << " was damaged by " << status << ".\n";
}

void StatusEffectText::DisplayHurtByBoundMsg(Player* player, BattlePokemon* pokemon) 
{
	std::cout << player->GetPlayerNameView() << "'s "
		<< pokemon->GetNameView() << " was hurt by "
		<< pokemon->GetBoundMoveName() << "!\n";
}

void StatusEffectText::DisplayFreedFromBoundMsg(Player* player, BattlePokemon* pokemon) 
{
	std::cout << player->GetPlayerNameView() << "'s "
		<< pokemon->GetNameView() << " was freed from "
		<< pokemon->GetBoundMoveName() << "!\n";
}

// Thrash
void StatusEffectText::DisplayThrashDisabledMsg() 
{
	std::cout << m_context.attackingPlayer->GetPlayerNameView() << "'s " << m_context.attackingPokemon->GetNameView() << "'s Thrash is disabled!\n";
}

void StatusEffectText::DisplayThrashConfusionMsg() 
{
	std::cout << m_context.attackingPlayer->GetPlayerNameView() << "'s " << m_context.attackingPokemon->GetNameView() << " became confused due to fatigue!\n";
}

// Rage
void StatusEffectText::DisplayRageStartedMsg() 
{
	std::cout << "(Rage started on " << m_context.attackingPlayer->GetPlayerNameView() << "'s " << m_context.attackingPokemon->GetNameView() << ")\n";
}

// Disable
void StatusEffectText::DisplayMoveDisabledMsg() 
{
	std::cout << m_context.defendingPlayer->GetPlayerNameView() << "'s " << m_context.defendingPokemon->GetNameView()
		<< "'s " << m_context.defendingPokemon->GetLastUsedMove()->mp_move->GetName() << " was disabled!\n";
}

void StatusEffectText::DisplayMoveIsDisabledMsg()
{
	std::cout << m_context.attackingPlayer->GetPlayerNameView() << "'s " << m_context.attackingPokemon->GetNameView() << "'s " << m_context.currentMove->mp_move->GetName() << " is disabled!\n";
}

void StatusEffectText::DisplayMoveNoLongerDisabledMsg(Player*, BattlePokemon*) 
{
	std::cout << m_context.attackingPlayer->GetPlayerNameView() << "'s "
		<< m_context.attackingPokemon->GetNameView() << "'s "
		<< m_context.attackingPokemon->GetDisabledMove()->mp_move->GetName()
		<< " is no longer disabled!\n";
}

// Substitute
void StatusEffectText::DisplayAlreadyHasSubstituteMsg() 
{
	std::cout << m_context.attackingPlayer->GetPlayerNameView() << "'s " << m_context.attackingPokemon->GetNameView() << " already has a substitute!\n";
}

void StatusEffectText::DisplayNotEnoughHPSubstituteMsg() 
{
	std::cout << "But it does not have enough HP left to make a substitute!\n";
}

void StatusEffectText::DisplayPutInSubstituteMsg() 
{
	std::cout << m_context.attackingPlayer->GetPlayerNameView() << "'s " << m_context.attackingPokemon->GetNameView() << " put in a substitute!\n";
}

void StatusEffectText::DisplaySubstituteFadedMsg() 
{
	std::cout << m_context.defendingPlayer->GetPlayerNameView() << "'s " << m_context.defendingPokemon->GetNameView() << " substitute faded!\n";
}

void StatusEffectText::DisplaySeededMsg() 
{
	std::cout << m_context.defendingPlayer->GetPlayerNameView() << "'s " << m_context.defendingPokemon->GetNameView() << " was seeded!\n";
}

void StatusEffectText::DisplayLearnedMimicMoveMsg() 
{
	std::cout << m_context.attackingPlayer->GetPlayerNameView() << "'s " << m_context.attackingPokemon->GetNameView()
		<< " learned " << m_context.defendingPokemon->GetLastUsedMove()->mp_move->GetName() << "!\n";
}

void StatusEffectText::DisplayTransformMsg() 
{
	std::cout << m_context.attackingPlayer->GetPlayerNameView() << "'s "
		      << m_context.attackingPokemon->GetNameView() << " transformed into "
		      << m_context.defendingPokemon->GetNameView() << "!\n";
}

void StatusEffectText::DisplayMetronomeMsg(Move* selectedMove) 
{
	std::cout << "Waggling its finger let " << m_context.attackingPlayer->GetPlayerNameView() << "'s "
		<< m_context.attackingPokemon->GetNameView() << " use " << selectedMove->GetName() << "!\n";
}

void StatusEffectText::DisplayConversionMsg() 
{
	std::cout << m_context.attackingPlayer->GetPlayerNameView() << "'s " << m_context.attackingPokemon->GetNameView()
		<< "'s type changed to " << m_context.attackingPokemon->GetTypeOne() << "!\n";
}

void StatusEffectText::DisplayBideUnleashedMsg() 
{
	std::cout << m_context.attackingPlayer->GetPlayerNameView() << "'s " << m_context.attackingPokemon->GetNameView()
		<< " unleashed its energy!\n";
}

void StatusEffectText::DisplayBideStoringEnergyMsg() 
{
	std::cout << m_context.attackingPlayer->GetPlayerNameView() << "'s " << m_context.attackingPokemon->GetNameView()
		<< " is storing energy!\n";
}

void StatusEffectText::DisplayBideDisabledMsg() 
{
	std::cout << m_context.attackingPlayer->GetPlayerNameView() << "'s " << m_context.attackingPokemon->GetNameView()
		<< "'s Bide is disabled!\n";
}

void StatusEffectText::DisplayNoMovesLeftStruggleMsg() 
{
	std::cout << m_context.attackingPlayer->GetPlayerNameView() << "'s " << m_context.attackingPokemon->GetNameView() << " has no moves left!\n";
}

void StatusEffectText::DisplayFaintedMsg(Player& player, BattlePokemon& pokemon) 
{
	std::cout << player.GetPlayerNameView() << "'s " << pokemon.GetNameView() << " has fainted!\n";
}