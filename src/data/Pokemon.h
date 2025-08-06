#pragma once

#include <vector>
#include <string>

enum class PokemonType;

class Pokemon
{
public:
    Pokemon
    (int, std::string_view,
     int, int, int,
     int, int, int,
     std::string_view, PokemonType, std::string_view, PokemonType, int);

    int        GetNationalDexNumber() const;
    std::string_view        GetName() const;
    int                   GetBaseHP() const;
    int               GetBaseAttack() const;
    int              GetBaseDefense() const;
    int        GetBaseSpecialAttack() const;
    int       GetBaseSpecialDefense() const;
    int                GetBaseSpeed() const;
    std::string_view   GetFirstType() const;
    PokemonType    GetFirstTypeEnum() const;
    std::string_view  GetSecondType() const;
    PokemonType   GetSecondTypeEnum() const;
    std::vector<size_t> GetMoveList() const;

    int GetPokemonWeightHg() const;
    double GetPokemonWeightKg() const;

    std::vector<size_t>::iterator MovelistBegin();
    std::vector<size_t>::iterator   MovelistEnd();

    std::vector<size_t>::const_iterator MovelistBegin() const;
    std::vector<size_t>::const_iterator   MovelistEnd() const;

    bool CheckPokemonMoveList(size_t) const;
    size_t FetchMoveNumber(std::vector<size_t>::iterator);

    void ReserveMoveListVector();
    void EmplaceBackIntoMoveList(size_t);
    void ResizeMoveList(size_t);
    void ShrinkToFitMoveList();
    size_t Size();
    size_t Capacity();
    void CopyMoveListVector(std::vector<size_t>&);
    void AssignMoveListVector(std::initializer_list<size_t>);

private:
    int  m_nationalDexNumber;
    std::string       m_name;
    int             m_basehp;
    int         m_baseattack;
    int        m_basedefense;
    int  m_basespecialattack;
    int m_basespecialdefense;
    int          m_basespeed;
    std::string      m_type1;
    PokemonType     m_type1e;
    std::string      m_type2;
    PokemonType     m_type2e;
    int             m_weight;

    std::vector<size_t> m_movelist;
};