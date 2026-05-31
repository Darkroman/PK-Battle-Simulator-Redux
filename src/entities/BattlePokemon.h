#pragma once

#include <array>
#include <string>
#include <span>

#include "pokemonMove.h"

#include "../common/PartyEditResults.h"

class Pokemon;

enum struct Status { Normal = 0, Burned, Frozen, Paralyzed, Poisoned, Badly_Poisoned, Sleeping };

class BattlePokemon
{
public:

    struct DetransformData
    {
        DetransformData();

        void BackupOriginalPokemonData(BattlePokemon*);

        std::string m_type1{};
        std::string m_type2{};

        std::array<pokemonMove, 4> m_array_moves{};

        const Pokemon* mp_pokemon{ nullptr };

        size_t m_attackstage{ 0 };
        size_t m_defensestage{ 0 };
        size_t m_specialattackstage{ 0 };
        size_t m_specialdefensestage{ 0 };
        size_t m_speedstage{ 0 };
        size_t m_evasionstage{ 0 };
        size_t m_accuracystage{ 0 };
        size_t m_criticalhitstage{ 0 };

        PokemonType m_type1e{};
        PokemonType m_type2e{};

        unsigned int m_ev_total{ 0 };

        unsigned int m_attack_iv{ 0 };
        unsigned int m_defense_iv{ 0 };
        unsigned int m_specialattack_iv{ 0 };
        unsigned int m_specialdefense_iv{ 0 };
        unsigned int m_speed_iv{ 0 };

        unsigned int m_attack_ev{ 0 };
        unsigned int m_defense_ev{ 0 };
        unsigned int m_specialattack_ev{ 0 };
        unsigned int m_specialdefense_ev{ 0 };
        unsigned int m_speed_ev{ 0 };

        unsigned int m_moveCount{ 0 };
    };

public:

    const Pokemon* GetPokemonDatabasePointer() const;

    SetPokemonOutcome SetPokemon(std::string_view);
    void SetPokemon(Pokemon*);
    void ReleasePokemon();

    SetMoveOutcome SetMove(size_t, std::string_view);
    std::span<pokemonMove> GetMoveArray();
    std::span<const pokemonMove> GetMoveArray() const;

    void SetNickname(std::string_view);
    bool HasNickname() const;
    std::string_view GetNickname() const;

    void SetLevel(unsigned int);

    void SetHPIV(unsigned int);
    void SetAttackIV(unsigned int);
    void SetDefenseIV(unsigned int);
    void SetSpecialAttackIV(unsigned int);
    void SetSpecialDefenseIV(unsigned int);
    void SetSpeedIV(unsigned int);

    SetEVResult SetHPEV(unsigned int);
    SetEVResult SetAttackEV(unsigned int);
    SetEVResult SetDefenseEV(unsigned int);
    SetEVResult SetSpecialAttackEV(unsigned int);
    SetEVResult SetSpecialDefenseEV(unsigned int);
    SetEVResult SetSpeedEV(unsigned int);

    unsigned int GetHPEV() const;
    unsigned int GetAttackEV() const;
    unsigned int GetDefenseEV() const;
    unsigned int GetSpecialAttackEV() const;
    unsigned int GetSpecialDefenseEV() const;
    unsigned int GetSpeedEV() const;

    bool ExceedsMaxAllowedEVs(unsigned int);

    unsigned int GetHPIV() const;
    unsigned int GetAttackIV() const;
    unsigned int GetDefenseIV() const;
    unsigned int GetSpecialAttackIV() const;
    unsigned int GetSpecialDefenseIV() const;
    unsigned int GetSpeedIV() const;

private:

    void UpdateStats();

public:

    const std::string& GetPokemonName() const;
    std::string_view GetPokemonNameView() const;
    const std::string& GetNickName() const;
    std::string_view GetNickNameView() const;

    const std::string& GetName() const;
    std::string_view GetNameView() const;

    unsigned int GetLevel() const;

    pokemonMove& GetMove(size_t);
    const pokemonMove& GetMove(size_t) const;
    void DeleteMove(size_t);
    void SwapMoves(size_t, size_t);
    void ReorderMoves(size_t, size_t);
    unsigned int GetPP(size_t) const;
    unsigned int GetMaxPP(size_t) const;
    bool WillPerformStruggle() const;

    bool HasPokemon() const;
    bool HasMove(size_t);

    void IncrementMoveCount();
    void DecrementMoveCount();

    unsigned int GetMoveCount() const;

    std::string_view GetMoveName(size_t) const;

    pokemonMove* GetLastUsedMove() const;
    void SetLastUsedMove(pokemonMove*);

    unsigned int GetTotalEVs() const;

    unsigned int GetCurrentHP() const;
    unsigned int GetMaxHP() const;
    unsigned int GetAttack() const;
    unsigned int GetDefense() const;
    unsigned int GetSpecialAttack() const;
    unsigned int GetSpecialDefense() const;
    unsigned int GetSpeed() const;

    void DamageCurrentHP(unsigned int);
    void HealCurrentHP(unsigned int);

    bool IsFainted() const;
    void SetFainted(bool);

    std::string_view GetTypeOne() const;
    PokemonType GetTypeOneEnum() const;
    std::string_view GetTypeTwo() const;
    PokemonType GetTypeTwoEnum() const;

    size_t GetAttackStage() const;
    size_t GetDefenseStage() const;
    size_t GetSpecialAttackStage() const;
    size_t GetSpecialDefenseStage() const;
    size_t GetSpeedStage() const;
    size_t GetEvasionStage() const;
    size_t GetAccuracyStage() const;
    size_t GetCriticalHitStage() const;

    void SetAttackStage(size_t);
    void SetDefenseStage(size_t);
    void SetSpecialAttackStage(size_t);
    void SetSpecialDefenseStage(size_t);
    void SetSpeedStage(size_t);
    void SetEvasionStage(size_t);
    void SetAccuracyStage(size_t);
    void SetCriticalHitStage(size_t);

    bool IsConfused() const;
    void SetConfusedStatus(bool);
    void SetConfusedTurnCount(unsigned int);
    unsigned int GetConfusedTurnCount() const;
    void IncrementConfusedCounter();
    unsigned int GetConfusedCounter() const;
    void ResetConfusedCounter();

    bool IsSeeded() const;
    void SetSeededStatus(bool);

    bool IsBound() const;
    void SetBound(bool);
    void SetBoundMoveName(size_t);
    std::string_view GetBoundMoveName() const;
    void SetBoundTurnCount(unsigned int);
    unsigned int GetBoundTurnCount() const;
    void IncrementBoundCounter();
    unsigned int GetBoundCounter() const;
    void ResetBoundCounter();

    bool IsSemiInvulnerable() const;
    bool IsSemiInvulnerableFromDig() const;
    bool IsSemiInvulnerableFromFly() const;
    void SetSemiInvulnerableDig(bool);
    void SetSemiInvulnerableFly(bool);

    bool IsCharging() const;
    void SetCharging(bool);

    bool IsRecharging() const;
    void SetRecharging(bool);

    void SetSleepTurnCount(unsigned int);
    unsigned int GetSleepTurnCount() const;
    void IncrementSleepCounter();
    unsigned int GetSleepCounter() const;
    void ResetSleepCounter();

    bool IsThrashing() const;
    void SetThrashing(bool);
    void SetThrashTurnCount(unsigned int);
    unsigned int GetThrashTurnCount() const;
    void IncrementThrashCounter();
    unsigned int GetThrashCounter() const;
    void ResetThrashCounter();

    void SetDisabledStatus(bool);
    void IncrementDisabledCounter();
    unsigned int GetDisabledCounter() const;
    void ResetDisabledCounter();
    bool MoveIsDisabled() const;
    pokemonMove* GetDisabledMove() const;

    bool HasPendingPostTurnEffect() const;

    unsigned int GetBadlyPoisonCounter() const;
    void ResetBadlyPoisonCounter();
    void IncrementBadlyPoisonCounter();

    bool IsFlinched() const;
    void SetIsFlinched(bool);

    bool HasUsedMinimize() const;
    void SetUsedMinimize(bool);

    bool IsRaging() const;
    void SetRaging(bool);

    bool HasUsedMimic() const;
    void SetUsedMimic(bool);
    int GetMimicPP() const;
    void SetMimicPP(int);

    bool HasFocusEnergy() const;
    void SetFocusEnergy(bool);

    Status GetStatus() const;
    void ChangeStatus(Status);

    bool IsBiding() const;
    void SetBide(bool);
    void SetBideTurnCount(unsigned int);
    unsigned int GetBideTurnCount() const;
    void IncrementBideCounter();
    unsigned int GetBideCounter() const;
    void ResetBideCounter();
    unsigned int GetBideDamage() const;
    void AddBideDamage(unsigned int);
    void ResetBideDamage();

    bool IsTransformed() const;
    void SetTransformation(BattlePokemon*);
    void Detransform();

    bool IsConverted() const;
    void SetConversion(pokemonMove*);
    void Deconvert();

    bool HasSubstitute() const;
    void SetSubstitute(bool);
    void SetSubstituteHP(unsigned int);
    unsigned int GetSubstituteHP() const;
    void DamageSubstitute(unsigned int);

    void SetMetronome(const Move& move);
    void SetMirrorMove(const Move& move);

private:
    void ResetStatsAndMoves();

public:
    void ResetStatsOnSwitch();
    void ResetValues();

private:
    DetransformData m_detransformData;

    std::string m_name{};
    std::string m_nickname{};
    std::string m_type1{};
    std::string m_type2{};
    std::string m_beforeConversionType{};
    std::string m_boundMove{};

    std::array<pokemonMove, 4> m_array_moves{};

    pokemonMove metronomeMove;
    pokemonMove mirrorMove;

    const Pokemon* mp_pokemon{ nullptr };

    pokemonMove* lastUsedMove{ nullptr };
    pokemonMove* disabledMove{ nullptr };

    unsigned int m_currentHP{ 0 };
    
    unsigned int m_ev_total{ 0 };

    unsigned int m_bideDamageTaken{ 0 };

    PokemonType m_type1e{};
    PokemonType m_type2e{};
    PokemonType m_beforeConversionType_e{};

    Status currentStatus{ Status::Normal };

    size_t m_attackstage{ 6 };
    size_t m_defensestage{ 6 };
    size_t m_specialattackstage{ 6 };
    size_t m_specialdefensestage{ 6 };
    size_t m_speedstage{ 6 };
    size_t m_evasionstage{ 6 };
    size_t m_accuracystage{ 6 };
    size_t m_criticalhitstage{ 0 };

    unsigned int m_level{ 50 };

    unsigned int m_hp_iv{ 31 };
    unsigned int m_attack_iv{ 31 };
    unsigned int m_defense_iv{ 31 };
    unsigned int m_specialattack_iv{ 31 };
    unsigned int m_specialdefense_iv{ 31 };
    unsigned int m_speed_iv{ 31 };

    unsigned int m_hp_ev{ 0 };
    unsigned int m_attack_ev{ 0 };
    unsigned int m_defense_ev{ 0 };
    unsigned int m_specialattack_ev{ 0 };
    unsigned int m_specialdefense_ev{ 0 };
    unsigned int m_speed_ev{ 0 };

    unsigned int m_moveCount{ 0 };

    unsigned int m_sleepCounter{ 0 };
    unsigned int m_sleepTurnCount{ 0 };
    unsigned int m_boundCounter{ 0 };
    unsigned int m_boundTurnCount{ 0 };
    unsigned int m_confusedCounter{ 0 };
    unsigned int m_confusedTurnCount{ 0 };
    unsigned int m_thrashCounter{ 0 };
    unsigned int m_thrashTurnCount{ 0 };
    unsigned int m_disabledCounter{ 0 };
    unsigned int m_badlyPoisonCounter{ 0 };
    unsigned int m_substituteHealth{ 0 };
    int m_mimicPP{ 0 };
    unsigned int m_bideCounter{ 0 };
    unsigned int m_bideTurnCount{ 0 };

    bool b_hasNickname{ false };

    bool b_isFainted{ false };

    bool b_isConfused{ false };
    bool b_isSeeded{ false };
    bool b_isSemiInvulnerableFly{ false };
    bool b_isSemiInvulnerableDig{ false };
    bool b_isRecharging{ false };
    bool b_isCharging{ false };
    bool b_isBound{ false };
    bool b_isFlinched{ false };
    bool b_hasUsedMinimize{ false };
    bool b_isThrashing{ false };
    bool b_moveIsDisabled{ false };
    bool b_hasSubstitute{ false };
    bool b_isRaging{ false };
    bool b_usedMimic{ false };
    bool b_hasFocusEnergy{ false };
    bool b_isBiding{ false };
    bool b_isTransformed{ false };
    bool b_isConverted{ false };

    bool b_transformBurnPenalty{ false };
    bool b_transformParalysisPenalty{ false };
};
