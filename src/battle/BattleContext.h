#pragma once
#include "../entities/Player.h"
#include "../entities/BattlePokemon.h"

struct BattleStateFlags
{
    bool hit{ false };
    bool isCriticalHit{ false };

    enum class Effectiveness { Normal, Less, Super, No, OHKO };

    Effectiveness currentEffectiveness{ Effectiveness::Normal };

    void ResetBattleFlags()
    {
        hit = false;
        isCriticalHit = false;
        currentEffectiveness = Effectiveness::Normal;
    }
};

struct BattleContext
{
    BattleContext(std::vector<std::unique_ptr<Player>>& vec_players)
        : playerOne( vec_players[0].get() ),
          playerTwo( vec_players[1].get() )
    {}

    BattleStateFlags flags;

    double damageTaken{};

    Player* playerOne;
    Player* playerTwo;

    BattlePokemon* playerOneCurrentPokemon{ nullptr };
    BattlePokemon* playerTwoCurrentPokemon{ nullptr };

    BattlePokemon::pokemonMove* playerOneCurrentMove{ nullptr };
    BattlePokemon::pokemonMove* playerTwoCurrentMove{ nullptr };

    Player* attackingPlayer{ nullptr };
    Player* defendingPlayer{ nullptr };

    BattlePokemon* attackingPokemon{ nullptr };
    BattlePokemon* defendingPokemon{ nullptr };

    BattlePokemon::pokemonMove* currentMove{ nullptr };

    BattlePokemon::pokemonMove* selectedMove{ nullptr };
};
