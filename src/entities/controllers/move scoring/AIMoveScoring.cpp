#include <algorithm>

#include "AIMoveScoring.h"

#include "ScoringResultsStruct.h"
#include "BasicScoring.h"
#include "../../../data/StringToTypes.h"
#include "../../Player.h"
#include "../../../battle/BattleCalculations.h"
#include "../../../battle/RandomEngine.h"
#include "../AIController.h"

namespace AIMoveScoring
{
	pokemonMove* GetWinningMove(Player& self, Player& targetPlayer, BattlePokemon& selfMon, BattlePokemon& targetMon, RandomEngine& rng)
	{
		if (selfMon.CheckPPCountForStruggle())
		{
			return &selfMon.Struggle();
		}

		std::vector<ScoringResults> results{};
		results.reserve(4);

		auto& moveArray = selfMon.GetMoveArray();

		size_t index{};
		for (size_t i = 0; i < 4; ++i)
		{
			if (moveArray[i].GetMovePointer() == nullptr || moveArray[i].m_currentPP == 0)
			{
				continue;
			}

			results.emplace_back();
			results[index].move = &moveArray[i];
			++index;
		}

		for (size_t i = 0; i < results.size(); ++i)
		{
			results[i] = RunScoringRoutine(results[i], self, targetPlayer, *results[i].move, selfMon, targetMon);
		}

		//pokemonMove* winningMove = EvaluateScoredMoves(results, rng);
		//return winningMove;

		return EvaluateScoredMoves(results, rng);
	}

	pokemonMove* EvaluateScoredMoves(const std::vector<ScoringResults>& results, RandomEngine& rng)
	{
		int highestScore = INT_MIN;
		for (const auto& result : results)
		{
			highestScore = std::max(highestScore, result.score);
		}

		std::vector<ScoringResults> topScores{};
		topScores.reserve(results.size());
		for (const auto& result : results)
		{
			if (result.score == highestScore)
			{
				topScores.emplace_back(result);
			}
		}

		int highestDamage = INT_MIN;
		for (const auto& result : topScores)
		{
			highestDamage = std::max(highestDamage, result.damage);
		}

		std::vector<ScoringResults> filtered;
		filtered.reserve(topScores.size());

		for (const auto& result : topScores)
		{
			if (result.damage == 0)
			{
				filtered.emplace_back(result);
			}

			else if (result.damage == highestDamage)
			{
				filtered.emplace_back(result);
			}
		}
		
		std::uniform_int_distribution<size_t> dist(0, filtered.size() - 1);
		return filtered[dist(rng.GetGenerator())].move;
	}

	ScoringResults RunScoringRoutine(ScoringResults& results, Player& self, Player& targetPlayer, pokemonMove& move, BattlePokemon& selfMon, BattlePokemon& targetMon)
	{
		if (move.GetCategoryEnum() == Category::Status)
		{
			results.score += BasicScoring::BaseStatusScoring(self, targetPlayer, move, selfMon, targetMon);
		}
		else
		{
			results.score += BasicScoring::BaseDamageScoring(self, targetPlayer, move, selfMon, targetMon);
			results.damage = self.GetAIController().AICalculateDamage(move, targetPlayer, selfMon, targetMon);
		}

		return results;
	}

	int SwitchDamageScoringRoutine(Player& self, Player& targetPlayer, pokemonMove& move, BattlePokemon& selfMon, BattlePokemon& targetMon)
	{
		return self.GetAIController().AICalculateDamage(move, targetPlayer, selfMon, targetMon);
	}
}