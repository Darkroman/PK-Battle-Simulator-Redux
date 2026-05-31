#include "AISwitchLogic.h"

#include "../../../battle/RandomEngine.h"
#include "../move scoring/AIMoveScoring.h"
#include "../../Player.h"
#include "../AIController.h"
#include "../../../data/StringToTypes.h"

namespace AISwitchLogic
{
	bool WantsToSwitch(const Player& self, const Player& targetPlayer, const BattlePokemon& selfMon, const BattlePokemon& targetMon, RandomEngine& rng)
	{
		// if on last pokemon, don't switch
		if (self.GetPokemonLeft() < 2)
		{
			return false;
		}

		if (selfMon.WillPerformStruggle())
		{
			int willPerformStruggleCount{ 0 };
			for (const auto& pokemon : self.GetBeltArray())
			{
				if (pokemon.IsFainted() || &pokemon == &selfMon)
				{
					continue;
				}

				if (pokemon.WillPerformStruggle())
				{
					++willPerformStruggleCount;
				}
			}

			if (willPerformStruggleCount < self.GetPokemonLeft() - 1)
			{
				return true;
			}
		}

		unsigned int highestDamage{ 0 };
		const pokemonMove* highestDamagingMove{};

		bool canKO{};
		bool priorityCanKO{};

		for (const auto& move : selfMon.GetMoveArray())
		{
			if (move.IsActive())
			{
				unsigned int damage = AIMoveScoring::SwitchDamageScoringRoutine(self, targetPlayer, move, selfMon, targetMon);
				
				if (damage > highestDamage)
				{
					highestDamage = damage;
					highestDamagingMove = &move;
				}

				if (highestDamagingMove && highestDamage >= targetMon.GetCurrentHP())
				{
					canKO = true;
				}

				if (move.GetPriority() > 0 && damage >= targetMon.GetCurrentHP())
				{
					priorityCanKO = true;
				}
			}
		}

		// if all damaging moves are immune, switch
		if (highestDamage == 0)
		{
			return true;
		}

		unsigned int selfMonSpeed = AIMoveScoring::CalculateSpeed(selfMon);
		unsigned int targetMonSpeed = AIMoveScoring::CalculateSpeed(targetMon);

		bool isFaster{ selfMonSpeed >= targetMonSpeed };

		// if selfMon is faster and has move that can one-hit KO targetMon, don't switch
		if (isFaster && canKO)
		{
			return false;
		}

		// if selfMon has priority move that can KO targetMon, don't switch (and self is hard difficulty)
		if (self.GetAIController().GetDifficulty() == Difficulty::Hard && priorityCanKO)
		{
			return false;
		}

		bool targetCantAttack = (targetMon.GetStatus() == Status::Frozen || targetMon.GetStatus() == Status::Sleeping);

		// if targetMon is asleep or frozen, don't switch
		if (targetCantAttack)
		{
			return false;
		}

		auto targetMonLastUsedMove = targetMon.GetLastUsedMove();
		bool lastMoveCanKO{};

		if (targetMonLastUsedMove)
		{
			unsigned int damage = AIMoveScoring::SwitchDamageScoringRoutine(targetPlayer, self, *targetMonLastUsedMove, targetMon, selfMon);

			if (damage >= selfMon.GetCurrentHP())
			{
				lastMoveCanKO = true;
			}
		}

		// if selfMon is slower and last used move by targetMon can KO, switch
		if (targetMonLastUsedMove && !isFaster && lastMoveCanKO)
		{
			return true;
		}

		if (self.GetAIController().GetDifficulty() == Difficulty::Hard)
		{
			bool selfTypingDisadvantageous{ PokemonTypeEffectiveness(self, targetMon, selfMon) > 4096 };

			// if selfMon is slower and typing is disadvantageous, switch
			if (!isFaster && selfTypingDisadvantageous)
			{
				return true;
			}

			bool isNotVeryEffective{ IsMoveNotVeryEffective(self, *highestDamagingMove, targetMon) };

			// if selfMon's typing is disadvantageous and highest damaging move is not very effective and cannot KO, switch
			if (selfTypingDisadvantageous && isNotVeryEffective && !canKO)
			{
				return true;
			}
			
			const auto& observedMoves = self.GetAIController().GetObservedMoves();
			std::array<const pokemonMove*, 4> observedDamagingMoves{};
			size_t observedCount{};

			bool targetMonHasKOMove{ false };

			for (const auto& observedMove : observedMoves)
			{
				if (observedMove != nullptr)
				{
					if (observedMove->GetCategoryEnum() != Category::Status && observedMove->IsActive())
					{
						observedDamagingMoves[observedCount] = observedMove;
						++observedCount;
					}
				}
				else
				{
					break;
				}

				unsigned int damageToSelf = AIMoveScoring::SwitchDamageScoringRoutine(targetPlayer, self, *observedMove, targetMon, selfMon);

				if (damageToSelf >= selfMon.GetCurrentHP())
				{
					targetMonHasKOMove = true;
					break;
				}
			}

			std::span<const pokemonMove*> validObservedMoves{ observedDamagingMoves.data(), observedCount };

			// If no observed damaging moves found, return early with no switch
			if (validObservedMoves.empty())
			{
				return false;
			}
			
			// If selfMon is slower, and targetMon's highest damaging observed move will KO selfMon, switch
			if (!isFaster && targetMonHasKOMove)
			{
				return true;
			}
		}
		
		return false;
	}

	BattlePokemon* ChooseSwitch(Player& self, const Player& targetPlayer, const BattlePokemon& selfMon, const BattlePokemon& targetMon)
	{
		struct CandidatePokemon
		{
			BattlePokemon* pokemon{};
			unsigned int highestDamageMove{};
			unsigned int typeEffectiveness{};
			bool isFaster{};
			bool canKO{};
			bool canSurviveTwoHits{};
			bool canSurviveOneHit{};
		};

		std::array<CandidatePokemon, 6> candidatePkmnPool{};

		auto canAttack = [&](const BattlePokemon& pokemon) -> bool { return !(pokemon.GetStatus() == Status::Frozen || pokemon.GetStatus() == Status::Sleeping); };

		size_t count{};
		for (auto& pokemon : self.GetBeltArray())
		{
			if (pokemon.HasPokemon() && !pokemon.IsFainted() && canAttack(pokemon))
			{
				candidatePkmnPool[count].pokemon = &pokemon;
				++count;
			}
		}

		std::span<CandidatePokemon> candidatePkmn{ candidatePkmnPool.data(), count };

		const auto& observedMoves = self.GetAIController().GetObservedMoves();
		std::array<const pokemonMove*, 4> observedDamagingMoves{};
		size_t observedCount{};

		for (const auto& observedMove : observedMoves)
		{
			if (observedMove == nullptr || !observedMove->IsActive())
			{
				break;
			}
			else if (observedMove->GetCategoryEnum() != Category::Status)
			{
				observedDamagingMoves[observedCount] = observedMove;
				++observedCount;
			}
		}

		std::span<const pokemonMove*> validObservedMoves{ observedDamagingMoves.data(), observedCount };

		unsigned int highestDamageToSelf{ 0 };
		const pokemonMove* mostLikelyMove{};
		if (!validObservedMoves.empty())
		{
			mostLikelyMove = validObservedMoves.front();
			// Evaluate most likely observed move AI will do against selfMon
			for (const auto& observedMove : validObservedMoves)
			{
				unsigned int damageToSelf = AIMoveScoring::SwitchDamageScoringRoutine(targetPlayer, self, *observedMove, targetMon, selfMon);

				if (damageToSelf > highestDamageToSelf)
				{
					highestDamageToSelf = damageToSelf;
					mostLikelyMove = observedMove;
				}
			}
		}

		for (auto& candidate : candidatePkmn)
		{
			for (const auto& move : candidate.pokemon->GetMoveArray())
			{
				if (move.IsActive() && move.GetCategoryEnum() != Category::Status)
				{
					unsigned int damage = AIMoveScoring::SwitchDamageScoringRoutine(self, targetPlayer, move, *candidate.pokemon, targetMon);

					if (damage > candidate.highestDamageMove)
					{
						candidate.highestDamageMove = damage;
					}
				}
			}

			candidate.typeEffectiveness = PokemonTypeEffectiveness(self, targetMon, *candidate.pokemon);

			unsigned int candidateMonSpeed = AIMoveScoring::CalculateSpeed(*candidate.pokemon);
			unsigned int targetMonSpeed = AIMoveScoring::CalculateSpeed(targetMon);

			candidate.isFaster = candidateMonSpeed >= targetMonSpeed;

			candidate.canKO = candidate.highestDamageMove >= targetMon.GetCurrentHP();

			if (!validObservedMoves.empty())
			{
				unsigned int firstAttackVsCandidate = AIMoveScoring::SwitchDamageScoringRoutine(targetPlayer, self, *mostLikelyMove, targetMon, *candidate.pokemon);
				unsigned int secondAttackVsCandidate{ 0 };

				// Evaluate most likely observed move AI will do against candidateMon
				for (const auto& observedMove : validObservedMoves)
				{
					unsigned int damageToCandidate = AIMoveScoring::SwitchDamageScoringRoutine(targetPlayer, self, *observedMove, targetMon, *candidate.pokemon);
					if (damageToCandidate > secondAttackVsCandidate)
					{
						secondAttackVsCandidate = damageToCandidate;
					}
				}

				candidate.canSurviveOneHit = firstAttackVsCandidate < candidate.pokemon->GetCurrentHP();

				candidate.canSurviveTwoHits = (firstAttackVsCandidate + secondAttackVsCandidate) < candidate.pokemon->GetCurrentHP();
			}
		}

		std::ranges::sort(candidatePkmn, [](const CandidatePokemon& a, const CandidatePokemon& b)
			{
				return std::tie(a.typeEffectiveness, b.highestDamageMove) <
					std::tie(b.typeEffectiveness, a.highestDamageMove);
			});

		auto it = candidatePkmn.end();

		if (!validObservedMoves.empty())
		{
			it = std::ranges::find_if(candidatePkmn, [](const CandidatePokemon& cp)
				{
					return cp.canKO && cp.isFaster && cp.canSurviveOneHit;
				});
			

			if (it == candidatePkmn.end())
			{
				it = std::ranges::find_if(candidatePkmn, [](const CandidatePokemon& cp)
					{
						return cp.canKO && cp.canSurviveTwoHits;
					});
			}
		}
		else
		{
			it = std::ranges::find_if(candidatePkmn, [](const CandidatePokemon& cp)
				{
					return cp.canKO && cp.typeEffectiveness <= 2048;
				});

			if (it == candidatePkmn.end())
			{
				it = std::ranges::find_if(candidatePkmn, [](const CandidatePokemon& cp)
					{
						return cp.canKO && cp.isFaster && cp.pokemon->GetCurrentHP() == cp.pokemon->GetMaxHP();
					});
			}

			if (it == candidatePkmn.end())
			{
				it = std::ranges::find_if(candidatePkmn, [](const CandidatePokemon& cp)
					{
						return cp.typeEffectiveness <= 2048;
					});
			}
		}
		BattlePokemon* chosenPokemon{};

		if (it == candidatePkmn.end())
		{
			return nullptr;
		}
		else
		{
			chosenPokemon = it->pokemon;
		}

		if (chosenPokemon == &selfMon)
		{
			return nullptr;
		}
		else
		{
			self.SetPokemonToSwitchTo(chosenPokemon);
			return chosenPokemon;
		}
	}

	BattlePokemon* ChoosePostKOSwitch(Player& self, const Player& targetPlayer, const BattlePokemon& selfMon, const BattlePokemon& targetMon)
	{
		struct CandidatePokemon
		{
			BattlePokemon* pokemon{};
			unsigned int highestDamageMove{};
			unsigned int typeEffectiveness{};
			bool isFaster{};
			bool canKO{};
			bool canSurviveOneHit{};
		};

		std::array<CandidatePokemon, 6> candidatePkmnPool{};

		auto canAttack = [&](const BattlePokemon& pokemon) -> bool { return !(pokemon.GetStatus() == Status::Frozen || pokemon.GetStatus() == Status::Sleeping); };

		size_t count{};
		for (auto& pokemon : self.GetBeltArray())
		{
			if (pokemon.HasPokemon() && !pokemon.IsFainted() && &pokemon != &selfMon)
			{
				candidatePkmnPool[count].pokemon = &pokemon;
				++count;
			}
		}

		std::span<CandidatePokemon> candidatePkmn{ candidatePkmnPool.data(), count };

		const auto& observedMoves = self.GetAIController().GetObservedMoves();
		std::array<const pokemonMove*, 4> observedDamagingMoves{};
		size_t observedCount{};

		for (const auto& observedMove : observedMoves)
		{
			if (observedMove == nullptr || !observedMove->IsActive())
			{
				break;
			}
			else if (observedMove->GetCategoryEnum() != Category::Status)
			{
				observedDamagingMoves[observedCount] = observedMove;
				++observedCount;
			}
		}

		std::span<const pokemonMove*> validObservedMoves{ observedDamagingMoves.data(), observedCount };

		for (auto& candidate : candidatePkmn)
		{
			for (const auto& move : candidate.pokemon->GetMoveArray())
			{
				if (move.IsActive() && move.GetCategoryEnum() != Category::Status)
				{
					unsigned int damage = AIMoveScoring::SwitchDamageScoringRoutine(self, targetPlayer, move, *candidate.pokemon, targetMon);

					if (damage > candidate.highestDamageMove)
					{
						candidate.highestDamageMove = damage;
					}
				}
			}

			candidate.typeEffectiveness = PokemonTypeEffectiveness(self, targetMon, *candidate.pokemon);

			unsigned int candidateMonSpeed = AIMoveScoring::CalculateSpeed(*candidate.pokemon);
			unsigned int targetMonSpeed = AIMoveScoring::CalculateSpeed(targetMon);

			candidate.isFaster = candidateMonSpeed >= targetMonSpeed;

			candidate.canKO = candidate.highestDamageMove >= targetMon.GetCurrentHP();

			if (!validObservedMoves.empty())
			{
				unsigned int highestDamageToCandidate{ 0 };
				for (const auto& observedMove : validObservedMoves)
				{
					unsigned int damageToCandidate = AIMoveScoring::SwitchDamageScoringRoutine(targetPlayer, self, *observedMove, targetMon, *candidate.pokemon);

					if (damageToCandidate > highestDamageToCandidate)
					{
						highestDamageToCandidate = damageToCandidate;
					}
				}

				candidate.canSurviveOneHit = highestDamageToCandidate < candidate.pokemon->GetCurrentHP();
			}
		}

		std::ranges::sort(candidatePkmn, [](const CandidatePokemon& a, const CandidatePokemon& b)
			{
				return std::tie(a.typeEffectiveness, b.highestDamageMove) <
					std::tie(b.typeEffectiveness, a.highestDamageMove);
			});

		auto it = candidatePkmn.end();

		if (!validObservedMoves.empty())
		{
			it = std::ranges::find_if(candidatePkmn, [](const CandidatePokemon& cp)
				{
					return cp.canKO && cp.isFaster;
				});

			if (it == candidatePkmn.end())
			{
				it = std::ranges::find_if(candidatePkmn, [](const CandidatePokemon& cp)
					{
						return cp.canKO && cp.canSurviveOneHit;
					});
			}

			if (it == candidatePkmn.end())
			{
				it = std::ranges::find_if(candidatePkmn, [](const CandidatePokemon& cp)
					{
						return cp.isFaster && cp.canSurviveOneHit;
					});
			}

			if (it == candidatePkmn.end())
			{
				it = std::ranges::find_if(candidatePkmn, [](const CandidatePokemon& cp)
					{
						return cp.canSurviveOneHit;
					});
			}

			if (it == candidatePkmn.end())
			{
				it = std::ranges::find_if(candidatePkmn, [](const CandidatePokemon& cp)
					{
						return cp.isFaster;
					});
			}
		}
		else
		{
			it = std::ranges::find_if(candidatePkmn, [](const CandidatePokemon& cp)
				{
					return cp.canKO && cp.isFaster;
				});

			if (it == candidatePkmn.end())
			{
				it = std::ranges::find_if(candidatePkmn, [](const CandidatePokemon& cp)
					{
						return cp.isFaster;
					});
			}
		}
		BattlePokemon* chosenPokemon{};

		if (it == candidatePkmn.end())
		{
			chosenPokemon = candidatePkmn.front().pokemon;
		}
		else
		{
			chosenPokemon = it->pokemon;
		}

		self.SetPokemonToSwitchTo(chosenPokemon);
		return chosenPokemon;
	}

	bool IsMoveSuperEffective(const Player& self, const pokemonMove& move, const BattlePokemon& pokemon)
	{
		auto MoveEffectiveness = [&](const pokemonMove& move, const BattlePokemon& targetMon) {
			return self.GetAIController().AICalculateMoveTypeEffectiveness(move, targetMon);
			};

		return MoveEffectiveness(move, pokemon) > 4096 && move.GetPower() > 0 && move.m_currentPP > 0;
	}

	bool IsMoveAtMostEffective(const Player& self, const pokemonMove& move, const BattlePokemon& pokemon)
	{
		auto MoveEffectiveness = [&](const pokemonMove& move, const BattlePokemon& targetMon) {
			return self.GetAIController().AICalculateMoveTypeEffectiveness(move, targetMon);
			};

		return MoveEffectiveness(move, pokemon) <= 4096 && move.GetPower() > 0 && move.m_currentPP > 0;
	}

	bool IsMoveAtLeastEffective(const Player& self, const pokemonMove& move, const BattlePokemon& pokemon)
	{
		auto MoveEffectiveness = [&](const pokemonMove& move, const BattlePokemon& targetMon) {
			return self.GetAIController().AICalculateMoveTypeEffectiveness(move, targetMon);
			};

		return MoveEffectiveness(move, pokemon) >= 4096 && move.GetPower() > 0 && move.m_currentPP > 0;
	}

	bool IsMoveNotEffective(const Player& self, const pokemonMove& move, const BattlePokemon& pokemon)
	{
		auto MoveEffectiveness = [&](const pokemonMove& move, const BattlePokemon& targetMon) {
			return self.GetAIController().AICalculateMoveTypeEffectiveness(move, targetMon);
			};

		return MoveEffectiveness(move, pokemon) == 0 && move.GetPower() > 0 && move.m_currentPP > 0;
	}

	bool IsMoveNotVeryEffective(const Player& self, const pokemonMove& move, const BattlePokemon& pokemon)
	{
		auto MoveEffectiveness = [&](const pokemonMove& move, const BattlePokemon& targetMon) {
			return self.GetAIController().AICalculateMoveTypeEffectiveness(move, targetMon);
			};

		return MoveEffectiveness(move, pokemon) <= 2048 && move.GetPower() > 0 && move.m_currentPP > 0;
	}

	bool IsStatusMoveEffective(const Player& self, const pokemonMove& move, const BattlePokemon& pokemon)
	{
		auto MoveEffectiveness = [&](const pokemonMove& move, const BattlePokemon& targetMon) {
			return self.GetAIController().AICalculateMoveTypeEffectiveness(move, targetMon);
			};

		return MoveEffectiveness(move, pokemon) <= 2048 && move.GetPower() > 0 && move.m_currentPP > 0;
	}

	unsigned int PokemonTypeEffectiveness(const Player& self, const BattlePokemon& source, const BattlePokemon& target)
	{
		return self.GetAIController().AICalculatePokemonTypeEffectiveness(source, target);
	}
}