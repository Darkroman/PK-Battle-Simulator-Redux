#include "RandomEngine.h"

RandomEngine::RandomEngine()
{
	seed = static_cast<int>(std::chrono::system_clock::now().time_since_epoch().count());

	std::mt19937 gen(seed);

	generator = gen;
}

std::mt19937& RandomEngine::GetGenerator()
{
	return generator;
}