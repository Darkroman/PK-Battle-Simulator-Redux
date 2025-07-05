#pragma once

#include <algorithm>
#include <future>
#include <iomanip>
#include <filesystem>

#include "Pokemon.h"
#include "Move.h"

class Database
{
private:
    Database();
    ~Database() = default;

public:
    Database(const Database&) = delete; // No copy constructor
    Database& operator=(const Database&) = delete; // no copy assignment

    Pokemon* GetPointerToPokedexNumber(size_t);
    Move*    GetPointerToMovedexNumber(size_t);

    const std::vector<Pokemon>& GetPokedexVector() const;
    const std::vector<Move>&    GetMovedexVector() const;
    
    std::vector<Pokemon>::iterator        PokedexBegin();
    std::vector<Pokemon>::iterator          PokedexEnd();
    std::vector<Pokemon>::const_iterator cPokedexBegin() const;
    std::vector<Pokemon>::const_iterator   cPokedexEnd() const;

    std::vector<Move>::iterator        MovedexBegin();
    std::vector<Move>::iterator          MovedexEnd();
    std::vector<Move>::const_iterator cMovedexBegin() const;
    std::vector<Move>::const_iterator   cMovedexEnd() const;

    void DisplayPokemon() const;
    void   DisplayMoves() const;

    static Database& GetInstance();

private:

    void LoadPokemon();
    void LoadMoves();
    void SetPokemonMoveList(std::vector<std::vector<size_t>>&);
    void TestingPokemonAndMoves();

    void TestPokemonCPPOutputFile();
    void LoadEmbedPokemon();

    void TestMoveCPPOutputFile();
    void LoadEmbedMoves();

    void TestLearnSetCPPOutputFile();
    void LoadEmbedLearnset();

private:

    std::vector<Pokemon> pokedex;
    std::vector<Move>    movedex;
};