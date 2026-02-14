#pragma once

#include <memory>

#include "BattlePokemon.h"

class IPlayerController;
class AIController;

enum class ControllerType : uint8_t { Human, AI };

class Player
{
public:
    explicit Player(std::string_view);

    ~Player();

    bool IsAI() const;
    IPlayerController& GetController() const;
    AIController& GetAIController();
    void SetController(std::unique_ptr<IPlayerController>, ControllerType);

    const std::array<BattlePokemon, 6>& GetBeltArray() const;
    std::array<BattlePokemon, 6> CopyBelt();
    void AssignBelt(std::array<BattlePokemon, 6>&);
    void SwapPokemon(size_t, size_t);
    void ReorderPokemon(size_t, size_t);

    const std::string& GetPlayerName() const;
    std::string_view GetPlayerNameView() const;
    void SetName(std::string_view);

    BattlePokemon& GetBelt(size_t);
    const BattlePokemon& GetBelt(size_t) const;

    void IncrementPokemonCount();
    void DecrementPokemonCount();
    int GetPokemonCount() const;
    void SetPokemonCount(int);

    int GetFaintedCount() const;
    void IncrementFaintedCount();
    void DecrementFaintedCount();

    bool IsFirst() const;
    void SetFirst(bool);

    bool CanSwitch() const;
    void SetCanSwitch(bool);

    bool IsSwitching() const;
    void SetIsSwitching(bool);

    bool HasSwitched() const;
    void SetHasSwitched(bool);

    void SetPokemonToSwitchTo(BattlePokemon*);
    BattlePokemon* GetPokemonToSwitchTo();

    void SetWinCondition(bool);

    bool HasWon() const;

    void SetForfeit(bool);
    bool HasForfeited() const;

    void SetMist(bool);
    bool HasMist() const;
    void IncrementMistCounter();
    int GetMistCounter() const;
    void ResetMistCounter();

    void SetLightScreen(bool);
    bool HasLightScreen() const;
    void IncrementLightScreenCounter();
    int GetLightScreenCounter() const;
    void ResetLightScreenCounter();

    void SetReflect(bool);
    bool HasReflect() const;
    void IncrementReflectCounter();
    int GetReflectCounter() const;
    void ResetReflectCounter();

    void ResetValues();

protected:
    std::unique_ptr<IPlayerController> uptr_controller;

    std::array<BattlePokemon, 6> belt;

    BattlePokemon* pokemonToSwitchTo{ nullptr };

    std::string m_name;

    int m_PokemonCount{ 0 };
    int m_faintedPokemon{ 0 };

    ControllerType e_type{};

    bool b_isAI{ false };

    bool b_isFirst{ false };

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