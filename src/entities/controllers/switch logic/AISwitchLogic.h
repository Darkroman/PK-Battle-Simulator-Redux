#pragma once

class Player;
class BattlePokemon;
struct pokemonMove;
class BattleCalculations;
class RandomEngine;

namespace AISwitchLogic
{
	bool WantsToSwitch(Player&, Player&, const BattlePokemon&, const BattlePokemon&);

	BattlePokemon* ChooseSwitch(Player&, Player&, BattlePokemon&, BattlePokemon&);

	BattlePokemon* ChoosePostKOSwitch(Player&, Player&, BattlePokemon&, BattlePokemon&);

	bool IsMoveSuperEffective(Player&, const pokemonMove&, const BattlePokemon&);

	bool IsMoveAtMostEffective(Player&, const pokemonMove&, const BattlePokemon&);

	bool IsMoveNotEffective(Player&, const pokemonMove&, const BattlePokemon&);

	bool IsMoveNotVeryEffective(Player&, const pokemonMove&, const BattlePokemon&);

	int PokemonTypeEffectiveness(Player& self, const BattlePokemon& source, const BattlePokemon& target);
}