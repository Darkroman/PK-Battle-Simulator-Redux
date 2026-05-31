#include "Pokemon.h"

Pokemon::Pokemon
(unsigned int nationalDexNumber, std::string_view name,
    unsigned int basehp, unsigned int baseattack, unsigned int basedefense,
    unsigned int basespecialattack, unsigned int basespecialdefense, unsigned int basespeed,
    std::string_view type1, PokemonType type1e, std::string_view type2, PokemonType type2e, unsigned int weight)
    : m_nationalDexNumber{ nationalDexNumber }, m_name{ name },
    m_basehp{ basehp }, m_baseattack{ baseattack }, m_basedefense{ basedefense },
    m_basespecialattack{ basespecialattack }, m_basespecialdefense{ basespecialdefense }, m_basespeed{ basespeed },
    m_type1{ type1 }, m_type1e{ type1e }, m_type2{ type2 }, m_type2e{ type2e }, m_weight{ weight }//, b_isDynamic(true)
{ }

unsigned int Pokemon::GetNationalDexNumber() const
{
    return m_nationalDexNumber;
}

std::string_view Pokemon::GetName() const
{
    return m_name;
}

unsigned int Pokemon::GetBaseHP() const
{
    return m_basehp;
}

unsigned int Pokemon::GetBaseAttack() const
{
    return m_baseattack;
}

unsigned int Pokemon::GetBaseDefense() const
{
    return m_basedefense;
}

unsigned int Pokemon::GetBaseSpecialAttack() const
{
    return m_basespecialattack;
}

unsigned int Pokemon::GetBaseSpecialDefense() const
{
    return m_basespecialdefense;
}

unsigned int Pokemon::GetBaseSpeed() const
{
    return m_basespeed;
}

std::string_view Pokemon::GetFirstType() const
{
    return m_type1;
}

PokemonType Pokemon::GetFirstTypeEnum() const
{
    return m_type1e;
}

std::string_view Pokemon::GetSecondType() const
{
    return m_type2;
}

PokemonType Pokemon::GetSecondTypeEnum() const
{
    return m_type2e;
}

int Pokemon::GetPokemonWeightHg() const
{
    return m_weight;
}

double Pokemon::GetPokemonWeightKg() const
{
    return static_cast<double>(m_weight) / 10;
}

std::span<const size_t> Pokemon::GetMoveList() const
{
    /*
    if (b_isDynamic)
    {
        return std::span<const size_t>(m_debugMovelist.data(), m_debugMoveCount);
    }
    */
    return m_movelist;
}

bool Pokemon::CheckPokemonMoveList(size_t movenum) const
{
    ++movenum;
    /*
    if (b_isDynamic)
    {
        for (size_t index = 1; index < 165; ++index)
        {
            if (movenum == index)
                return true;
        }
        return false;
    }
    */
    for (auto index = m_movelist.begin(); index != m_movelist.end(); ++index)
    {
        if (movenum == *index)
            return true;
    }
    return false;
}

size_t Pokemon::FetchMoveNumber(size_t index) const
{
    return index - 1;
}
/*
void Pokemon::DebugAddMove(size_t moveId)
{
    if (b_isDynamic && m_debugMoveCount < m_debugMovelist.size())
    {
        m_debugMovelist[m_debugMoveCount++] = moveId;
    }
}
*/