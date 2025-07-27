#include "Move.h"

#include "StringToTypes.h"

Move::Move
(size_t movenum, std::string_view name,
    std::string_view type, PokemonType typee, std::string_view category, Category categorye,
    int priority, MoveEffect moveEffectEnum, int effectChance,
    int pp, int maxpp, int power, int accuracy,
    bool contact, bool protect, bool magicCoat,
    bool snatch, bool mirrorMove, bool kingRock, bool soundBased, bool bypassSubstitute)
    : m_movenum{ movenum }, m_name{ name },
    m_type{ type }, m_typee{ typee }, m_category{ category }, m_categorye{ categorye },
    m_priority{ priority }, m_moveEffectEnum{ moveEffectEnum },
    m_pp{ pp }, m_maxpp{ maxpp }, m_power{ power }, m_accuracy{ accuracy },
    b_DoesMakeContact{ contact }, b_IsAffectedByProtect{ protect }, b_IsAffectedByMagicCoat{ magicCoat },
    b_IsAffectedBySnatch{ snatch }, b_IsAffectedByMirrorMove{ mirrorMove }, b_IsAffectedByKingRock{ kingRock }, b_isSoundBased{ soundBased }, b_bypassSubstitute{ bypassSubstitute }
{}

const size_t Move::GetMoveIndex() const
{
    return m_movenum;
}

std::string_view Move::GetName() const
{
    return m_name;
}

std::string_view Move::GetCategory() const
{
    return m_category;
}

const Category Move::GetCategoryEnum() const
{
    return m_categorye;
}

std::string_view Move::GetMoveType() const
{
    return m_type;
}

const PokemonType Move::GetMoveTypeEnum() const
{
    return m_typee;
}

const int Move::GetPriority() const
{
    return m_priority;
}

const MoveEffect Move::GetMoveEffectEnum() const
{
    return m_moveEffectEnum;
}

const int Move::GetEffectChance() const
{
    return m_effectChance;
}

const int Move::GetPP() const
{
    return m_pp;
}

const int Move::GetMaxPP() const
{
    return m_maxpp;
}

const int Move::GetPower() const
{
    return m_power;
}

const int Move::GetAccuracy() const
{
    return m_accuracy;
}

const bool Move::DoesMakeContact() const
{
    return b_DoesMakeContact;
}

const bool Move::IsAffectedByProtect() const
{
    return b_IsAffectedByProtect;
}

const bool Move::IsAffectedByMagicCoat() const
{
    return b_IsAffectedByMagicCoat;
}

const bool Move::IsAffectedBySnatch() const
{
    return b_IsAffectedBySnatch;
}

const bool Move::IsAffectedByMirrorMove() const
{
    return b_IsAffectedByMirrorMove;
}

const bool Move::IsAffectedByKingRock() const
{
    return b_IsAffectedByKingRock;
}

const bool Move::IsSoundBased() const
{
    return b_isSoundBased;
}

const bool Move::CanBypassSubstitute() const
{
    return b_bypassSubstitute;
}

