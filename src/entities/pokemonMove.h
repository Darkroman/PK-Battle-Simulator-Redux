#pragma once

#include <string_view>

class Move;
enum class PokemonType;
enum class Category;
enum class MoveEffect;

struct pokemonMove
{
    //pokemonMove();

    bool IsActive() const;
    bool IsDisabled() const;

    void SetMovePointer(Move*);
    Move* GetMovePointer() const;
    void ResetMove();

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