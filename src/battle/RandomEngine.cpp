#include <chrono>

#include "RandomEngine.h"

RandomEngine::RandomEngine()
	: seed(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count())),
	generator(seed)
{

}

RandomEngine::RandomEngine(int t)
	: seed(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count() + t)),
	generator(seed)
{

}

std::mt19937& RandomEngine::GetGenerator()
{
	return generator;
}