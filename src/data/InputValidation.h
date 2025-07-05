#pragma once

#include <string>
#include <algorithm>

static bool IsDigits(const std::string& str)
{
	return std::all_of(str.begin(), str.end(), ::isdigit);
}

static void CheckUserInput(std::string& input)
{
	bool startOfWord = true;
	for (char& c : input)
	{
		if (startOfWord)
		{
			c = toupper(c);
		}

		else
		{
			c = tolower(c);
		}
		startOfWord = (c == ' ' || c == '-');
	}
}