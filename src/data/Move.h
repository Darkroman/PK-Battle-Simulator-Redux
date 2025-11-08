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

    size_t         GetMoveIndex() const;
    std::string_view    GetName() const;
    
    std::string_view     GetCategory() const;
    Category         GetCategoryEnum() const;
    std::string_view     GetMoveType() const;
    PokemonType      GetMoveTypeEnum() const;
    
    int              GetPriority() const;
    MoveEffect GetMoveEffectEnum() const;
    int          GetEffectChance() const;
    
    int       GetPP() const;
    int    GetMaxPP() const;
    int    GetPower() const;
    int GetAccuracy() const;

    bool        DoesMakeContact() const;
    bool    IsAffectedByProtect() const;
    bool  IsAffectedByMagicCoat() const;
    bool     IsAffectedBySnatch() const;
    bool IsAffectedByMirrorMove() const;
    bool   IsAffectedByKingRock() const;
    bool           IsSoundBased() const;
    bool    CanBypassSubstitute() const;

private:
    size_t            m_movenum;
    std::string_view     m_name;

    std::string_view     m_type;
    PokemonType         m_typee;
    std::string_view m_category;
    Category        m_categorye;

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