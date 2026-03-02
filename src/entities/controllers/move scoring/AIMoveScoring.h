#pragma once

#include <vector>

struct BattleContext;
struct ScoringResults;
class Player;
class BattlePokemon;
struct pokemonMove;
class BattleCalculations;
class RandomEngine;

namespace AIMoveScoring
{
	pokemonMove* GetWinningMove(Player& self, Player& targetPlayer, BattlePokemon& selfMon, BattlePokemon& targetMon, RandomEngine& rng);
	pokemonMove* EvaluateScoredMoves(const std::vector<ScoringResults>& results, RandomEngine& rng);
	ScoringResults RunScoringRoutine(ScoringResults& results, Player& self, Player& targetPlayer, pokemonMove& move, BattlePokemon& selfMon, BattlePokemon& targetMon);
	int SwitchDamageScoringRoutine(Player& self, Player& targetPlayer, pokemonMove& move, BattlePokemon& selfMon, BattlePokemon& targetMon);
}