#include <iostream>

#include "../battle/BattleContext.h"

#include "MoveResultsText.h"

MoveResultsText::MoveResultsText(BattleContext& context) : m_context(context) {}

void MoveResultsText::UsedTextDialog(Player* currentPlayer, BattlePokemon::pokemonMove* currentMove, BattlePokemon* source)
{
	std::cout << currentPlayer->GetPlayerNameView() << "'s " << source->GetNameView() << " used " << currentMove->GetName() << "\n";
}

void MoveResultsText::DisplayCritTextDialog()
{
	if (m_context.flags.isCriticalHit && m_context.flags.hit)
	{
		std::cout << "Critical hit!\n";
	}
}

void MoveResultsText::DisplayEffectivenessTextDialog(Player* targetPlayer, BattlePokemon* targetPokemon)
{
	if (m_context.flags.currentEffectiveness == BattleStateFlags::Effectiveness::Less && m_context.flags.hit)
	{
		std::cout << "It was not very effective...\n";
	}

	if (m_context.flags.currentEffectiveness == BattleStateFlags::Effectiveness::Super && m_context.flags.hit)
	{
		std::cout << "It was super effective!\n";
	}

	if (m_context.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No && m_context.flags.hit)
	{
		std::cout << "It doesn't affect " << targetPlayer->GetPlayerNameView() << "'s " << targetPokemon->GetNameView() << "...\n";
	}

	if (m_context.flags.currentEffectiveness == BattleStateFlags::Effectiveness::OHKO && m_context.flags.hit)
	{
		std::cout << "It was a one-hit KO!\n";
	}
}

void MoveResultsText::DisplayAttackMissedTextDialog(Player* player, BattlePokemon* pokemon)
{
	std::cout << player->GetPlayerNameView() << "'s " << pokemon->GetNameView() << "'s attack missed!\n";
}

void MoveResultsText::DisplayAttackAvoidedTextDialog(Player* player, BattlePokemon* pokemon)
{
	std::cout << player->GetPlayerNameView() << "'s " << pokemon->GetNameView() << " avoided the attack!\n";
}

void MoveResultsText::DisplayFailedTextDialog()
{
	std::cout << "But it failed!\n";
}

void MoveResultsText::DisplaySubstituteDamageTextDialog(Player* player, BattlePokemon* pokemon)
{
	if (pokemon->GetSubstituteHP() > 0 && m_context.flags.hitSubstitute)
	{
		std::cout << "The substitute took damage for " << player->GetPlayerNameView() << "'s " << pokemon->GetNameView() << "!\n";
	}
}

void MoveResultsText::BoundMoveText(Player* sourcePlayer, Player* targetPlayer, BattlePokemon* sourcePokemon, BattlePokemon* targetPokemon, BattlePokemon::pokemonMove* currentMove)
{
	if (currentMove->GetMoveIndex() == 20)
	{
		std::cout << targetPlayer->GetPlayerNameView() << "'s " << targetPokemon->GetNameView() << " was squeezed by " << sourcePlayer->GetPlayerNameView() << "'s " << sourcePokemon->GetNameView() << "!\n";
	}

	else if (currentMove->GetMoveIndex() == 35)
	{
		std::cout << targetPlayer->GetPlayerNameView() << "'s " << targetPokemon->GetNameView() << " was wrapped by " << sourcePlayer->GetPlayerNameView() << "'s " << sourcePokemon->GetNameView() << "!\n";
	}

	else if (currentMove->GetMoveIndex() == 83)
	{
		std::cout << targetPlayer->GetPlayerNameView() << "'s " << targetPokemon->GetNameView() << " became trapped in the fiery vortex!\n";
	}

	else if (currentMove->GetMoveIndex() == 128)
	{
		std::cout << sourcePlayer->GetPlayerNameView() << "'s " << sourcePokemon->GetNameView() << " clamped down on " << targetPlayer->GetPlayerNameView() << "'s " << targetPokemon->GetNameView() << "!\n";
	}
}

void MoveResultsText::DisplayNoopMsg()
{
	std::cout << "This isn't implemented yet!\n\n";
}

void MoveResultsText::DisplayDirectDamageInflictedMsg(double damage)
{
	std::cout << static_cast<int>(damage) << " damage inflicted.\n";
}

void MoveResultsText::DisplayMultiAttackMsg(int timesHit)
{
	std::cout << m_context.defendingPokemon->GetNameView() << " was hit " << timesHit << " times!\n";
}

void MoveResultsText::DisplayRazorWindChargeMsg()
{
	std::cout << m_context.attackingPlayer->GetPlayerNameView() << "'s " << m_context.attackingPokemon->GetNameView() << " made a whirlwind!\n";
}

void MoveResultsText::DisplayEnemySwitchMsg(BattlePokemon& pokemon)
{
	std::cout << "The opposing " << pokemon.GetNameView() << " was dragged out!\n";
}

void MoveResultsText::DisplayFlyChargeMsg()
{
	std::cout << m_context.attackingPokemon->GetNameView() << " flew up high in the sky!\n";
}

void MoveResultsText::DisplayJumpKickCrashMsg()
{
	std::cout << m_context.attackingPlayer->GetPlayerNameView() << "'s " << m_context.attackingPokemon->GetNameView() << " kept going and crashed!\n";
}

void MoveResultsText::DisplayRecoilMsg()
{
	std::cout << m_context.attackingPlayer->GetPlayerNameView() << "'s " << m_context.attackingPokemon->GetNameView() << " was damaged by the recoil!\n";
}

void MoveResultsText::DisplayRechargeMsg()
{
	std::cout << m_context.attackingPlayer->GetPlayerNameView() << "'s " << m_context.attackingPokemon->GetNameView() << " must recharge!\n";
}

void MoveResultsText::DisplayEnergyDrainedMsg()
{
	std::cout << m_context.defendingPlayer->GetPlayerNameView() << "'s " << m_context.defendingPokemon->GetNameView() << " had its energy drained!\n";
}

void MoveResultsText::DisplaySolarBeamChargeMsg()
{
	std::cout << m_context.attackingPlayer->GetPlayerNameView() << "'s " << m_context.attackingPokemon->GetNameView() << " absorbed light!\n";
}

void MoveResultsText::DisplayDigChargeMsg()
{
	std::cout << m_context.attackingPokemon->GetNameView() << " burrowed its way under the ground!\n";
}

void MoveResultsText::DisplayRecoveredHPRestoredMsg(double healAmount)
{
	std::cout << m_context.attackingPlayer->GetPlayerNameView() << "'s " << m_context.attackingPokemon->GetNameView()
		<< " had its HP restored!\n";
}

void MoveResultsText::DisplayHPFullMsg()
{
	std::cout << m_context.attackingPlayer->GetPlayerNameView() << "'s " << m_context.attackingPokemon->GetNameView()
		<< " HP is full!\n";
}

void MoveResultsText::DisplaySkullBashChargeMsg()
{
	std::cout << m_context.attackingPlayer->GetPlayerNameView() << "'s " << m_context.attackingPokemon->GetNameView() << " tucked in its head!\n";
}

void MoveResultsText::DisplaySkyAttackChargeMsg()
{
	std::cout << m_context.attackingPlayer->GetPlayerNameView() << "'s " << m_context.attackingPokemon->GetNameView() << " became cloaked in a harsh light!\n";
}

void MoveResultsText::DisplaySplashMsg()
{
	std::cout << "But nothing happened!\n";
}

void MoveResultsText::DisplayRestMsg()
{
	std::cout << m_context.attackingPlayer->GetPlayerNameView() << "'s " << m_context.attackingPokemon->GetNameView() << " slept and became healthy!\n";
}