#pragma once

#include <string>
#include <algorithm>

inline bool IsDigits(std::string_view str)
{
	return std::all_of(str.begin(), str.end(), ::isdigit);
}

inline std::string NormalizeUserInput(std::string_view input)
{
	std::string str{ input };

	bool startOfWord = true;
	for (char& c : str)
	{
		if (startOfWord)
		{
			c = static_cast<char>(static_cast<unsigned char>(toupper(c)));
		}

		else
		{
			c = static_cast<char>(static_cast<unsigned char>(tolower(c)));
		}
		startOfWord = (c == ' ' || c == '-');
	}

	return str;
}