#include <iostream>

#include "AISwitchLogic.h"

#include "../move scoring/AIMoveScoring.h"
#include "../../Player.h"
#include "../AIController.h"
#include "../../../data/StringToTypes.h"

namespace AISwitchLogic
{
	bool WantsToSwitch(const Player& self, const Player& targetPlayer, const BattlePokemon& selfMon, const BattlePokemon& targetMon)
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

		int selfMonSpeed = AIMoveScoring::CalculateSpeed(selfMon);
		int targetMonSpeed = AIMoveScoring::CalculateSpeed(targetMon);

		bool isFaster = selfMonSpeed >= targetMonSpeed ? true : false;
		bool canKO{};

		bool priorityCanKO{};

		bool targetCantAttack = (targetMon.GetStatus() == Status::Frozen || targetMon.GetStatus() == Status::Sleeping) ? true : false;
		int typeEffectiveness{ PokemonTypeEffectiveness(self, targetMon, selfMon) };
		bool selfTypingDisadvantageous = typeEffectiveness > 4096 ? true : false;
		bool selfTypingSDisadvantageous = PokemonTypeEffectiveness(self, targetMon, selfMon) > 8192 ? true : false;

		auto targetMonLastUsedMove = targetMon.GetLastUsedMove();
		bool lastMoveSE{};
		bool lastMoveCanKO{};

		bool hasEffectiveMove{};

		if (targetMonLastUsedMove)
		{
			lastMoveSE = IsMoveSuperEffective(self, *targetMonLastUsedMove, selfMon);
			
			int damage = AIMoveScoring::SwitchDamageScoringRoutine(targetPlayer, self, *targetMonLastUsedMove, targetMon, selfMon);

			if (damage >= selfMon.GetCurrentHP())
			{
				lastMoveCanKO = true;
			}
		}

		int highestDamage{ 0 };
		for (const auto& move : selfMon.GetMoveArray())
		{
			if (move.IsActive())
			{
				int damage = AIMoveScoring::SwitchDamageScoringRoutine(self, targetPlayer, move, selfMon, targetMon);
				if (damage >= targetMon.GetCurrentHP())
				{
					if (damage > highestDamage)
					{
						highestDamage = damage;
						canKO = true;
					}

					if (move.GetPriority() > 0)
					{
						priorityCanKO = true;
					}
				}

				if (IsMoveAtLeastEffective(self, move, targetMon) && move.GetCategoryEnum() != Category::Status)
				{
					hasEffectiveMove = true;
				}
			}
		}

		// if selfMon is faster and has move that can one-hit KO targetMon, don't switch
		if (isFaster && canKO)
		{
			return false;
		}

		// if selfMon has priority move that can KO targetMon, don't switch
		if (priorityCanKO)
		{
			return false;
		}

		// if targetMon is asleep or frozen, don't switch
		if (targetCantAttack)
		{
			return false;
		}

		// if selfMon is slower and last used move by targetMon was super effective and it can KO, switch
		if (!isFaster && lastMoveSE && lastMoveCanKO)
		{
			return true;
		}

		// if selfMon's typing is super disadvantageous, switch
		if (selfTypingSDisadvantageous)
		{
			return true;
		}

		if (self.GetAIController().GetDifficulty() == Difficulty::Hard)
		{
			// if selfMon's typing is disadvantageous and is slower than targetMon, switch
			if (selfTypingDisadvantageous && !isFaster)
			{
				return true;
			}

			// if selfMon's typing is disadvantageous and has no neutral effective moves, switch
			if (selfTypingDisadvantageous && !hasEffectiveMove)
			{
				return true;
			}

			const auto& observedMoves = self.GetAIController().GetObservedMoves();
			std::vector<const pokemonMove*> observedDamagingMoves{};
			observedDamagingMoves.reserve(4);

			bool targetMonHasKOMove{ false };

			int highestDamageToSelf{ 0 };
			for (const auto& observedMove : observedMoves)
			{
				if (observedMove != nullptr)
				{
					if (observedMove->GetCategoryEnum() != Category::Status)
					{
						observedDamagingMoves.emplace_back(observedMove);
					}
				}
				else
				{
					break;
				}

				int damageToSelf = AIMoveScoring::SwitchDamageScoringRoutine(targetPlayer, self, *observedMove, targetMon, selfMon);

				if (damageToSelf > highestDamageToSelf)
				{
					highestDamageToSelf = damageToSelf;
				}

				if (highestDamageToSelf >= selfMon.GetCurrentHP())
				{
					targetMonHasKOMove = true;
				}
			}

			// If no observed damaging moves found, return early with no switch
			if (observedDamagingMoves.empty())
			{
				return false;
			}

			// If selfMon is slower, and targetMon's highest damaging observed move will KO selfMon
			if (!isFaster && targetMonHasKOMove)
			{
				return true;
			}
		}

		return false;
	}

	BattlePokemon* ChooseSwitch(Player& self, const Player& targetPlayer, const BattlePokemon& selfMon, const BattlePokemon& targetMon)
	{
		std::vector<BattlePokemon*> viablePokemon{};

		auto canAttack = [&](const BattlePokemon& pokemon) -> bool { return !(pokemon.GetStatus() == Status::Frozen || pokemon.GetStatus() == Status::Sleeping); };

		for (auto& pokemon : self.GetBeltArray())
		{
			if (pokemon.HasPokemon() && !pokemon.IsFainted() && canAttack(pokemon))
			{
				viablePokemon.emplace_back(&pokemon);
			}
		}

		int targetMonSpeed = AIMoveScoring::CalculateSpeed(targetMon);

		BattlePokemon* chosenPokemon{};

		int highestDamage{ INT_MIN };

		const auto& observedMoves = self.GetAIController().GetObservedMoves();
		std::vector<const pokemonMove*> observedDamagingMoves{};
		observedDamagingMoves.reserve(4);

		for (const auto& observedMove : observedMoves)
		{
			if (observedMove == nullptr)
			{
				break;
			}
			else if (observedMove->GetCategoryEnum() != Category::Status)
			{	
				observedDamagingMoves.emplace_back(observedMove);
			}
		}

		if (!observedDamagingMoves.empty())
		{
			int highestDamageToSelf{ INT_MIN };
			const pokemonMove* mostLikelyMove{};

			// Evaluate most likely observed move AI will do against selfMon
			for (const auto& observedMove : observedDamagingMoves)
			{
				int damageToSelf = AIMoveScoring::SwitchDamageScoringRoutine(targetPlayer, self, *observedMove, targetMon, selfMon);

				if (damageToSelf > highestDamageToSelf)
				{
					highestDamageToSelf = damageToSelf;
					mostLikelyMove = observedMove;
				}
			}

			// if viablePokemon is faster and has move that can one-hit KO targetMon and won't be KO'd by switch-in, choose highest damage dealer
			for (const auto& pokemon : viablePokemon)
			{
				if (PokemonTypeEffectiveness(self, targetMon, *pokemon) > 8192)
				{
					continue;
				}

				int candidateSpeed = AIMoveScoring::CalculateSpeed(*pokemon);

				if (candidateSpeed < targetMonSpeed)
				{
					continue;
				}

				int damageToCandidate = AIMoveScoring::SwitchDamageScoringRoutine(targetPlayer, self, *mostLikelyMove, targetMon, *pokemon);

				if (damageToCandidate >= pokemon->GetCurrentHP())
				{
					continue;
				}

				for (const auto& move : pokemon->GetMoveArray())
				{
					if (move.IsActive() && move.GetCategoryEnum() != Category::Status)
					{
						int damage = AIMoveScoring::SwitchDamageScoringRoutine(self, targetPlayer, move, *pokemon, targetMon);
					
						if (damage > highestDamage)
						{
							highestDamage = damage;
						}
					}
				}
				if (highestDamage >= targetMon.GetCurrentHP())
				{
					chosenPokemon = pokemon;
				}
			}

			if (chosenPokemon == &selfMon)
			{
				chosenPokemon = nullptr;
				return chosenPokemon;
			}

			if (chosenPokemon && chosenPokemon != &selfMon)
			{
				self.SetPokemonToSwitchTo(chosenPokemon);
				return chosenPokemon;
			}

			highestDamage = INT_MIN;

			// If none found above, find pokemon that can survive at least 2 hits and has highest damage move
			for (const auto& pokemon : viablePokemon)
			{
				if (PokemonTypeEffectiveness(self, targetMon, *pokemon) > 8192)
				{
					continue;
				}

				int firstAttackVsCandidate = AIMoveScoring::SwitchDamageScoringRoutine(targetPlayer, self, *mostLikelyMove, targetMon, *pokemon);
				int secondAttackVsCandidate{INT_MIN};

				// Evaluate most likely observed move AI will do against candidate
				for (const auto& observedMove : observedDamagingMoves)
				{
					int damageToCandidate = AIMoveScoring::SwitchDamageScoringRoutine(targetPlayer, self, *observedMove, targetMon, *pokemon);
					if (damageToCandidate > secondAttackVsCandidate)
					{
						secondAttackVsCandidate = damageToCandidate;
					}
				}

				if ((firstAttackVsCandidate + secondAttackVsCandidate) < pokemon->GetCurrentHP())
				{
					for (const auto& move : pokemon->GetMoveArray())
					{
						if (move.IsActive() && move.GetCategoryEnum() != Category::Status)
						{
							int damage = AIMoveScoring::SwitchDamageScoringRoutine(self, targetPlayer, move, *pokemon, targetMon);
							if (damage > highestDamage)
							{
								highestDamage = damage;
								chosenPokemon = pokemon;
							}
						}
					}
				}
			}

			if (chosenPokemon == &selfMon)
			{
				chosenPokemon = nullptr;
				return chosenPokemon;
			}

			if (chosenPokemon && chosenPokemon != &selfMon)
			{
				self.SetPokemonToSwitchTo(chosenPokemon);
				return chosenPokemon;
			}
		}

		highestDamage = INT_MIN;

		auto targetMonLastUsedMove = targetMon.GetLastUsedMove();
		bool isDamagingMove{};

		if (targetMonLastUsedMove)
		{
			isDamagingMove = targetMonLastUsedMove->GetCategoryEnum() != Category::Status;
		}
		
		// if still nothing found, pick pokemon that will not be KO'd by targetMon's last used move, is faster, and pick the one that can do highest damage
		if (targetMonLastUsedMove && isDamagingMove)
		{
			for (const auto& pokemon : viablePokemon)
			{
				int candidateSpeed = AIMoveScoring::CalculateSpeed(*pokemon);
				int damageToCandidate = AIMoveScoring::SwitchDamageScoringRoutine(targetPlayer, self, *targetMonLastUsedMove, targetMon, *pokemon);

				if (candidateSpeed >= targetMonSpeed && damageToCandidate < pokemon->GetCurrentHP())
				{
					for (const auto& move : pokemon->GetMoveArray())
					{
						if (move.IsActive() && move.GetCategoryEnum() != Category::Status)
						{
							int damage = AIMoveScoring::SwitchDamageScoringRoutine(self, targetPlayer, move, *pokemon, targetMon);

							if (damage > highestDamage)
							{
								highestDamage = damage;
								chosenPokemon = pokemon;
							}
						}
					}
				}
			}

			if (chosenPokemon == &selfMon)
			{
				chosenPokemon = nullptr;
				return chosenPokemon;
			}

			if (chosenPokemon && chosenPokemon != &selfMon)
			{
				self.SetPokemonToSwitchTo(chosenPokemon);
				return chosenPokemon;
			}

			// if still nothing found, pick pokemon that will take zero damage by targetMon's last used move, is faster, and pick the one that can do highest damage
			for (const auto& pokemon : viablePokemon)
			{
				int candidateSpeed = AIMoveScoring::CalculateSpeed(*pokemon);
				int damageToCandidate = AIMoveScoring::SwitchDamageScoringRoutine(targetPlayer, self, *targetMonLastUsedMove, targetMon, *pokemon);

				if (candidateSpeed >= targetMonSpeed && damageToCandidate == 0)
				{
					for (const auto& move : pokemon->GetMoveArray())
					{
						if (move.IsActive() && move.GetCategoryEnum() != Category::Status)
						{
							int damage = AIMoveScoring::SwitchDamageScoringRoutine(self, targetPlayer, move, *pokemon, targetMon);

							if (damage > highestDamage)
							{
								highestDamage = damage;
								chosenPokemon = pokemon;
							}
						}
					}
				}
			}

			if (chosenPokemon == &selfMon)
			{
				chosenPokemon = nullptr;
				return chosenPokemon;
			}

			if (chosenPokemon && chosenPokemon != &selfMon)
			{
				self.SetPokemonToSwitchTo(chosenPokemon);
				return chosenPokemon;
			}
		}

		std::vector<BattlePokemon*> bestCandidates{};
		bestCandidates.reserve(6);
		int bestTypeEffectiveness{ INT_MAX };

		// if still nothing found, pick pokemon that has the best defensive type advantage and if pokemon tie, pick the one that can do highest damage
		for (const auto& pokemon : viablePokemon)
		{
			int typeEffectiveness = PokemonTypeEffectiveness(self, targetMon, *pokemon);

			if (typeEffectiveness < bestTypeEffectiveness)
			{
				bestTypeEffectiveness = typeEffectiveness;
			}
		}

		for (const auto& pokemon : viablePokemon)
		{
			if (bestTypeEffectiveness == PokemonTypeEffectiveness(self, targetMon, *pokemon) && pokemon->GetCurrentHP() == pokemon->GetMaxHP())
			{
				bestCandidates.emplace_back(pokemon);
			}
		}

		for (const auto& pokemon : bestCandidates)
		{
			for (const auto& move : pokemon->GetMoveArray())
			{
				if (move.IsActive() && move.GetCategoryEnum() != Category::Status)
				{
					int damage = AIMoveScoring::SwitchDamageScoringRoutine(self, targetPlayer, move, *pokemon, targetMon);
					if (damage > highestDamage)
					{
						chosenPokemon = pokemon;
						highestDamage = damage;
					}
				}
			}
		}

		if (chosenPokemon != nullptr && chosenPokemon != &selfMon)
		{
			self.SetPokemonToSwitchTo(chosenPokemon);
		}

		if (chosenPokemon == &selfMon)
		{
			chosenPokemon = nullptr;
		}

		return chosenPokemon;
	}

	BattlePokemon* ChoosePostKOSwitch(Player& self, const Player& targetPlayer, const BattlePokemon& selfMon, const BattlePokemon& targetMon)
	{
		std::vector<BattlePokemon*> viablePokemon;

		for (auto& pokemon : self.GetBeltArray())
		{
			if (pokemon.HasPokemon() && !pokemon.IsFainted() && &pokemon != &selfMon)
			{
				viablePokemon.emplace_back(&pokemon);
			}
		}

		int targetMonSpeed = AIMoveScoring::CalculateSpeed(targetMon);

		BattlePokemon* chosenPokemon{};

		int highestDamage{ INT_MIN };

		auto canAttack = [&](const BattlePokemon& pokemon) -> bool { return !(pokemon.GetStatus() == Status::Frozen || pokemon.GetStatus() == Status::Sleeping); };

		// if viablePokemon is faster and has move that can one-hit KO targetMon
		for (const auto& pokemon : viablePokemon)
		{
			int candidateMonSpeed = AIMoveScoring::CalculateSpeed(*pokemon);

			if (candidateMonSpeed >= targetMonSpeed && canAttack(*pokemon))
			{
				for (const auto& move : pokemon->GetMoveArray())
				{
					if (move.IsActive() && move.GetCategoryEnum() != Category::Status)
					{
						int damage = AIMoveScoring::SwitchDamageScoringRoutine(self, targetPlayer, move, *pokemon, targetMon);
						if (damage > highestDamage && damage >= targetMon.GetCurrentHP())
						{
							highestDamage = damage;
							chosenPokemon = pokemon;
						}
					}
				}
			}
		}

		if (chosenPokemon)
		{
			self.SetPokemonToSwitchTo(chosenPokemon);
			return chosenPokemon;
		}

		highestDamage = INT_MIN;

		const auto& observedMoves = self.GetAIController().GetObservedMoves();
		std::vector<const pokemonMove*> observedDamagingMoves{};
		observedDamagingMoves.reserve(4);

		for (const auto& observedMove : observedMoves)
		{
			if (observedMove == nullptr)
			{
				break;
			}
			else if (observedMove->GetCategoryEnum() != Category::Status)
			{
				observedDamagingMoves.emplace_back(observedMove);
			}
		}

		// If none found above, find pokemon that doesn't have super disadvantageous type, can survive at least 1 hit and does highest damage
		if (!observedDamagingMoves.empty())
		{
			for (const auto& pokemon : viablePokemon)
			{
				if (PokemonTypeEffectiveness(self, targetMon, *pokemon) > 8192 || !canAttack(*pokemon))
				{
					continue;
				}

				int highestDamageToSelf{ INT_MIN };

				for (const auto& observedMove : observedDamagingMoves)
				{
					int damageToSelf = AIMoveScoring::SwitchDamageScoringRoutine(targetPlayer, self, *observedMove, targetMon, *pokemon);

					if (damageToSelf > highestDamageToSelf)
					{
						highestDamageToSelf = damageToSelf;
					}

					if (highestDamageToSelf >= pokemon->GetCurrentHP())
					{
						break;
					}
				}

				if (highestDamageToSelf < pokemon->GetCurrentHP())
				{
					for (const auto& move : pokemon->GetMoveArray())
					{
						if (move.IsActive() && move.GetCategoryEnum() != Category::Status)
						{
							int damage = AIMoveScoring::SwitchDamageScoringRoutine(self, targetPlayer, move, *pokemon, targetMon);
							if (damage > highestDamage)
							{
								highestDamage = damage;
								chosenPokemon = pokemon;
							}
						}
					}
				}
			}
		}

		if (chosenPokemon)
		{
			self.SetPokemonToSwitchTo(chosenPokemon);
			return chosenPokemon;
		}

		highestDamage = INT_MIN;

		std::vector<BattlePokemon*> bestCandidates{};
		bestCandidates.reserve(6);
		int bestTypeEffectiveness{ INT_MAX };

		// if still nothing found, prioritize pokemon that are faster than targetMon and have the best defensive type advantage. Pick the one that can do most damage
		// if no Pokemon are found that are faster, go with slower
		for (const auto& pokemon : viablePokemon)
		{
			int typeEffectiveness = PokemonTypeEffectiveness(self, targetMon, *pokemon);

			if (typeEffectiveness < bestTypeEffectiveness)
			{
				bestTypeEffectiveness = typeEffectiveness;
			}
		}

		for (const auto& pokemon : viablePokemon)
		{
			int candidateMonSpeed = AIMoveScoring::CalculateSpeed(*pokemon);

			if (bestTypeEffectiveness == PokemonTypeEffectiveness(self, targetMon, *pokemon) && candidateMonSpeed >= targetMonSpeed)
			{
				bestCandidates.emplace_back(pokemon);
			}
		}

		if (bestCandidates.empty())
		{
			for (const auto& pokemon : viablePokemon)
			{
				if (bestTypeEffectiveness == PokemonTypeEffectiveness(self, targetMon, *pokemon))
				{
					bestCandidates.emplace_back(pokemon);
				}
			}
		}

		for (const auto& pokemon : bestCandidates)
		{
			for (const auto& move : pokemon->GetMoveArray())
			{
				if (move.IsActive() && move.GetCategoryEnum() != Category::Status)
				{
					int damage = AIMoveScoring::SwitchDamageScoringRoutine(self, targetPlayer, move, *pokemon, targetMon);
					if (damage > highestDamage)
					{
						chosenPokemon = pokemon;
						highestDamage = damage;
					}
				}
			}
		}

		// default: if none found from above, switch to next in party line
		if (!chosenPokemon)
		{
			self.SetPokemonToSwitchTo(viablePokemon[0]);
			return viablePokemon[0];
		}
		else
		{
			self.SetPokemonToSwitchTo(chosenPokemon);
			return chosenPokemon;
		}
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

	int PokemonTypeEffectiveness(const Player& self, const BattlePokemon& source, const BattlePokemon& target)
	{
		return self.GetAIController().AICalculatePokemonTypeEffectiveness(source, target);
	}
}