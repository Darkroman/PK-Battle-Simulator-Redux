#pragma once

#include <fstream>
#include <filesystem>

class Player;

class SaveParty
{
public:
	explicit SaveParty(Player* player, std::filesystem::path);
	void Save();
	void SavePlayerName();
	void SavePokemonName(int);
	void SavePokemonLevel(int);
	void SavePokemonEVs(int);
	void SavePokemonIVs(int);
	void SavePokemonMoves(int);

private:
	Player* player{};
	std::ofstream outFile{};
	int num{};
	int numberOfPokemon{};
	int numberOfMoves{};
	size_t beltpos{};
	std::streampos filepos;
};