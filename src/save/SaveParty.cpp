#include <iostream>

#include "../entities/Player.h"

#include "SaveParty.h"

SaveParty::SaveParty(std::array<Player*, 2> players, int numPlayer) : num(numPlayer)
{
	SaveParty::players = players;
}

void SaveParty::SaveFile(std::ofstream& outFile)
{
	if (num == 0)
	{
		outFile.open("PlayerOneSave.txt", std::ios::trunc);
	}

	else if (num == 1)
	{
		outFile.open("PlayerTwoSave.txt", std::ios::trunc);
	}
	else
	{
		std::cerr << "Oops\n\n";
		return;
	}
}

void SaveParty::SavePlayerName(std::ofstream& outFile)
{
	outFile << "Player Name: " << players[num]->GetPlayerNameView() << "\n\n";
}

void SaveParty::SavePokemonName(std::ofstream& outFile, int i)
{
	outFile << "Pokemon " << i << ":\n";

	if (!players[num]->GetBelt(i).HasPokemon())
	{
		outFile << "-0\n\n";
		++numberOfPokemon;
	}

	if (players[num]->GetBelt(i).HasNickname())
	{
		outFile << players[num]->GetBelt(i).GetNickname()
			<< " (" << players[num]->GetBelt(i).GetPokemonNameView() << ")\n";
	}
	else
	{
		outFile << players[num]->GetBelt(i).GetPokemonNameView() << "\n";
	}
}

void SaveParty::SavePokemonLevel(std::ofstream& outFile, int i)
{
	outFile << "Level: " << players[num]->GetBelt(i).GetLevel() << "\n";
}

void SaveParty::SavePokemonEVs(std::ofstream& outFile, int i)
{
	outFile << "EVs: "
		<< players[num]->GetBelt(i).GetHPEV()
		<< " HP " << " / "
		<< players[num]->GetBelt(i).GetAttackEV()
		<< " Atk " << " / "
		<< players[num]->GetBelt(i).GetDefenseEV()
		<< " Def " << " / "
		<< players[num]->GetBelt(i).GetSpecialAttackEV()
		<< " SpA " << " / "
		<< players[num]->GetBelt(i).GetSpecialDefenseEV()
		<< " SpD " << " / "
		<< players[num]->GetBelt(i).GetSpeedEV()
		<< " Spe\n";
}

void SaveParty::SavePokemonIVs(std::ofstream& outFile, int i)
{
	outFile << "IVs: "
		<< players[num]->GetBelt(i).GetHPIV()
		<< " HP " << " / "
		<< players[num]->GetBelt(i).GetAttackIV()
		<< " Atk " << " / "
		<< players[num]->GetBelt(i).GetDefenseIV()
		<< " Def " << " / "
		<< players[num]->GetBelt(i).GetSpecialAttackIV()
		<< " SpA " << " / "
		<< players[num]->GetBelt(i).GetSpecialDefenseIV()
		<< " SpD " << " / "
		<< players[num]->GetBelt(i).GetSpeedIV()
		<< " Spe\n";
}

void SaveParty::SavePokemonMoves(std::ofstream& outFile, int i)
{
	for (int j = 1; j <= numberOfMoves; ++j)
	{
		if (!players[num]->GetBelt(i).HasMove(j) && j <= numberOfMoves)
		{
			++numberOfMoves;
			continue;
		}

		if (players[num]->GetBelt(i).HasMove(j))
		{
			outFile << "- " << players[num]->GetBelt(i).GetMove(j).GetName() << '\n';
			if (j == numberOfMoves && i != numberOfPokemon)
			{
				outFile << '\n';
			}
		}
	}
}

void SaveParty::Save()
{
	SaveFile(outFile);

	SavePlayerName(outFile);

	numberOfPokemon = players[num]->GetPokemonCount();

	for (int i = 1; i <= numberOfPokemon; ++i)
	{
		SavePokemonName(outFile, i);

		SavePokemonLevel(outFile, i);

		SavePokemonEVs(outFile, i);

		SavePokemonIVs(outFile, i);

		numberOfMoves = players[num]->GetBelt(i).GetMoveCount();

		if (numberOfMoves <= 0 && (i != numberOfPokemon))
		{
			outFile << "\n";
		}

		else
		{
			SavePokemonMoves(outFile, i);
		}
	}

	std::cout << "Saved Successfully!\n\n";

	outFile.close();
}