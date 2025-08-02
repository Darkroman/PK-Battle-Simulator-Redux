#include "Pokemon.h"

Pokemon::Pokemon
(int nationalDexNumber, std::string_view name,
    int basehp, int baseattack, int basedefense,
    int basespecialattack, int basespecialdefense, int basespeed,
    std::string_view type1, PokemonType type1e, std::string_view type2, PokemonType type2e, double weight)
    : m_nationalDexNumber{ nationalDexNumber }, m_name{ name },
    m_basehp{ basehp }, m_baseattack{ baseattack }, m_basedefense{ basedefense },
    m_basespecialattack{ basespecialattack }, m_basespecialdefense{ basespecialdefense }, m_basespeed{ basespeed },
    m_type1{ type1 }, m_type1e{ type1e }, m_type2{ type2 }, m_type2e{ type2e }, m_weight{ weight }
{

}

const int Pokemon::GetNationalDexNumber() const
{
    return m_nationalDexNumber;
}

std::string_view Pokemon::GetName() const
{
    return m_name;
}

const int Pokemon::GetBaseHP() const
{
    return m_basehp;
}

const int Pokemon::GetBaseAttack() const
{
    return m_baseattack;
}

const int Pokemon::GetBaseDefense() const
{
    return m_basedefense;
}

const int Pokemon::GetBaseSpecialAttack() const
{
    return m_basespecialattack;
}

const int Pokemon::GetBaseSpecialDefense() const
{
    return m_basespecialdefense;
}

const int Pokemon::GetBaseSpeed() const
{
    return m_basespeed;
}

std::string_view Pokemon::GetFirstType() const
{
    return m_type1;
}

const PokemonType Pokemon::GetFirstTypeEnum() const
{
    return m_type1e;
}

std::string_view Pokemon::GetSecondType() const
{
    return m_type2;
}

const PokemonType Pokemon::GetSecondTypeEnum() const
{
    return m_type2e;
}

const double Pokemon::GetPokemonWeight() const
{
    return m_weight;
}

const std::vector<size_t> Pokemon::GetMoveList() const
{
    return m_movelist;
}

std::vector<size_t>::iterator Pokemon::MovelistBegin()
{
    return m_movelist.begin();
}

std::vector<size_t>::iterator Pokemon::MovelistEnd()
{
    return m_movelist.end();
}

std::vector<size_t>::const_iterator Pokemon::MovelistBegin() const
{
    return m_movelist.cbegin();
}

std::vector<size_t>::const_iterator Pokemon::MovelistEnd() const
{
    return m_movelist.cend();
}

bool Pokemon::CheckPokemonMoveList(size_t movenum) const
{
    ++movenum;
    for (auto index = m_movelist.begin(); index != m_movelist.end(); ++index)
    {
        if (movenum == *index)
            return true;
    }
    return false;
}

size_t Pokemon::FetchMoveNumber(std::vector<size_t>::iterator index)
{
    size_t element = *index - 1;
    return element;
}

void Pokemon::ReserveMoveListVector()
{
    m_movelist.reserve(57);
}

void Pokemon::EmplaceBackIntoMoveList(size_t number)
{
    m_movelist.emplace_back(number);
}

void Pokemon::ResizeMoveList(size_t number)
{
    m_movelist.resize(number);
}

void Pokemon::ShrinkToFitMoveList()
{
    m_movelist.shrink_to_fit();
}

size_t Pokemon::Size()
{
    return m_movelist.size();
}

size_t Pokemon::Capacity()
{
    return m_movelist.capacity();
}

void Pokemon::CopyMoveListVector(std::vector<size_t>& vec)
{
    m_movelist = vec;
}

void Pokemon::AssignMoveListVector(std::initializer_list<size_t> li)
{
    m_movelist.assign(li);
}