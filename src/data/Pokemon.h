#pragma once

//#include <array>
#include <span>
#include <string_view>

enum class PokemonType : size_t;

class Pokemon
{
public:
    
    Pokemon
    (unsigned int nationalDexNumber, std::string_view name,
        unsigned int basehp, unsigned int baseattack, unsigned int basedefense,
        unsigned int basespecialattack, unsigned int basespecialdefense, unsigned int basespeed,
        std::string_view type1, PokemonType type1e, std::string_view type2, PokemonType type2e, unsigned int weight);
    
    constexpr Pokemon
    (unsigned int nationalDexNumber, std::string_view name,
        unsigned int basehp, unsigned int baseattack, unsigned int basedefense,
        unsigned int basespecialattack, unsigned int basespecialdefense, unsigned int basespeed,
        std::string_view type1, PokemonType type1e, std::string_view type2, PokemonType type2e, unsigned int weight, std::span<const size_t> moves)
        : m_nationalDexNumber{ nationalDexNumber }, m_name{ name },
        m_basehp{ basehp }, m_baseattack{ baseattack }, m_basedefense{ basedefense },
        m_basespecialattack{ basespecialattack }, m_basespecialdefense{ basespecialdefense }, m_basespeed{ basespeed },
        m_type1{ type1 }, m_type1e{ type1e }, m_type2{ type2 }, m_type2e{ type2e }, m_weight{ weight }, m_movelist(moves)//, b_isDynamic(false)
    {
    }

    unsigned int        GetNationalDexNumber() const;
    std::string_view                 GetName() const;
    unsigned int                   GetBaseHP() const;
    unsigned int               GetBaseAttack() const;
    unsigned int              GetBaseDefense() const;
    unsigned int        GetBaseSpecialAttack() const;
    unsigned int       GetBaseSpecialDefense() const;
    unsigned int                GetBaseSpeed() const;
    std::string_view            GetFirstType() const;
    PokemonType             GetFirstTypeEnum() const;
    std::string_view           GetSecondType() const;
    PokemonType            GetSecondTypeEnum() const;
    std::span<const size_t>      GetMoveList() const;

    int GetPokemonWeightHg() const;
    double GetPokemonWeightKg() const;

    bool CheckPokemonMoveList(size_t) const;
    size_t FetchMoveNumber(size_t) const;

    //void DebugAddMove(size_t moveId);

private:
    std::span<const size_t> m_movelist;
    std::string_view      m_name;
    std::string_view      m_type1;
    std::string_view      m_type2;

    unsigned int  m_nationalDexNumber;
    unsigned int             m_basehp;
    unsigned int         m_baseattack;
    unsigned int        m_basedefense;
    unsigned int  m_basespecialattack;
    unsigned int m_basespecialdefense;
    unsigned int          m_basespeed;
    PokemonType              m_type1e;
    PokemonType              m_type2e;
    unsigned int             m_weight;

    //bool b_isDynamic{ false };

    //std::array<size_t, 164> m_debugMovelist{};
    //size_t m_debugMoveCount{};
};