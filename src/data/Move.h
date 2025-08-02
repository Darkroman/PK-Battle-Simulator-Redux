// Move data layout in movedex.txt is as follows:
// Index number, name, type, category, priority, secondary effect flag, secondary effect chance, pp, max pp, power, accuracy.
// Each bool corresponds with whether it:
// Makes contact, affected by Protect, affected by Magic Coat, affected by Snatch, affected by Mirror Move, affected by King's Rock, sound based, bypasses substitute

#pragma once

#include <string>

enum class PokemonType;
enum class Category;
enum class MoveEffect;

class Move
{
public:
    Move
    (size_t, std::string_view,
     std::string_view, PokemonType, std::string_view, Category,
     int, MoveEffect, int,
     int, int, int, int,
     bool, bool, bool,
     bool, bool, bool, bool, bool);

    const size_t   GetMoveIndex() const;
    std::string_view    GetName() const;
    
    std::string_view      GetCategory() const;
    const Category    GetCategoryEnum() const;
    std::string_view      GetMoveType() const;
    const PokemonType GetMoveTypeEnum() const;
    
    const int              GetPriority() const;
    const MoveEffect GetMoveEffectEnum() const;
    const int          GetEffectChance() const;
    
    const int       GetPP() const;
    const int    GetMaxPP() const;
    const int    GetPower() const;
    const int GetAccuracy() const;

    const bool        DoesMakeContact() const;
    const bool    IsAffectedByProtect() const;
    const bool  IsAffectedByMagicCoat() const;
    const bool     IsAffectedBySnatch() const;
    const bool IsAffectedByMirrorMove() const;
    const bool   IsAffectedByKingRock() const;
    const bool           IsSoundBased() const;
    const bool    CanBypassSubstitute() const;

private:
    size_t      m_movenum;
    std::string m_name;

    std::string     m_type;
    PokemonType    m_typee;
    std::string m_category;
    Category   m_categorye;

    int              m_priority;
    MoveEffect m_moveEffectEnum;
    int          m_effectChance;

    int       m_pp;
    int    m_maxpp;
    int    m_power;
    int m_accuracy;

    bool        b_DoesMakeContact;
    bool    b_IsAffectedByProtect;
    bool  b_IsAffectedByMagicCoat;
    bool     b_IsAffectedBySnatch;
    bool b_IsAffectedByMirrorMove;
    bool   b_IsAffectedByKingRock;
    bool           b_isSoundBased;
    bool       b_bypassSubstitute;
};