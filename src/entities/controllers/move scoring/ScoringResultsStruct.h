#pragma once

#include "AIScoreTag.h"

struct pokemonMove;

struct ScoringResults
{
	unsigned int damage{};
	int score{ 100 };
	pokemonMove* move{};
	AIScoreTag tag{};
};