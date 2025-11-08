#pragma once

struct BattleContext;

namespace BattleAIProcedures
{
	void InitAIPlayers(BattleContext& context);
	void UpdateEnemyActivePokemon(BattleContext& context);
	void RefineEnemyModelFirstTurn(BattleContext& context);
	void RefineEnemyModelSecondTurn(BattleContext& context);
}