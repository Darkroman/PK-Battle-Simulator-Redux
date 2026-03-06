#include <algorithm>

#include "MediumMoveScoring.h"

#include "ScoringResultsStruct.h"
#include "AIMoveClassifier.h"
#include "../../../data/StringToTypes.h"
#include "../../Player.h"
#include "../AIController.h"

namespace MediumMoveScoring
{
	void EvaluateBestDamageMove(std::vector<ScoringResults>& results, BattlePokemon& targetMon)
	{
		std::vector<ScoringResults*> damagingMoves{};
		damagingMoves.reserve(results.size());

		int highestDamage = INT_MIN;
		for (auto& result : results)
		{
			if (result.damage > 0)
			{
				damagingMoves.emplace_back(&result);
				if (!(result.tag == AIScoreTag::RechargeMove)) // don't consider recharge moves into highest damage calculation
				{
					highestDamage = std::max(highestDamage, result.damage);
				}

				if (result.tag == AIScoreTag::ChargeMove || result.tag == AIScoreTag::SkyAttack) // halve the damage if two turn move and is not recharge move
				{
					result.damage /= 2;
				}
			}
		}

		for (auto* result : damagingMoves)
		{
			auto tag = result->tag;

			bool isRechargeMove = tag == AIScoreTag::RechargeMove ? true : false;

			bool canKill{ false };
			if (result->damage >= targetMon.GetCurrentHP())
			{
				canKill = true;
			}

			// If move is enough to kill target
			if (canKill)
			{
				if (result->move->GetMoveEffectEnum() == MoveEffect::Explosion)
				{
					// do nothing
				}
				else if (result->move->GetPriority() >= 1) // If move is priority +1 or more (ala quick attack)
				{
					result->score += 6;
				}
				else
				{
					result->score += 4; // add +4 to move score if above don't qualify
				}
			}

			// if move is not the highest damaging move, subtract score by 1 (whether it kills or not)
			if (result->damage < highestDamage)
			{
				result->score -= 1;
			}

			// This makes it so the recharge move should be the top scored move ONLY if other moves aren't guaranteed to kill at max damage
			if (isRechargeMove)
			{
				result->score -= 2;
			}

			// if self-fainting move
			if (result->move->GetMoveEffectEnum() == MoveEffect::Explosion)
			{
				result->score -= 2;
			}
		}
	}
}