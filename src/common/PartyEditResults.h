#pragma once

enum class SetPokemonResult {
    Success,
    InvalidRange,
    InvalidPokemon,
    InvalidSlot
};

enum class SetMoveResult {
    Success,
    InvalidSlot,
    InvalidRange,
    InvalidMove,
    NotLearnable,
    DuplicateMove
};

enum class SetLevelResult {
    Success,
    InvalidLevel
};

enum class SetIVResult {
    Success,
    InvalidValue,
};

enum class SetEVResult {
    Success,
    InvalidValue,
    ExceedsTotal
};

struct SetPokemonOutcome
{
    SetPokemonResult result{};
    std::string_view pokemonName{};
};

struct SetMoveOutcome
{
    SetMoveResult result{};
    std::string_view moveName{};
};