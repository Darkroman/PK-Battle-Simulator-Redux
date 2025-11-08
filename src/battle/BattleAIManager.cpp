#include <iostream>
#include <limits>

#include "BattleContext.h"

#include "BattleAIManager.h"

namespace BattleAIProcedures
{
	void InitAIPlayers(BattleContext& context)
	{
		if (context.aiPlayerOne)
		{
			context.aiPlayerOne->GetOpponentParty(context.playerTwo);
			context.aiPlayerOne->InitEstimatedStatRanges(context.playerTwo);
			context.aiPlayerOne->PrecomputeRealStats(context.playerTwo);
		}

		if (context.aiPlayerTwo)
		{
			context.aiPlayerTwo->GetOpponentParty(context.playerOne);
			context.aiPlayerTwo->InitEstimatedStatRanges(context.playerOne);
			context.aiPlayerTwo->PrecomputeRealStats(context.playerOne);
		}
	}

	void UpdateEnemyActivePokemon(BattleContext& context)
	{
		if (context.aiPlayerOne)
		{
			context.aiPlayerOne->UpdateActivePokemon(context.playerTwoCurrentPokemon);
		}

		if (context.aiPlayerTwo)
		{
			context.aiPlayerTwo->UpdateActivePokemon(context.playerOneCurrentPokemon);
		}
	}

	void RefineEnemyModelFirstTurn(BattleContext& context)
	{
		if (context.aiPlayerOne && context.playerOne == context.attackingPlayer)
		{
			context.aiPlayerOne->UpdateEnemyHPandDefenseStats(context, context.defendingPlayer, context.currentMove, context.attackingPokemon, context.defendingPokemon);
		}

		if (context.aiPlayerTwo && context.playerTwo == context.attackingPlayer)
		{
			context.aiPlayerTwo->UpdateEnemyHPandDefenseStats(context, context.defendingPlayer, context.currentMove, context.attackingPokemon, context.defendingPokemon);
		}

		if (context.aiPlayerOne && context.playerOne == context.defendingPlayer)
		{
			context.aiPlayerOne->UpdateEnemyOffenseStats(context, context.currentMove, context.defendingPokemon, context.attackingPokemon);
		}

		if (context.aiPlayerTwo && context.playerTwo == context.defendingPlayer)
		{
			context.aiPlayerTwo->UpdateEnemyOffenseStats(context, context.currentMove, context.defendingPokemon, context.attackingPokemon);
		}

		if (context.aiPlayerOne)
		{
			context.aiPlayerOne->UpdateEnemySpeedStats(context, context.playerOneCurrentMove, context.playerTwoCurrentMove, context.playerOneCurrentPokemon, context.playerTwoCurrentPokemon);
		}

		if (context.aiPlayerTwo)
		{
			context.aiPlayerTwo->UpdateEnemySpeedStats(context, context.playerTwoCurrentMove, context.playerOneCurrentMove, context.playerTwoCurrentPokemon, context.playerOneCurrentPokemon);
		}
	}

	void RefineEnemyModelSecondTurn(BattleContext& context)
	{
		if (context.aiPlayerOne && context.playerOne == context.attackingPlayer)
		{
			context.aiPlayerOne->UpdateEnemyHPandDefenseStats(context, context.defendingPlayer, context.currentMove, context.attackingPokemon, context.defendingPokemon);
		}

		if (context.aiPlayerTwo && context.playerTwo == context.attackingPlayer)
		{
			context.aiPlayerTwo->UpdateEnemyHPandDefenseStats(context, context.defendingPlayer, context.currentMove, context.attackingPokemon, context.defendingPokemon);
		}

		if (context.aiPlayerOne && context.playerOne == context.defendingPlayer)
		{
			context.aiPlayerOne->UpdateEnemyOffenseStats(context, context.currentMove, context.defendingPokemon, context.attackingPokemon);
		}

		if (context.aiPlayerTwo && context.playerTwo == context.defendingPlayer)
		{
			context.aiPlayerTwo->UpdateEnemyOffenseStats(context, context.currentMove, context.defendingPokemon, context.attackingPokemon);
		}
	}
}