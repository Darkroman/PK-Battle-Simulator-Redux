#include <iostream>
#include <sstream>
#include <algorithm>

#include "LoadParty.h"

#include "../entities/Player.h"

LoadParty::LoadParty(Player* player, std::filesystem::path path) : player(player)
{
	inFile.open(path);
}

void LoadParty::Load()
{
	if (!inFile.is_open())
	{
		std::cerr << "File does not exist!\n\n";
		return;
	}

	for (auto& pokemon : player->GetBeltArray())
	{
		if (pokemon.HasPokemon())
		{
			pokemon.ReleasePokemon();
		}
	}

	LoadPlayerName();

	while (std::getline(inFile, line))
	{
		int check{ 0 };
		if (beltpos > 6)
		{
			return;
		}
		else
		{
			check = LoadPokemonName();
		}


		if (check < 0)
		{
			continue;
		}
		else
		{
			LoadPokemonLevel();
			LoadPokemonEVs();
			LoadPokemonIVs();
			LoadPokemonMoves();
		}
	}

	std::cout << "File Loaded Successfully!\n\n";

	inFile.close();
}

void LoadParty::LoadPlayerName()
{
	for (int i = player->GetPokemonCount(); i > 0; --i)
	{
		player->DecrementPokemonCount();
	}

	std::getline(inFile, line, ':');
	inFile >> std::ws;

	std::getline(inFile, line);
	player->SetName(line);
	inFile >> std::ws;
}

int LoadParty::LoadPokemonName()
{
	std::string c = line.substr(8);
	beltpos = std::stoi(c);

	if (beltpos > 6)
	{
		std::cout << "You cannot have more than 6 Pokemon!\n";
		return -1;
	}

	std::getline(inFile, line);
	line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);

	if (line.compare("-0") == 0)
	{
		if (player->GetBelt(beltpos).HasPokemon())
		{
			player->GetBelt(beltpos).ReleasePokemon();
		}

		inFile >> std::ws;
		return -1;
	}

	size_t begpos = line.find_first_of('(');
	size_t endpos = line.find_last_of(')');

	if (begpos == std::string::npos)
	{
		player->GetBelt(beltpos).SetPokemon(line);
		player->IncrementPokemonCount();
	}
	else
	{
		std::istringstream iss(line);
		std::string nick;

		std::getline(iss, nick, ' ');

		std::string pokemonName = line.substr(begpos + 1, endpos - begpos - 1);

		player->GetBelt(beltpos).SetPokemon(pokemonName);
		player->GetBelt(beltpos).SetNickname(nick);

		player->IncrementPokemonCount();
	}

	inFile >> std::ws;

	return 1;
}

void LoadParty::LoadPokemonLevel()
{
	std::getline(inFile, line);
	line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);

	size_t endpos = line.length();
	std::string levelNum = line.substr(7, endpos);
	player->GetBelt(beltpos).SetLevel(std::stoi(levelNum));
}

void LoadParty::LoadPokemonEVs()
{
	filepos = inFile.tellg();

	std::getline(inFile, line);
	line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);

	std::istringstream iss(line);

	std::string section;
	std::getline(iss, section, ':');
	iss >> std::ws;

	if (section.compare("EVs") == 0)
	{
		auto count = std::ranges::count(line, '/');

		size_t posHP = line.find("HP");
		size_t posAtk = line.find("Atk");
		size_t posDef = line.find("Def");
		size_t posSpA = line.find("SpA");
		size_t posSpD = line.find("SpD");
		size_t posSpe = line.find("Spe");

		int i = -1;
		do
		{
			++i;

			if (count == 0)
			{
				std::getline(iss, section);
				iss >> std::ws;
			}
			else if (count > 0)
			{
				std::getline(iss, section, '/');
				iss >> std::ws;
			}

			std::string substr{};

			if (posHP != std::string::npos)
			{
				substr = LoadPosition(section, substr, posHP, "HP");
				player->GetBelt(beltpos).SetHPEV(std::stoi(substr));

				posHP = std::string::npos;
			}

			else if (posAtk != std::string::npos)
			{
				substr = LoadPosition(section, substr, posAtk, "Atk");
				player->GetBelt(beltpos).SetAttackEV(std::stoi(substr));

				posAtk = std::string::npos;
			}

			else if (posDef != std::string::npos)
			{
				substr = LoadPosition(section, substr, posDef, "Def");
				player->GetBelt(beltpos).SetDefenseEV(std::stoi(substr));

				posDef = std::string::npos;
			}

			else if (posSpA != std::string::npos)
			{
				substr = LoadPosition(section, substr, posSpA, "SpA");
				player->GetBelt(beltpos).SetSpecialAttackEV(std::stoi(substr));

				posSpA = std::string::npos;
			}

			else if (posSpD != std::string::npos)
			{
				substr = LoadPosition(section, substr, posSpD, "SpD");
				player->GetBelt(beltpos).SetSpecialDefenseEV(std::stoi(substr));

				posSpD = std::string::npos;
			}

			else if (posSpe != std::string::npos)
			{
				substr = LoadPosition(section, substr, posSpe, "Spe");
				player->GetBelt(beltpos).SetSpeedEV(std::stoi(substr));

				posSpe = std::string::npos;
			}

		} while (i < count);
	}

	else
	{
		inFile.seekg(filepos);
		return;
	}
}

void LoadParty::LoadPokemonIVs()
{
	filepos = inFile.tellg();

	std::getline(inFile, line);
	line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);

	std::istringstream iss(line);

	std::string section{};
	std::getline(iss, section, ':');
	iss >> std::ws;

	if (section.compare("IVs") == 0)
	{
		auto count = std::ranges::count(line, '/');

		size_t posHP = line.find("HP");
		size_t posAtk = line.find("Atk");
		size_t posDef = line.find("Def");
		size_t posSpA = line.find("SpA");
		size_t posSpD = line.find("SpD");
		size_t posSpe = line.find("Spe");

		int i = -1;
		do
		{
			++i;

			if (count == 0)
			{
				std::getline(iss, section);
				iss >> std::ws;
			}
			else if (count > 0)
			{
				std::getline(iss, section, '/');
				iss >> std::ws;
			}

			std::string substr{};

			if (posHP != std::string::npos)
			{
				substr = LoadPosition(section, substr, posHP, "HP");
				player->GetBelt(beltpos).SetHPIV(std::stoi(substr));

				posHP = std::string::npos;
			}
			else
			{
				player->GetBelt(beltpos).SetHPIV(31);
			}

			if (posAtk != std::string::npos)
			{
				substr = LoadPosition(section, substr, posAtk, "Atk");
				player->GetBelt(beltpos).SetAttackIV(std::stoi(substr));

				posAtk = std::string::npos;
			}
			else
			{
				player->GetBelt(beltpos).SetAttackIV(31);
			}

			if (posDef != std::string::npos)
			{
				substr = LoadPosition(section, substr, posDef, "Def");
				player->GetBelt(beltpos).SetDefenseIV(std::stoi(substr));

				posDef = std::string::npos;
			}
			else
			{
				player->GetBelt(beltpos).SetDefenseIV(31);
			}

			if (posSpA != std::string::npos)
			{
				substr = LoadPosition(section, substr, posSpA, "SpA");
				player->GetBelt(beltpos).SetSpecialAttackIV(std::stoi(substr));

				posSpA = std::string::npos;
			}
			else
			{
				player->GetBelt(beltpos).SetSpecialAttackIV(31);
			}

			if (posSpD != std::string::npos)
			{
				substr = LoadPosition(section, substr, posSpD, "SpD");
				player->GetBelt(beltpos).SetSpecialDefenseIV(std::stoi(substr));

				posSpD = std::string::npos;
			}
			else
			{
				player->GetBelt(beltpos).SetSpecialDefenseIV(31);
			}

			if (posSpe != std::string::npos)
			{
				substr = LoadPosition(section, substr, posSpe, "Spe");
				player->GetBelt(beltpos).SetSpeedIV(std::stoi(substr));

				posSpe = std::string::npos;
			}
			else
			{
				player->GetBelt(beltpos).SetSpeedIV(31);
			}

		} while (i < count);
	}

	else
	{
		inFile.seekg(filepos);
		player->GetBelt(beltpos).SetHPIV(31);
		player->GetBelt(beltpos).SetAttackIV(31);
		player->GetBelt(beltpos).SetDefenseIV(31);
		player->GetBelt(beltpos).SetSpecialAttackIV(31);
		player->GetBelt(beltpos).SetSpecialDefenseIV(31);
		player->GetBelt(beltpos).SetSpeedIV(31);
	}
}

void LoadParty::LoadPokemonMoves()
{
	int moveNum{ 1 };
	while (!line.empty())
	{
		std::getline(inFile, line);
		line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);

		if (line.empty())
		{
			return;
		}

		size_t found = line.find("-");

		if (found != std::string::npos)
		{
			std::string moveName = line.substr(2);
			player->GetBelt(beltpos).SetMove(moveNum, moveName);
			++moveNum;
			player->GetBelt(beltpos).IncrementMoveCount();
		}

		if (inFile.eof())
		{
			return;
		}
	}
}

std::string& LoadPosition(const std::string& section, std::string& substr, size_t posStat, std::string_view stat)
{
	posStat = section.find(stat);

	substr = section.substr(0, posStat - 1);
	return substr;
}