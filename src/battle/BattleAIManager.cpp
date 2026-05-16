#include "BattleAIManager.h"

#include "BattleContext.h"
#include "../entities/Player.h"
#include "../entities/controllers/AIController.h"

namespace BattleAIProcedures
{
	void InitAIPlayers(BattleContext& context)
	{
		for (const auto& player : context.vec_aiPlayers)
		{
			player->GetAIController().OnBattleStart(*player, context);
			player->GetAIController().OnActivePokemonChanged(context);
		}
	}

	void UpdateEnemyActivePokemon(const BattleContext& context)
	{
		for (const auto& player : context.vec_aiPlayers)
		{
			player->GetAIController().OnActivePokemonChanged(context);
		}
	}

	void RefineEnemyModelFirstTurn(const BattleContext& context)
	{
		for (const auto& player : context.vec_aiPlayers)
		{
			player->GetAIController().OnMoveResolved(context);
		}
	}

	void RefineEnemyModelSecondTurn(const BattleContext& context)
	{
		for (const auto& player : context.vec_aiPlayers)
		{
			player->GetAIController().OnMoveResolved(context);
		}
	}
}