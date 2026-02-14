#pragma once
#include <array>
#include <vector>

#include "IPlayerController.h"

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
	Player* selfPlayer{};
	Player* opponentPlayer{};
	std::array<PersistentMemory, 6> opponentMemory{};
	ActiveOpponentPokemonMemory activeOpponent{};
	int slotOfActivePokemon{};
};

class AIController : public IPlayerController
{
public:

	explicit AIController(IAIStrategy&);

	PlayerDecisionOutcome ChooseAction(Player&, BattlePokemon&) override;
	BattlePokemon* PromptForSwitch(Player&, BattlePokemon&) override;

	void OnBattleStart(Player&, BattleContext&);
	void OnActivePokemonChanged(BattleContext&);

	void OnMoveResolved(BattleContext&);
	void OnFirstMoveResolved(BattleContext&);
	void UpdateEnemyOffenseStats(BattleContext&, BattlePokemon::pokemonMove&, BattlePokemon&, BattlePokemon&);
	void UpdateEnemyHPandDefenseStats(BattleContext&, Player&, BattlePokemon::pokemonMove&, BattlePokemon&, BattlePokemon&);
	void UpdateEnemySpeedStats(BattleContext&, Player& self, BattlePokemon&, BattlePokemon&);

private:
	BattlePokemon::pokemonMove* FightAction(Player&, BattlePokemon&);
	BattlePokemon* SwitchAction(Player&, BattlePokemon&);
	BattleAction ForfeitAction(Player&);

	void GetOpponentParty(Player&);
	void InitEstimatedStatRanges();
	void PrecomputeRealStats(Player&);

	void UpdateObservedMoves(BattlePokemon::pokemonMove&);
	void UpdateActivePokemon(BattlePokemon&);

	std::array<PersistentMemory, 6>::iterator FindActivePokemonSlot();

public:
	AIMemory memory;

private:
	IAIStrategy& m_aiStrategy;

};