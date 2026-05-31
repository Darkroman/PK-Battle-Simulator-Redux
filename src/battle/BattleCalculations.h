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

	unsigned int CalculatePokemonSpeed(const BattlePokemon&);

	void RandomizeTurnOrder();

	void RandomizePostTurnOrder();

	void SetFirst(Player&, Player&);

	bool CalculateCriticalHit(BattleContext&, const BattlePokemon&);

	unsigned int MultiplyEffectiveness(unsigned int, unsigned int);

	void CalculateTypeEffectiveness(BattleContext&, const pokemonMove&, const BattlePokemon&);

	bool CalculateHitChance(const pokemonMove&, const BattlePokemon&, const BattlePokemon&);

	unsigned int CalculateDamage(BattleContext&, const Player&, const pokemonMove&, const BattlePokemon&, BattlePokemon&);

	void ApplyDamage(const pokemonMove&, BattlePokemon&, unsigned int);

	unsigned int CalculateLowKickPower(const BattlePokemon&);

private:
	RandomEngine& m_rng;
	BattleContext& m_context;

public:
	const std::array<int, 4> m_arr_CriticalHitStageThresholds{ 1, 3, 12, 24 }; // For critical hit

};