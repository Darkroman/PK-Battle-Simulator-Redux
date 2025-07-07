#pragma once

#include "BattlePokemon.h"

struct BattleContext;

class Player
{
public:
    explicit Player(std::string_view);

    virtual ~Player() = default;

    virtual const bool IsHuman() const = 0;

    virtual const std::string& GetPlayerName() const;
    virtual std::string_view GetPlayerNameView() const;
    virtual void SetName(std::string_view);

    virtual BattlePokemon* GetBelt(size_t);
    virtual void DisplayPlayerPokemon();
    virtual void DisplayAllPokemon();

    virtual void IncrementPokemonCount();
    virtual void DecrementPokemonCount();
    virtual const int GetPokemonCount() const;

    virtual const int GetFaintedCount() const;
    virtual void IncrementFaintedCount();
    virtual void DecrementFaintedCount();

    virtual const bool IsFirst() const;
    virtual void SetFirst(bool);

    virtual const bool CanSwitch() const;
    virtual void SetCanSwitch(bool);

    virtual const bool IsSwitching() const;
    virtual void SetIsSwitching(bool);

    virtual const bool HasSwitched() const;
    virtual void SetHasSwitched(bool);

    virtual void SetPokemonToSwitchTo(BattlePokemon*);
    virtual BattlePokemon* GetPokemonToSwitchTo();

    virtual void SetWinCondition(bool);

    virtual const bool HasWon() const;

    virtual void SetForfeit(bool);
    virtual const bool HasForfeited() const;

    virtual void SetMist(bool);
    virtual const bool HasMist() const;
    virtual void IncrementMistCounter();
    virtual const int GetMistCounter() const;
    virtual void ResetMistCounter();

    virtual void SetLightScreen(bool);
    virtual const bool HasLightScreen() const;
    virtual void IncrementLightScreenCounter();
    virtual const int GetLightScreenCounter() const;
    virtual void ResetLightScreenCounter();

    virtual void SetReflect(bool);
    virtual const bool HasReflect() const;
    virtual void IncrementReflectCounter();
    virtual const int GetReflectCounter() const;
    virtual void ResetReflectCounter();

    virtual void ResetValues();

protected:
    std::array<BattlePokemon, 6> belt;

    BattlePokemon* pokemonToSwitchTo{ nullptr };

    std::string m_name;

    int m_PokemonCount{ 0 };
    int m_faintedPokemon{ 0 };

    bool b_isFirst = false;

    bool b_canSwitch{ true };
    bool b_isSwitching{ false };
    bool b_hasSwitched{ false };
    bool b_hasWon{ false };
    bool b_hasForfeited{ false };

    bool b_hasMist{ false };
    bool b_hasLightScreen{ false };
    bool b_hasReflect{ false };

    int m_mistCounter{};
    int m_lightscreenCounter{};
    int m_reflectCounter{};
};