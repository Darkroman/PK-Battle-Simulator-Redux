#pragma once

#include <vector>
#include <memory>

class Player;
class BattlePokemon;
struct pokemonMove;

class IPlayerController;

struct BattleStateFlags
{
    bool hit{ false };
    bool hitSubstitute{ false };
    bool isCriticalHit{ false };

    enum class Effectiveness { Normal, Less, Super, No, OHKO };

    Effectiveness currentEffectiveness{ Effectiveness::Normal };

    void ResetBattleFlags()
    {
        hit = false;
        hitSubstitute = false;
        isCriticalHit = false;
        currentEffectiveness = Effectiveness::Normal;
    }
};

struct BattleContext
{
    BattleContext(std::vector<std::unique_ptr<Player>>& vec_players);

    std::vector<Player*> vec_aiPlayers{};

    std::vector<Player*> vec_outOfPokemon{};

    BattleStateFlags flags;

    const int HP_BAR_WIDTH{ 400 };

    int battleTurn{ 0 };

    int damageTaken{};

    int pixelsLost{};
    int prevPixels{ 400 };
    int damageInPixels{};

    Player* playerOne;
    Player* playerTwo;

    BattlePokemon* playerOneCurrentPokemon{ nullptr };
    BattlePokemon* playerTwoCurrentPokemon{ nullptr };

    pokemonMove* playerOneCurrentMove{ nullptr };
    pokemonMove* playerTwoCurrentMove{ nullptr };

    Player* attackingPlayer{ nullptr };
    Player* defendingPlayer{ nullptr };

    BattlePokemon* attackingPokemon{ nullptr };
    BattlePokemon* defendingPokemon{ nullptr };

    pokemonMove* currentMove{ nullptr };

    int initialPowerMultiplier{ 10 };
    int effectiveness{ 4096 };
};