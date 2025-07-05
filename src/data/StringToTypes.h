#pragma once

#include <sstream>
#include <string>
#include <map>

enum class Category // Used in Moves
{
    Physical,
    Special,
    Status
};

using mapOfStringsToCategories = std::map<std::string_view, Category>;

Category ConvertStringToCategory(std::string_view);

enum class PokemonType // Used in Moves, Pokemon, and BattlePokemon
{
    Normal,
    Fire,
    Water,
    Electric,
    Grass,
    Ice,
    Fighting,
    Poison,
    Ground,
    Flying,
    Psychic,
    Bug,
    Rock,
    Ghost,
    Dragon,
    Dark,
    Steel,
    Fairy,
    None
};

using mapOfStringsToPokemonTypes = std::map<std::string_view, PokemonType>;

PokemonType ConvertStringToPokemonType(std::string_view);

bool ConvertStringToBool(const std::string&);