#include "StringToTypes.h"

static inline mapOfStringsToCategories mostc = {
    { "Physical", Category::Physical},
    { "Special",  Category::Special },
    { "Status",   Category::Status }
};

Category ConvertStringToCategory(std::string_view input) // Used to set enum values in Moves from strings grabbed from movedex file
{
    Category result;
    return result = mostc.at(input);
}

static inline mapOfStringsToPokemonTypes mostpt{
        {"Normal",   PokemonType::Normal},
        {"Fire",     PokemonType::Fire},
        {"Water",    PokemonType::Water},
        {"Grass",    PokemonType::Grass},
        {"Electric", PokemonType::Electric},
        {"Ice",      PokemonType::Ice},
        {"Fighting", PokemonType::Fighting},
        {"Poison",   PokemonType::Poison},
        {"Ground",   PokemonType::Ground},
        {"Flying",   PokemonType::Flying},
        {"Psychic",  PokemonType::Psychic},
        {"Bug",      PokemonType::Bug},
        {"Rock",     PokemonType::Rock},
        {"Ghost",    PokemonType::Ghost},
        {"Dark",     PokemonType::Dark},
        {"Dragon",   PokemonType::Dragon},
        {"Steel",    PokemonType::Steel},
        {"Fairy",    PokemonType::Fairy},
        {"None",     PokemonType::None}
};

PokemonType ConvertStringToPokemonType(std::string_view input) // Used to set enum values in Moves and Pokemon from strings grabbed from movedex and pokedex files
{
    PokemonType result;
    return result = mostpt.at(input);
}

bool ConvertStringToBool(const std::string& stob)
{
    bool b;
    std::istringstream(stob) >> b;

    return b;
}