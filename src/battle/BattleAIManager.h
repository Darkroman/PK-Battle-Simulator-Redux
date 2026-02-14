#pragma once

struct BattleContext;

namespace BattleAIProcedures
{
	void InitAIPlayers(BattleContext& context);
	void UpdateEnemyActivePokemon(BattleContext& context);
	void OnMoveResolved(BattleContext&);
	void RefineEnemyModelFirstTurn(BattleContext& context);
	void RefineEnemyModelSecondTurn(BattleContext& context);
}