#pragma once

class RandomEngine;
struct BattleContext;
class Player;
class BattlePokemon;

class BattleCalculations
{
public:
	BattleCalculations(BattleContext&, RandomEngine&);

	int CalculatePokemonSpeed(BattlePokemon&);

	void RandomizeTurnOrder();

	void RandomizePostTurnOrder();

	void SetFirst(Player&, Player&);

	void CalculateCriticalHit(BattlePokemon&);

	std::pair<int, int> GetStageRatio(int);

	int MultiplyEffectiveness(uint16_t, uint16_t);

	void CalculateTypeEffectiveness(BattlePokemon::pokemonMove&, BattlePokemon&);

	bool CalculateHitChance(BattlePokemon::pokemonMove&, BattlePokemon&, BattlePokemon&);

	int CalculateDamage(Player&, BattlePokemon::pokemonMove&, BattlePokemon&, BattlePokemon&, bool isAI = false);

	void ApplyDamage(Player&, BattlePokemon::pokemonMove&, BattlePokemon&, BattlePokemon&, int);

	int CalculateLowKickPower(BattlePokemon&);

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

	const std::array<std::array<uint16_t, 18>, 18> typeChart
	{{
			         // NOR  FIR  WAT  ELE  GRA  ICE  FIG  POI  GRO  FLY  PSY  BUG  ROC  GHO  DRA  DAR  STE  FAI
			/* NOR */ {4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,2048,   0,4096,4096,2048,4096},
			/* FIR */ {4096,2048,2048,4096,8192,8192,4096,4096,4096,4096,4096,8192,2048,4096,2048,4096,8192,4096},
			/* WAT */ {4096,8192,2048,4096,2048,4096,4096,4096,8192,4096,4096,4096,8192,4096,2048,4096,4096,4096},
			/* ELE */ {4096,4096,8192,2048,2048,4096,4096,4096,   0,8192,4096,4096,4096,4096,2048,4096,4096,4096},
			/* GRA */ {4096,2048,8192,4096,2048,4096,4096,2048,8192,2048,4096,4096,8192,4096,2048,4096,2048,4096},
			/* ICE */ {4096,2048,2048,4096,8192,2048,4096,4096,8192,8192,4096,4096,4096,4096,8192,4096,2048,4096},
			/* FIG */ {8192,4096,4096,4096,4096,8192,4096,2048,4096,2048,2048,2048,8192,   0,4096,8192,8192,2048},
			/* POI */ {4096,4096,4096,4096,8192,4096,4096,2048,2048,4096,4096,4096,2048,2048,4096,4096,   0,8192},
			/* GRO */ {4096,8192,4096,8192,2048,4096,4096,8192,4096,   0,4096,2048,8192,4096,4096,4096,8192,4096},
			/* FLY */ {4096,4096,4096,2048,8192,4096,8192,4096,4096,4096,4096,8192,2048,4096,4096,4096,2048,4096},
			/* PSY */ {4096,4096,4096,4096,4096,4096,8192,8192,4096,4096,2048,4096,4096,4096,4096,   0,2048,4096},
			/* BUG */ {4096,2048,4096,4096,8192,4096,2048,2048,4096,2048,8192,4096,4096,2048,4096,8192,2048,2048},
			/* ROC */ {4096,8192,4096,4096,4096,8192,2048,4096,2048,8192,4096,8192,4096,4096,4096,4096,2048,4096},
			/* GHO */ {   0,4096,4096,4096,4096,4096,4096,4096,4096,4096,8192,4096,4096,8192,4096,2048,4096,4096},
			/* DRA */ {4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,8192,4096,2048,   0},
			/* DAR */ {4096,4096,4096,4096,4096,4096,2048,4096,4096,4096,8192,4096,4096,8192,4096,2048,4096,2048},
			/* STE */ {4096,2048,2048,2048,4096,8192,4096,4096,4096,4096,4096,4096,8192,4096,4096,4096,2048,8192},
			/* FAI */ {4096,2048,4096,4096,4096,4096,8192,2048,4096,4096,4096,4096,4096,4096,8192,8192,2048,4096}
	}};
};