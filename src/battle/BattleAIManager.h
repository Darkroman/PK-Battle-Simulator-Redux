#pragma once

struct BattleContext;

namespace BattleAIProcedures
{
	void InitAIPlayers(BattleContext& context);
	void UpdateEnemyActivePokemon(const BattleContext& context);
	void RefineEnemyModelFirstTurn(const BattleContext& context);
	void RefineEnemyModelSecondTurn(const BattleContext& context);
}