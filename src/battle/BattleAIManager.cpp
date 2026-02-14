#include <iostream>
#include <limits>

#include "BattleContext.h"
#include "../entities/Player.h"
#include "../entities/controllers/AIController.h"

#include "BattleAIManager.h"

namespace BattleAIProcedures
{
	void InitAIPlayers(BattleContext& context)
	{
		for (auto player : context.vec_aiPlayers)
		{
			player->GetAIController().OnBattleStart(*player, context);
		}
	}

	void UpdateEnemyActivePokemon(BattleContext& context)
	{
		for (auto player : context.vec_aiPlayers)
		{
			player->GetAIController().OnActivePokemonChanged(context);
		}
	}

	void RefineEnemyModelFirstTurn(BattleContext& context)
	{
		for (auto player : context.vec_aiPlayers)
		{
			player->GetAIController().OnMoveResolved(context);
			player->GetAIController().OnFirstMoveResolved(context);
		}
	}

	void RefineEnemyModelSecondTurn(BattleContext& context)
	{
		for (auto player : context.vec_aiPlayers)
		{
			player->GetAIController().OnMoveResolved(context);
		}
	}
}