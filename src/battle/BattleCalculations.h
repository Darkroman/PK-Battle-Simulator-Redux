#pragma once

#include <array>
#include <utility>

class RandomEngine;
struct BattleContext;
class Player;
class BattlePokemon;
struct pokemonMove;

class BattleCalculations
{
public:
	BattleCalculations(BattleContext&, RandomEngine&);

	int CalculatePokemonSpeed(const BattlePokemon&);

	void RandomizeTurnOrder();

	void RandomizePostTurnOrder();

	void SetFirst(Player&, Player&);

	bool CalculateCriticalHit(BattleContext&, const BattlePokemon&);

	std::pair<int, int> GetStageRatio(int);

	int MultiplyEffectiveness(uint16_t, uint16_t);

	void CalculateTypeEffectiveness(BattleContext&, const pokemonMove&, const BattlePokemon&);

	bool CalculateHitChance(const pokemonMove&, const BattlePokemon&, const BattlePokemon&);

	int CalculateDamage(BattleContext&, const Player&, const pokemonMove&, const BattlePokemon&, BattlePokemon&);

	void ApplyDamage(const pokemonMove&, BattlePokemon&, int);

	int CalculateLowKickPower(const BattlePokemon&);

private:
	RandomEngine& m_rng;
	BattleContext& m_context;

public:

	const std::array<std::pair<int, int>, 13> m_arr_StageRatio
	{
		std::make_pair(2, 8),  // -6
		std::make_pair(2, 7),  // -5
		std::make_pair(2, 6),  // -4
		std::make_pair(2, 5),  // -3
		std::make_pair(2, 4),  // -2
		std::make_pair(2, 3),  // -1
		std::make_pair(2, 2),  //  0 (neutral)
		std::make_pair(3, 2),  // +1
		std::make_pair(4, 2),  // +2
		std::make_pair(5, 2),  // +3
		std::make_pair(6, 2),  // +4
		std::make_pair(7, 2),  // +5
		std::make_pair(8, 2),  // +6
	};

	const std::array<std::pair<int, int>, 13> m_arr_accuracyStageRatio
	{
		std::make_pair(6, 18),  // -6
		std::make_pair(6, 16),  // -5
		std::make_pair(6, 14),  // -4
		std::make_pair(6, 12),  // -3
		std::make_pair(6, 10),  // -2
		std::make_pair(6, 8),  // -1
		std::make_pair(6, 6),  //  0 (neutral)
		std::make_pair(8, 6),  // +1
		std::make_pair(10, 6),  // +2
		std::make_pair(12, 6),  // +3
		std::make_pair(14, 6),  // +4
		std::make_pair(16, 6),  // +5
		std::make_pair(18, 6),  // +6
	};

	const std::array<int, 4> m_arr_CriticalHitStageThresholds{ 1, 3, 12, 24 }; // For critical hit
};