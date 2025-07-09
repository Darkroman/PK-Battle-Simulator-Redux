#include "Menu.h"
#include "../data/InputValidation.h"
#include "../battle/RandomEngine.h"
#include "../entities/ai strategies/EasyAIStrategy.h"

Menu::Menu(std::vector<std::unique_ptr<Player>>& playerStorage, RandomEngine& rng)
	: playerStorage(playerStorage), m_rng(rng)
{
	players[0] = playerStorage[0].get();
	players[1] = playerStorage[1].get();

#if !defined NDEBUG
	SetDefaultPokemon();
#endif
}

void Menu::SetPlayerPokemon(Player* player, size_t beltslot, size_t pknum)
{
	player->GetBelt(beltslot)->SetPokemon(pknum);
}

void Menu::SetPlayerPokemon(Player* player, size_t beltslot, std::string_view pkname)
{
	player->GetBelt(beltslot)->SetPokemon(pkname);
}

void Menu::SetPlayerPokemonMove(Player* player, size_t beltslot, size_t moveslot, size_t movenum)
{
	player->GetBelt(beltslot)->SetMove(moveslot, movenum);
}

void Menu::SetPlayerPokemonMove(Player* player, size_t beltslot, size_t moveslot, std::string_view movename)
{
	player->GetBelt(beltslot)->SetMove(moveslot, movename);
}

bool Menu::SetPlayerPokemonNickname(Player* player, size_t beltslot)
{
	std::cout << "Enter a nickname, or just 0 to erase nickname: ";
	std::string nickname{};
	std::cin.ignore();
	std::getline(std::cin, nickname);

	if (nickname == "00")
	{
		return true;
	}

	player->GetBelt(beltslot)->SetNickname(nickname);

	return false;
}

bool Menu::SetPlayerPokemonLevel(Player* player, size_t beltslot)
{
	bool exit = false;
	while (exit == false)
	{
		std::cout << "Set level 1 to 100: ";
		std::string input{};
		std::cin >> input;

		if (input == "00")
		{
			return true;
		}

		if (!IsDigits(input))
		{
			std::cout << "Invalid input! Please only enter a number.\n";
			continue;
		}

		int value{ std::stoi(input) };

		if (value < 1 || value > 100)
		{
			std::cout << "Invalid input! Enter a number between 1 and 100.\n";
			continue;
		}

		else
		{
			player->GetBelt(beltslot)->SetLevel(value);
			return false;
		}
	}
	return exit;
}

bool Menu::SetPlayerPokemonHPIV(Player* player, size_t beltslot)
{
	bool exit = false;
	while (exit == false)
	{
		player->GetBelt(beltslot)->DisplayIVs();
		std::cout << "Set HP IV between 0 and 31: ";
		std::string input{};
		std::cin >> input;

		if (input == "00")
		{
			return true;
		}

		if (!IsDigits(input))
		{
			std::cout << "Invalid input! Please only enter a number.\n";
			continue;
		}

		int value{ std::stoi(input) };

		if (value < 0 || value > 31)
		{
			std::cout << "Invalid input! Please enter a number between 0 and 31.\n";
			continue;
		}

		else
		{
			player->GetBelt(beltslot)->SetHPIV(value);
			return false;
		}
	}
	return exit;
}

bool Menu::SetPlayerPokemonAttackIV(Player* player, size_t beltslot)
{
	bool exit = false;
	while (exit == false)
	{
		player->GetBelt(beltslot)->DisplayIVs();
		std::cout << "Set Attack IV between 0 and 31: ";
		std::string input{};
		std::cin >> input;

		if (input == "00")
		{
			return true;
		}

		if (!IsDigits(input))
		{
			std::cout << "Invalid input! Please only enter a number.\n";
			continue;
		}

		int value{ std::stoi(input) };

		if (value < 0 || value > 31)
		{
			std::cout << "Invalid input! Please enter a number between 0 and 31.\n";
			continue;
		}

		else
		{
			player->GetBelt(beltslot)->SetAttackIV(value);
			return false;
		}
	}
	return exit;
}

bool Menu::SetPlayerPokemonDefenseIV(Player* player, size_t beltslot)
{
	bool exit = false;
	while (exit == false)
	{
		player->GetBelt(beltslot)->DisplayIVs();
		std::cout << "Set Defense IV between 0 and 31: ";
		std::string input{};
		std::cin >> input;

		if (input == "00")
		{
			return true;
		}

		if (!IsDigits(input))
		{
			std::cout << "Invalid input! Please only enter a number.\n";
			continue;
		}

		int value{ std::stoi(input) };

		if (value < 0 || value > 31)
		{
			std::cout << "Invalid input! Please enter a number between 0 and 31.\n";
			continue;
		}

		else
		{
			player->GetBelt(beltslot)->SetDefenseIV(value);
			return false;
		}
	}
	return exit;
}

bool Menu::SetPlayerPokemonSpecialAttackIV(Player* player, size_t beltslot)
{
	bool exit = false;
	while (exit == false)
	{
		player->GetBelt(beltslot)->DisplayIVs();
		std::cout << "Set Special Attack IV between 0 and 31: ";
		std::string input{};
		std::cin >> input;

		if (input == "00")
		{
			return true;
		}

		if (!IsDigits(input))
		{
			std::cout << "Invalid input! Please only enter a number.\n";
			continue;
		}

		int value{ std::stoi(input) };

		if (value < 0 || value > 31)
		{
			std::cout << "Invalid input! Please enter a number between 0 and 31.\n";
			continue;
		}

		else
		{
			player->GetBelt(beltslot)->SetSpecialAttackIV(value);
			return false;
		}
	}
	return exit;
}

bool Menu::SetPlayerPokemonSpecialDefenseIV(Player* player, size_t beltslot)
{
	bool exit = false;
	while (exit == false)
	{
		player->GetBelt(beltslot)->DisplayIVs();
		std::cout << "Set Special Defense IV between 0 and 31: ";
		std::string input{};
		std::cin >> input;

		if (input == "00")
		{
			return true;
		}

		if (!IsDigits(input))
		{
			std::cout << "Invalid input! Please only enter a number.\n";
			continue;
		}

		int value{ std::stoi(input) };

		if (value < 0 || value > 31)
		{
			std::cout << "Invalid input! Please enter a number between 0 and 31.\n";
			continue;
		}

		else
		{
			player->GetBelt(beltslot)->SetSpecialDefenseIV(value);
			return false;
		}
	}
	return exit;
}

bool Menu::SetPlayerPokemonSpeedIV(Player* player, size_t beltslot)
{
	bool exit = false;
	while (exit == false)
	{
		player->GetBelt(beltslot)->DisplayIVs();
		std::cout << "Set Speed IV between 0 and 31: ";
		std::string input{};
		std::cin >> input;

		if (input == "00")
		{
			return true;
		}

		if (!IsDigits(input))
		{
			std::cout << "Invalid input! Please only enter a number.\n";
			continue;
		}

		int value{ std::stoi(input) };

		if (value < 0 || value > 31)
		{
			std::cout << "Invalid input! Please enter a number between 0 and 31.\n";
			continue;
		}

		else
		{
			player->GetBelt(beltslot)->SetSpeedIV(value);
			return false;
		}
	}
	return exit;
}

bool Menu::SetPlayerPokemonHPEV(Player* player, size_t beltslot)
{
	bool exit = false;
	bool evCheck = false;
	while (exit == false && evCheck == false)
	{
		player->GetBelt(beltslot)->DisplayEVs();
		std::cout << player->GetBelt(beltslot)->GetPokemonNameView() << " currently has " << player->GetBelt(beltslot)->GetTotalEVs() << " EV points used.\n";
		std::cout << "Set HP EV between 0 and 252: ";
		std::string input{};
		std::cin >> input;

		if (input == "00")
		{
			return true;
		}

		if (!IsDigits(input))
		{
			std::cout << "Invalid input! Please only enter a number.\n\n";
			continue;
		}

		int value{ std::stoi(input) };

		if (value < 0 || value > 252)
		{
			std::cout << "Invalid number. Must be a number between 0 and 252.\n\n";
			continue;
		}

		evCheck = player->GetBelt(beltslot)->SetHPEV(value);
	}
	return exit;
}

bool Menu::SetPlayerPokemonAttackEV(Player* player, size_t beltslot)
{
	bool exit = false;
	bool evCheck = false;
	while (exit == false && evCheck == false)
	{
		player->GetBelt(beltslot)->DisplayEVs();
		std::cout << player->GetBelt(beltslot)->GetPokemonNameView() << " currently has " << player->GetBelt(beltslot)->GetTotalEVs() << " EV points used.\n";
		std::cout << "Set Attack EV between 0 and 252: ";
		std::string input{};
		std::cin >> input;

		if (input == "00")
		{
			return true;
		}

		if (!IsDigits(input))
		{
			std::cout << "Invalid input! Please only enter a number.\n";
			continue;
		}

		int value{ std::stoi(input) };

		if (value < 0 || value > 252)
		{
			std::cout << "Invalid number. Must be a number between 0 and 252.\n";
			continue;
		}

		evCheck = player->GetBelt(beltslot)->SetAttackEV(value);
	}
	return exit;
}

bool Menu::SetPlayerPokemonDefenseEV(Player* player, size_t beltslot)
{
	bool exit = false;
	bool evCheck = false;
	while (exit == false && evCheck == false)
	{
		player->GetBelt(beltslot)->DisplayEVs();
		std::cout << player->GetBelt(beltslot)->GetPokemonNameView() << " currently has " << player->GetBelt(beltslot)->GetTotalEVs() << " EV points used.\n";
		std::cout << "Set Defense EV between 0 and 252: ";
		std::string input{};
		std::cin >> input;

		if (input == "00")
		{
			return true;
		}

		if (!IsDigits(input))
		{
			std::cout << "Invalid input! Please only enter a number.\n";
			continue;
		}

		int value{ std::stoi(input) };

		if (value < 0 || value > 252)
		{
			std::cout << "Invalid number. Must be a number between 0 and 252.\n";
			continue;
		}
		
		evCheck = player->GetBelt(beltslot)->SetDefenseEV(value);
	}
	return exit;
}

bool Menu::SetPlayerPokemonSpecialAttackEV(Player* player, size_t beltslot)
{
	bool exit = false;
	bool evCheck = false;
	while (exit == false && evCheck == false)
	{
		player->GetBelt(beltslot)->DisplayEVs();
		std::cout << player->GetBelt(beltslot)->GetPokemonNameView() << " currently has " << player->GetBelt(beltslot)->GetTotalEVs() << " EV points used.\n";
		std::cout << "Set Special Attack EV between 0 and 252: ";
		std::string input{};
		std::cin >> input;

		if (input == "00")
		{
			return true;
		}

		if (!IsDigits(input))
		{
			std::cout << "Invalid input! Please only enter a number.\n";
			continue;
		}

		int value{ std::stoi(input) };

		if (value < 0 || value > 252)
		{
			std::cout << "Invalid number. Must be a number between 0 and 252.\n";
			continue;
		}

		evCheck = player->GetBelt(beltslot)->SetSpecialAttackEV(value);
	}
	return exit;
}

bool Menu::SetPlayerPokemonSpecialDefenseEV(Player* player, size_t beltslot)
{
	bool exit = false;
	bool evCheck = false;
	while (exit == false && evCheck == false)
	{
		player->GetBelt(beltslot)->DisplayEVs();
		std::cout << player->GetBelt(beltslot)->GetPokemonNameView() << " currently has " << player->GetBelt(beltslot)->GetTotalEVs() << " EV points used.\n";
		std::cout << "Set Special Defense EV between 0 and 252: ";
		std::string input{};
		std::cin >> input;

		if (input == "00")
		{
			return true;
		}

		if (!IsDigits(input))
		{
			std::cout << "Invalid input! Please only enter a number.\n";
			continue;
		}

		int value{ std::stoi(input) };

		if (value < 0 || value > 252)
		{
			std::cout << "Invalid number. Must be a number between 0 and 252.\n";
			continue;
		}

		evCheck = player->GetBelt(beltslot)->SetSpecialDefenseEV(value);
	}
	return exit;
}

bool Menu::SetPlayerPokemonSpeedEV(Player* player, size_t beltslot)
{
	bool exit = false;
	bool evCheck = false;
	while (exit == false && evCheck == false)
	{
		player->GetBelt(beltslot)->DisplayEVs();
		std::cout << player->GetBelt(beltslot)->GetPokemonNameView() << " currently has " << player->GetBelt(beltslot)->GetTotalEVs() << " EV points used.\n";
		std::cout << "Set Speed EV between 0 and 252: ";
		std::string input{};
		std::cin >> input;

		if (input == "00")
		{
			return true;
		}

		if (!IsDigits(input))
		{
			std::cout << "Invalid input! Please only enter a number.\n";
			continue;
		}

		int value{ std::stoi(input) };

		if (value < 0 || value > 252)
		{
			std::cout << "Invalid number. Must be a number between 0 and 252.\n";
			continue;
		}

		evCheck = player->GetBelt(beltslot)->SetSpeedEV(value);
	}
	return exit;
}

bool Menu::RunMenu()
{
	bool exit = false;

	while (exit == false)
	{
		std::cout << "---Main Menu:---\n";

		std::cout << "1 - Change Player one's name\n";
		std::cout << "2 - Edit player one's pokemon\n";
		std::cout << "3 - Change player one's type (human or A.I)\n";
		std::cout << "4 - Change Player two's name\n";
		std::cout << "5 - Edit player two's pokemon\n";
		std::cout << "6 - Change player two's type (human or A.I)\n";
		std::cout << "7 - Save Party\n";
		std::cout << "8 - Load Party\n";
		std::cout << "9 - BATTLE!!!\n";
		std::cout << "0 - Exit Game\n";

		std::cout << "\"00\" from any sub menu will return you to the Main Menu.\n";

		std::string input{};
		std::cout << "Option: ";
		std::cin >> input;
		std::cout << '\n';

		if (!IsDigits(input) || input.size() > 10)
		{
			std::cout << "Invalid input!\n\n";
			continue;
		}

		int choice{ std::stoi(input) };

		switch (choice)
		{
		case 0:
			exit = true;
			return exit;
		
		case 1:
			ChangePlayerOneName();
			break;

		case 2:
			EditPlayerOnePokemon();
			break;

		case 3:
			ChangePlayerOneType();
			break;

		case 4:
			ChangePlayerTwoName();
			break;

		case 5:
			EditPlayerTwoPokemon();
			break;

		case 6:
			ChangePlayerTwoType();
			break;

		case 7:
			SaveYourParty(players);
			break;

		case 8:
			LoadYourParty(players);
			break;

		case 9:
			if (!IsPokemonSetupIncomplete())
			{
				return false; 
			}
			break;

		default:
			std::cout << "Invalid input!\n\n";
			break;
		}
	}
	return exit;
}

void Menu::ChangePlayerOneName()
{
	std::string oldName = players[0]->GetPlayerName();
	std::cout << "Please enter a new name for " << oldName << ": ";
	std::string input;
	std::cin.ignore();
	std::getline (std::cin, input);

	if (input == "00")
	{
		return;
	}

	players[0]->SetName(input);
	std::cout << '\n';
	
	std::cout << oldName << " changed to " << players[0]->GetPlayerNameView() << " successfully!\n\n";

}

void Menu::EditPlayerOnePokemon()
{
	bool exit = false;

	while (exit == false)
	{
		players[0]->DisplayPlayerPokemon();

		std::cout << "---Edit Player One's Pokemon---\n";
		std::cout << "1. Add Pokemon\n"
			"2. Change Pokemon\n"
			"3. Release Pokemon\n"
			"4. Edit Pokemon Stats\n"
			"5. Edit Pokemon Moves\n"
			"0. Back to Main Menu\n";

		std::string input{};
		std::cout << "Option: ";
		std::cin >> input;
		std::cout << '\n';

		if (input == "00")
		{
			exit = true;
			return;
		}

		if (!IsDigits(input) || input.size() > 10)
		{
			std::cout << "Invalid input!\n\n";
			continue;
		}

		int choice{ std::stoi(input) };

		switch (choice)
		{
		case 0:
			exit = true;
			return;

		case 1:
			exit = AddPokemon(players[0]);
			break;

		case 2:
			exit = ChangePokemon(players[0]);
			break;

		case 3:
			exit = DeletePokemon(players[0]);
			break;

		case 4:
			exit = EditPokemonStats(players[0]);
			break;

		case 5:
			exit = EditPokemonMoves(players[0]);
			break;

		default:
			std::cout << "Invalid input!\n\n";
			break;
		}
	}
	return;
}

void Menu::ChangePlayerOneType()
{
	bool exit = false;
	while(exit == false)
	{
		std::cout << "---Change Player One's Controller Type---\n";
		PrintPlayerOneType();
		std::cout << "1. Human\n";
		std::cout << "2. Easy A.I.\n";
		//std::cout << "3. Medium A.I.\n";
		std::cout << "0. Go Back\n";

		std::string input{};
		std::cout << "Option: ";
		std::cin >> input;

		if (input == "00")
		{
			exit = true;
			return;
		}

		if (!IsDigits(input))
		{
			std::cout << "Invalid input!\n\n";
			continue;
		}

		int choice{ std::stoi(input) };

		switch (choice)
		{
			case 0:
			{
				exit = true;
				return;
			}

			case 1:
			{
				std::array<BattlePokemon, 6> oldBelt = playerStorage[0]->CopyBelt();
				std::string_view playerName = playerStorage[0]->GetPlayerNameView();

				auto newHuman = std::make_unique<HumanPlayer>(playerName);

				players[0] = newHuman.get();
				playerStorage[0] = std::move(newHuman);

				playerStorage[0]->AssignBelt(oldBelt);

				std::cout << "Switched Player One to Human.\n\n";
				break;
			}

			case 2:
			{
				std::array<BattlePokemon, 6> oldBelt = playerStorage[0]->CopyBelt();
				std::string_view playerName = playerStorage[0]->GetPlayerNameView();

				auto strategy = std::make_unique<EasyAIStrategy>(m_rng);
				auto* strategyPtr = strategy.get();
				strategies.push_back(std::move(strategy));

				auto newAI = std::make_unique<AIPlayer>(playerName, *strategyPtr);
				players[0] = newAI.get();
				playerStorage[0] = std::move(newAI);

				playerStorage[0]->AssignBelt(oldBelt);

				std::cout << "Switched Player One to Easy A.I.\n\n";
				break;
			}
			/*
			case 3:
			{	//players[0]->SwitchTypeToMediumAI();
				std::cout << "Switched Player One to Medium A.I.\n\n";
				break;
			}
			*/
			default:
				std::cout << "Invalid input!\n\n";
				break;
		}
	}
}

void Menu::PrintPlayerOneType()
{
	if (players[0]->IsHuman())
	{
		std::cout << "---" << players[0]->GetPlayerNameView() << " is currently Human---\n";
	}
	else
	{
		std::cout << "---" << players[0]->GetPlayerNameView() << " is currently A.I.---\n";
	}
}

void Menu::ChangePlayerTwoName()
{
	std::string oldName = players[1]->GetPlayerName();
	std::cout << "Please enter a new name for " << oldName << ": ";
	std::string input;
	std::cin.ignore();
	std::getline(std::cin, input);

	if (input == "00")
	{
		return;
	}

	players[1]->SetName(input);
	std::cout << '\n';

	std::cout << oldName << " changed to " << players[1]->GetPlayerNameView() << " successfully!\n\n";
}

void Menu::EditPlayerTwoPokemon()
{
	bool exit = false;

	while (exit == false)
	{
		players[1]->DisplayPlayerPokemon();

		std::cout << "---Edit Player Two's Pokemon---\n";
		std::cout << "1. Add Pokemon\n"
			"2. Change Pokemon\n"
			"3. Release Pokemon\n"
			"4. Edit Pokemon Stats\n"
			"5. Edit Pokemon Moves\n"
			"0. Back to Main Menu\n";

		std::string input{};
		std::cout << "Option: ";
		std::cin >> input;
		std::cout << '\n';

		if (input == "00")
		{
			exit = true;
			return;
		}

		if (!IsDigits(input) || input.size() > 10)
		{
			std::cout << "Invalid input!\n\n";
			continue;
		}

		int choice{ std::stoi(input) };

		switch (choice)
		{
		case 0:
			exit = true;
			return;

		case 1:
			exit = AddPokemon(players[1]);
			break;

		case 3:
			exit = DeletePokemon(players[1]);
			break;

		case 4:
			exit = EditPokemonStats(players[1]);
			break;

		case 5:
			exit = EditPokemonMoves(players[1]);
			break;

		default:
			std::cout << "Invalid input!\n\n";
			break;
		}
	}
}

void Menu::ChangePlayerTwoType()
{
	bool exit = false;
	while (exit == false)
	{
		std::cout << "---Change Player Two's Controller Type---\n";
		PrintPlayerTwoType();
		std::cout << "1. Human\n";
		std::cout << "2. Easy A.I.\n";
		//std::cout << "3. Medium A.I.\n";
		std::cout << "0. Go Back\n";

		std::string input{};
		std::cout << "Option: ";
		std::cin >> input;

		if (input == "00")
		{
			exit = true;
			return;
		}

		if (!IsDigits(input))
		{
			std::cout << "Invalid input!\n\n";
			continue;
		}

		int choice{ std::stoi(input) };

		switch (choice)
		{
		case 0:
		{
			exit = true;
			return;
		}

		case 1:
		{
			std::array<BattlePokemon, 6> oldBelt = playerStorage[1]->CopyBelt();
			std::string_view playerName = playerStorage[1]->GetPlayerNameView();

			auto newHuman = std::make_unique<HumanPlayer>(playerName);
			players[1] = newHuman.get();
			playerStorage[1] = std::move(newHuman);

			playerStorage[1]->AssignBelt(oldBelt);

			std::cout << "Switched Player Two to Human.\n\n";
			break;
		}

		case 2:
		{
			std::array<BattlePokemon, 6> oldBelt = playerStorage[1]->CopyBelt();
			std::string_view playerName = playerStorage[1]->GetPlayerNameView();

			auto strategy = std::make_unique<EasyAIStrategy>(m_rng);
			auto* strategyPtr = strategy.get();
			strategies.push_back(std::move(strategy));

			auto newAI = std::make_unique<AIPlayer>(playerName, *strategyPtr);
			players[1] = newAI.get();
			playerStorage[1] = std::move(newAI);

			playerStorage[1]->AssignBelt(oldBelt);

			std::cout << "Switched Player Two to Easy A.I.\n\n";
			break;
		}
		/*
		case 3:
		{	//players[0]->SwitchTypeToMediumAI();
			std::cout << "Switched Player One to Medium A.I.\n\n";
			break;
		}
		*/
		default:
			std::cout << "Invalid input!\n\n";
			break;
		}
	}
}

void Menu::PrintPlayerTwoType()
{
	if (players[1]->IsHuman())
	{
		std::cout << "---" << players[1]->GetPlayerNameView() << " is currently Human---\n";
	}
	else
	{
		std::cout << "---" << players[1]->GetPlayerNameView() << " is currently A.I.---\n";
	}
}

bool Menu::AddPokemon(Player* player)
{
	if (player->GetPokemonCount() >= 6)
	{
		std::cout << "You can't add anymore Pokemon! Go to last menu option and select Edit Pokemon.\n\n";
		return false;
	}

	std::cin.ignore();
	while (player->GetPokemonCount() < 6)
	{
		player->DisplayAllPokemon();
		player->DisplayPlayerPokemon();

		std::cout << "Choose Pokemon by number or name, or 0 to go back: ";

		std::string input{};
		std::getline(std::cin, input);

		if (input == "0")
		{
			std::cout << '\n';
			return false;
		}

		if (input == "00")
		{
			return true;
		}


		size_t beltslot{ 1 };
		size_t result{ 1 };
		for (result = 1; result <= 6; ++result)
		{
			if (!player->GetBelt(result)->HasPokemon())
			{
				beltslot = result;
				break;
			}
		}

		if (!IsDigits(input))
		{
			input[0] = std::toupper(input[0]);
			std::transform(input.begin() + 1, input.end(), input.begin() + 1,
				[](unsigned char c) { return std::tolower(c); }); 
			SetPlayerPokemon(player, beltslot, input);

			if (!player->GetBelt(beltslot)->HasPokemon())
			{
				std::cout << "Could not add " << input << ".\n";
				std::cout << "Please enter a number between 1 and 151, or valid name of a Pokemon.\n\n";
			}
			else
			{
				player->IncrementPokemonCount();
				std::cout << "Added " << player->GetBelt(beltslot)->GetPokemonNameView() << "\n\n";
			}
		}
		else
		{
			size_t value = std::stoi(input);

			if (value < 0 || value > 151)
			{
				std::cout << "Invalid input! Please enter a number between 1 and 151, or valid name of a Pokemon.\n\n";
			}
			else
			{
				SetPlayerPokemon(player, beltslot, value);
				player->IncrementPokemonCount();
				std::cout << "Added " << player->GetBelt(beltslot)->GetPokemonNameView() << "\n\n";
			}
		}
	}
	return false;
}

bool Menu::ChangePokemon(Player* player)
{
	std::cout << "---Change Pokemon---\n"
	"Be aware that when you change a Pokemon, its moves, IVs, and EVs will be reset!\n";

	bool exit = false;

	while (exit == false)
	{
		player->DisplayPlayerPokemon();

		std::cout << "Enter number of Pokemon slot to change, 0 to exit: ";

		std::string beltinput{};
		std::cin >> beltinput;

		if (beltinput == "00")
		{
			return true;
		}

		if (!IsDigits(beltinput))
		{
			std::cout << "Invalid input! Must be a number!\n\n";
			continue;
		}

		int beltslot{ std::stoi(beltinput) };

		if (beltslot < 0 || beltslot > 6)
		{
			std::cout << "Invalid input! Must be between 1 and 6. 0 to exit.\n\n";
			continue;
		}
		else if (beltslot == 0)
		{
			std::cout << "Going back to main menu...\n\n";
			return false;
		}
		else
		{
			std::string OldPokemonName = player->GetBelt(beltslot)->GetPokemonName();
			player->DisplayAllPokemon();

			std::string pokemonInput{};
			std::cout << "Choose a Pokemon: ";
			std::cin >> pokemonInput;

			if (pokemonInput == "00")
			{
				return true;
			}

			if (!IsDigits(pokemonInput))
			{
				pokemonInput[0] = std::toupper(pokemonInput[0]);
				std::transform(pokemonInput.begin() + 1, pokemonInput.end(), pokemonInput.begin() + 1,
					[](unsigned char c) { return std::tolower(c); });

				if (!player->GetBelt(beltslot)->HasPokemon())
				{
					player->IncrementPokemonCount();
				}

				SetPlayerPokemon(player, beltslot, pokemonInput);
				std::cout << "Changed " << OldPokemonName << " to " << player->GetBelt(beltslot)->GetPokemonNameView() << "\n\n";
			}
			else
			{
				size_t value = std::stoi(pokemonInput);

				if (value < 0 || value > 151)
				{
					std::cout << "Invalid input! Please enter a number between 1 and 151, or valid name of a Pokemon.\n\n";
				}
				else
				{
					if (!player->GetBelt(beltslot)->HasPokemon())
					{
						player->IncrementPokemonCount();
					}

					SetPlayerPokemon(player, beltslot, value);
					std::cout << "Changed " << OldPokemonName << " to " << player->GetBelt(beltslot)->GetPokemonNameView() << "\n\n";
				}
			}
		}
	}
	return exit;
	
}

bool Menu::DeletePokemon(Player* player)
{
	std::cout << "---Release Pokemon---\n";

	bool exit = false;

	while (exit == false)
	{
		player->DisplayPlayerPokemon();

		std::cout << "Enter number of Pokemon slot to release it, 0 to exit: ";

		std::string beltinput{};
		std::cin >> beltinput;

		if (beltinput == "00")
		{
			return true;
		}

		if (!IsDigits(beltinput))
		{
			std::cout << "Invalid input! Must be a number!\n\n";
			continue;
		}

		int beltslot{ std::stoi(beltinput) };

		if (beltslot < 0 || beltslot > 6)
		{
			std::cout << "Invalid input! Must be between 1 and 6. 0 to exit.\n\n";
			continue;
		}
		else if (beltslot == 0)
		{
			std::cout << "Going back to main menu...\n\n";
			return false;
		}
		else
		{
			if (!player->GetBelt(beltslot)->HasPokemon())
			{
				std::cout << "There is no pokemon to release in this slot!\n\n";
				continue;
			}
			else
			{
				std::cout << "Goodbye " << player->GetBelt(beltslot)->GetPokemonNameView() << "!\n\n";
				player->GetBelt(beltslot)->ReleasePokemon();
				player->DecrementPokemonCount();
			}
			
		}
	}
	return exit;

}

bool Menu::EditPokemonStats(Player* player)
{
	std::cout << "Edit your Pokemons' nicknames, IV and EV values here\n";

	bool exit = false;
	while (exit == false)
	{
		player->DisplayPlayerPokemon();
	
		std::cout << "Choose which Pokemon to edit by entering number 1-6 or 0 to go back: ";

		std::string input{};
		size_t beltslot{ 0 };
		std::cin >> input;

		if (input == "00")
		{
			return true;
		}

		if (!IsDigits(input))
		{
			std::cout << "Invalid input! Only enter in numerical values between 0 and 6.\n\n";
			continue;
		}
	
		beltslot = std::stoi(input);

		if (beltslot == 0)
		{
			std::cout << "Going back to main menu...\n\n";
			return false;
		}

		if (beltslot < 1 || beltslot > 6)
		{
			std::cout << "Must be a number between 1 and 6.\n\n";
			continue;
		}

		if (!player->GetBelt(beltslot)->HasPokemon())
		{
			std::cout << "There is no Pokemon to edit here.\n\n";
			continue;
		}
		else
		{
			exit = EditChosenPokemonStats(player, beltslot);
		}
	}
	return exit;
}

bool Menu::EditChosenPokemonStats(Player* player, size_t beltslot)
{
	bool exit = false;

	while (exit == false)
	{
		std::cout << "Editing " << player->GetBelt(beltslot)->GetPokemonNameView() << "'s stats\n";

		std::cout << "----------\n";
		player->GetBelt(beltslot)->DisplayStats();
		std::cout << "----------\n\n";

		std::cout << "IVs can only be set from 0 to 31. EVs from 0 to 252, and max allowed EVs per Pokemon is 510.\n\n";

		std::cout << "1. Set Nickname\n";
		std::cout << "2. Set Level\n";

		std::cout << "3. Set HP IV\n";
		std::cout << "4. Set Attack IV\n";
		std::cout << "5. Set Defense IV\n";
		std::cout << "6. Set Special Attack IV\n";
		std::cout << "7. Set Special Defense IV\n";
		std::cout << "8. Set Speed IV\n";

		std::cout << "9. Set HP EV\n";
		std::cout << "10. Set Attack EV\n";
		std::cout << "11. Set Defense EV\n";
		std::cout << "12. Set Special Attack EV\n";
		std::cout << "13. Set Special Defense EV\n";
		std::cout << "14. Set Speed EV\n";
		std::cout << "0. Exit\n";

		std::string input{};
		std::cin >> input;

		if (input == "00")
		{
			return true;
		}

		if (!IsDigits(input) || input.size() > 10)
		{
			std::cout << "Invalid input!\n\n";
			continue;
		}

		int choice{ std::stoi(input) };

		switch (choice)
		{
		case 1:
			exit = SetPlayerPokemonNickname(player, beltslot);
			break;

		case 2:
			exit = SetPlayerPokemonLevel(player, beltslot);
			break;

		case 3:
			exit = SetPlayerPokemonHPIV(player, beltslot);
			break;

		case 4:
			exit = SetPlayerPokemonAttackIV(player, beltslot);
			break;

		case 5:
			exit = SetPlayerPokemonDefenseIV(player, beltslot);
			break;

		case 6:
			exit = SetPlayerPokemonSpecialAttackIV(player, beltslot);
			break;

		case 7:
			exit = SetPlayerPokemonSpecialDefenseIV(player, beltslot);
			break;

		case 8:
			exit = SetPlayerPokemonSpeedIV(player, beltslot);
			break;

		case 9:
			exit = SetPlayerPokemonHPEV(player, beltslot);
			break;

		case 10:
			exit = SetPlayerPokemonAttackEV(player, beltslot);
			break;

		case 11:
			exit = SetPlayerPokemonDefenseEV(player, beltslot);
			break;

		case 12:
			exit = SetPlayerPokemonSpecialAttackEV(player, beltslot);
			break;

		case 13:
			exit = SetPlayerPokemonSpecialDefenseEV(player, beltslot);
			break;

		case 14:
			exit = SetPlayerPokemonSpeedEV(player, beltslot);
			break;

		case 0:
			return false;
		}

	}
	return exit;
}

bool Menu::EditPokemonMoves(Player* player)
{
	std::cout << "Edit your Pokemons' moves here\n";

	bool exit = false;
	while (exit == false)
	{
		player->DisplayPlayerPokemon();

		std::cout << "Choose which Pokemon to edit by entering number 1-6 or 0 to go back: ";

		std::string input{};
		size_t beltslot{ 0 };
		std::cin >> input;

		if (input == "00")
		{
			return true;
		}

		if (!IsDigits(input))
		{
			std::cout << "Invalid input! Only enter in numerical values between 0 and 6.\n\n";
			continue;
		}

		beltslot = std::stoi(input);

		if (beltslot == 0)
		{
			std::cout << "Going back to main menu...\n\n";
			return false;
		}

		if (beltslot < 1 || beltslot > 6)
		{
			std::cout << "Must be a number between 1 and 6.\n\n";
			continue;
		}

		if (!player->GetBelt(beltslot)->HasPokemon())
		{
			std::cout << "There is no Pokemon to edit here.\n\n";
			continue;
		}
		else
		{
			exit = EditChosenPokemonMoves(player, beltslot);
		}
	}
	return exit;
}

bool Menu::EditChosenPokemonMoves(Player* player, size_t beltslot)
{
	bool exit = false;
	while (exit == false)
	{
		std::cout << player->GetBelt(beltslot)->GetPokemonNameView() << ": ";
		player->GetBelt(beltslot)->DisplayLearnedMoves();

		std::cout << "Choose an option\n\n";

		std::cout << "1. Add a move\n";
		std::cout << "2. Change a move\n";
		std::cout << "0. Go Back\n";

		std::string input{};
		std::cout << "Option: ";
		std::cin >> input;

		if (input == "00")
		{
			return true;
		}

		if (!IsDigits(input) || input.size() > 10)
		{
			std::cout << "Invalid input!\n\n";
			continue;
		}

		int choice{ std::stoi(input) };

		switch (choice)
		{
		case 1:
			exit = AddMove(player, beltslot);
			break;

		case 2:
			exit = ChangeMove(player, beltslot);
			break;

		case 0:
			return false;
		}
	}
	return exit;

}

bool Menu::AddMove(Player* player, size_t beltslot)
{
	bool exit = false;

	if (player->GetBelt(beltslot)->GetMoveCount() >= 4)
	{
		std::cout << "You can't add anymore moves! Go to last menu option and select change moves.\n\n";
		return false;
	}

	while (player->GetBelt(beltslot)->GetMoveCount() < 4)
	{
		player->GetBelt(beltslot)->DisplayLearnableMoves();
		std::cout << player->GetBelt(beltslot)->GetPokemonNameView() << ": ";
		player->GetBelt(beltslot)->DisplayLearnedMoves();
		std::cout << '\n';

		std::cout << "NOTE: move names are sensitive to spaces, and number is based on number given from displaying learnable moves\n";
		std::cout << "Enter move name or number (0 to go back): ";

		std::string input;
		std::getline(std::cin >> std::ws, input);

		if (input == "00")
		{
			return true;
		}

		if (input == "0")
		{
			return false;
		}

		size_t result{ 1 };
		size_t moveslot{ 1 };
		for (result = 1; result <= 4; ++result)
		{
			if (!player->GetBelt(beltslot)->HasMove(result))
			{
				moveslot = result;
				break;
			}
		}

		if (!IsDigits(input))
		{
			CheckUserInput(input);

			player->GetBelt(beltslot)->SetMove(moveslot, input);

			if (!player->GetBelt(beltslot)->HasMove(moveslot))
			{
				std::cout << "Could not add " << input << ".\n";
				std::cout << "Please enter a valid move name.\n\n";
			}
			else
			{
				player->GetBelt(beltslot)->IncrementMoveCount();
				std::cout << "Added the move - " << input << "\n\n";
			}

		}
		else
		{
			size_t value = std::stoi(input);

			if (value < 0 || value > 165)
			{
				std::cout << "Invalid input! Please enter a number between 1 and 165, or valid name of a move.\n\n";
			}
			else
			{
				player->GetBelt(beltslot)->SetMove(moveslot, value);
				if (player->GetBelt(beltslot)->HasMove(moveslot))
				{
					player->GetBelt(beltslot)->IncrementMoveCount();
				}
				std::cout << '\n';
				
			}
		}
	}
	return exit;
}

bool Menu::ChangeMove(Player* player, size_t beltslot)
{
	std::cout << "---Change Move---\n";

	bool exit = false;

	while (exit == false)
	{
		std::cout << player->GetBelt(beltslot)->GetPokemonNameView() << ": ";
		player->GetBelt(beltslot)->DisplayLearnedMoves();

		std::cout << "Enter number of move slot to change, 0 to exit: ";

		std::string moveslot_input{};
		std::cin >> moveslot_input;

		if (moveslot_input == "00")
		{
			return true;
		}

		if (!IsDigits(moveslot_input))
		{
			std::cout << "Invalid input! Must be a number!\n";
			continue;
		}

		int moveslot{ std::stoi(moveslot_input) };
		std::string OldMoveName = "None";

		if (moveslot < 0 || moveslot > 4)
		{
			std::cout << "Invalid input! Must be between 1 and 4. 0 to exit.\n";
			continue;
		}
		else if (moveslot == 0)
		{
			std::cout << "Going back to main menu...\n";
			return false;
		}
		else
		{
			player->GetBelt(beltslot)->DisplayLearnableMoves();

			if (player->GetBelt(beltslot)->HasMove(moveslot))
			{
				OldMoveName = player->GetBelt(beltslot)->GetMoveName(moveslot);
			}
			
			player->GetBelt(beltslot)->DisplayLearnedMoves();

			std::string moveInput{};
			std::cout << "Choose a move: ";
			std::getline(std::cin >> std::ws, moveInput);

			if (!IsDigits(moveInput))
			{
				CheckUserInput(moveInput);

				if (!player->GetBelt(beltslot)->HasMove(moveslot))
				{
					player->GetBelt(beltslot)->IncrementMoveCount();
				}

				player->GetBelt(beltslot)->SetMove(moveslot, moveInput);
				if (OldMoveName != player->GetBelt(beltslot)->GetMoveName(moveslot))
				{
					std::cout << "Changed " << OldMoveName << " to " << player->GetBelt(beltslot)->GetMoveName(moveslot) << "\n";
				}
				
			}
			else
			{
				size_t value = std::stoi(moveInput);

				if (value < 0 || value > 164)
				{
					std::cout << "Invalid input! Please enter a number between 1 and 164, or valid name of a move. 0 to exit\n";
				}
				else if (value == 0)
				{
					return false;
				}
				else
				{
					if (!player->GetBelt(beltslot)->HasMove(moveslot))
					{
						player->GetBelt(beltslot)->IncrementMoveCount();
					}

					player->GetBelt(beltslot)->SetMove(moveslot, value);
					if (OldMoveName != player->GetBelt(beltslot)->GetMoveName(moveslot))
					{
						std::cout << "Changed " << OldMoveName << " to " << player->GetBelt(beltslot)->GetMoveName(moveslot) << "\n";
					}
				}
			}
		}
	}
	return exit;
}

void Menu::SetDefaultPokemon()
{
	players[0]->GetBelt(1)->SetPokemon("Poketest");
	players[1]->GetBelt(1)->SetPokemon("Poketest");

	if (players[0]->GetBelt(1)->GetCurrentHP() != 0)
	{
		players[0]->IncrementPokemonCount();
		players[1]->IncrementPokemonCount();

		players[0]->GetBelt(1)->SetMove(1, "Tackle");
		players[0]->GetBelt(1)->SetMove(2, "Karate Chop");
		players[0]->GetBelt(1)->SetMove(3, "Fury Attack");
		players[0]->GetBelt(1)->SetMove(4, "Fire Punch");
		players[1]->GetBelt(1)->SetMove(1, "Ice Punch");
		players[1]->GetBelt(1)->SetMove(2, "Thunder Punch");
		players[1]->GetBelt(1)->SetMove(3, "Pound");
		players[1]->GetBelt(1)->SetMove(4, "Slash");

		players[0]->GetBelt(1)->IncrementMoveCount();
		players[0]->GetBelt(1)->IncrementMoveCount();
		players[0]->GetBelt(1)->IncrementMoveCount();
		players[0]->GetBelt(1)->IncrementMoveCount();
		players[1]->GetBelt(1)->IncrementMoveCount();
		players[1]->GetBelt(1)->IncrementMoveCount();
		players[1]->GetBelt(1)->IncrementMoveCount();
		players[1]->GetBelt(1)->IncrementMoveCount();

		players[0]->GetBelt(1)->SetLevel(100);
		players[1]->GetBelt(1)->SetLevel(100);

		players[0]->GetBelt(1)->SetHPEV(252);
		//players[0]->GetBelt(1)->SetAttackEV(252);
		players[0]->GetBelt(1)->SetDefenseEV(252);
		players[0]->GetBelt(1)->SetHPIV(31);
		players[0]->GetBelt(1)->SetAttackIV(31);
		players[0]->GetBelt(1)->SetDefenseIV(31);
		players[0]->GetBelt(1)->SetSpeedIV(30);
		players[1]->GetBelt(1)->SetHPEV(252);
		//players[1]->GetBelt(1)->SetAttackEV(252);
		players[1]->GetBelt(1)->SetDefenseEV(252);
		players[1]->GetBelt(1)->SetHPIV(31);
		players[1]->GetBelt(1)->SetDefenseIV(31);
		players[1]->GetBelt(1)->SetSpeedIV(31);
	}
	else
	{
		std::cout << "Failed to set default Pokemon!\n\n";
		return;
	}
}

void Menu::SaveYourParty(std::array<Player*, 2> players)
{
	bool exit = false;

	while (exit == false)
	{
		std::cout << "---Save Party---\n";
		std::cout << "1. Save Player One's saved party\n";
		std::cout << "2. Save Player Two's saved party\n";
		std::cout << "0 to go back\n";

		std::string input{};
		std::cout << "Option: ";
		std::cin >> input;
		std::cout << '\n';

		if (!IsDigits(input) || input.size() > 10)
		{
			std::cout << "Invalid input!\n\n";
			continue;
		}

		int choice{ std::stoi(input) };

		switch (choice)
		{
		case 0:
			exit = true;
			return;

		case 1:
			SavePlayerNumberParty(players, choice);
			break;

		case 2:
			SavePlayerNumberParty(players, choice);
			break;

		default:
			std::cout << "Invalid input!\n\n";
			break;
		}
	}
}

void Menu::SavePlayerNumberParty(std::array<Player*, 2> players, int num)
{
	--num;
	SaveParty savedParty(players, num);
	savedParty.Save();
}

void Menu::LoadYourParty(std::array<Player*, 2> players)
{
	bool exit = false;

	while (exit == false)
	{
		std::cout << "---Load Party---\n";
		std::cout << "1. Load Player One's saved party\n";
		std::cout << "2. Load Player Two's saved party\n";
		std::cout << "0 to go back\n";

		std::string input{};
		std::cout << "Option: ";
		std::cin >> input;
		std::cout << '\n';

		if (!IsDigits(input) || input.size() > 10)
		{
			std::cout << "Invalid input!\n\n";
			continue;
		}

		int choice{ std::stoi(input) };

		switch (choice)
		{
		case 0:
			exit = true;
			return;

		case 1:
			LoadPlayerNumberParty(players, choice);
			break;

		case 2:
			LoadPlayerNumberParty(players, choice);
			break;

		default:
			std::cout << "Invalid input!\n\n";
			break;
		}
	}
}

void Menu::LoadPlayerNumberParty(std::array<Player*, 2> players, int num)
{
	--num;
	LoadParty savedParty(players, num);
	savedParty.Load();
}

bool Menu::IsPokemonSetupIncomplete()
{
	if (!players[0]->GetPokemonCount() && !players[1]->GetPokemonCount())
	{
		std::cout << "Neither player has pokemon to use!\n\n";
		return true;
	}
	else if (players[0]->GetPokemonCount() && !(players[1]->GetPokemonCount()))
	{
		std::cout << players[1]->GetPlayerNameView() << " does not have any Pokemon!\n\n";
		return true;
	}
	else if (players[1]->GetPokemonCount() && !(players[0]->GetPokemonCount()))
	{
		std::cout << players[0]->GetPlayerNameView() << " does not have any Pokemon!\n\n";
		return true;
	}
	else
	{
		return false;
	}
}