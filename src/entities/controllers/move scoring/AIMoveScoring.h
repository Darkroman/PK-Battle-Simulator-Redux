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
	pokemonMove* GetWinningMove(const Player& self, const Player& targetPlayer, BattlePokemon& selfMon, const BattlePokemon& targetMon, RandomEngine& rng);
	pokemonMove* EvaluateScoredMoves(const std::vector<ScoringResults>& results, RandomEngine& rng);
	ScoringResults RunScoringRoutine(ScoringResults& results, const Player& self, const Player& targetPlayer, const pokemonMove& move, const BattlePokemon& selfMon, const BattlePokemon& targetMon);
	unsigned int SwitchDamageScoringRoutine(const Player& self, const Player& targetPlayer, const pokemonMove& move, const BattlePokemon& selfMon, const BattlePokemon& targetMon);
	unsigned int CalculateSpeed(const BattlePokemon&);
}