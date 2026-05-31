#pragma once

#include <unordered_map>
#include <string>

enum class Category // Used in Moves
{
    Physical,
    Special,
    Status,
    None
};

using mapOfStringsToCategories = std::unordered_map<std::string_view, Category>;

Category ConvertStringToCategory(std::string_view);

enum class PokemonType : size_t // Used in Moves, Pokemon, and BattlePokemon
{
    Normal = 0,
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

using mapOfStringsToPokemonTypes = std::unordered_map<std::string_view, PokemonType>;

PokemonType ConvertStringToPokemonType(std::string_view);

bool ConvertStringToBool(const std::string&);