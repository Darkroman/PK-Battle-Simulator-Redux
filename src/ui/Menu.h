#pragma once

#include "../entities/Player.h"
#include "../entities/HumanPlayer.h"
#include "../entities/AIPlayer.h"
#include "../save/SaveParty.h"
#include "../save/LoadParty.h"
#include "../data/InputValidation.h"

class RandomEngine;
struct BattleContext;

class Menu
{
private:
	std::vector<std::unique_ptr<Player>>& playerStorage;
	std::array<Player*, 2> players;
	RandomEngine& m_rng;
	std::vector<std::unique_ptr<IAIStrategy>> strategies;

public:
	Menu() = delete;
	explicit Menu(std::vector<std::unique_ptr<Player>>&, RandomEngine&);

	void SetPlayerPokemon(Player*, size_t, size_t);
	void SetPlayerPokemon(Player*, size_t, std::string_view);

	void SetPlayerPokemonMove(Player*, size_t, size_t, size_t);
	void SetPlayerPokemonMove(Player*, size_t, size_t, std::string_view);

	bool SetPlayerPokemonNickname(Player*, size_t);

	bool SetPlayerPokemonLevel(Player*, size_t);

	bool SetPlayerPokemonHPIV(Player*, size_t);
	bool SetPlayerPokemonAttackIV(Player*, size_t);
	bool SetPlayerPokemonDefenseIV(Player*, size_t);
	bool SetPlayerPokemonSpecialAttackIV(Player*, size_t);
	bool SetPlayerPokemonSpecialDefenseIV(Player*, size_t);
	bool SetPlayerPokemonSpeedIV(Player*, size_t);

	bool SetPlayerPokemonHPEV(Player*, size_t);
	bool SetPlayerPokemonAttackEV(Player*, size_t);
	bool SetPlayerPokemonDefenseEV(Player*, size_t);
	bool SetPlayerPokemonSpecialAttackEV(Player*, size_t);
	bool SetPlayerPokemonSpecialDefenseEV(Player*, size_t);
	bool SetPlayerPokemonSpeedEV(Player*, size_t);

	bool RunMenu();
	void ChangePlayerOneName();
	void EditPlayerOnePokemon();
	void ChangePlayerOneType();
	void PrintPlayerOneType();
	void ChangePlayerTwoName();
	void EditPlayerTwoPokemon();
	void ChangePlayerTwoType();
	void PrintPlayerTwoType();

	bool AddPokemon(Player*);
	bool ChangePokemon(Player*);
	bool DeletePokemon(Player*);
	bool EditPokemonStats(Player*);
	bool EditPokemonMoves(Player*);

	bool EditChosenPokemonStats(Player*, size_t);
	bool EditChosenPokemonMoves(Player*, size_t);

	bool AddMove(Player*, size_t);
	bool ChangeMove(Player*, size_t);

	void SaveYourParty(std::array<Player*, 2>);
	void SavePlayerNumberParty(std::array<Player*, 2>, int);

	void LoadYourParty(std::array<Player*, 2>);
	void LoadPlayerNumberParty(std::array<Player*, 2>, int);

	bool IsPokemonSetupIncomplete();

	void SetDefaultPokemon();
};