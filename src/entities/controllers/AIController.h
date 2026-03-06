#pragma once
#include <array>
#include <vector>

#include "IPlayerController.h"

struct pokemonMove;
class Move;

enum class Difficulty { Easy, Medium, Hard };

struct ObservedPokemonMoves
{
	std::array<pokemonMove*, 4> moves{};
	std::array<bool, 4> revealed{};
};

struct PersistentMemory
{
	BattlePokemon* pokemon;
	ObservedPokemonMoves observedMoves{};
	pokemonMove* strongestMove{};
};

struct ActiveOpponentPokemonMemory
{
	BattlePokemon* opponentActivePokemon{};
	pokemonMove* opponentLastUsedMove{};
};

struct AIMemory
{
	Player* selfPlayer{};
	Player* opponentPlayer{};
	std::array<PersistentMemory, 6> opponentMemory{};
	ActiveOpponentPokemonMemory activeOpponent{};
	int slotOfActivePokemon{};
};

class AIController : public IPlayerController
{
public:

	explicit AIController(Difficulty);

	PlayerDecisionOutcome ChooseAction(Player&, Player&, BattlePokemon&, BattlePokemon&, RandomEngine&) override;
	BattlePokemon* PromptForSwitch(Player&, Player&, BattlePokemon&, BattlePokemon&) override;

	Difficulty GetDifficulty();

	void OnBattleStart(Player&, BattleContext&);
	void OnActivePokemonChanged(BattleContext&);

	std::array<pokemonMove*, 4> GetObservedMoves();
	void ResetObservedMoves();

	void OnMoveResolved(BattleContext&);
	int  AICalculatePokemonTypeEffectiveness(const BattlePokemon& source, const BattlePokemon& target);
	int  AICalculateMoveTypeEffectiveness(const pokemonMove& currentMove, const BattlePokemon& target);
	int  AICalculateDamage(const pokemonMove&, const Player&, const BattlePokemon&, const BattlePokemon&);

private:
	pokemonMove* FightAction(Player&, Player&, BattlePokemon&, BattlePokemon&, RandomEngine&);

	BattlePokemon* SwitchAction(Player&, Player&, BattlePokemon&, BattlePokemon&);
	BattlePokemon* SwitchActionPostKO(Player&, Player&, BattlePokemon&, BattlePokemon&);

	BattleAction ForfeitAction(Player&);

	void GetOpponentParty(Player&);

	void UpdateObservedMoves(pokemonMove&);
	void UpdateOpponentActivePokemon(BattlePokemon&);

	std::array<PersistentMemory, 6>::iterator FindActivePokemonSlot();

public:
	AIMemory memory;
	

private:
	Difficulty m_difficulty{};
};