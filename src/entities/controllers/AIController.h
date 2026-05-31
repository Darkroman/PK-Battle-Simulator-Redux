#pragma once

#include <array>
#include <span>

#include "IPlayerController.h"

struct pokemonMove;
class Move;

enum class Difficulty { Easy, Medium, Hard };

struct ObservedPokemonMoves
{
	std::array<const pokemonMove*, 4> moves{};
	std::array<bool, 4> revealed{};
};

struct PersistentMemory
{
	const BattlePokemon* pokemon;
	ObservedPokemonMoves observedMoves{};
};

struct ActiveOpponentPokemonMemory
{
	const BattlePokemon* opponentActivePokemon{};
	const pokemonMove* opponentLastUsedMove{};
};

struct AIMemory
{
	const Player* selfPlayer{};
	const Player* opponentPlayer{};
	std::array<PersistentMemory, 6> opponentMemory{};
	ActiveOpponentPokemonMemory activeOpponent{};
	PersistentMemory* slotOfActivePokemon{};
};

class AIController : public IPlayerController
{
public:

	explicit AIController(Difficulty);
	AIController(const AIController& other) = default;
	std::unique_ptr<IPlayerController> clone() const override;

	PlayerDecisionOutcome ChooseAction(Player&, const Player&, BattlePokemon&, const BattlePokemon&, RandomEngine&) override;
	BattlePokemon* PromptForSwitch(Player&, const Player&, const BattlePokemon&, const BattlePokemon&) override;

	Difficulty GetDifficulty() const;

	void OnBattleStart(const Player&, BattleContext&);
	void OnActivePokemonChanged(const BattleContext&);

	//std::array<const pokemonMove*, 4> GetObservedMoves() const;
	std::span<const pokemonMove*> GetObservedMoves() const;

	void ResetObservedMoves();

	void OnMoveResolved(const BattleContext&);
	unsigned int  AICalculatePokemonTypeEffectiveness(const BattlePokemon& source, const BattlePokemon& target) const;
	unsigned int  AICalculateMoveTypeEffectiveness(const pokemonMove& currentMove, const BattlePokemon& target) const;
	unsigned int  AICalculateDamage(const pokemonMove&, const Player&, const BattlePokemon&, const BattlePokemon&) const;
	bool CalculateStatusMoveEffectiveness(const pokemonMove& currentMove, const Player& targetPlayer, const BattlePokemon& source, const BattlePokemon& target) const;

private:
	pokemonMove* FightAction(const Player&, const Player&, BattlePokemon&, const BattlePokemon&, RandomEngine&);

	BattlePokemon* SwitchAction(Player&, const Player&, const BattlePokemon&, const BattlePokemon&);
	BattlePokemon* SwitchActionPostKO(Player&, const Player&, const BattlePokemon&, const BattlePokemon&);

	BattleAction ForfeitAction(const Player&);

	void GetOpponentParty(const Player&);

	void UpdateObservedMoves(const pokemonMove&);
	void UpdateOpponentActivePokemon(const BattlePokemon&);

	PersistentMemory* FindActivePokemonSlot();

public:
	AIMemory memory;
	
private:
	Difficulty m_difficulty{};
};