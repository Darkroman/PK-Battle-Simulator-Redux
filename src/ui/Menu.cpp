#include <iostream>
#include <algorithm>

#include "../entities/Player.h"
#include "../entities/controllers/AIController.h"
#include "../entities/controllers/HumanControllerConsole.h"

#include "../data/Database.h"
#include "../save/SaveParty.h"
#include "../save/LoadParty.h"
#include "../ui/views/PokemonTextView.h"
#include "../ui/views/DatabaseTextView.h"

#include "Menu.h"
#include "../common/InputValidation.h"
#include "../battle/RandomEngine.h"
#include "../entities/ai strategies/EasyAIStrategy.h"

Menu::Menu(std::vector<std::unique_ptr<Player>>& playerStorage, RandomEngine& rng, IAIStrategy& persistentStrategy)
	: playerStorage(playerStorage), m_rng(rng), m_persistentStrategy(persistentStrategy)
{
	players[0] = playerStorage[0].get();
	players[1] = playerStorage[1].get();

#if !defined NDEBUG
	SetDefaultPokemon();
#endif
}

SetPokemonResult Menu::SetPlayerPokemon(BattlePokemon& pokemon, std::string_view pkname)
{
	SetPokemonOutcome outcome{};

	outcome = pokemon.SetPokemon(pkname);

	switch (outcome.result)
	{
		case SetPokemonResult::InvalidRange:
			std::cout << "Could not add pokemon #: " << pkname << ".\n";
			std::cout << "Please enter a number between 1 and 151, or valid name of a Pokemon.\n\n";
			return outcome.result;

		case SetPokemonResult::InvalidPokemon:
			std::cout << "Could not add " << outcome.pokemonName << ".\n";
			std::cout << "Please enter a number between 1 and 151, or valid name of a Pokemon.\n\n";
			return outcome.result;

		case SetPokemonResult::Success:
			return outcome.result;

		case SetPokemonResult::Exit:
			return outcome.result;
	}
	return outcome.result;
}

SetMoveResult Menu::SetPlayerPokemonMove(BattlePokemon& pokemon, size_t moveslot, std::string_view movename)
{
	SetMoveOutcome outcome = pokemon.SetMove(moveslot, movename);

	switch (outcome.result)
	{
	case SetMoveResult::InvalidSlot:
		std::cout << "Invalid moveslot, must be between 1 and 4.\n\n";
		return outcome.result;

	case SetMoveResult::InvalidRange:
		std::cout << "Could not add move #: " << movename << ". Please enter a number between 1 and 165, or valid name of a move.\n\n";
		return outcome.result;

	case SetMoveResult::InvalidMove:
		std::cout << "Could not add move " << outcome.moveName << ".\n";
		std::cout << "Please enter a number between 1 and 165, or valid name of a move.\n\n";
		return outcome.result;

	case SetMoveResult::NotLearnable:
		std::cout << pokemon.GetPokemonNameView() << " cannot learn " << outcome.moveName << ".\n\n";
		return outcome.result;

	case SetMoveResult::DuplicateMove:
		std::cout << pokemon.GetPokemonNameView() << " already has the move " << outcome.moveName << ".\n\n";
		return outcome.result;

	case SetMoveResult::Success:
		return outcome.result;

	case SetMoveResult::Exit:
		return outcome.result;
	}
	return outcome.result;
}

bool Menu::SetPlayerPokemonNickname(BattlePokemon& pokemon)
{
	std::cout << "Enter a nickname, or just 0 to erase nickname: ";
	std::string nickname{};
	std::getline(std::cin >> std::ws, nickname);
	std::cout << '\n';

	if (nickname == "00")
	{
		return true;
	}

	pokemon.SetNickname(nickname);

	return false;
}

bool Menu::SetPlayerPokemonLevel(BattlePokemon& pokemon)
{
	bool exit = false;
	while (exit == false)
	{
		std::cout << "Set level 1 to 100: ";
		std::string input{};
		std::getline(std::cin >> std::ws, input);
		std::cout << '\n';

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
			pokemon.SetLevel(value);
			return false;
		}
	}
	return exit;
}

bool Menu::SetPlayerPokemonHPIV(BattlePokemon& pokemon)
{
	bool exit = false;
	while (exit == false)
	{
		PokemonTextView::DisplayIVs(pokemon);
		std::cout << "Set HP IV between 0 and 31: ";
		std::string input{};
		std::getline(std::cin >> std::ws, input);
		std::cout << '\n';

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
			pokemon.SetHPIV(value);
			return false;
		}
	}
	return exit;
}

bool Menu::SetPlayerPokemonAttackIV(BattlePokemon& pokemon)
{
	bool exit = false;
	while (exit == false)
	{
		PokemonTextView::DisplayIVs(pokemon);
		std::cout << "Set Attack IV between 0 and 31: ";
		std::string input{};
		std::getline(std::cin >> std::ws, input);
		std::cout << '\n';

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
			pokemon.SetAttackIV(value);
			return false;
		}
	}
	return exit;
}

bool Menu::SetPlayerPokemonDefenseIV(BattlePokemon& pokemon)
{
	bool exit = false;
	while (exit == false)
	{
		PokemonTextView::DisplayIVs(pokemon);
		std::cout << "Set Defense IV between 0 and 31: ";
		std::string input{};
		std::getline(std::cin >> std::ws, input);
		std::cout << '\n';

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
			pokemon.SetDefenseIV(value);
			return false;
		}
	}
	return exit;
}

bool Menu::SetPlayerPokemonSpecialAttackIV(BattlePokemon& pokemon)
{
	bool exit = false;
	while (exit == false)
	{
		PokemonTextView::DisplayIVs(pokemon);
		std::cout << "Set Special Attack IV between 0 and 31: ";
		std::string input{};
		std::getline(std::cin >> std::ws, input);
		std::cout << '\n';

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
			pokemon.SetSpecialAttackIV(value);
			return false;
		}
	}
	return exit;
}

bool Menu::SetPlayerPokemonSpecialDefenseIV(BattlePokemon& pokemon)
{
	bool exit = false;
	while (exit == false)
	{
		PokemonTextView::DisplayIVs(pokemon);
		std::cout << "Set Special Defense IV between 0 and 31: ";
		std::string input{};
		std::getline(std::cin >> std::ws, input);
		std::cout << '\n';

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
			pokemon.SetSpecialDefenseIV(value);
			return false;
		}
	}
	return exit;
}

bool Menu::SetPlayerPokemonSpeedIV(BattlePokemon& pokemon)
{
	bool exit = false;
	while (exit == false)
	{
		PokemonTextView::DisplayIVs(pokemon);
		std::cout << "Set Speed IV between 0 and 31: ";
		std::string input{};
		std::getline(std::cin >> std::ws, input);
		std::cout << '\n';

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
			pokemon.SetSpeedIV(value);
			return false;
		}
	}
	return exit;
}

bool Menu::SetPlayerPokemonHPEV(BattlePokemon& pokemon)
{
	bool exit = false;
	SetEVResult result{};
	while (exit == false)
	{
		PokemonTextView::DisplayEVs(pokemon);
		std::cout << pokemon.GetPokemonNameView() << " currently has " << pokemon.GetTotalEVs() << " EV points used.\n";
		std::cout << "Set HP EV between 0 and 252: ";
		std::string input{};
		std::getline(std::cin >> std::ws, input);
		std::cout << '\n';

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
		
		int oldValue = pokemon.GetHPEV();
		result = pokemon.SetHPEV(value);

		switch (result)
		{
			case SetEVResult::InvalidValue:
				std::cout << "Invalid number. Must be a number between 0 and 252.\n\n";
				continue;

			case SetEVResult::ExceedsTotal:
				std::cout << "You are not allowed to exceed 510 total EVs.\n\n";
				continue;

			case SetEVResult::Success:
				std::cout << "Changed HP EV from " << oldValue << " to " << value << ".\n\n";
				return false;
		}
	}
	return exit;
}

bool Menu::SetPlayerPokemonAttackEV(BattlePokemon& pokemon)
{
	bool exit = false;
	SetEVResult result{};
	while (exit == false)
	{
		PokemonTextView::DisplayEVs(pokemon);
		std::cout << pokemon.GetPokemonNameView() << " currently has " << pokemon.GetTotalEVs() << " EV points used.\n";
		std::cout << "Set Attack EV between 0 and 252: ";
		std::string input{};
		std::getline(std::cin >> std::ws, input);
		std::cout << '\n';

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

		int oldValue = pokemon.GetAttackEV();
		result = pokemon.SetAttackEV(value);

		switch (result)
		{
		case SetEVResult::InvalidValue:
			std::cout << "Invalid number. Must be a number between 0 and 252.\n\n";
			continue;

		case SetEVResult::ExceedsTotal:
			std::cout << "You are not allowed to exceed 510 total EVs.\n\n";
			continue;

		case SetEVResult::Success:
			std::cout << "Changed Attack EV from " << oldValue << " to " << value << ".\n\n";
			return false;
		}
	}
	return exit;
}

bool Menu::SetPlayerPokemonDefenseEV(BattlePokemon& pokemon)
{
	bool exit = false;
	SetEVResult result{};
	while (exit == false)
	{
		PokemonTextView::DisplayEVs(pokemon);
		std::cout << pokemon.GetPokemonNameView() << " currently has " << pokemon.GetTotalEVs() << " EV points used.\n";
		std::cout << "Set Defense EV between 0 and 252: ";
		std::string input{};
		std::getline(std::cin >> std::ws, input);
		std::cout << '\n';

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

		int oldValue = pokemon.GetDefenseEV();
		result = pokemon.SetDefenseEV(value);

		switch (result)
		{
		case SetEVResult::InvalidValue:
			std::cout << "Invalid number. Must be a number between 0 and 252.\n\n";
			continue;

		case SetEVResult::ExceedsTotal:
			std::cout << "You are not allowed to exceed 510 total EVs.\n\n";
			continue;

		case SetEVResult::Success:
			std::cout << "Changed Defense EV from " << oldValue << " to " << value << ".\n\n";
			return false;
		}
	}
	return exit;
}

bool Menu::SetPlayerPokemonSpecialAttackEV(BattlePokemon& pokemon)
{
	bool exit = false;
	SetEVResult result{};
	while (exit == false)
	{
		PokemonTextView::DisplayEVs(pokemon);
		std::cout << pokemon.GetPokemonNameView() << " currently has " << pokemon.GetTotalEVs() << " EV points used.\n";
		std::cout << "Set Special Attack EV between 0 and 252: ";
		std::string input{};
		std::getline(std::cin >> std::ws, input);
		std::cout << '\n';

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

		int oldValue = pokemon.GetSpecialAttackEV();
		result = pokemon.SetSpecialAttackEV(value);

		switch (result)
		{
		case SetEVResult::InvalidValue:
			std::cout << "Invalid number. Must be a number between 0 and 252.\n\n";
			continue;

		case SetEVResult::ExceedsTotal:
			std::cout << "You are not allowed to exceed 510 total EVs.\n\n";
			continue;

		case SetEVResult::Success:
			std::cout << "Changed Special Attack EV from " << oldValue << " to " << value << ".\n\n";
			return false;
		}
	}
	return exit;
}

bool Menu::SetPlayerPokemonSpecialDefenseEV(BattlePokemon& pokemon)
{
	bool exit = false;
	SetEVResult result{};
	while (exit == false)
	{
		PokemonTextView::DisplayEVs(pokemon);
		std::cout << pokemon.GetPokemonNameView() << " currently has " << pokemon.GetTotalEVs() << " EV points used.\n";
		std::cout << "Set Special Defense EV between 0 and 252: ";
		std::string input{};
		std::getline(std::cin >> std::ws, input);
		std::cout << '\n';

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

		int oldValue = pokemon.GetSpecialDefenseEV();
		result = pokemon.SetSpecialDefenseEV(value);

		switch (result)
		{
		case SetEVResult::InvalidValue:
			std::cout << "Invalid number. Must be a number between 0 and 252.\n\n";
			continue;

		case SetEVResult::ExceedsTotal:
			std::cout << "You are not allowed to exceed 510 total EVs.\n\n";
			continue;

		case SetEVResult::Success:
			std::cout << "Changed Special Defense EV from " << oldValue << " to " << value << ".\n\n";
			return false;
		}
	}
	return exit;
}

bool Menu::SetPlayerPokemonSpeedEV(BattlePokemon& pokemon)
{
	bool exit = false;
	SetEVResult result{};
	while (exit == false)
	{
		PokemonTextView::DisplayEVs(pokemon);
		std::cout << pokemon.GetPokemonNameView() << " currently has " << pokemon.GetTotalEVs() << " EV points used.\n";
		std::cout << "Set Speed EV between 0 and 252: ";
		std::string input{};
		std::getline(std::cin >> std::ws, input);
		std::cout << '\n';

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

		int oldValue = pokemon.GetSpeedEV();
		result = pokemon.SetSpeedEV(value);

		switch (result)
		{
		case SetEVResult::InvalidValue:
			std::cout << "Invalid number. Must be a number between 0 and 252.\n\n";
			continue;

		case SetEVResult::ExceedsTotal:
			std::cout << "You are not allowed to exceed 510 total EVs.\n\n";
			continue;

		case SetEVResult::Success:
			std::cout << "Changed Speed EV from " << oldValue << " to " << value << ".\n\n";
			return false;
		}
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
		std::getline(std::cin >> std::ws, input);
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
			if (IsPokemonSetupIncomplete())
			{
				std::cout << "Wtf?\n";
				continue;
			}
			return false;

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
	std::getline(std::cin >> std::ws, input);
	std::cout << '\n';

	if (input == "00")
	{
		return;
	}

	players[0]->SetName(input);
	
	std::cout << oldName << " changed to " << players[0]->GetPlayerNameView() << " successfully!\n\n";

}

void Menu::EditPlayerOnePokemon()
{
	bool exit = false;

	while (exit == false)
	{
		PokemonTextView::DisplayPlayerPokemon(*players[0]);

		std::cout << "---Edit Player One's Pokemon---\n";
		std::cout << "1. Add Pokemon\n"
			"2. Change Pokemon\n"
			"3. Release Pokemon\n"
			"4. Swap Pokemon\n"
			"5. Reposition Pokemon\n"
			"6. Edit Pokemon Stats\n"
			"7. Edit Pokemon Moves\n"
			"0. Back to Main Menu\n";

		std::string input{};
		std::cout << "Option: ";
		std::getline(std::cin >> std::ws, input);
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
			std::cout << "Going back to main menu...\n\n";
			return;

		case 1:
			exit = AddPokemon(*players[0]);
			break;

		case 2:
			exit = ChangePokemon(*players[0]);
			break;

		case 3:
			exit = DeletePokemon(*players[0]);
			break;

		case 4:
			exit = SwapPokemon(*players[0]);
			break;

		case 5:
			exit = ReorderPokemon(*players[0]);
			break;

		case 6:
			exit = EditPokemonStats(*players[0]);
			break;

		case 7:
			exit = EditPokemonMoves(*players[0]);
			break;

		default:
			std::cout << "Invalid number!\n\n";
			continue;
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
		std::getline(std::cin >> std::ws, input);
		std::cout << '\n';

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
				playerStorage[0]->SetController(std::make_unique<HumanControllerConsole>(), ControllerType::Human);
				players[0] = playerStorage[0].get();

				std::cout << "Switched Player One to Human.\n\n";
				break;
			}

			case 2:
			{
				playerStorage[0]->SetController(std::make_unique<AIController>(m_persistentStrategy), ControllerType::AI);
				players[0] = playerStorage[0].get();

				std::cout << "Switched Player One to Easy A.I.\n\n";
				break;
			}
			/*
			case 3:
			{
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
	if (!players[0]->IsAI())
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
	std::getline(std::cin >> std::ws, input);
	std::cout << '\n';

	if (input == "00")
	{
		return;
	}

	players[1]->SetName(input);

	std::cout << oldName << " changed to " << players[1]->GetPlayerNameView() << " successfully!\n\n";
}

void Menu::EditPlayerTwoPokemon()
{
	bool exit = false;

	while (exit == false)
	{
		PokemonTextView::DisplayPlayerPokemon(*players[1]);

		std::cout << "---Edit Player Two's Pokemon---\n";
		std::cout << "1. Add Pokemon\n"
			"2. Change Pokemon\n"
			"3. Release Pokemon\n"
			"4. Swap Pokemon\n"
			"5. Reposition Pokemon\n"
			"6. Edit Pokemon Stats\n"
			"7. Edit Pokemon Moves\n"
			"0. Back to Main Menu\n";

		std::string input{};
		std::cout << "Option: ";
		std::getline(std::cin >> std::ws, input);
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
			std::cout << "Going back to main menu...\n\n";
			return;

		case 1:
			exit = AddPokemon(*players[1]);
			break;

		case 2:
			exit = ChangePokemon(*players[1]);
			break;

		case 3:
			exit = DeletePokemon(*players[1]);
			break;

		case 4:
			exit = SwapPokemon(*players[1]);
			break;

		case 5:
			exit = ReorderPokemon(*players[1]);
			break;

		case 6:
			exit = EditPokemonStats(*players[1]);
			break;

		case 7:
			exit = EditPokemonMoves(*players[1]);
			break;

		default:
			std::cout << "Invalid number!\n\n";
			continue;
		}
	}
	return;
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
		std::getline(std::cin >> std::ws, input);
		std::cout << '\n';

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
			playerStorage[1]->SetController(std::make_unique<HumanControllerConsole>(), ControllerType::Human);
			players[1] = playerStorage[1].get();

			std::cout << "Switched Player Two to Human.\n\n";
			break;
		}

		case 2:
		{
			playerStorage[1]->SetController(std::make_unique<AIController>(m_persistentStrategy), ControllerType::AI);
			players[1] = playerStorage[1].get();

			std::cout << "Switched Player Two to Easy A.I.\n\n";
			break;
		}
		/*
		case 3:
		{	
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
	if (!players[1]->IsAI())
	{
		std::cout << "---" << players[1]->GetPlayerNameView() << " is currently Human---\n";
	}
	else
	{
		std::cout << "---" << players[1]->GetPlayerNameView() << " is currently A.I.---\n";
	}
}

bool Menu::AddPokemon(Player& player)
{
	if (player.GetPokemonCount() >= 6)
	{
		std::cout << "You can't add anymore Pokemon! Go to last menu option and select Edit Pokemon.\n\n";
		return false;
	}

	while (player.GetPokemonCount() < 6)
	{
		DatabaseTextView::DisplayAllPokemon(Database::GetInstance());
		PokemonTextView::DisplayPlayerPokemon(player);

		std::cout << "Enter Pokemon name or number (0 to go back): ";

		std::string input{};
		std::getline(std::cin >> std::ws, input);
		std::cout << '\n';

		if (input == "0")
		{
			return false;
		}

		if (input == "00")
		{
			return true;
		}

		size_t beltslot{ 1 };
		size_t slot{ 1 };
		for (slot = 1; slot <= 6; ++slot)
		{
			if (!player.GetBelt(slot).HasPokemon())
			{
				beltslot = slot;
				break;
			}
		}

		if (!IsDigits(input))
		{
			input = NormalizeUserInput(input);
		}

		SetPokemonResult setResult = SetPlayerPokemon(player.GetBelt(slot), input);

		if (setResult != SetPokemonResult::Success)
		{
			continue;
		}

		player.IncrementPokemonCount();
		std::cout << "Added " << player.GetBelt(beltslot).GetPokemonNameView() << "\n\n";
	}
	return false;
}

bool Menu::ChangePokemon(Player& player)
{
	std::cout << "---Change Pokemon---\n"
	"Be aware that when you change a Pokemon, its moves, IVs, and EVs will be reset!\n";

	bool exit = false;

	while (exit == false)
	{
		PokemonTextView::DisplayPlayerPokemon(player);

		std::cout << "Enter number of Pokemon slot to change, 0 to exit: ";

		std::string beltinput{};
		std::getline(std::cin >> std::ws, beltinput);
		std::cout << '\n';

		if (beltinput == "00")
		{
			return true;
		}

		if (!IsDigits(beltinput))
		{
			std::cout << "Invalid input! Must be a number!\n\n";
			continue;
		}

		size_t beltslot{ std::stoul(beltinput) };

		if (beltslot == 0)
		{
			return false;
		}

		if (beltslot > 6)
		{
			std::cout << "Invalid pokemon slot! Must be a number between 1 and 6.\n\n";
			continue;
		}

		BattlePokemon& pokemon = player.GetBelt(beltslot);
		std::string OldPokemonName{};

		if (player.GetBelt(beltslot).HasPokemon())
		{
			OldPokemonName = pokemon.GetPokemonName();
		}

		DatabaseTextView::DisplayAllPokemon(Database::GetInstance());

		std::string pokemonInput{};
		std::cout << "Enter Pokemon name or number (0 to go back): ";
		std::getline(std::cin >> std::ws, pokemonInput);
		std::cout << '\n';

		if (pokemonInput == "00")
		{
			return true;
		}

		SetPokemonResult result{};

		if (!IsDigits(pokemonInput))
		{
			pokemonInput = NormalizeUserInput(pokemonInput);
		}

		bool hadPokemon = pokemon.HasPokemon();

		result = SetPlayerPokemon(pokemon, pokemonInput);

		if (result != SetPokemonResult::Success)
		{
			continue;
		}

		if (!hadPokemon)
		{
			player.IncrementPokemonCount();
			std::cout << "Added " << pokemon.GetPokemonNameView() << "\n\n";
		}
		else
		{
			std::cout << "Changed " << OldPokemonName << " to " << pokemon.GetPokemonNameView() << "\n\n";
		}
	}
	return exit;
}

bool Menu::DeletePokemon(Player& player)
{
	std::cout << "---Release Pokemon---\n";

	bool exit = false;

	while (exit == false)
	{
		PokemonTextView::DisplayPlayerPokemon(player);

		std::cout << "Enter number of Pokemon slot to release it (0 to go back): ";

		std::string beltinput{};
		std::getline(std::cin >> std::ws, beltinput);
		std::cout << '\n';

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
			return false;
		}
		else
		{
			BattlePokemon& pokemon = player.GetBelt(beltslot);

			if (!pokemon.HasPokemon())
			{
				std::cout << "There is no pokemon to release in this slot!\n\n";
				continue;
			}
			else
			{
				std::cout << "Goodbye " << pokemon.GetPokemonNameView() << "!\n\n";
				pokemon.ReleasePokemon();
				player.DecrementPokemonCount();
			}
			
		}
	}
	return exit;
}

bool Menu::SwapPokemon(Player& player)
{
	std::cout << "---Swap Pokemon---\n";

	bool exit = false;

	while (exit == false)
	{
		PokemonTextView::DisplayPlayerPokemon(player);

		std::cout << "Enter Pokemon slot number to swap (0 to exit): ";

		std::string firstPickInput{};
		std::getline(std::cin >> std::ws, firstPickInput);

		if (firstPickInput == "00")
		{
			return true;
		}

		if (!IsDigits(firstPickInput))
		{
			std::cout << "Invalid input! Must be a number!\n\n";
			continue;
		}

		size_t firstSlot{ std::stoul(firstPickInput) };

		if (firstSlot == 0)
		{
			return false;
		}

		if (firstSlot > 6)
		{
			std::cout << "Invalid pokemon slot! Must be a number between 1 and 6.\n\n";
			continue;
		}

		BattlePokemon& firstPokemonSlot = player.GetBelt(firstSlot);

		std::string firstPokemonName{};

		if (!firstPokemonSlot.HasPokemon())
		{
			firstPokemonName = "Empty Slot";
		}
		else
		{
			firstPokemonName = firstPokemonSlot.GetPokemonNameView();
		}

		std::cout << "Picked " << firstPokemonName << " in slot " << firstSlot << "\n";

		while (true)
		{
			std::cout << "Enter slot number to swap " << firstPokemonName << " with: ";

			std::string secondPickInput{};
			std::getline(std::cin >> std::ws, secondPickInput);
			std::cout << '\n';

			if (secondPickInput == "00")
			{
				exit = true;
				break;
			}

			if (!IsDigits(secondPickInput))
			{
				std::cout << "Invalid input! Must be a number!\n\n";
				continue;
			}

			size_t secondSlot{ std::stoul(secondPickInput) };

			if (secondSlot == 0)
			{
				break;
			}

			if (secondSlot > 6)
			{
				std::cout << "Invalid pokemon slot! Must be a number between 1 and 6.\n\n";
				continue;
			}

			if (secondSlot == firstSlot)
			{
				std::cout << "You can't swap a Pokemon with itself.\n\n";
				continue;
			}

			BattlePokemon& secondPokemonSlot = player.GetBelt(secondSlot);

			std::string secondPokemonName{};

			if (!secondPokemonSlot.HasPokemon())
			{
				secondPokemonName = "Empty Slot";
			}
			else
			{
				secondPokemonName = secondPokemonSlot.GetPokemonNameView();
			}

			std::cout << firstPokemonName << " and " << secondPokemonName << " swapped slots.\n\n";
			player.SwapPokemon(firstSlot, secondSlot);
			break;
		}
	}
	return exit;
}

bool Menu::ReorderPokemon(Player& player)
{
	std::cout << "---Reposition Pokemon---\n";

	bool exit = false;

	while (exit == false)
	{
		PokemonTextView::DisplayPlayerPokemon(player);

		std::cout << "Choose Pokemon to reposition (0 to exit): ";

		std::string firstPickInput{};
		std::getline(std::cin >> std::ws, firstPickInput);

		if (firstPickInput == "00")
		{
			return true;
		}

		if (!IsDigits(firstPickInput))
		{
			std::cout << "Invalid input! Must be a number!\n\n";
			continue;
		}

		size_t slotToMove{ std::stoul(firstPickInput) };

		if (slotToMove == 0)
		{
			return false;
		}

		if (slotToMove > 6)
		{
			std::cout << "Invalid pokemon slot! Must be a number between 1 and 6.\n\n";
			continue;
		}

		BattlePokemon& pokemonSlotToMove = player.GetBelt(slotToMove);

		std::string firstPokemonName{};

		if (!pokemonSlotToMove.HasPokemon())
		{
			firstPokemonName = "Empty Slot";
		}
		else
		{
			firstPokemonName = pokemonSlotToMove.GetPokemonNameView();
		}

		std::cout << "Picked " << firstPokemonName << " in slot " << slotToMove << "\n";

		while (true)
		{
			std::cout << "Enter slot number to move " << firstPokemonName << " to: ";

			std::string secondPickInput{};
			std::getline(std::cin >> std::ws, secondPickInput);
			std::cout << '\n';

			if (secondPickInput == "00")
			{
				exit = true;
				break;
			}

			if (!IsDigits(secondPickInput))
			{
				std::cout << "Invalid input! Must be a number!\n\n";
				continue;
			}

			size_t targetSlot{ std::stoul(secondPickInput) };

			if (targetSlot == 0)
			{
				break;
			}

			if (targetSlot > 6)
			{
				std::cout << "Invalid pokemon slot! Must be a number between 1 and 6.\n\n";
				continue;
			}

			if (targetSlot == slotToMove)
			{
				std::cout << "You can't move a Pokemon with itself.\n\n";
				continue;
			}

			std::cout << "Moved " << firstPokemonName << " to slot " << targetSlot << ".\n\n";
			player.ReorderPokemon(slotToMove, targetSlot);
			break;
		}
	}
	return exit;
}

bool Menu::EditPokemonStats(Player& player)
{
	std::cout << "Edit your Pokemons' nicknames, IV and EV values here\n";

	bool exit = false;
	while (exit == false)
	{
		PokemonTextView::DisplayPlayerPokemon(player);
	
		std::cout << "Choose which Pokemon to edit by entering number 1-6 (0 to go back): ";

		std::string input{};
		size_t beltslot{ 0 };
		std::getline(std::cin >> std::ws, input);
		std::cout << '\n';

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
			return false;
		}

		if (beltslot < 1 || beltslot > 6)
		{
			std::cout << "Must be a number between 1 and 6.\n\n";
			continue;
		}

		if (!player.GetBelt(beltslot).HasPokemon())
		{
			std::cout << "There is no Pokemon to edit here.\n\n";
			continue;
		}
		else
		{
			exit = EditChosenPokemonStats(player.GetBelt(beltslot));
		}
	}
	return exit;
}

bool Menu::EditChosenPokemonStats(BattlePokemon& pokemon)
{
	bool exit = false;

	while (exit == false)
	{
		std::cout << "Editing " << pokemon.GetPokemonNameView() << "'s stats\n";

		std::cout << "----------\n";
		PokemonTextView::DisplayStats(pokemon);
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

		std::cout << "Choose option: ";
		std::string input{};
		std::getline(std::cin >> std::ws, input);
		std::cout << '\n';

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
			exit = SetPlayerPokemonNickname(pokemon);
			break;

		case 2:
			exit = SetPlayerPokemonLevel(pokemon);
			break;

		case 3:
			exit = SetPlayerPokemonHPIV(pokemon);
			break;

		case 4:
			exit = SetPlayerPokemonAttackIV(pokemon);
			break;

		case 5:
			exit = SetPlayerPokemonDefenseIV(pokemon);
			break;

		case 6:
			exit = SetPlayerPokemonSpecialAttackIV(pokemon);
			break;

		case 7:
			exit = SetPlayerPokemonSpecialDefenseIV(pokemon);
			break;

		case 8:
			exit = SetPlayerPokemonSpeedIV(pokemon);
			break;

		case 9:
			exit = SetPlayerPokemonHPEV(pokemon);
			break;

		case 10:
			exit = SetPlayerPokemonAttackEV(pokemon);
			break;

		case 11:
			exit = SetPlayerPokemonDefenseEV(pokemon);
			break;

		case 12:
			exit = SetPlayerPokemonSpecialAttackEV(pokemon);
			break;

		case 13:
			exit = SetPlayerPokemonSpecialDefenseEV(pokemon);
			break;

		case 14:
			exit = SetPlayerPokemonSpeedEV(pokemon);
			break;

		case 0:
			return false;

		default:
			std::cout << "Invalid number!\n\n";
			continue;
		}

	}
	return exit;
}

bool Menu::EditPokemonMoves(Player& player)
{
	std::cout << "Edit your Pokemons' moves here\n";

	bool exit = false;
	while (exit == false)
	{
		PokemonTextView::DisplayPlayerPokemon(player);

		std::cout << "Choose which Pokemon to edit by entering number 1-6 (0 to go back): ";

		std::string input{};
		size_t beltslot{ 0 };
		std::getline(std::cin >> std::ws, input);
		std::cout << '\n';

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
			return false;
		}

		if (beltslot < 1 || beltslot > 6)
		{
			std::cout << "Must be a number between 1 and 6.\n\n";
			continue;
		}

		if (!player.GetBelt(beltslot).HasPokemon())
		{
			std::cout << "There is no Pokemon to edit here.\n\n";
			continue;
		}
		else
		{
			exit = EditChosenPokemonMoves(player.GetBelt(beltslot));
		}
	}
	return exit;
}

bool Menu::EditChosenPokemonMoves(BattlePokemon& pokemon)
{
	bool exit = false;
	while (exit == false)
	{
		std::cout << "Editing " << pokemon.GetPokemonNameView() << "'s moves\n";

		std::cout << "----------\n";
		PokemonTextView::DisplayLearnedMoves(pokemon);
		std::cout << "----------\n\n";

		std::cout << "1. Add a move\n";
		std::cout << "2. Change a move\n";
		std::cout << "3. Delete a move\n";
		std::cout << "4. Swap Moves\n";
		std::cout << "5. Reposition Moves\n";
		std::cout << "0. Go Back\n";

		std::string input{};
		std::cout << "Option: ";
		std::getline(std::cin >> std::ws, input);
		std::cout << '\n';

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
			exit = AddMove(pokemon);
			break;

		case 2:
			exit = ChangeMove(pokemon);
			break;

		case 3:
			exit = DeleteMove(pokemon);
			break;

		case 4:
			exit = SwapMoves(pokemon);
			break;

		case 5:
			exit = ReorderMoves(pokemon);
			break;

		case 0:
			return false;

		default:
			std::cout << "Invalid number!\n\n";
			continue;
		}
	}
	return exit;

}

bool Menu::AddMove(BattlePokemon& pokemon)
{
	bool exit = false;

	if (pokemon.GetMoveCount() >= 4)
	{
		std::cout << "You can't add anymore moves! Go to last menu option and select change moves.\n\n";
		return false;
	}

	while (pokemon.GetMoveCount() < 4)
	{
		PokemonTextView::DisplayLearnableMoves(pokemon);
		std::cout << pokemon.GetPokemonNameView() << ": ";
		PokemonTextView::DisplayLearnedMoves(pokemon);
		std::cout << '\n';

		std::cout << "NOTE: move names are sensitive to spaces, and number is based on number given from displaying learnable moves\n";
		std::cout << "Enter move name or number (0 to go back): ";

		std::string input;
		std::getline(std::cin >> std::ws, input);
		std::cout << '\n';

		if (input == "00")
		{
			return true;
		}

		if (input == "0")
		{
			return false;
		}

		size_t slot{ 1 };
		size_t moveslot{ 1 };
		for (slot = 1; slot <= 4; ++slot)
		{
			if (!pokemon.HasMove(slot))
			{
				moveslot = slot;
				break;
			}
		}

		if (!IsDigits(input))
		{
			input = NormalizeUserInput(input);
		}

		SetMoveResult result = SetPlayerPokemonMove(pokemon, moveslot, input);

		if (result != SetMoveResult::Success)
		{
			continue;
		}

		pokemon.IncrementMoveCount();
		std::cout << "Added move " << pokemon.GetMoveName(moveslot) << ".\n\n";
		
	}
	return exit;
}

bool Menu::ChangeMove(BattlePokemon& pokemon)
{
	std::cout << "---Change Move---\n";

	bool exit = false;

	while (exit == false)
	{
		std::cout << pokemon.GetPokemonNameView() << ": ";
		PokemonTextView::DisplayLearnedMoves(pokemon);

		std::cout << "Enter number of move slot to change (0 to exit): ";

		std::string moveslot_input{};
		std::getline(std::cin >> std::ws, moveslot_input);
		std::cout << '\n';

		if (moveslot_input == "00")
		{
			return true;
		}

		if (!IsDigits(moveslot_input))
		{
			std::cout << "Invalid input! Must be a number!\n";
			continue;
		}

		size_t moveslot{ std::stoul(moveslot_input) };

		if (moveslot == 0)
		{
			std::cout << "Going back to main menu...\n";
			return false;
		}

		if (moveslot > 4)
		{
			std::cout << "Invalid input! Must be between 1 and 4. 0 to exit.\n";
			continue;
		}

		std::string OldMoveName{};

		if (pokemon.HasMove(moveslot))
		{
			OldMoveName = pokemon.GetMoveName(moveslot);
		}

		PokemonTextView::DisplayLearnableMoves(pokemon);
			
		PokemonTextView::DisplayLearnedMoves(pokemon);

		std::string moveInput{};
		std::cout << "Enter move name or number (0 to go back): ";
		std::getline(std::cin >> std::ws, moveInput);
		std::cout << '\n';

		if (!IsDigits(moveInput))
		{
			moveInput = NormalizeUserInput(moveInput);
		}

		bool hadMove = pokemon.HasMove(moveslot);

		SetMoveResult moveResult = SetPlayerPokemonMove(pokemon, moveslot, moveInput);

		if (moveResult != SetMoveResult::Success)
		{
			continue;
		}

		if (!hadMove)
		{
			pokemon.IncrementMoveCount();
			std::cout << "Added move " << pokemon.GetMoveName(moveslot) << ".\n\n";
		}
		else
		{
			std::cout << "Changed " << OldMoveName << " to " << pokemon.GetMoveName(moveslot) << "\n\n";
		}
	}
	return exit;
}

bool Menu::DeleteMove(BattlePokemon& pokemon)
{
	std::cout << "---Delete Move---\n";

	bool exit = false;

	while (exit == false)
	{
		std::cout << pokemon.GetPokemonNameView() << ": ";
		PokemonTextView::DisplayLearnedMoves(pokemon);

		std::cout << "Enter number of move slot to delete it (0 to go back): ";

		std::string moveSlotInput{};
		std::getline(std::cin >> std::ws, moveSlotInput);
		std::cout << '\n';

		if (moveSlotInput == "00")
		{
			return true;
		}

		if (!IsDigits(moveSlotInput))
		{
			std::cout << "Invalid input! Must be a number!\n\n";
			continue;
		}

		int moveSlot{ std::stoi(moveSlotInput) };

		if (moveSlot < 0 || moveSlot > 4)
		{
			std::cout << "Invalid input! Must be between 1 and 4. 0 to exit.\n\n";
			continue;
		}
		else if (moveSlot == 0)
		{
			return false;
		}
		else
		{
			if (!pokemon.GetMove(moveSlot).IsActive())
			{
				std::cout << "There is no move to delete in this slot!\n\n";
				continue;
			}
			else
			{
				std::cout << "No more " << pokemon.GetMove(moveSlot).GetName() << "!\n\n";
				pokemon.DeleteMove(moveSlot);
				pokemon.DecrementMoveCount();
			}

		}
	}
	return exit;
}

bool Menu::SwapMoves(BattlePokemon& pokemon)
{
	std::cout << "---Swap Moves---\n";

	bool exit = false;

	while (exit == false)
	{
		PokemonTextView::DisplayMovesInBattle(pokemon);

		std::cout << "Enter move slot number to swap (0 to exit): ";

		std::string firstPickInput{};
		std::getline(std::cin >> std::ws, firstPickInput);

		if (firstPickInput == "00")
		{
			return true;
		}

		if (!IsDigits(firstPickInput))
		{
			std::cout << "Invalid input! Must be a number!\n\n";
			continue;
		}

		size_t firstSlot{ std::stoul(firstPickInput) };

		if (firstSlot == 0)
		{
			return false;
		}

		if (firstSlot > 4)
		{
			std::cout << "Invalid move slot! Must be a number between 1 and 4.\n\n";
			continue;
		}

		std::string firstMoveName{};

		if (!pokemon.GetMove(firstSlot).IsActive())
		{
			firstMoveName = "Empty Slot";
		}
		else
		{
			firstMoveName = pokemon.GetMove(firstSlot).GetName();
		}

		std::cout << "Picked " << firstMoveName << " in slot " << firstSlot << "\n";

		while (true)
		{
			std::cout << "Enter slot number to swap " << firstMoveName << " with: ";

			std::string secondPickInput{};
			std::getline(std::cin >> std::ws, secondPickInput);
			std::cout << '\n';

			if (secondPickInput == "00")
			{
				exit = true;
				break;
			}

			if (!IsDigits(secondPickInput))
			{
				std::cout << "Invalid input! Must be a number!\n\n";
				continue;
			}

			size_t secondSlot{ std::stoul(secondPickInput) };

			if (secondSlot == 0)
			{
				break;
			}

			if (secondSlot > 6)
			{
				std::cout << "Invalid pokemon slot! Must be a number between 1 and 6.\n\n";
				continue;
			}

			if (secondSlot == firstSlot)
			{
				std::cout << "You can't swap a Pokemon with itself.\n\n";
				continue;
			}

			std::string secondMoveName{};

			if (!pokemon.GetMove(secondSlot).IsActive())
			{
				secondMoveName = "Empty Slot";
			}
			else
			{
				secondMoveName = pokemon.GetMove(secondSlot).GetName();
			}

			std::cout << firstMoveName << " and " << secondMoveName << " swapped slots.\n\n";
			pokemon.SwapMoves(firstSlot, secondSlot);
			break;
		}
	}
	return exit;
}

bool Menu::ReorderMoves(BattlePokemon& pokemon)
{
	std::cout << "---Reposition Moves---\n";

	bool exit = false;

	while (exit == false)
	{
		PokemonTextView::DisplayMovesInBattle(pokemon);

		std::cout << "Choose move to reposition (0 to exit): ";

		std::string firstPickInput{};
		std::getline(std::cin >> std::ws, firstPickInput);

		if (firstPickInput == "00")
		{
			return true;
		}

		if (!IsDigits(firstPickInput))
		{
			std::cout << "Invalid input! Must be a number!\n\n";
			continue;
		}

		size_t slotToMove{ std::stoul(firstPickInput) };

		if (slotToMove == 0)
		{
			return false;
		}

		if (slotToMove > 4)
		{
			std::cout << "Invalid move slot! Must be a number between 1 and 4.\n\n";
			continue;
		}

		std::string firstMoveName{};

		if (!pokemon.GetMove(slotToMove).IsActive())
		{
			firstMoveName = "Empty Slot";
		}
		else
		{
			firstMoveName = pokemon.GetMove(slotToMove).GetName();
		}

		std::cout << "Picked " << firstMoveName << " in slot " << slotToMove << "\n";

		while (true)
		{
			std::cout << "Enter slot number to move " << firstMoveName << " to: ";

			std::string secondPickInput{};
			std::getline(std::cin >> std::ws, secondPickInput);
			std::cout << '\n';

			if (secondPickInput == "00")
			{
				exit = true;
				break;
			}

			if (!IsDigits(secondPickInput))
			{
				std::cout << "Invalid input! Must be a number!\n\n";
				continue;
			}

			size_t targetSlot{ std::stoul(secondPickInput) };

			if (targetSlot == 0)
			{
				break;
			}

			if (targetSlot > 4)
			{
				std::cout << "Invalid move slot! Must be a number between 1 and 4.\n\n";
				continue;
			}

			if (targetSlot == slotToMove)
			{
				std::cout << "You can't reposition a move with itself.\n\n";
				continue;
			}

			std::cout << "Moved " << firstMoveName << " to slot " << targetSlot << ".\n\n";
			pokemon.ReorderMoves(slotToMove, targetSlot);
			break;
		}
	}
	return exit;
}

void Menu::SetDefaultPokemon()
{
	if (players[0]->GetBelt(1).HasPokemon() && players[1]->GetBelt(1).HasPokemon())
	{
		return;
	}

	players[0]->GetBelt(1).SetPokemon("Poketest");
	players[1]->GetBelt(1).SetPokemon("Poketest");

	if (players[0]->GetBelt(1).GetCurrentHP() != 0)
	{
		players[0]->GetBelt(1).SetMove(1, "Recover");
		players[1]->GetBelt(1).SetMove(1, "Recover");

		players[0]->GetBelt(1).IncrementMoveCount();
		players[1]->GetBelt(1).IncrementMoveCount();

		//players[0]->GetBelt(1)->SetLevel(100);
		//players[1]->GetBelt(1)->SetLevel(100);

		//players[0]->GetBelt(1)->SetHPEV(252);
		//players[0]->GetBelt(1)->SetAttackEV(252);
		//players[0]->GetBelt(1)->SetDefenseEV(252);
		//players[0]->GetBelt(1)->SetHPIV(31);
		//players[0]->GetBelt(1)->SetAttackIV(31);
		//players[0]->GetBelt(1)->SetDefenseIV(31);
		//players[0]->GetBelt(1)->SetSpeedIV(31);

		//players[1]->GetBelt(1)->SetHPEV(252);
		//players[1]->GetBelt(1)->SetAttackEV(252);
		//players[1]->GetBelt(1)->SetDefenseEV(252);
		//players[1]->GetBelt(1)->SetSpecialDefenseEV(252);
		//players[1]->GetBelt(1)->SetHPIV(31);
		//players[1]->GetBelt(1)->SetAttackIV(0);
		//players[1]->GetBelt(1)->SetDefenseIV(31);
		//players[1]->GetBelt(1)->SetSpecialDefenseIV(31);
		//players[1]->GetBelt(1)->SetSpeedIV(31);

		//players[0]->GetBelt(1)->SetAttackStage(6);
		//players[1]->GetBelt(1)->SetDefenseStage(-6);
	}
	else
	{
		std::cout << "Failed to set default Pokemon!\n\n";
		return;
	}

	int pokemonCount{};
	for (size_t i = 1; i < 7; ++i)
	{
		if (players[0]->GetBelt(i).HasPokemon())
		{
			++pokemonCount;
		}
	}

	players[0]->SetPokemonCount(pokemonCount);

	pokemonCount = 0;
	for (size_t i = 1; i < 7; ++i)
	{
		if (players[1]->GetBelt(i).HasPokemon())
		{
			++pokemonCount;
		}
	}

	players[1]->SetPokemonCount(pokemonCount);
}

void Menu::SaveYourParty(std::array<Player*, 2> players)
{
	bool exit = false;

	while (exit == false)
	{
		std::cout << "---Save Party---\n";
		std::cout << "1. Save Player One's party\n";
		std::cout << "2. Save Player Two's party\n";
		std::cout << "0 to go back\n";

		std::string input{};
		std::cout << "Option: ";
		std::getline(std::cin >> std::ws, input);
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
		std::getline(std::cin >> std::ws, input);
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