#pragma once

#include <array>
#include <ranges>
#include <string>

class Pokemon;
class Move;
enum class PokemonType;
enum class Category;
enum class MoveEffect;

enum struct Status { Normal = 0, Burned, Frozen, Paralyzed, Poisoned, Badly_Poisoned, Sleeping };

class BattlePokemon
{
public:
    struct pokemonMove
    {
        pokemonMove();

        bool IsActive() const;

        void SetMovePointer(Move*);
        Move* GetMovePointer() const;

        size_t   GetMoveIndex() const;
        std::string_view    GetName() const;

        std::string_view GetCategory() const;
        Category     GetCategoryEnum() const;
        std::string_view GetMoveType() const;
        PokemonType  GetMoveTypeEnum() const;

        int              GetPriority() const;
        MoveEffect GetMoveEffectEnum() const;
        int          GetEffectChance() const;

        int       GetPP() const;
        int    GetMaxPP() const;
        int    GetPower() const;
        int GetAccuracy() const;

        void DeductPP();

        bool        DoesMakeContact() const;
        bool    IsAffectedByProtect() const;
        bool  IsAffectedByMagicCoat() const;
        bool     IsAffectedBySnatch() const;
        bool IsAffectedByMirrorMove() const;
        bool   IsAffectedByKingRock() const;
        bool           IsSoundBased() const;
        bool    CanBypassSubstitute() const;

    private:
        Move* mp_move{ nullptr };

    public:
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
    Pokemon* GetPokemonDatabasePointer() const;

    void SetPokemon(size_t);
    void SetPokemon(std::string_view);
    void ReleasePokemon();

    void SetMove(size_t, size_t);
    void SetMove(size_t, std::string_view);

    void SetNickname(std::string_view);
    bool HasNickname() const;
    std::string_view GetNickname() const;

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

    int GetHPEV() const;
    int GetAttackEV() const;
    int GetDefenseEV() const;
    int GetSpecialAttackEV() const;
    int GetSpecialDefenseEV() const;
    int GetSpeedEV() const;

    int GetHPIV() const;
    int GetAttackIV() const;
    int GetDefenseIV() const;
    int GetSpecialAttackIV() const;
    int GetSpecialDefenseIV() const;
    int GetSpeedIV() const;

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

    int GetLevel() const;
    void DisplayLearnedMoves() const;
    void DisplayMovesInBattle() const;

    BattlePokemon::pokemonMove* GetMove(size_t);
    int GetPP(size_t) const;
    int GetMaxPP(size_t) const;

    bool HasPokemon();
    bool HasMove(size_t);

    void IncrementMoveCount();
    void DecrementMoveCount();

    int GetMoveCount() const;

    std::string_view GetMoveName(size_t) const;

    pokemonMove* GetLastUsedMove() const;
    void SetLastUsedMove(pokemonMove*);

    int GetTotalEVs() const;

    int GetCurrentHP() const;
    int GetMaxHP() const;
    int GetAttack() const;
    int GetDefense() const;
    int GetSpecialAttack() const;
    int GetSpecialDefense() const;
    int GetSpeed() const;

    void DamageCurrentHP(int);
    void HealCurrentHP(int);

    bool IsFainted() const;
    void SetFainted(bool);

    std::string_view GetTypeOne() const;
    PokemonType GetTypeOneEnum() const;
    std::string_view GetTypeTwo() const;
    PokemonType GetTypeTwoEnum() const;

    int GetAttackStage() const;
    int GetDefenseStage() const;
    int GetSpecialAttackStage() const;
    int GetSpecialDefenseStage() const;
    int GetSpeedStage() const;
    int GetEvasionStage() const;
    int GetAccuracyStage() const;
    int GetCriticalHitStage() const;

    void SetAttackStage(int);
    void SetDefenseStage(int);
    void SetSpecialAttackStage(int);
    void SetSpecialDefenseStage(int);
    void SetSpeedStage(int);
    void SetEvasionStage(int);
    void SetAccuracyStage(int);
    void SetCriticalHitStage(int);

    bool IsConfused() const;
    void SetConfusedStatus(bool);
    void SetConfusedTurnCount(int);
    int GetConfusedTurnCount() const;
    void IncrementConfusedCounter();
    int GetConfusedCounter() const;
    void ResetConfusedCounter();

    bool IsSeeded() const;
    void SetSeededStatus(bool);

    bool IsBound() const;
    void SetBound(bool);
    void SetBoundMoveName(size_t);
    std::string_view GetBoundMoveName() const;
    void SetBoundTurnCount(int);
    int GetBoundTurnCount() const;
    void IncrementBoundCounter();
    int GetBoundCounter() const;
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

    void SetSleepTurnCount(int);
    int GetSleepTurnCount() const;
    void IncrementSleepCounter();
    int GetSleepCounter() const;
    void ResetSleepCounter();

    bool IsThrashing() const;
    void SetThrashing(bool);
    void SetThrashTurnCount(int);
     int GetThrashTurnCount() const;
    void IncrementThrashCounter();
    int GetThrashCounter() const;
    void ResetThrashCounter();

    void SetDisabledStatus(bool);
    void IncrementDisabledCounter();
    int GetDisabledCounter() const;
    void ResetDisabledCounter();
    bool MoveIsDisabled() const;
    pokemonMove* GetDisabledMove() const;

    int GetBadlyPoisonCounter() const;
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
    void SetBideTurnCount(int);
    int GetBideTurnCount() const;
    void IncrementBideCounter();
    int GetBideCounter() const;
    void ResetBideCounter();
    int GetBideDamage() const;
    void AddBideDamage(int);
    void ResetBideDamage();

    bool IsTransformed() const;
    void SetTransformation(BattlePokemon*);
    void Detransform();

    bool IsConverted() const;
    void SetConversion(BattlePokemon::pokemonMove*);
    void Deconvert();

    bool HasSubstitute() const;
    void SetSubstitute(bool);
    void SetSubstituteHP(int);
    int GetSubstituteHP() const;
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
    int m_currentHP{ 0 };
    //int m_maxHP{0};
    //int m_attack{0};
    //int m_defense{0};
    //int m_specialattack{0};
    //int m_specialdefense{0};
    //int m_speed{0};

    int m_hp_iv{ 0 };
    int m_attack_iv{ 0 };
    int m_defense_iv{ 0 };
    int m_specialattack_iv{ 0 };
    int m_specialdefense_iv{ 0 };
    int m_speed_iv{ 0 };

    int m_hp_ev{ 0 };
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

    // backup of original type before using conversion
    std::string m_beforeConversionType{};
    PokemonType m_beforeConversionType_e{};

    int m_attackstage{ 0 };
    int m_defensestage{ 0 };
    int m_specialattackstage{ 0 };
    int m_specialdefensestage{ 0 };
    int m_speedstage{ 0 };
    int m_evasionstage{ 0 };
    int m_accuracystage{ 0 };
    int m_criticalhitstage{ 0 };

    int m_moveCount{ 0 };

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
