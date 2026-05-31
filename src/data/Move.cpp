#include "Move.h"

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

unsigned int Move::GetPower() const
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