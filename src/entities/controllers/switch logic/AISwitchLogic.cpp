#include <deque>

#include "AISwitchLogic.h"
#include "../move scoring/AIMoveScoring.h"
#include "../../../battle/BattleCalculations.h"
#include "../../Player.h"
#include "../AIController.h"

namespace AISwitchLogic
{
	bool WantsToSwitch(Player& self, Player& targetPlayer, const BattlePokemon& selfMon, const BattlePokemon& targetMon)
	{
		// if on last pokemon, don't switch
		if (self.GetPokemonLeft() < 2)
		{
			return false;
		}

		// if selfMon has at least 1 move that is >= 2x super effective, don't switch
		for (const auto& move : selfMon.GetMoveArray())
		{
			if (move.IsActive() && IsMoveSuperEffective(self, move, targetMon))
			{
				return false;
			}
		}

		// if selfMon has no super effective moves and takes super effective damage from target, find if we have mon that can take at most normal damage and has super effective move
		if (self.GetAIController().GetObservedMoves().at(0) == nullptr)
		{
			return false;
		}

		for (const auto& observedMove : self.GetAIController().GetObservedMoves())
		{
			if (observedMove == nullptr)
			{
				break;
			}

			if (IsMoveSuperEffective(self, *observedMove, selfMon))
			{
				for (const auto& pokemon : self.GetBeltArray())
				{
					if (!pokemon.IsFainted() && IsMoveAtMostEffective(self, *observedMove, pokemon))
					{
						for (auto& move : pokemon.GetMoveArray())
						{
							if (move.IsActive() && IsMoveSuperEffective(self, move, targetMon))
							{
								return true;
							}
						}
					}
				}
			}
		}

		return false;
	}

	BattlePokemon* ChooseSwitch(Player& self, Player& targetPlayer, BattlePokemon& selfMon, BattlePokemon& targetMon)
	{
		std::deque<BattlePokemon*> viablePokemon;

		for (size_t i = 1; i <= 6; ++i)
		{
			if (self.GetBelt(i).HasPokemon() && !self.GetBelt(i).IsFainted() && &self.GetBelt(i) != &selfMon)
			{
				viablePokemon.push_back(&self.GetBelt(i));
			}
		}

		BattlePokemon* chosenPokemon{};

		int highestDamage{ INT_MIN };
		for (const auto& pokemon : viablePokemon)
		{
			if (PokemonTypeEffectiveness(self, targetMon, *pokemon) <= 4096)
			{
				for (auto& move : pokemon->GetMoveArray())
				{
					if (move.IsActive())
					{
						int damage = AIMoveScoring::SwitchDamageScoringRoutine(self, targetPlayer, move, selfMon, targetMon);
						if (damage > highestDamage)
						{
							chosenPokemon = pokemon;
							highestDamage = damage;
						}
					}
				}
			}
		}

		if (chosenPokemon != nullptr)
		{
			self.SetPokemonToSwitchTo(chosenPokemon);
		}

		return chosenPokemon;
	}

	BattlePokemon* ChoosePostKOSwitch(Player & self, Player & targetPlayer, BattlePokemon & selfMon, BattlePokemon & targetMon)
	{
		std::deque<BattlePokemon*> viablePokemon;

		for (size_t i = 1; i <= 6; ++i)
		{
			if (self.GetBelt(i).HasPokemon() && !self.GetBelt(i).IsFainted() && &self.GetBelt(i) != &selfMon)
			{
				viablePokemon.push_back(&self.GetBelt(i));
			}
		}

		// phase 1a: Check for any observed moves
		BattlePokemon* chosenPokemon{};
		for (const auto& observedMove : self.GetAIController().GetObservedMoves())
		{
			if (observedMove == nullptr)
			{
				break;
			}

			for (const auto pokemon : viablePokemon)
			{
				// phase 2a: Check for first pokemon that does not take super effective damage from target
				if (IsMoveAtMostEffective(self, *observedMove, *pokemon))
				{
					for (const auto& move : pokemon->GetMoveArray())
					{
						// phase 3a: check for move that does super effective damage against target
						if (move.IsActive() && IsMoveSuperEffective(self, move, targetMon))
						{
							chosenPokemon = pokemon;
							self.SetPokemonToSwitchTo(chosenPokemon);
							return chosenPokemon;
						}
					}
				}
			}
		}

		// phase 1b: if no observed moves, and previous checks didn't find anything
		for (const auto pokemon : viablePokemon)
		{
			// phase 2b: Check which of our pokemon takes at the most normal effective damage
			if (PokemonTypeEffectiveness(self, targetMon, *pokemon) <= 4096)
			{
				for (const auto& move : pokemon->GetMoveArray())
				{
					// phase 3b: Check if pokemon has any super effective moves
					if (move.IsActive() && IsMoveSuperEffective(self, move, targetMon))
					{
						chosenPokemon = pokemon;
						self.SetPokemonToSwitchTo(chosenPokemon);
						return chosenPokemon;
					}
				}
			}
		}

		// default: if none found from above, switch to next in party line
		self.SetPokemonToSwitchTo(viablePokemon[0]);
		return viablePokemon[0];
	}

	bool IsMoveSuperEffective(Player & self, const pokemonMove & move, const BattlePokemon & pokemon)
	{
		auto MoveEffectiveness = [&](const pokemonMove& move, const BattlePokemon& targetMon) {
			return self.GetAIController().AICalculateMoveTypeEffectiveness(move, targetMon);
			};

		return MoveEffectiveness(move, pokemon) > 4096 && move.GetPower() > 0 && move.m_currentPP > 0;
	}

	bool IsMoveAtMostEffective(Player & self, const pokemonMove & move, const BattlePokemon & pokemon)
	{
		auto MoveEffectiveness = [&](const pokemonMove& move, const BattlePokemon& targetMon) {
			return self.GetAIController().AICalculateMoveTypeEffectiveness(move, targetMon);
			};

		return MoveEffectiveness(move, pokemon) <= 4096 && move.GetPower() > 0 && move.m_currentPP > 0;
	}

	bool IsMoveNotEffective(Player& self, const pokemonMove& move, const BattlePokemon& pokemon)
	{
		auto MoveEffectiveness = [&](const pokemonMove& move, const BattlePokemon& targetMon) {
			return self.GetAIController().AICalculateMoveTypeEffectiveness(move, targetMon);
			};

		return MoveEffectiveness(move, pokemon) == 0 && move.GetPower() > 0 && move.m_currentPP > 0;
	}

	bool IsMoveNotVeryEffective(Player& self, const pokemonMove& move, const BattlePokemon& pokemon)
	{
		auto MoveEffectiveness = [&](const pokemonMove& move, const BattlePokemon& targetMon) {
			return self.GetAIController().AICalculateMoveTypeEffectiveness(move, targetMon);
			};

		return MoveEffectiveness(move, pokemon) <= 2048 && move.GetPower() > 0 && move.m_currentPP > 0;
	}

	int PokemonTypeEffectiveness(Player& self, const BattlePokemon& source, const BattlePokemon& target)
	{
		return self.GetAIController().AICalculatePokemonTypeEffectiveness(source, target);
	}
}