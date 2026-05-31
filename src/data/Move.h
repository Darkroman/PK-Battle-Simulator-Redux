// Move data layout in movedex.txt is as follows:
// Index number, name, type, category, priority, secondary effect flag, secondary effect chance, pp, max pp, power, accuracy.
// Each bool corresponds with whether it:
// Makes contact, affected by Protect, affected by Magic Coat, affected by Snatch, affected by Mirror Move, affected by King's Rock, sound based, bypasses substitute

#pragma once

#include <string_view>

enum class PokemonType : size_t;
enum class Category;
enum class MoveEffect;

class Move
{
public:
    constexpr Move
    (size_t movenum, std::string_view name,
        std::string_view type, PokemonType typee, std::string_view category, Category categorye,
        int priority, MoveEffect moveEffectEnum, int effectChance,
        int pp, int maxpp, unsigned int power, int accuracy,
        bool contact, bool protect, bool magicCoat,
        bool snatch, bool mirrorMove, bool kingRock, bool soundBased, bool bypassSubstitute)
        : m_movenum{ movenum }, m_name{ name },
        m_type{ type }, m_typee{ typee }, m_category{ category }, m_categorye{ categorye },
        m_priority{ priority }, m_moveEffectEnum{ moveEffectEnum }, m_effectChance{ effectChance },
        m_pp{ pp }, m_maxpp{ maxpp }, m_power{ power }, m_accuracy{ accuracy },
        b_DoesMakeContact{ contact }, b_IsAffectedByProtect{ protect }, b_IsAffectedByMagicCoat{ magicCoat },
        b_IsAffectedBySnatch{ snatch }, b_IsAffectedByMirrorMove{ mirrorMove }, b_IsAffectedByKingRock{ kingRock }, b_isSoundBased{ soundBased }, b_bypassSubstitute{ bypassSubstitute }
    {
    }

    size_t         GetMoveIndex() const;
    std::string_view    GetName() const;
    
    std::string_view     GetCategory() const;
    Category         GetCategoryEnum() const;
    std::string_view     GetMoveType() const;
    PokemonType      GetMoveTypeEnum() const;
    
    int              GetPriority() const;
    MoveEffect GetMoveEffectEnum() const;
    int          GetEffectChance() const;
    
    int             GetPP() const;
    int          GetMaxPP() const;
    unsigned int GetPower() const;
    int       GetAccuracy() const;

    bool        DoesMakeContact() const;
    bool    IsAffectedByProtect() const;
    bool  IsAffectedByMagicCoat() const;
    bool     IsAffectedBySnatch() const;
    bool IsAffectedByMirrorMove() const;
    bool   IsAffectedByKingRock() const;
    bool           IsSoundBased() const;
    bool    CanBypassSubstitute() const;

private:
    std::string_view     m_name;
    std::string_view     m_type;
    std::string_view m_category;

    size_t            m_movenum;

    PokemonType         m_typee;
    Category        m_categorye;
    MoveEffect m_moveEffectEnum;

    int              m_priority;
    int          m_effectChance;

    int                    m_pp;
    int                 m_maxpp;
    unsigned int        m_power;
    int        m_accuracy;

    bool        b_DoesMakeContact;
    bool    b_IsAffectedByProtect;
    bool  b_IsAffectedByMagicCoat;
    bool     b_IsAffectedBySnatch;
    bool b_IsAffectedByMirrorMove;
    bool   b_IsAffectedByKingRock;
    bool           b_isSoundBased;
    bool       b_bypassSubstitute;
};