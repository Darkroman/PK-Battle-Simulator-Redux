#include <algorithm>

#include "AIMoveScoring.h"

#include "ScoringResultsStruct.h"
#include "BasicScoring.h"
#include "MediumMoveScoring.h"

#include "AIMoveClassifier.h"
#include "../../../data/StringToTypes.h"
#include "../../Player.h"
#include "../../../battle/RandomEngine.h"
#include "../AIController.h"

namespace AIMoveScoring
{
	pokemonMove* GetWinningMove(const Player& self, const Player& targetPlayer, BattlePokemon& selfMon, const BattlePokemon& targetMon, RandomEngine& rng)
	{
		std::vector<ScoringResults> results{};
		results.reserve(4);

		const auto moveArray = selfMon.GetMoveArray();

		size_t index{};
		for (size_t i = 0; i < 4; ++i)
		{
			if (!moveArray[i].IsActive())
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

	ScoringResults RunScoringRoutine(ScoringResults& results, const Player& self, const Player& targetPlayer, const pokemonMove& move, const BattlePokemon& selfMon, const BattlePokemon& targetMon)
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

	int SwitchDamageScoringRoutine(const Player& self, const Player& targetPlayer, const pokemonMove& move, const BattlePokemon& selfMon, const BattlePokemon& targetMon)
	{
		return self.GetAIController().AICalculateDamage(move, targetPlayer, selfMon, targetMon);
	}

	int CalculateSpeed(const BattlePokemon& pokemon)
	{
		auto GetStageRatio = [](int stage) -> std::pair<int, int>
			{
				if (stage < 0)
				{
					return { 2, -stage + 2 };
				}

				if (stage == 0)
				{
					return { 2, 2 };
				}

				return { 2 + stage, 2 };
			};

		auto [numerator, denominator] = GetStageRatio(pokemon.GetSpeedStage());

		int speed = pokemon.GetSpeed() * numerator / denominator;

		if (pokemon.GetStatus() == Status::Paralyzed)
		{
			speed /= 2;
		}

		return speed;
	}
}