#pragma once

#include "BattlePokemon.h"

class Player
{
public:

    Player(std::string_view);

    BattlePokemon* GetBelt(size_t);
    const std::string& GetPlayerName() const;
    std::string_view GetPlayerNameView() const;
    void SetName(std::string_view);
    void DisplayPlayerPokemon();
    void DisplayAllPokemon();
    void IncrementPokemonCount();
    void DecrementPokemonCount();
    const int GetPokemonCount() const;

    const bool IsFirst() const;
    void SetFirst(bool);

    const bool IsPlayerOne() const;
    void SetPlayerOne(bool);

    const bool IsAI() const;
    const bool IsAIEasy() const;
    const bool IsAIMedium() const;

    void SwitchTypeToHuman();
    void SwitchTypeToAI();
    void SwitchTypeToEasyAI();
    void SwitchTypeToMediumAI();
    void PrintType() const;

    const int GetFaintedCount() const;
    void IncrementFaintedCount();
    void DecrementFaintedCount();

    const bool CanSwitch() const;
    void SetCanSwitch(bool);

    const bool IsSwitching() const;
    void SetIsSwitching(bool);

    const bool HasSwitched() const;
    void SetHasSwitched(bool);

    void SetPokemonToSwitchTo(BattlePokemon*);
    BattlePokemon* GetPokemonToSwitchTo();

    void SetWinCondition(bool);

    const bool HasWon() const;

    void SetForfeit(bool);
    const bool HasForfeited() const;

    void SetMist(bool);
    const bool HasMist() const;
    void IncrementMistCounter();
    const int GetMistCounter() const;
    void ResetMistCounter();

    void SetLightScreen(bool);
    const bool HasLightScreen() const;
    void IncrementLightScreenCounter();
    const int GetLightScreenCounter() const;
    void ResetLightScreenCounter();

    void SetReflect(bool);
    const bool HasReflect() const;
    void IncrementReflectCounter();
    const int GetReflectCounter() const;
    void ResetReflectCounter();

    void ResetValues();

private:
    std::array<BattlePokemon, 6> belt;

    BattlePokemon* pokemonToSwitchTo{ nullptr };
    
    std::string m_name;

    int m_PokemonCount{ 0 };

    bool b_isFirst = false;
    bool b_isPlayerOne = false;

    bool b_isAI = false;
    bool b_isEasyAI = false;
    bool b_isMediumAI = false;
    
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

    int m_faintedPokemon{ 0 };
};