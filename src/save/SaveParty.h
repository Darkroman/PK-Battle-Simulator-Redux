#pragma once

#include <fstream>
#include <array>

class Player;

class SaveParty
{
public:

private:
	std::ofstream outFile;
	int num{};
	int numberOfPokemon{};
	int numberOfMoves{};
	size_t beltpos{};
	std::streampos filepos;

	std::array<Player*, 2> players;

public:
	explicit SaveParty(std::array<Player*, 2>, int);
	void SaveFile(std::ofstream&);
	void SavePlayerName(std::ofstream&);
	void SavePokemonName(std::ofstream&, int);
	void SavePokemonLevel(std::ofstream&, int);
	void SavePokemonEVs(std::ofstream&, int);
	void SavePokemonIVs(std::ofstream&, int);
	void SavePokemonMoves(std::ofstream&, int);
	void Save();
};