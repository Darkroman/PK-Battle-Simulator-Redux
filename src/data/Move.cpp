#include "Move.h"

Move::Move
(size_t movenum, std::string_view name,
    std::string_view type, PokemonType typee, std::string_view category, Category categorye,
    int priority, MoveEffect moveEffectEnum, int effectChance,
    int pp, int maxpp, int power, int accuracy,
    bool contact, bool protect, bool magicCoat,
    bool snatch, bool mirrorMove, bool kingRock, bool soundBased, bool bypassSubstitute)
    : m_movenum{ movenum }, m_name{ name },
    m_type{ type }, m_typee{ typee }, m_category{ category }, m_categorye{ categorye },
    m_priority{ priority }, m_moveEffectEnum{ moveEffectEnum }, m_effectChance{effectChance},
    m_pp{ pp }, m_maxpp{ maxpp }, m_power{ power }, m_accuracy{ accuracy },
    b_DoesMakeContact{ contact }, b_IsAffectedByProtect{ protect }, b_IsAffectedByMagicCoat{ magicCoat },
    b_IsAffectedBySnatch{ snatch }, b_IsAffectedByMirrorMove{ mirrorMove }, b_IsAffectedByKingRock{ kingRock }, b_isSoundBased{ soundBased }, b_bypassSubstitute{ bypassSubstitute }
{}

size_t Move::GetMoveIndex() const
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

Category Move::GetCategoryEnum() const
{
    return m_categorye;
}

std::string_view Move::GetMoveType() const
{
    return m_type;
}

PokemonType Move::GetMoveTypeEnum() const
{
    return m_typee;
}

int Move::GetPriority() const
{
    return m_priority;
}

MoveEffect Move::GetMoveEffectEnum() const
{
    return m_moveEffectEnum;
}

int Move::GetEffectChance() const
{
    return m_effectChance;
}

int Move::GetPP() const
{
    return m_pp;
}

int Move::GetMaxPP() const
{
    return m_maxpp;
}

int Move::GetPower() const
{
    return m_power;
}

int Move::GetAccuracy() const
{
    return m_accuracy;
}

bool Move::DoesMakeContact() const
{
    return b_DoesMakeContact;
}

bool Move::IsAffectedByProtect() const
{
    return b_IsAffectedByProtect;
}

bool Move::IsAffectedByMagicCoat() const
{
    return b_IsAffectedByMagicCoat;
}

bool Move::IsAffectedBySnatch() const
{
    return b_IsAffectedBySnatch;
}

bool Move::IsAffectedByMirrorMove() const
{
    return b_IsAffectedByMirrorMove;
}

bool Move::IsAffectedByKingRock() const
{
    return b_IsAffectedByKingRock;
}

bool Move::IsSoundBased() const
{
    return b_isSoundBased;
}

bool Move::CanBypassSubstitute() const
{
    return b_bypassSubstitute;
}

