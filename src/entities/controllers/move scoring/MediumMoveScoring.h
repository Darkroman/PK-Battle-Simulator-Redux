#pragma once

#include <vector>

class Player;
class BattlePokemon;
struct pokemonMove;
class BattleCalculations;
class RandomEngine;
struct ScoringResults;

namespace MediumMoveScoring
{
	void EvaluateBestDamageMove(std::vector<ScoringResults>& results, BattlePokemon& targetMon);
}