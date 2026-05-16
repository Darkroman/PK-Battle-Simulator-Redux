#include <iostream>

#include "SaveParty.h"

#include "../entities/Player.h"

SaveParty::SaveParty(Player* player, std::filesystem::path path) : player(player)
{
	outFile.open(path);
}

void SaveParty::Save()
{
	SavePlayerName();

	numberOfPokemon = player->GetPokemonCount();

	for (int i = 1; i <= numberOfPokemon; ++i)
	{
		SavePokemonName(i);

		SavePokemonLevel(i);

		SavePokemonEVs(i);

		SavePokemonIVs(i);

		numberOfMoves = player->GetBelt(i).GetMoveCount();

		if (numberOfMoves <= 0 && (i != numberOfPokemon))
		{
			outFile << "\n";
		}

		else
		{
			SavePokemonMoves(i);
		}
	}

	std::cout << "Saved Successfully!\n\n";

	outFile.close();
}

void SaveParty::SavePlayerName()
{
	outFile << "Player Name: " << player->GetPlayerNameView() << "\n\n";
}

void SaveParty::SavePokemonName(int i)
{
	outFile << "Pokemon " << i << ":\n";

	if (!player->GetBelt(i).HasPokemon())
	{
		outFile << "-0\n\n";
		++numberOfPokemon;
	}

	if (player->GetBelt(i).HasNickname())
	{
		outFile << player->GetBelt(i).GetNickname()
			<< " (" << player->GetBelt(i).GetPokemonNameView() << ")\n";
	}
	else
	{
		outFile << player->GetBelt(i).GetPokemonNameView() << "\n";
	}
}

void SaveParty::SavePokemonLevel(int i)
{
	outFile << "Level: " << player->GetBelt(i).GetLevel() << "\n";
}

void SaveParty::SavePokemonEVs(int i)
{
	outFile << "EVs: "
		<< player->GetBelt(i).GetHPEV()
		<< " HP " << " / "
		<< player->GetBelt(i).GetAttackEV()
		<< " Atk " << " / "
		<< player->GetBelt(i).GetDefenseEV()
		<< " Def " << " / "
		<< player->GetBelt(i).GetSpecialAttackEV()
		<< " SpA " << " / "
		<< player->GetBelt(i).GetSpecialDefenseEV()
		<< " SpD " << " / "
		<< player->GetBelt(i).GetSpeedEV()
		<< " Spe\n";
}

void SaveParty::SavePokemonIVs(int i)
{
	outFile << "IVs: "
		<< player->GetBelt(i).GetHPIV()
		<< " HP " << " / "
		<< player->GetBelt(i).GetAttackIV()
		<< " Atk " << " / "
		<< player->GetBelt(i).GetDefenseIV()
		<< " Def " << " / "
		<< player->GetBelt(i).GetSpecialAttackIV()
		<< " SpA " << " / "
		<< player->GetBelt(i).GetSpecialDefenseIV()
		<< " SpD " << " / "
		<< player->GetBelt(i).GetSpeedIV()
		<< " Spe\n";
}

void SaveParty::SavePokemonMoves(int i)
{
	for (int j = 1; j <= numberOfMoves; ++j)
	{
		if (!player->GetBelt(i).HasMove(j) && j <= numberOfMoves)
		{
			++numberOfMoves;
			continue;
		}

		if (player->GetBelt(i).HasMove(j))
		{
			outFile << "- " << player->GetBelt(i).GetMove(j).GetName() << '\n';
			if (j == numberOfMoves && i != numberOfPokemon)
			{
				outFile << '\n';
			}
		}
	}
}