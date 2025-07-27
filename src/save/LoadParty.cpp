#include <iostream>
#include <sstream>
#include <algorithm>

#include "LoadParty.h"

LoadParty::LoadParty(std::array<Player*, 2> players, int numPlayer) : num(numPlayer)
{
	LoadParty::players = players;
}

void LoadParty::LoadFile(std::ifstream& inFile)
{
	if (num == 0)
	{
		inFile.open("PlayerOneSave.txt");
	}

	if (num == 1)
	{
		inFile.open("PlayerTwoSave.txt");
	}
}

void LoadParty::LoadPlayerName(std::ifstream& inFile)
{
	for (int i = players[num]->GetPokemonCount(); i > 0; --i)
	{
		players[num]->DecrementPokemonCount();
	}

	std::getline(inFile, line, ':');
	inFile >> std::ws;

	std::getline(inFile, line);
	players[num]->SetName(line);
	inFile >> std::ws;
}

int LoadParty::LoadPokemonName(std::ifstream& inFile)
{
	std::getline(inFile, line);
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
		if (players[num]->GetBelt(beltpos)->HasPokemon())
		{
			players[num]->GetBelt(beltpos)->ReleasePokemon();
		}

		inFile >> std::ws;
		return -1;
	}

	size_t begpos = line.find_first_of('(');
	size_t endpos = line.find_last_of(')');

	if (begpos == std::string::npos)
	{
		players[num]->GetBelt(beltpos)->SetPokemon(line);
		players[num]->IncrementPokemonCount();
	}
	else
	{
		std::istringstream iss(line);
		std::string nick;

		std::getline(iss, nick, ' ');

		std::string pokemonName = line.substr(begpos + 1, endpos - begpos - 1);

		players[num]->GetBelt(beltpos)->SetPokemon(pokemonName);
		players[num]->GetBelt(beltpos)->SetNickname(nick);

		players[num]->IncrementPokemonCount();
	}

	inFile >> std::ws;

	return 1;
}

void LoadParty::LoadPokemonLevel(std::ifstream& inFile)
{
	std::getline(inFile, line);
	line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);

	size_t endpos = line.length();
	std::string levelNum = line.substr(7, endpos);
	players[num]->GetBelt(beltpos)->SetLevel(std::stoi(levelNum));
}

void LoadParty::LoadPokemonEVs(std::ifstream& inFile)
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
				players[num]->GetBelt(beltpos)->SetHPEV(std::stoi(substr));

				posHP = std::string::npos;
			}

			else if (posAtk != std::string::npos)
			{
				substr = LoadPosition(section, substr, posAtk, "Atk");
				players[num]->GetBelt(beltpos)->SetAttackEV(std::stoi(substr));

				posAtk = std::string::npos;
			}

			else if (posDef != std::string::npos)
			{
				substr = LoadPosition(section, substr, posDef, "Def");
				players[num]->GetBelt(beltpos)->SetDefenseEV(std::stoi(substr));

				posDef = std::string::npos;
			}

			else if (posSpA != std::string::npos)
			{
				substr = LoadPosition(section, substr, posSpA, "SpA");
				players[num]->GetBelt(beltpos)->SetSpecialAttackEV(std::stoi(substr));

				posSpA = std::string::npos;
			}

			else if (posSpD != std::string::npos)
			{
				substr = LoadPosition(section, substr, posSpD, "SpD");
				players[num]->GetBelt(beltpos)->SetSpecialDefenseEV(std::stoi(substr));

				posSpD = std::string::npos;
			}

			else if (posSpe != std::string::npos)
			{
				substr = LoadPosition(section, substr, posSpe, "Spe");
				players[num]->GetBelt(beltpos)->SetSpeedEV(std::stoi(substr));

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

void LoadParty::LoadPokemonIVs(std::ifstream& inFile)
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
				players[num]->GetBelt(beltpos)->SetHPIV(std::stoi(substr));

				posHP = std::string::npos;
			}

			else if (posAtk != std::string::npos)
			{
				substr = LoadPosition(section, substr, posAtk, "Atk");
				players[num]->GetBelt(beltpos)->SetAttackIV(std::stoi(substr));

				posAtk = std::string::npos;
			}

			else if (posDef != std::string::npos)
			{
				substr = LoadPosition(section, substr, posDef, "Def");
				players[num]->GetBelt(beltpos)->SetDefenseIV(std::stoi(substr));

				posDef = std::string::npos;
			}

			else if (posSpA != std::string::npos)
			{
				substr = LoadPosition(section, substr, posSpA, "SpA");
				players[num]->GetBelt(beltpos)->SetSpecialAttackIV(std::stoi(substr));

				posSpA = std::string::npos;
			}

			else if (posSpD != std::string::npos)
			{
				substr = LoadPosition(section, substr, posSpD, "SpD");
				players[num]->GetBelt(beltpos)->SetSpecialDefenseIV(std::stoi(substr));

				posSpD = std::string::npos;
			}

			else if (posSpe != std::string::npos)
			{
				substr = LoadPosition(section, substr, posSpe, "Spe");
				players[num]->GetBelt(beltpos)->SetSpeedIV(std::stoi(substr));

				posSpe = std::string::npos;
			}

		} while (i < count);
	}

	else
	{
		inFile.seekg(filepos);
	}
}

void LoadParty::LoadPokemonMoves(std::ifstream& inFile)
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
			players[num]->GetBelt(beltpos)->SetMove(moveNum, moveName);
			++moveNum;
			players[num]->GetBelt(beltpos)->IncrementMoveCount();
		}

		if (inFile.eof())
		{
			return;
		}
	}
}

void LoadParty::Load()
{
	LoadFile(inFile);

	if (!inFile.is_open())
	{
		std::cerr << "File does not exist! Try saving a party beforehand.\n\n";
		return;
	}

	LoadPlayerName(inFile);

	while (!inFile.eof())
	{
		int check{ 0 };
		if (beltpos > 6)
		{
			return;
		}
		else
		{
			check = LoadPokemonName(inFile);
		}
		

		if (check < 0)
		{
			continue;
		}
		else
		{
			LoadPokemonLevel(inFile);
			LoadPokemonEVs(inFile);
			LoadPokemonIVs(inFile);
			LoadPokemonMoves(inFile);
		}
	}

	std::cout << "File Loaded Successfully!\n\n";
	
	inFile.close();
}

std::string& LoadPosition(const std::string& section, std::string& substr, size_t posStat, std::string_view stat)
{
	posStat = section.find_first_of(stat);

	substr = section.substr(0, posStat - 1);
	return substr;
}
