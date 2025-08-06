#pragma once

#include <fstream>
#include <array>

class Player;

class LoadParty
{
public:
	explicit LoadParty(std::array<Player*, 2>, int);
	void LoadFile(std::ifstream&);
	void LoadPlayerName(std::ifstream&);
	int  LoadPokemonName(std::ifstream&);
	void LoadPokemonLevel(std::ifstream&);
	void LoadPokemonEVs(std::ifstream&);
	void LoadPokemonIVs(std::ifstream&);
	void LoadPokemonMoves(std::ifstream&);
	void Load();

private:
	std::array<Player*, 2> players;
	std::ifstream inFile;
	std::string line;
	int num{};
	size_t beltpos{};
	std::streampos filepos;
};

std::string& LoadPosition(const std::string&, std::string&, size_t, std::string_view);
