#pragma once

#include <random>
#include <chrono>

class RandomEngine
{
private:

	unsigned int seed;
	std::mt19937 generator;

public:

	RandomEngine();

	std::mt19937& GetGenerator();
};