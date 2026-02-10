#pragma once

#include <string>
#include <vector>

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

    static Database& GetInstance();

private:

    void TestingPokemonAndMoves();

    void LoadEmbedPokemon();

    void LoadEmbedMoves();

    void LoadEmbedLearnset();

private:

    std::vector<Pokemon> pokedex;
    std::vector<Move>    movedex;
};