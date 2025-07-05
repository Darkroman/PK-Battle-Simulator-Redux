#include "MoveResultsText.h"

MoveResultsText::MoveResultsText(BattleContext& context) : context(context) {}

void MoveResultsText::UsedTextDialog(Player* currentPlayer, BattlePokemon::pokemonMove* currentMove, BattlePokemon* source)
{
	std::cout << currentPlayer->GetPlayerNameView() << "'s " << source->GetNameView() << " used " << currentMove->mp_move->GetName() << "\n";
}

void MoveResultsText::DisplayCritTextDialog()
{
	if (context.flags.isCriticalHit && context.flags.hit)
	{
		std::cout << "Critical hit!\n";
	}
}

void MoveResultsText::DisplayEffectivenessTextDialog(Player* targetPlayer, BattlePokemon* targetPokemon)
{
	if (context.flags.currentEffectiveness == BattleStateFlags::Effectiveness::Less && context.flags.hit)
	{
		std::cout << "It was not very effective...\n";
	}

	if (context.flags.currentEffectiveness == BattleStateFlags::Effectiveness::Super && context.flags.hit)
	{
		std::cout << "It was super effective!\n";
	}

	if (context.flags.currentEffectiveness == BattleStateFlags::Effectiveness::No && context.flags.hit)
	{
		std::cout << "It doesn't affect " << targetPlayer->GetPlayerNameView() << "'s " << targetPokemon->GetNameView() << "...\n";
	}

	if (context.flags.currentEffectiveness == BattleStateFlags::Effectiveness::OHKO && context.flags.hit)
	{
		std::cout << "It was a one-hit KO!\n";
	}
}

void MoveResultsText::DisplayAccuracyLoweredTextDialog(Player* player, BattlePokemon* pokemon)
{
	std::cout << player->GetPlayerNameView() << "'s " << pokemon->GetNameView() << "'s accuracy fell!\n";
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
	if (pokemon->GetSubstituteHP() > 0)
	{
		std::cout << "The substitute took damage for " << player->GetPlayerNameView() << "'s " << pokemon->GetNameView() << '!\n';
	}
}

void MoveResultsText::BoundMoveText(Player* sourcePlayer, Player* targetPlayer, BattlePokemon* sourcePokemon, BattlePokemon* targetPokemon, BattlePokemon::pokemonMove* currentMove)
{
	if (currentMove->mp_move->GetMoveIndex() == 20)
	{
		std::cout << targetPlayer->GetPlayerNameView() << "'s " << targetPokemon->GetNameView() << " was squeezed by " << sourcePlayer->GetPlayerNameView() << "'s " << sourcePokemon->GetNameView() << "!\n";
	}

	else if (currentMove->mp_move->GetMoveIndex() == 35)
	{
		std::cout << targetPlayer->GetPlayerNameView() << "'s " << targetPokemon->GetNameView() << " was wrapped by " << sourcePlayer->GetPlayerNameView() << "'s " << sourcePokemon->GetNameView() << "!\n";
	}

	else if (currentMove->mp_move->GetMoveIndex() == 83)
	{
		std::cout << targetPlayer->GetPlayerNameView() << "'s " << targetPokemon->GetNameView() << " became trapped in the fiery vortex!\n";
	}

	else if (currentMove->mp_move->GetMoveIndex() == 128)
	{
		std::cout << sourcePlayer->GetPlayerNameView() << "'s " << sourcePokemon->GetNameView() << " clamped down on " << targetPlayer->GetPlayerNameView() << "'s " << targetPokemon->GetNameView() << "!\n";
	}
}