#pragma once

#include <array>
#include <ranges>
#include <string>

class Pokemon;
class Move;
enum class PokemonType;

enum struct Status { Normal = 0, Burned, Frozen, Paralyzed, Poisoned, Badly_Poisoned, Sleeping };

class BattlePokemon
{
public:
    struct pokemonMove
    {
        pokemonMove();

        Move* mp_move{ nullptr };
        int m_currentPP{ 0 };
        int m_maxPP{ 0 };

        bool b_isDisabled{ false };
        bool b_isMimicked{ false };
    };

    struct DetransformData
    {
        DetransformData();

        void BackupOriginalPokemonData(BattlePokemon*);

        std::array<BattlePokemon::pokemonMove, 4> m_array_moves{};

        Pokemon* mp_pokemon{ nullptr };

        int m_attack_iv{ 0 };
        int m_defense_iv{ 0 };
        int m_specialattack_iv{ 0 };
        int m_specialdefense_iv{ 0 };
        int m_speed_iv{ 0 };

        int m_attack_ev{ 0 };
        int m_defense_ev{ 0 };
        int m_specialattack_ev{ 0 };
        int m_specialdefense_ev{ 0 };
        int m_speed_ev{ 0 };

        int m_ev_total{ 0 };

        std::string m_type1{};
        PokemonType m_type1e{};
        std::string m_type2{};
        PokemonType m_type2e{};

        int m_attackstage{ 0 };
        int m_defensestage{ 0 };
        int m_specialattackstage{ 0 };
        int m_specialdefensestage{ 0 };
        int m_speedstage{ 0 };
        int m_evasionstage{ 0 };
        int m_accuracystage{ 0 };
        int m_criticalhitstage{ 0 };

        int m_moveCount{ 0 };
    };

public:
    //BattlePokemon();
    Pokemon* GetPokemonDatabasePointer();

    void SetPokemon(size_t);
    void SetPokemon(std::string_view);
    void ReleasePokemon();
    
    void SetMove(size_t, size_t);
    void SetMove(size_t, std::string_view);

    void SetNickname(std::string_view);
    const bool HasNickname() const;
    std::string_view GetNickname();

    void SetLevel(int);

    void SetHPIV(int);
    void SetAttackIV(int);
    void SetDefenseIV(int);
    void SetSpecialAttackIV(int);
    void SetSpecialDefenseIV(int);
    void SetSpeedIV(int);

    bool SetHPEV(int);
    bool SetAttackEV(int);
    bool SetDefenseEV(int);
    bool SetSpecialAttackEV(int);
    bool SetSpecialDefenseEV(int);
    bool SetSpeedEV(int);

    const int GetHPEV() const;
    const int GetAttackEV() const;
    const int GetDefenseEV() const;
    const int GetSpecialAttackEV() const;
    const int GetSpecialDefenseEV() const;
    const int GetSpeedEV() const;

    const int GetHPIV() const;
    const int GetAttackIV() const;
    const int GetDefenseIV() const;
    const int GetSpecialAttackIV() const;
    const int GetSpecialDefenseIV() const;
    const int GetSpeedIV() const;

private:
    bool ExceedsMaxAllowedEVs(int);
    
    void UpdateStats();

public:
    void DisplayStats() const;
    void DisplayLearnableMoves() const;
    void DisplayIVs() const;
    void DisplayEVs() const;

    const std::string& GetPokemonName() const;
    std::string_view GetPokemonNameView() const;
    const std::string& GetNickName() const;
    std::string_view GetNickNameView() const;

    const std::string& GetName() const;
    std::string_view GetNameView() const;

    const int GetLevel() const;
    void DisplayLearnedMoves() const;
    void DisplayMovesInBattle() const;

    BattlePokemon::pokemonMove* GetMove(size_t);
    const int GetPP(size_t) const;
    const int GetMaxPP(size_t) const;

    bool HasPokemon();
    bool HasMove(size_t);

    void IncrementMoveCount();
    void DecrementMoveCount();

    const int GetMoveCount() const;

    std::string_view GetMoveName(size_t) const;

    pokemonMove* GetLastUsedMove();
    void SetLastUsedMove(pokemonMove*);

    const int GetTotalEVs() const;

    const int GetCurrentHP() const;
    const int GetMaxHP() const;
    const int GetAttack() const;
    const int GetDefense() const;
    const int GetSpecialAttack() const;
    const int GetSpecialDefense() const;
    const int GetSpeed() const;

    void DamageCurrentHP(int);
    void HealCurrentHP(int);

    const bool IsFainted() const;
    void SetFainted(bool);

    std::string_view GetTypeOne() const;
    const PokemonType GetTypeOneEnum() const;
    std::string_view GetTypeTwo() const;
    const PokemonType GetTypeTwoEnum() const;

    const int GetAttackStage() const;
    const int GetDefenseStage() const;
    const int GetSpecialAttackStage() const;
    const int GetSpecialDefenseStage() const;
    const int GetSpeedStage() const;
    const int GetEvasionStage() const;
    const int GetAccuracyStage() const;
    const int GetCriticalHitStage() const;

    void SetAttackStage(int);
    void SetDefenseStage(int);
    void SetSpecialAttackStage(int);
    void SetSpecialDefenseStage(int);
    void SetSpeedStage(int);
    void SetEvasionStage(int);
    void SetAccuracyStage(int);
    void SetCriticalHitStage(int);

    const bool IsConfused() const;
    void SetConfusedStatus(bool);
    void SetConfusedTurnCount(int);
    const int GetConfusedTurnCount() const;
    void IncrementConfusedCounter();
    const int GetConfusedCounter() const;
    void ResetConfusedCounter();

    const bool IsSeeded() const;
    void SetSeededStatus(bool);

    const bool IsBound() const;
    void SetBound(bool);
    void SetBoundMoveName(size_t);
    std::string_view GetBoundMoveName() const;
    void SetBoundTurnCount(int);
    const int GetBoundTurnCount() const;
    void IncrementBoundCounter();
    const int GetBoundCounter() const;
    void ResetBoundCounter();

    const bool IsSemiInvulnerable() const;
    const bool IsSemiInvulnerableFromDig() const;
    const bool IsSemiInvulnerableFromFly() const;
    void SetSemiInvulnerableDig(bool);
    void SetSemiInvulnerableFly(bool);

    const bool IsCharging() const;
    void SetCharging(bool);

    const bool IsRecharging() const;
    void SetRecharging(bool);

    void SetSleepTurnCount(int);
    const int GetSleepTurnCount() const;
    void IncrementSleepCounter();
    const int GetSleepCounter() const;
    void ResetSleepCounter();

    const bool IsThrashing() const;
    void SetThrashing(bool);
    void SetThrashTurnCount(int);
    const int GetThrashTurnCount() const;
    void IncrementThrashCounter();
    const int GetThrashCounter() const;
    void ResetThrashCounter();

    void SetDisabledStatus(bool);
    void IncrementDisabledCounter();
    const int GetDisabledCounter() const;
    void ResetDisabledCounter();
    const bool MoveIsDisabled() const;
    const pokemonMove* GetDisabledMove() const;

    const int GetBadlyPoisonCounter() const;
    void ResetBadlyPoisonCounter();
    void IncrementBadlyPoisonCounter();

    const bool IsFlinched() const;
    void SetIsFlinched(bool);

    const bool HasUsedMinimize() const;
    void SetUsedMinimize(bool);

    const bool IsRaging() const;
    void SetRaging(bool);

    const bool HasUsedMimic() const;
    void SetUsedMimic(bool);
    const int GetMimicPP() const;
    void SetMimicPP(int);

    const bool HasFocusEnergy() const;
    void SetFocusEnergy(bool);

    const Status GetStatus() const;
    void ChangeStatus(Status);

    const bool IsBiding() const;
    void SetBide(bool);
    void SetBideTurnCount(int);
    const int GetBideTurnCount() const;
    void IncrementBideCounter();
    const int GetBideCounter() const;
    void ResetBideCounter();
    const int GetBideDamage() const;
    void AddBideDamage(int);
    void ResetBideDamage();

    const bool IsTransformed() const;
    void SetTransformation(BattlePokemon*);
    void Detransform();

    const bool IsConverted() const;
    void SetConversion(BattlePokemon::pokemonMove*);
    void Deconvert();

    const bool HasSubstitute() const;
    void SetSubstitute(bool);
    void SetSubstituteHP(int);
    const int GetSubstituteHP() const;
    void DamageSubstitute(int);

    BattlePokemon::pokemonMove* Struggle();

    Status currentStatus{ Status::Normal };

private:
    void ResetStatsAndMoves();

public:
    void ResetStatsOnSwitch();
    void ResetValues();
    pokemonMove metronomeMove;
    pokemonMove mirrorMove;

private:
    DetransformData m_detransformData;

    std::string m_name{};
    std::string m_nickname{};
    bool b_hasNickname{ false };

    Pokemon* mp_pokemon{ nullptr };
    //Database* mp_db{ nullptr };

    std::array<pokemonMove, 4> m_array_moves{};

    pokemonMove m_struggle;

    int m_level{ 50 };
    int m_currentHP{0};
    //int m_maxHP{0};
    //int m_attack{0};
    //int m_defense{0};
    //int m_specialattack{0};
    //int m_specialdefense{0};
    //int m_speed{0};

    int m_hp_iv{0};
    int m_attack_iv{0};
    int m_defense_iv{0};
    int m_specialattack_iv{0};
    int m_specialdefense_iv{0};
    int m_speed_iv{0};

    int m_hp_ev{0};
    int m_attack_ev{0};
    int m_defense_ev{0};
    int m_specialattack_ev{0};
    int m_specialdefense_ev{0};
    int m_speed_ev{0};

    int m_ev_total{ 0 };

    std::string m_type1{};
    PokemonType m_type1e{};
    std::string m_type2{};
    PokemonType m_type2e{};

    // backup of original type before using conversion
    std::string m_beforeConversionType{}; 
    PokemonType m_beforeConversionType_e{};

    int m_attackstage{0};
    int m_defensestage{0};
    int m_specialattackstage{0};
    int m_specialdefensestage{0};
    int m_speedstage{0};
    int m_evasionstage{0};
    int m_accuracystage{0};
    int m_criticalhitstage{0};

    int m_moveCount{0};

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

    std::string m_boundMove{};

    int m_sleepCounter{ 0 };
    int m_sleepTurnCount{ 0 };
    int m_boundCounter{ 0 };
    int m_boundTurnCount{ 0 };
    int m_confusedCounter{ 0 };
    int m_confusedTurnCount{ 0 };
    int m_thrashCounter{ 0 };
    int m_thrashTurnCount{ 0 };
    int m_disabledCounter{ 0 };
    int m_badlyPoisonCounter{ 0 };
    int m_substituteHealth{ 0 };
    int m_mimicPP{ 0 };
    int m_bideCounter{ 0 };
    int m_bideTurnCount{ 0 };
    int m_bideDamageTaken{ 0 };

    pokemonMove* lastUsedMove{ nullptr };
    pokemonMove* disabledMove{ nullptr };
};
