#pragma once

struct pokemonMove;

struct ScoringResults
{
	int damage{};
	int score{ 100 };
	pokemonMove* move{};
};