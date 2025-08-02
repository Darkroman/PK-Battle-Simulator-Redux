#pragma once

class RandomEngine;
struct BattleContext;
class Player;
class BattlePokemon;
class IMoveResultsUI;

class BattleCalculations
{
public:
	BattleCalculations(BattleContext&, RandomEngine&, IMoveResultsUI&);

	int CalculateCriticalHitStageModifier(size_t);

	double CalculateCriticalHit(BattlePokemon*);

	double CalculateStageModifier(size_t);

	double CalculateTypeEffectiveness(BattlePokemon::pokemonMove*, BattlePokemon*);

	double CalculateAccuracyModifiers(int);

	bool CalculateHitChance(BattlePokemon::pokemonMove* currentMove, BattlePokemon* source, BattlePokemon* target);

	void CalculateDamage(Player*, BattlePokemon::pokemonMove*, BattlePokemon*, BattlePokemon*);

	int CalculateLowKickPower(BattlePokemon*);

private:
	RandomEngine& m_rng;
	BattleContext& m_context;
	IMoveResultsUI& m_resultsUI;

public:

	const std::array<double, 13> m_arr_StageMultiplier{ 2.0 / 8.0, 2.0 / 7.0, 2.0 / 6.0, 2.0 / 5.0, 2.0 / 4.0, 2.0 / 3.0, 2.0 / 2.0, 3.0 / 2.0, 4.0 / 2.0, 5.0 / 2.0, 6.0 / 2.0, 7.0 / 2.0, 8.0 / 2.0 }; // For attack, defense, special attack, special defense, and speed (not accuracy, evasion, or crit)
	const std::array<double, 13> m_arr_AccuracyStageMultiplier{ 3.0 / 9.0, 3.0 / 8.0, 3.0 / 7.0, 3.0 / 6.0, 3.0 / 5.0, 3.0 / 4.0, 3.0 / 3.0, 4.0 / 3.0, 5.0 / 3.0, 6.0 / 3.0, 7.0 / 3.0, 8.0 / 3.0, 9.0 / 3.0 }; // For accuracy and evasion
	const std::array<int, 4> m_arr_CriticalHitStageMultiplier{ 417, 1250, 5000, 10000 }; // For critical hit

	enum class TypeEffect : uint8_t
	{
		Immune = 0,
		NotVery = 1,
		Normal = 2,
		Super = 3
	};

	const std::array<std::array<TypeEffect, 18>, 18> typeEffectivenessChart = { {
	{{ TypeEffect::Normal, TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::NotVery, TypeEffect::Immune,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::NotVery, TypeEffect::Normal }},
	{{ TypeEffect::Normal, TypeEffect::NotVery, TypeEffect::NotVery, TypeEffect::Normal,  TypeEffect::Super,   TypeEffect::Super,   TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Super,   TypeEffect::NotVery, TypeEffect::Normal,  TypeEffect::NotVery, TypeEffect::Normal,  TypeEffect::Super,   TypeEffect::Normal }},
	{{ TypeEffect::Normal, TypeEffect::Super,   TypeEffect::NotVery, TypeEffect::Normal,  TypeEffect::NotVery, TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Super,   TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Super,   TypeEffect::Normal,  TypeEffect::NotVery, TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal }},
	{{ TypeEffect::Normal, TypeEffect::Normal,  TypeEffect::Super,   TypeEffect::NotVery, TypeEffect::NotVery, TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Immune,  TypeEffect::Super,   TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::NotVery, TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal }},
	{{ TypeEffect::Normal, TypeEffect::NotVery, TypeEffect::Super,   TypeEffect::Normal,  TypeEffect::NotVery, TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::NotVery, TypeEffect::Super,   TypeEffect::NotVery, TypeEffect::Normal,  TypeEffect::NotVery, TypeEffect::Super,   TypeEffect::Normal,  TypeEffect::NotVery, TypeEffect::Normal,  TypeEffect::NotVery, TypeEffect::Normal }},
	{{ TypeEffect::Normal, TypeEffect::NotVery, TypeEffect::NotVery, TypeEffect::Normal,  TypeEffect::Super,   TypeEffect::NotVery, TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Super,   TypeEffect::Super,   TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Super,   TypeEffect::Normal,  TypeEffect::NotVery, TypeEffect::Normal }},
	{{ TypeEffect::Super,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Super,   TypeEffect::Normal,  TypeEffect::NotVery, TypeEffect::Normal,  TypeEffect::NotVery, TypeEffect::NotVery, TypeEffect::NotVery, TypeEffect::Super,   TypeEffect::Immune,  TypeEffect::Normal,  TypeEffect::Super,   TypeEffect::Super,   TypeEffect::NotVery }},
	{{ TypeEffect::Normal, TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Super,   TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::NotVery, TypeEffect::NotVery, TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::NotVery, TypeEffect::NotVery, TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Immune,  TypeEffect::Super }},
	{{ TypeEffect::Normal, TypeEffect::Super,   TypeEffect::Normal,  TypeEffect::Super,   TypeEffect::NotVery, TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Super,   TypeEffect::Normal,  TypeEffect::Immune,  TypeEffect::Normal,  TypeEffect::NotVery, TypeEffect::Super,   TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Super,   TypeEffect::Normal }},
	{{ TypeEffect::Normal, TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::NotVery, TypeEffect::Super,   TypeEffect::Normal,  TypeEffect::Super,   TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Super,   TypeEffect::NotVery, TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::NotVery, TypeEffect::Normal }},
	{{ TypeEffect::Normal, TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Super,   TypeEffect::Super,   TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::NotVery, TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Immune,  TypeEffect::NotVery, TypeEffect::Normal }},
	{{ TypeEffect::Normal, TypeEffect::NotVery, TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Super,   TypeEffect::Normal,  TypeEffect::NotVery, TypeEffect::NotVery, TypeEffect::Normal,  TypeEffect::NotVery, TypeEffect::Super,   TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::NotVery, TypeEffect::Normal,  TypeEffect::Super,   TypeEffect::NotVery, TypeEffect::NotVery }},
	{{ TypeEffect::Normal, TypeEffect::Super,   TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Super,   TypeEffect::NotVery, TypeEffect::Normal,  TypeEffect::NotVery, TypeEffect::Super,   TypeEffect::Normal,  TypeEffect::Super,   TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::NotVery, TypeEffect::Normal }},
	{{ TypeEffect::Immune, TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Super,   TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Super,   TypeEffect::Normal,  TypeEffect::NotVery, TypeEffect::Normal,  TypeEffect::Normal }},
	{{ TypeEffect::Normal, TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Super,   TypeEffect::Normal,  TypeEffect::NotVery, TypeEffect::Immune }},
	{{ TypeEffect::Normal, TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::NotVery, TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Super,   TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Super,   TypeEffect::Normal,  TypeEffect::NotVery, TypeEffect::Normal,  TypeEffect::NotVery }},
	{{ TypeEffect::Normal, TypeEffect::NotVery, TypeEffect::NotVery, TypeEffect::NotVery, TypeEffect::Normal,  TypeEffect::Super,   TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Super,   TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::NotVery, TypeEffect::Super }},
	{{ TypeEffect::Normal, TypeEffect::NotVery, TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Super,   TypeEffect::NotVery, TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Normal,  TypeEffect::Super,   TypeEffect::Super,   TypeEffect::NotVery, TypeEffect::Normal }}
} };

	double DecodeEffectiveness(TypeEffect effect)
	{
		switch (effect)
		{
			case TypeEffect::Immune:
				return 0.0;

			case TypeEffect::NotVery:
				return 0.5;

			case TypeEffect::Normal:   
				return 1.0;

			case TypeEffect::Super:    
				return 2.0;

			default:                   
				return 1.0;
		}
	}

	double MultiplyEffectiveness(TypeEffect effect1, TypeEffect effect2)
	{
		if (effect1 == TypeEffect::Immune || effect2 == TypeEffect::Immune)
		{
			return 0.0;
		}
		return DecodeEffectiveness(effect1) * DecodeEffectiveness(effect2);
	}
};