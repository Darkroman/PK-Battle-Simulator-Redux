#pragma once

#include <array>
#include <vector>

#include "Player.h"

struct BattleContext;
class IAIStrategy;
class Move;

enum class Difficulty { Easy, Medium, Hard };

struct ObservedPokemonMoves
{
	std::array<BattlePokemon::pokemonMove*, 4> moves{};
	std::array<bool, 4> revealed{};
};

struct PersistentMemory
{
	BattlePokemon* pokemon;
	ObservedPokemonMoves observedMoves{};
	BattlePokemon::pokemonMove* strongestMove{};

	std::pair<int, int> hpRange{};
	std::pair<int, int> attackRange{};
	std::pair<int, int> defenseRange{};
	std::pair<int, int> specialAttackRange{};
	std::pair<int, int> specialDefenseRange{};
	std::pair<int, int> speedRange{};

	std::vector<int> realHPValues{};
	std::vector<int> realAttackValues{};
	std::vector<int> realDefenseValues{};
	std::vector<int> realSpecialAttackValues{};
	std::vector<int> realSpecialDefenseValues{};
	std::vector<int> realSpeedValues{};
};

struct ActiveOpponentPokemonMemory
{
	BattlePokemon* opponentActivePokemon{};
	BattlePokemon::pokemonMove* opponentLastUsedMove{};

	int opponentAttackStage{};
	int opponentDefenseStage{};
	int opponentSpecialAttackStage{};
	int opponentSpecialDefenseStage{};
	int opponentSpeedStage{};
};

struct AIMemory
{
	std::array<PersistentMemory, 6> opponentMemory{};
	ActiveOpponentPokemonMemory activeOpponent{};
	int slotOfActivePokemon{};
};

class AIPlayer : public Player
{
public:
	explicit AIPlayer(std::string_view, IAIStrategy&);
	void ChooseAction(BattlePokemon*, BattleContext&);

	bool IsHuman() const override;
	BattlePokemon::pokemonMove* ChooseMove(BattlePokemon&);
	bool WantsToSwitch(BattlePokemon&);
	void ChooseSwitch(BattlePokemon&);

	void GetOpponentParty(Player*);
	void InitEstimatedStatRanges(Player*);
	void PrecomputeRealStats(Player*);

	void UpdateObservedMoves(BattlePokemon::pokemonMove*);
	void UpdateActivePokemon(BattlePokemon*);

	void UpdateEnemyOffenseStats(BattleContext&, BattlePokemon::pokemonMove*, BattlePokemon*, BattlePokemon*);
	void UpdateEnemyHPandDefenseStats(BattleContext&, Player*, BattlePokemon::pokemonMove*, BattlePokemon*, BattlePokemon*);
	void UpdateEnemySpeedStats(BattleContext&, BattlePokemon::pokemonMove*, BattlePokemon::pokemonMove*, BattlePokemon*, BattlePokemon*);

	std::array<PersistentMemory, 6>::iterator FindActivePokemonSlot();

public:
	AIMemory memory;

private:
	IAIStrategy& m_aiStrategy;
};