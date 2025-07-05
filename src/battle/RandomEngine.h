#pragma once

#include <random>
#include <chrono>

class RandomEngine
{
public:

	unsigned int seed;
	std::mt19937 generator;

	RandomEngine();

	std::mt19937& GetGenerator();
};