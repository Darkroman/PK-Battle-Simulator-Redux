#pragma once

#include <vector>
#include <array>
#include <memory>

#include "../common/PartyEditResults.h"

class Player;
class RandomEngine;
class IAIStrategy;

class Menu
{
private:
	std::vector<std::unique_ptr<Player>>& playerStorage;
	std::array<Player*, 2> players;
	RandomEngine& m_rng;
	IAIStrategy& m_persistentStrategy;

public:
	Menu() = delete;
	explicit Menu(std::vector<std::unique_ptr<Player>>&, RandomEngine&, IAIStrategy&);

	SetPokemonResult SetPlayerPokemon(BattlePokemon&, std::string_view);
	SetMoveResult SetPlayerPokemonMove(BattlePokemon&, size_t, std::string_view);

	bool SetPlayerPokemonNickname(BattlePokemon&);

	bool SetPlayerPokemonLevel(BattlePokemon&);

	bool SetPlayerPokemonHPIV(BattlePokemon&);
	bool SetPlayerPokemonAttackIV(BattlePokemon&);
	bool SetPlayerPokemonDefenseIV(BattlePokemon&);
	bool SetPlayerPokemonSpecialAttackIV(BattlePokemon&);
	bool SetPlayerPokemonSpecialDefenseIV(BattlePokemon&);
	bool SetPlayerPokemonSpeedIV(BattlePokemon&);

	bool SetPlayerPokemonHPEV(BattlePokemon&);
	bool SetPlayerPokemonAttackEV(BattlePokemon&);
	bool SetPlayerPokemonDefenseEV(BattlePokemon&);
	bool SetPlayerPokemonSpecialAttackEV(BattlePokemon&);
	bool SetPlayerPokemonSpecialDefenseEV(BattlePokemon&);
	bool SetPlayerPokemonSpeedEV(BattlePokemon&);

	bool RunMenu();
	void ChangePlayerOneName();
	void EditPlayerOnePokemon();
	void ChangePlayerOneType();
	void PrintPlayerOneType();
	void ChangePlayerTwoName();
	void EditPlayerTwoPokemon();
	void ChangePlayerTwoType();
	void PrintPlayerTwoType();

	bool AddPokemon(Player&);
	bool ChangePokemon(Player&);
	bool DeletePokemon(Player&);
	bool SwapPokemon(Player&);
	bool ReorderPokemon(Player&);
	bool EditPokemonStats(Player&);
	bool EditPokemonMoves(Player&);

	bool EditChosenPokemonStats(BattlePokemon&);
	bool EditChosenPokemonMoves(BattlePokemon&);

	bool AddMove(BattlePokemon&);
	bool ChangeMove(BattlePokemon&);
	bool DeleteMove(BattlePokemon&);
	bool SwapMoves(BattlePokemon&);
	bool ReorderMoves(BattlePokemon&);

	void SaveYourParty(std::array<Player*, 2>);
	void SavePlayerNumberParty(std::array<Player*, 2>, int);

	void LoadYourParty(std::array<Player*, 2>);
	void LoadPlayerNumberParty(std::array<Player*, 2>, int);

	bool IsPokemonSetupIncomplete();

	void SetDefaultPokemon();
};