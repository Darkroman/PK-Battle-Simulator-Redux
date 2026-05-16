#pragma once

#include <vector>
#include <array>
#include <memory>
#include <filesystem>

#include "../common/PartyEditResults.h"

class BattlePokemon;
class Player;

class Menu
{
private:
	std::vector<std::unique_ptr<Player>>& playerStorage;
	std::array<Player*, 2> players;
	std::string path = "./SavedParties";

public:
	Menu() = delete;
	explicit Menu(std::vector<std::unique_ptr<Player>>&);

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

	void SaveYourParty();
	bool SavePlayerParty(size_t choice);

	void LoadYourParty();
	bool LoadToPlayer(std::filesystem::path);
	std::vector<std::filesystem::path> ListSavedParties();
	std::vector<std::filesystem::path> GetSavedParties();

	bool IsPokemonSetupIncomplete();

	void SetDefaultPokemon();
};