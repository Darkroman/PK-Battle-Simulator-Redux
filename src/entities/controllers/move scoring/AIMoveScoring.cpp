#include <algorithm>

#include "AIMoveScoring.h"

#include "ScoringResultsStruct.h"
#include "BasicScoring.h"
#include "MediumMoveScoring.h"

#include "AIMoveClassifier.h"
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
			results[index].tag = AIMoveClassifier::Classify(*results[index].move);

			++index;
		}

		for (auto& result : results)
		{
			result = RunScoringRoutine(result, self, targetPlayer, *result.move, selfMon, targetMon);
		}

		if (self.GetAIController().GetDifficulty() >= Difficulty::Medium)
		{
			MediumMoveScoring::EvaluateBestDamageMove(results, targetMon);
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
		
		std::uniform_int_distribution<size_t> dist(0, topScores.size() - 1);
		return topScores[dist(rng.GetGenerator())].move;
	}

	ScoringResults RunScoringRoutine(ScoringResults& results, Player& self, Player& targetPlayer, pokemonMove& move, BattlePokemon& selfMon, BattlePokemon& targetMon)
	{
		if (move.GetCategoryEnum() == Category::Status)
		{
			results.score += BasicScoring::BaseStatusScoring(results, self, targetPlayer, move, selfMon, targetMon);
		}
		else
		{
			results.score += BasicScoring::BaseDamageScoring(results, self, targetPlayer, move, selfMon, targetMon);
			results.damage = self.GetAIController().AICalculateDamage(move, targetPlayer, selfMon, targetMon);
		}

		return results;
	}

	int SwitchDamageScoringRoutine(Player& self, Player& targetPlayer, pokemonMove& move, BattlePokemon& selfMon, BattlePokemon& targetMon)
	{
		return self.GetAIController().AICalculateDamage(move, targetPlayer, selfMon, targetMon);
	}
}