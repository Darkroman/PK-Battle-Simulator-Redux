#pragma once

#include <span>

#include "PokedexConstexpr.h"
#include "MovedexConstexpr.h"

class Pokemon;
class Move;

class Database
{
private:
    Database();
    ~Database() = default;

public:
    Database(const Database&) = delete; // No copy constructor
    Database& operator=(const Database&) = delete; // no copy assignment

    const Pokemon* GetPointerToPokedexNumber(size_t) const;
    const Move*    GetPointerToMovedexNumber(size_t) const;

    std::span<const Pokemon> GetPokedexView() const;
    std::span<const Move> GetMovedexView() const;

    static Database& GetInstance();

    void TestingPokemonAndMoves(Pokemon*&, Pokemon*&);

private:

private:

    std::span<const Pokemon> pokedex = global_pokedex;
    std::span<const Move> movedex = global_movedex;
};