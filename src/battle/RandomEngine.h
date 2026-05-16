#pragma once

#include <random>

class RandomEngine
{
private:

	unsigned int seed;
	std::mt19937 generator;

public:

	RandomEngine();
	RandomEngine(int t);

	std::mt19937& GetGenerator();
};