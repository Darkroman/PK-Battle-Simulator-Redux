#pragma once

class Player;
class BattlePokemon;
struct pokemonMove;
class RandomEngine;

namespace AISwitchLogic
{
	bool WantsToSwitch(const Player&, const Player&, const BattlePokemon&, const BattlePokemon&);

	BattlePokemon* ChooseSwitch(Player&, const Player&, const BattlePokemon&, const BattlePokemon&);

	BattlePokemon* ChoosePostKOSwitch(Player&, const Player&, const BattlePokemon&, const BattlePokemon&);

	bool IsMoveSuperEffective(const Player&, const pokemonMove&, const BattlePokemon&);

	bool IsMoveAtMostEffective(const Player&, const pokemonMove&, const BattlePokemon&);

	bool IsMoveAtLeastEffective(const Player&, const pokemonMove&, const BattlePokemon&);

	bool IsMoveNotEffective(const Player&, const pokemonMove&, const BattlePokemon&);

	bool IsMoveNotVeryEffective(const Player&, const pokemonMove&, const BattlePokemon&);

	int PokemonTypeEffectiveness(const Player& self, const BattlePokemon& source, const BattlePokemon& target);
}