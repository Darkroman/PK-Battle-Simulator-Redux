#pragma once

#include <fstream>
#include <filesystem>

class Player;

class LoadParty
{
public:
	explicit LoadParty(Player* player, std::filesystem::path file);
	void Load();
	void LoadPlayerName();
	int  LoadPokemonName();
	void LoadPokemonLevel();
	void LoadPokemonEVs();
	void LoadPokemonIVs();
	void LoadPokemonMoves();

private:
	Player* player{};
	std::ifstream inFile{};
	std::string line{};
	int num{};
	size_t beltpos{};
	std::streampos filepos{};
};

std::string& LoadPosition(const std::string&, std::string&, size_t, std::string_view);
