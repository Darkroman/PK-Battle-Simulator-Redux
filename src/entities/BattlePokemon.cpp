#include <iostream>
#include <iomanip>

#include "../data/Pokemon.h"
#include "../data/Move.h"
#include "../data/Database.h"

#include "BattlePokemon.h"

//BattlePokemon::BattlePokemon() {}

const int EV_TOTAL_ALLOWED = 510;

BattlePokemon::pokemonMove::pokemonMove() {}

BattlePokemon::DetransformData::DetransformData() {}

void BattlePokemon::DetransformData::BackupOriginalPokemonData(BattlePokemon* pokemon)
{
    mp_pokemon = pokemon->GetPokemonDatabasePointer();

    m_attack_iv = pokemon->GetAttackIV();
    m_defense_iv = pokemon->GetDefenseIV();
    m_specialattack_iv = pokemon->GetSpecialAttackIV();
    m_specialdefense_iv = pokemon->GetSpecialDefenseIV();
    m_speed_iv = pokemon->GetSpeedIV();

    m_attack_ev = pokemon->GetAttackEV();
    m_defense_ev = pokemon->GetDefenseEV();
    m_specialattack_ev = pokemon->GetSpecialAttackEV();
    m_specialdefense_ev = pokemon->GetSpecialDefenseEV();
    m_speed_ev = pokemon->GetSpeedEV();

    m_ev_total = pokemon->GetTotalEVs();

    m_type1 = pokemon->GetTypeOne();
    m_type1e = pokemon->GetTypeOneEnum();
    m_type2 = pokemon->GetTypeTwo();
    m_type2e = pokemon->GetTypeTwoEnum();

    m_attackstage = pokemon->GetAttackStage();
    m_defensestage = pokemon->GetDefenseStage();
    m_specialattackstage = pokemon->GetSpecialAttackStage();
    m_specialdefensestage = pokemon->GetSpecialDefenseStage();
    m_speedstage = pokemon->GetSpeedStage();
    m_evasionstage = pokemon->GetEvasionStage();
    m_accuracystage = pokemon->GetAccuracyStage();
    m_criticalhitstage = pokemon->GetCriticalHitStage();

    m_moveCount = pokemon->GetMoveCount();

    for (size_t i = 0; i < m_array_moves.size(); ++i)
    {
        m_array_moves[i].SetMovePointer(pokemon->GetMove(i + 1)->GetMovePointer());
        m_array_moves[i].m_currentPP = pokemon->GetMove(i + 1)->m_currentPP;
        m_array_moves[i].m_maxPP = pokemon->GetMove(i + 1)->m_maxPP;
        m_array_moves[i].b_isDisabled = pokemon->GetMove(i + 1)->b_isDisabled;
        m_array_moves[i].b_isMimicked = pokemon->GetMove(i + 1)->b_isMimicked;
    }
}

Pokemon* BattlePokemon::GetPokemonDatabasePointer() const
{
    return mp_pokemon;
}

void BattlePokemon::SetPokemon(size_t pknum)
{
    if (pknum < 1 || pknum > 151)
    {
        std::cout << "Invalid input!\n";
        return;
    }
    --pknum;

    if (mp_pokemon != nullptr)
    {
        ResetStatsAndMoves();
    }

    mp_pokemon = Database::GetInstance().GetPointerToPokedexNumber(pknum);
    m_name = mp_pokemon->GetName();
    m_type1 = mp_pokemon->GetFirstType();
    m_type1e = mp_pokemon->GetFirstTypeEnum();
    m_type2 = mp_pokemon->GetSecondType();
    m_type2e = mp_pokemon->GetSecondTypeEnum();

    ResetValues();
    
    UpdateStats();
}

void BattlePokemon::SetPokemon(std::string_view pkname)
{
    auto FoundPokemon = [&pkname](std::string_view n) { return n == pkname; };

    size_t result{};
    if (auto it = std::ranges::find_if(Database::GetInstance().GetPokedexVector(), FoundPokemon, &Pokemon::GetName); it != Database::GetInstance().cPokedexEnd())
    {
       result = std::ranges::distance(Database::GetInstance().cPokedexBegin(), it);
    }
    else
    {
        std::cout << "That Pokemon doesn't exist!\n";
        return;
    }

    ResetStatsAndMoves();

    mp_pokemon = Database::GetInstance().GetPointerToPokedexNumber(result);
    m_name = mp_pokemon->GetName();
    m_type1 = mp_pokemon->GetFirstType();
    m_type1e = mp_pokemon->GetFirstTypeEnum();
    m_type2 = mp_pokemon->GetSecondType();
    m_type2e = mp_pokemon->GetSecondTypeEnum();
    b_hasNickname = false;

    ResetValues();

    UpdateStats();
}

void BattlePokemon::ReleasePokemon()
{
    ResetStatsAndMoves();
}

void BattlePokemon::SetMove(size_t moveslot, size_t movenum)
{
    --moveslot;
    --movenum;
    bool CanLearnMove = false;
    CanLearnMove = mp_pokemon->CheckPokemonMoveList(movenum);

    for (auto& i : m_array_moves)
    {
        if (Database::GetInstance().GetPointerToMovedexNumber(movenum) == i.GetMovePointer())
        {
            std::cout << this->m_name << " already knows " << Database::GetInstance().GetPointerToMovedexNumber(movenum)->GetName() << "!\n";
            return;
        }
    }

    if (!CanLearnMove)
    {
        //std::cout << this->m_name << " cannot learn " << Database::GetInstance().GetPointerToMovedexNumber(movenum)->GetName() << "!\n";
        return;
    }
    else
    {
        m_array_moves[moveslot].SetMovePointer(Database::GetInstance().GetPointerToMovedexNumber(movenum));
        m_array_moves[moveslot].m_currentPP = m_array_moves[moveslot].GetPP();
        m_array_moves[moveslot].m_maxPP = m_array_moves[moveslot].GetPP();
        //std::cout << this->m_name <<  " was able to learn move " << m_array_moves[moveslot].GetName() << " successfully!" << '\n';
    }
}

void BattlePokemon::SetMove(size_t moveslot, std::string_view movename)
{
    --moveslot;

    auto FoundMove = [&movename](std::string_view n) { return n == movename; };

    size_t result{};
    if (auto it = std::ranges::find_if(Database::GetInstance().GetMovedexVector(), FoundMove, &Move::GetName); it != Database::GetInstance().cMovedexEnd())
    {
        result = std::ranges::distance(Database::GetInstance().cMovedexBegin(), it);
    }
    else
    {
        std::cout << "That move doesn't exist!\n";
        return;
    }

    bool CanLearnMove = false;
    CanLearnMove = mp_pokemon->CheckPokemonMoveList(result);

    for (const auto& i : m_array_moves)
    {
        if (Database::GetInstance().GetPointerToMovedexNumber(result) == i.GetMovePointer())
        {
            std::cout << this->m_name << " already knows " << Database::GetInstance().GetPointerToMovedexNumber(result)->GetName() << "!\n";
            return;
        }
    }

    if (!CanLearnMove)
    {
        std::cout << this->m_name << " cannot learn " << Database::GetInstance().GetPointerToMovedexNumber(result)->GetName() << "!\n";
    }
    else
    {
        m_array_moves[moveslot].SetMovePointer(Database::GetInstance().GetPointerToMovedexNumber(result));
        m_array_moves[moveslot].m_currentPP = m_array_moves[moveslot].GetPP();
        m_array_moves[moveslot].m_maxPP = m_array_moves[moveslot].GetPP();
        //std::cout << this->m_name << " was able to learn move " << m_array_moves[moveslot].GetName() << " successfully!" << '\n';
    }
}

void BattlePokemon::SetNickname(std::string_view input)
{
    if (input == "0")
    {
        m_nickname = "";
        b_hasNickname = false;
    }
    else
    {
        m_nickname = input;
        b_hasNickname = true;
    }
}

bool BattlePokemon::HasNickname() const
{
    return b_hasNickname;
}

std::string_view BattlePokemon::GetNickname() const
{
    return m_nickname;
}

void BattlePokemon::SetLevel(int input)
{
    bool exit = false;
    while (exit == false)
    {
        if (input < 1 || input > 100)
        {
            std::cout << "Level can only be between 1 and 100!\n";
            return;
        }
        else
        {
            m_level = input;
            UpdateStats();
            return;
        }
    }
}

void BattlePokemon::SetHPIV(int input)
{
    if (input > 31)
    {
        std::cout << "Cannot exceed 31 IVs, setting HP IV to 31.\n";
        m_hp_iv = 31;
        UpdateStats();
    }
    else if (input < 0)
    {
        std::cout << "Cannot go below 0 IVs, setting HP IV to 0.\n";
        m_hp_iv = 0;
        UpdateStats();
    }
    else
    {
        m_hp_iv = input;
        UpdateStats();
    }
}

void BattlePokemon::SetAttackIV(int input)
{
    if (input > 31)
    {
        std::cout << "Cannot exceed 31 IVs, setting Attack IV to 31.\n";
        m_attack_iv = 31;
        UpdateStats();
    }
    else if (input < 0)
    {
        std::cout << "Cannot go below 0 IVs, setting Attack IV to 0.\n";
        m_attack_iv = 0;
        UpdateStats();
    }
    else
    {
        m_attack_iv = input;
        UpdateStats();
    }
}

void BattlePokemon::SetDefenseIV(int input)
{
    if (input > 31)
    {
        std::cout << "Cannot exceed 31 IVs, setting Defense IV to 31.\n";
        m_defense_iv = 31;
        UpdateStats();
    }
    else if (input < 0)
    {
        std::cout << "Cannot go below 0 IVs, setting Defense IV to 0.\n";
        m_defense_iv = 0;
        UpdateStats();
    }
    else
    {
        m_defense_iv = input;
        UpdateStats();
    }
}

void BattlePokemon::SetSpecialAttackIV(int input)
{
    if (input > 31)
    {
        std::cout << "Cannot exceed 31 IVs, setting Special Attack IV to 31.\n";
        m_specialattack_iv = 31;
        UpdateStats();
    }
    else if (input < 0)
    {
        std::cout << "Cannot go below 0 IVs, setting Special Attack IV to 0.\n";
        m_specialattack_iv = 0;
        UpdateStats();
    }
    else
    {
        m_specialattack_iv = input;
        UpdateStats();
    }
}

void BattlePokemon::SetSpecialDefenseIV(int input)
{
    if (input > 31)
    {
        std::cout << "Cannot exceed 31 IVs, setting Special Defense IV to 31.\n";
        m_specialdefense_iv = 31;
        UpdateStats();
    }
    else if (input < 0)
    {
        std::cout << "Cannot go below 0 IVs, setting Special Defense IV to 0.\n";
        m_specialdefense_iv = 0;
        UpdateStats();
    }
    else
    {
        m_specialdefense_iv = input;
        UpdateStats();
    }
}

void BattlePokemon::SetSpeedIV(int input)
{
    if (input > 31)
    {
        std::cout << "Cannot exceed 31 IVs, setting Speed IV to 31.\n";
        m_speed_iv = 31;
        UpdateStats();
    }
    else if (input < 0)
    {
        std::cout << "Cannot go below 0 IVs, setting Speed IV to 0.\n";
        m_speed_iv = 0;
        UpdateStats();
    }
    else
    {
        m_speed_iv = input;
        UpdateStats();
    }
}

bool BattlePokemon::SetHPEV(int input)
{
    int ev_value{ input };
    ev_value -= m_hp_ev;

    int ev_total_temp{ 0 };
    ev_total_temp = m_ev_total;
    ev_total_temp += ev_value;

    if (ExceedsMaxAllowedEVs(ev_total_temp))
    {
        return false;
    }

    m_hp_ev = input;
    m_ev_total += ev_value;
    UpdateStats();

    return true;
}

bool BattlePokemon::SetAttackEV(int input)
{
    int ev_value{ input };
    ev_value -= m_attack_ev;

    int ev_total_temp{ 0 };
    ev_total_temp = m_ev_total;
    ev_total_temp += ev_value;

    if (ExceedsMaxAllowedEVs(ev_total_temp))
    {
        return false;
    }

    m_attack_ev = input;
    m_ev_total += ev_value;
    UpdateStats();

    return true;
}

bool BattlePokemon::SetDefenseEV(int input)
{
    int ev_value{ input };
    ev_value = ev_value - m_defense_ev;

    int ev_total_temp{ 0 };
    ev_total_temp = m_ev_total;
    ev_total_temp += ev_value;

    if (ExceedsMaxAllowedEVs(ev_total_temp))
    {
        return false;
    }

    m_defense_ev = input;
    m_ev_total = m_ev_total + ev_value;
    UpdateStats();

    return true;
}

bool BattlePokemon::SetSpecialAttackEV(int input)
{
    int ev_value{ input };
    ev_value -= m_specialattack_ev;

    int ev_total_temp{ 0 };
    ev_total_temp = m_ev_total;
    ev_total_temp += ev_value;

    if (ExceedsMaxAllowedEVs(ev_total_temp))
    {
        return false;
    }

    m_specialattack_ev = input;
    m_ev_total += ev_value;
    UpdateStats();

    return true;
}

bool BattlePokemon::SetSpecialDefenseEV(int input)
{
    int ev_value{ input };
    ev_value -= m_specialdefense_ev;

    int ev_total_temp{ 0 };
    ev_total_temp = m_ev_total;
    ev_total_temp += ev_value;

    if (ExceedsMaxAllowedEVs(ev_total_temp))
    {
        return false;
    }

    m_specialdefense_ev = input;
    m_ev_total += ev_value;
    UpdateStats();

    return true;
}

bool BattlePokemon::SetSpeedEV(int input)
{
    int ev_value{ input };
    ev_value -= m_speed_ev;

    int ev_total_temp{ 0 };
    ev_total_temp = m_ev_total;
    ev_total_temp += ev_value;

    if (ExceedsMaxAllowedEVs(ev_total_temp))
    {
        return false;
    }

    m_speed_ev = input;
    m_ev_total += ev_value;
    UpdateStats();

    return true;
}

int BattlePokemon::GetHPEV() const
{
    return m_hp_ev;
}

int BattlePokemon::GetAttackEV() const
{
    return m_attack_ev;
}

int BattlePokemon::GetDefenseEV() const
{
    return m_defense_ev;
}

int BattlePokemon::GetSpecialAttackEV() const
{
    return m_specialattack_ev;
}

int BattlePokemon::GetSpecialDefenseEV() const
{
    return m_specialdefense_ev;
}

int BattlePokemon::GetSpeedEV() const
{
    return m_speed_ev;
}

int BattlePokemon::GetHPIV() const
{
    return m_hp_iv;
}

int BattlePokemon::GetAttackIV() const
{
    return m_attack_iv;
}

int BattlePokemon::GetDefenseIV() const
{
    return m_defense_iv;
}

int BattlePokemon::GetSpecialAttackIV() const
{
    return m_specialattack_iv;
}

int BattlePokemon::GetSpecialDefenseIV() const
{
    return m_specialdefense_iv;
}

int BattlePokemon::GetSpeedIV() const
{
    return m_speed_iv;
}

bool BattlePokemon::ExceedsMaxAllowedEVs(int ev_total_temp)
{
    if (ev_total_temp > EV_TOTAL_ALLOWED)
    {
        std::cout << "You are not allowed to exceed 510 total EVs\n";
        return true;
    }
    else
    {
        return false;
    }
}

void BattlePokemon::UpdateStats()
{
    //m_maxHP = ((m_hp_iv + 2 * mp_pokemon->GetBaseHP() + (m_hp_ev / 4)) * m_level / 100) + 10 + m_level;
    m_currentHP = GetMaxHP();
    //m_attack = (((m_attack_iv + 2 * mp_pokemon->GetBaseAttack() + (m_attack_ev / 4)) * m_level / 100) + 5);
    //m_defense = (((m_defense_iv + 2 * mp_pokemon->GetBaseDefense() + (m_defense_ev / 4)) * m_level / 100) + 5);
    //m_specialattack = (((m_specialattack_iv + 2 * mp_pokemon->GetBaseSpecialAttack() + (m_specialattack_ev / 4)) * m_level / 100) + 5);
    //m_specialdefense = (((m_specialdefense_iv + 2 * mp_pokemon->GetBaseSpecialDefense() + (m_specialdefense_ev / 4)) * m_level / 100) + 5);
    //m_speed = (((m_speed_iv + 2 * mp_pokemon->GetBaseSpeed() + (m_speed_ev / 4)) * m_level / 100) + 5);
}

void BattlePokemon::DisplayStats() const
{
    std::cout << "Pokemon: " << m_name << '\n';
    ((b_hasNickname == false) ? std::cout << "No nickname\n" : std::cout << "Nickname: " << m_nickname << '\n');
    std::cout << "Type: " << m_type1 << "/" << m_type2 << '\n';
    std::cout << "Level: " << m_level << '\n';
    std::cout << "HP: " << GetMaxHP() << " -- [IV: " << m_hp_iv << "]" << " -- [EV: " << m_hp_ev << "]" << '\n';
    std::cout << "Attack: " << GetAttack() << " -- [IV: " << m_attack_iv << "]" << " -- [EV: " << m_attack_ev << "]" << '\n';
    std::cout << "Defense: " << GetDefense() << " -- [IV: " << m_defense_iv << "]" << " -- [EV: " << m_defense_ev << "]" << '\n';
    std::cout << "Special Attack: " << GetSpecialAttack() << " -- [IV: " << m_specialattack_iv << "]" << " -- [EV: " << m_specialattack_ev << "]" << '\n';
    std::cout << "Special Defense: " << GetSpecialDefense() << " -- [IV: " << m_specialdefense_iv << "]" << " -- [EV: " << m_specialdefense_ev << "]" << '\n';
    std::cout << "Speed: " << GetSpeed() << " -- [IV: " << m_speed_iv << "]" << " -- [EV: " << m_speed_ev << "]" << '\n';
}

void BattlePokemon::DisplayLearnableMoves() const
{
    Move* move;

    int colCount = 0;

    for (auto index = mp_pokemon->MovelistBegin(); index != mp_pokemon->MovelistEnd(); ++index)
    {
        ++colCount;
        auto moveNum = mp_pokemon->FetchMoveNumber(index);
        
        move = Database::GetInstance().GetPointerToMovedexNumber(moveNum);

        //std::cout << move->GetMoveIndex() << ": " << move->GetName() << " --- ";

        ++moveNum;
        if (moveNum < 10)
        {
            std::cout << move->GetMoveIndex() << ":   ";
            std::cout << std::setw(15) << std::left << move->GetName();
        }
        else if (moveNum >= 10 && moveNum <= 99)
        {
            std::cout << move->GetMoveIndex() << ":  ";
            std::cout << std::setw(15) << std::left << move->GetName();
        }
        else if (moveNum > 99)
        {
            std::cout << move->GetMoveIndex() << ": ";
            std::cout << std::setw(15) << std::left << move->GetName();
        }

        if (colCount % 6 == 0)
        {
            std::cout << '\n';
        }
    }
    std::cout << '\n';
}

void BattlePokemon::DisplayIVs() const
{
    std::cout << "HP IV: " << m_hp_iv << '\n';
    std::cout << "Attack IV: " << m_attack_iv << '\n';
    std::cout << "Defense IV: " << m_defense_iv << '\n';
    std::cout << "Special Attack IV: " << m_specialattack_iv << '\n';
    std::cout << "Special Defense IV: " << m_specialdefense_iv << '\n';
    std::cout << "Speed IV: " << m_speed_iv << "\n\n";
}

void BattlePokemon::DisplayEVs() const
{
    std::cout << "HP EV: " << m_hp_ev << '\n';
    std::cout << "Attack EV: " << m_attack_ev << '\n';
    std::cout << "Defense EV: " << m_defense_ev << '\n';
    std::cout << "Special Attack EV: " << m_specialattack_ev << '\n';
    std::cout << "Special Defense EV: " << m_specialdefense_ev << '\n';
    std::cout << "Speed EV: " << m_speed_ev << "\n\n";
}

const std::string& BattlePokemon::GetPokemonName() const
{
    return m_name;
}

std::string_view BattlePokemon::GetPokemonNameView() const
{
    return m_name;
}

const std::string& BattlePokemon::GetNickName() const
{
    return m_nickname;
}

std::string_view BattlePokemon::GetNickNameView() const
{
    return m_nickname;
}

const std::string& BattlePokemon::GetName() const
{
    return ((b_hasNickname == false) ? m_name : m_nickname);
}

std::string_view BattlePokemon::GetNameView() const
{
    return ((b_hasNickname == false) ? m_name : m_nickname);
}

int BattlePokemon::GetLevel() const
{
    return m_level;
}

void BattlePokemon::DisplayLearnedMoves() const
{
    for (const auto& i : m_array_moves)
    {
        if (i.IsActive())
            std::cout << i.GetName() << "/";
        else
            std::cout << "---/";
    }
    std::cout << '\n';
}

void BattlePokemon::DisplayMovesInBattle() const
{
    int num = 1;

    for (const auto& i: m_array_moves)
    {
        if (i.IsActive())
        {
            if (i.b_isDisabled)
            {
                std::cout << num << ". " << i.GetName() << " (Disabled!)\n";
            }
            else
            {
                std::cout << num << ". " << i.GetName() << " PP(" << i.m_currentPP << "/" << i.m_maxPP << ")" << "\n";
            }
            
        }
        else
            std::cout << num << ". " << "---\n";
        ++num;
    }
}

BattlePokemon::pokemonMove* BattlePokemon::GetMove(size_t moveslot)
{
    --moveslot;
    return &(m_array_moves[moveslot]);
}

void BattlePokemon::pokemonMove::SetMovePointer(Move* move)
{
    mp_move = move;
}

Move* BattlePokemon::pokemonMove::GetMovePointer() const
{
    return mp_move;
}

bool BattlePokemon::pokemonMove::IsActive() const
{
    return mp_move != nullptr;
}

size_t BattlePokemon::pokemonMove::GetMoveIndex() const
{
    return mp_move->GetMoveIndex();
}

std::string_view BattlePokemon::pokemonMove::GetName() const
{
    return mp_move->GetName();
}

std::string_view BattlePokemon::pokemonMove::GetCategory() const
{
    return mp_move->GetCategory();
}

Category BattlePokemon::pokemonMove::GetCategoryEnum() const
{
    return mp_move->GetCategoryEnum();
}

std::string_view BattlePokemon::pokemonMove::GetMoveType() const
{
    return mp_move->GetMoveType();
}

PokemonType BattlePokemon::pokemonMove::GetMoveTypeEnum() const
{
    return mp_move->GetMoveTypeEnum();
}

int BattlePokemon::pokemonMove::GetPriority() const
{
    return mp_move->GetPriority();
}

MoveEffect BattlePokemon::pokemonMove::GetMoveEffectEnum() const
{
    return mp_move->GetMoveEffectEnum();
}

int BattlePokemon::pokemonMove::GetEffectChance() const
{
    return mp_move->GetEffectChance();
}

int BattlePokemon::pokemonMove::GetPP() const
{
    return mp_move->GetPP();
}

int BattlePokemon::pokemonMove::GetMaxPP() const
{
    return mp_move->GetMaxPP();
}

int BattlePokemon::pokemonMove::GetPower() const
{
    return mp_move->GetPower();
}

int BattlePokemon::pokemonMove::GetAccuracy() const
{
    return mp_move->GetAccuracy();
}

bool BattlePokemon::pokemonMove::DoesMakeContact() const
{
    return mp_move->DoesMakeContact();
}

bool BattlePokemon::pokemonMove::IsAffectedByProtect() const
{
    return mp_move->IsAffectedByProtect();
}

bool BattlePokemon::pokemonMove::IsAffectedByMagicCoat() const
{
    return mp_move->IsAffectedByMagicCoat();
}

bool BattlePokemon::pokemonMove::IsAffectedBySnatch() const
{
    return mp_move->IsAffectedBySnatch();
}

bool BattlePokemon::pokemonMove::IsAffectedByMirrorMove() const
{
    return mp_move->IsAffectedByMirrorMove();
}

bool BattlePokemon::pokemonMove::IsAffectedByKingRock() const
{
    return mp_move->IsAffectedByKingRock();
}

bool BattlePokemon::pokemonMove::IsSoundBased() const
{
    return mp_move->IsSoundBased();
}

bool BattlePokemon::pokemonMove::CanBypassSubstitute() const
{
    return mp_move->CanBypassSubstitute();
}

int BattlePokemon::GetPP(size_t moveslot) const
{
    --moveslot;
    return m_array_moves[moveslot].m_currentPP;
}

int BattlePokemon::GetMaxPP(size_t moveslot) const
{
    --moveslot;
    return m_array_moves[moveslot].m_maxPP;
}

bool BattlePokemon::HasPokemon()
{
    return (mp_pokemon == nullptr) ? false : true;
}

bool BattlePokemon::HasMove(size_t moveslot)
{
    --moveslot;
    return (m_array_moves[moveslot].IsActive()) ? true : false;
}

void BattlePokemon::IncrementMoveCount()
{
    ++m_moveCount;
}

void BattlePokemon::DecrementMoveCount()
{
    --m_moveCount;
}

std::string_view BattlePokemon::GetMoveName(size_t moveslot) const
{
    --moveslot;
    return m_array_moves[moveslot].GetName();
}

BattlePokemon::pokemonMove* BattlePokemon::GetLastUsedMove() const
{
    return lastUsedMove;
}

void BattlePokemon::SetLastUsedMove(BattlePokemon::pokemonMove* move)
{
    lastUsedMove = move;
}

int BattlePokemon::GetTotalEVs() const
{
    return m_ev_total;
}

int BattlePokemon::GetCurrentHP() const
{
    return m_currentHP;
}

int BattlePokemon::GetMaxHP() const
{
    if (b_isTransformed == true)
    {
        return ((m_hp_iv + 2 * m_detransformData.mp_pokemon->GetBaseHP() + (m_hp_ev / 4)) * m_level / 100) + 10 + m_level;
    }
    else
    {
        return ((m_hp_iv + 2 * mp_pokemon->GetBaseHP() + (m_hp_ev / 4)) * m_level / 100) + 10 + m_level;
    }
}

int BattlePokemon::GetAttack() const
{
    if (b_isTransformed && b_transformBurnPenalty && currentStatus != Status::Burned)
    {
        return (((m_attack_iv + 2 * mp_pokemon->GetBaseAttack() + (m_attack_ev / 4)) * m_level / 100) + 5) / 2;
    }

    return (((m_attack_iv + 2 * mp_pokemon->GetBaseAttack() + (m_attack_ev / 4)) * m_level / 100) + 5);
}

int BattlePokemon::GetDefense() const
{
    return  (((m_defense_iv + 2 * mp_pokemon->GetBaseDefense() + (m_defense_ev / 4)) * m_level / 100) + 5);
}

int BattlePokemon::GetSpecialAttack() const
{
    return (((m_specialattack_iv + 2 * mp_pokemon->GetBaseSpecialAttack() + (m_specialattack_ev / 4)) * m_level / 100) + 5);
}

int BattlePokemon::GetSpecialDefense() const
{
    return (((m_specialdefense_iv + 2 * mp_pokemon->GetBaseSpecialDefense() + (m_specialdefense_ev / 4)) * m_level / 100) + 5);
}

int BattlePokemon::GetSpeed() const
{
    if (b_isTransformed && b_transformParalysisPenalty && currentStatus != Status::Paralyzed)
    {
        return (((m_speed_iv + 2 * mp_pokemon->GetBaseSpeed() + (m_speed_ev / 4)) * m_level / 100) + 5) / 2;
    }

    return (((m_speed_iv + 2 * mp_pokemon->GetBaseSpeed() + (m_speed_ev / 4)) * m_level / 100) + 5);
}

void BattlePokemon::DamageCurrentHP(int damage)
{
    m_currentHP -= damage;

    if (m_currentHP < 0)
    {
        m_currentHP = 0;
    }
}

void BattlePokemon::HealCurrentHP(int amount)
{
    m_currentHP += amount;

    if (m_currentHP > GetMaxHP())
    {
        m_currentHP = GetMaxHP();
    }
}

bool BattlePokemon::IsFainted() const
{
    return b_isFainted;
}

void BattlePokemon::SetFainted(bool fainted)
{
    if (fainted)
    {
        b_isFainted = true;
    }
    else
    {
        b_isFainted = false;
    }
}

std::string_view BattlePokemon::GetTypeOne() const
{
    return m_type1;
}

PokemonType BattlePokemon::GetTypeOneEnum() const
{
    return m_type1e;
}

std::string_view BattlePokemon::GetTypeTwo() const
{
    return m_type2;
}

PokemonType BattlePokemon::GetTypeTwoEnum() const
{
    return m_type2e;
}

int BattlePokemon::GetAttackStage() const
{
    return m_attackstage;
}

int BattlePokemon::GetDefenseStage() const
{
    return m_defensestage;
}

int BattlePokemon::GetSpecialAttackStage() const
{
    return m_specialattackstage;
}

int BattlePokemon::GetSpecialDefenseStage() const
{
    return m_specialdefensestage;
}

int BattlePokemon::GetSpeedStage() const
{
    return m_speedstage;
}

int BattlePokemon::GetEvasionStage() const
{
    return m_evasionstage;
}

int BattlePokemon::GetAccuracyStage() const
{
    return m_accuracystage;
}

int BattlePokemon::GetCriticalHitStage() const
{
    return m_criticalhitstage;
}

void BattlePokemon::SetAttackStage(int num)
{
    m_attackstage = num;
}

void BattlePokemon::SetDefenseStage(int num)
{
    m_defensestage = num;
}

void BattlePokemon::SetSpecialAttackStage(int num)
{
    m_specialattackstage = num;
}

void BattlePokemon::SetSpecialDefenseStage(int num)
{
    m_specialdefensestage = num;
}

void BattlePokemon::SetSpeedStage(int num)
{
    m_speedstage = num;
}

void BattlePokemon::SetEvasionStage(int num)
{
    m_evasionstage = num;
}

void BattlePokemon::SetAccuracyStage(int num)
{
    m_accuracystage = num;
}

void BattlePokemon::SetCriticalHitStage(int num)
{
    m_criticalhitstage = num;
}

int BattlePokemon::GetMoveCount() const
{
    return m_moveCount;
}

Status BattlePokemon::GetStatus() const
{
    return currentStatus;
}

void BattlePokemon::ChangeStatus(Status status)
{
    currentStatus = status;
}

bool BattlePokemon::IsConfused() const
{
    return b_isConfused;
}

void BattlePokemon::SetConfusedStatus(bool confused)
{
    b_isConfused = confused;
}

void BattlePokemon::SetConfusedTurnCount(int count)
{
    m_confusedTurnCount = count;
}

int BattlePokemon::GetConfusedTurnCount() const
{
    return m_confusedTurnCount;
}

void BattlePokemon::IncrementConfusedCounter()
{
    ++m_confusedCounter;
}

int BattlePokemon::GetConfusedCounter() const
{
    return m_confusedCounter;
}

void BattlePokemon::ResetConfusedCounter()
{
    m_confusedCounter = 0;
}

bool BattlePokemon::IsSeeded() const
{
    return b_isSeeded;
}

void BattlePokemon::SetSeededStatus(bool seeded)
{
    b_isSeeded = seeded;
}

bool BattlePokemon::IsBound() const
{
    return b_isBound;
}

void BattlePokemon::SetBound(bool bound)
{
    b_isBound = bound;
}

void BattlePokemon::SetBoundMoveName(size_t movenum)
{
    if (movenum == 20)
    {
        m_boundMove = "Bind";
    }
    else if (movenum == 35)
    {
        m_boundMove = "Wrap";
    }
    else if (movenum == 83)
    {
        m_boundMove = "Fire Spin";
    }
}

std::string_view BattlePokemon::GetBoundMoveName() const
{
    return m_boundMove;
}

void BattlePokemon::SetBoundTurnCount(int number)
{
    m_boundTurnCount = number;
}

int BattlePokemon::GetBoundTurnCount() const
{
    return m_boundTurnCount;
}

void BattlePokemon::IncrementBoundCounter()
{
    m_boundCounter++;
}

int BattlePokemon::GetBoundCounter() const
{
    return m_boundCounter;
}

void BattlePokemon::ResetBoundCounter()
{
    m_boundCounter = 0;
}

bool BattlePokemon::IsSemiInvulnerable() const
{
    return (b_isSemiInvulnerableDig || b_isSemiInvulnerableFly);
}

bool BattlePokemon::IsSemiInvulnerableFromDig() const
{
    return b_isSemiInvulnerableDig;
}

bool BattlePokemon::IsSemiInvulnerableFromFly() const
{
    return b_isSemiInvulnerableFly;
}

void BattlePokemon::SetSemiInvulnerableDig(bool dig)
{
    b_isSemiInvulnerableDig = dig;
}

void BattlePokemon::SetSemiInvulnerableFly(bool fly)
{
    b_isSemiInvulnerableFly = fly;
}

bool BattlePokemon::IsCharging() const
{
    return b_isCharging;
}

void BattlePokemon::SetCharging(bool charging)
{
    b_isCharging = charging;
}

bool BattlePokemon::IsRecharging() const
{
    return b_isRecharging;
}

void BattlePokemon::SetRecharging(bool recharging)
{
    b_isRecharging = recharging;
}

void BattlePokemon::SetSleepTurnCount(int number)
{
    m_sleepTurnCount = number;
}

int BattlePokemon::GetSleepTurnCount() const
{
    return m_sleepTurnCount;
}

void BattlePokemon::IncrementSleepCounter()
{
    m_sleepCounter++;
}

int BattlePokemon::GetSleepCounter() const
{
    return m_sleepCounter;
}

void BattlePokemon::ResetSleepCounter()
{
    m_sleepCounter = 0;
}

bool BattlePokemon::IsThrashing() const
{
    return b_isThrashing;
}

void BattlePokemon::SetThrashing(bool thrashing)
{
    b_isThrashing = thrashing;
}

void BattlePokemon::SetThrashTurnCount(int number)
{
    m_thrashTurnCount = number;
}

int BattlePokemon::GetThrashTurnCount() const
{
    return m_thrashTurnCount;
}

void BattlePokemon::IncrementThrashCounter()
{
    m_thrashCounter++;
}

int BattlePokemon::GetThrashCounter() const
{
    return m_thrashCounter;
}

void BattlePokemon::ResetThrashCounter()
{
    m_thrashCounter = 0;
}

void BattlePokemon::SetDisabledStatus(bool disabled)
{
    if (disabled == true)
    {
        if (lastUsedMove != nullptr)
        {
            b_moveIsDisabled = disabled;
            lastUsedMove->b_isDisabled = true;
            disabledMove = lastUsedMove;
        }
    }
    else
    {
        b_moveIsDisabled = false;
        disabledMove->b_isDisabled = false;
        disabledMove = nullptr;
    }
}

void BattlePokemon::IncrementDisabledCounter()
{
    m_disabledCounter++;
}

int BattlePokemon::GetDisabledCounter() const
{
    return m_disabledCounter;
}

void BattlePokemon::ResetDisabledCounter()
{
    m_disabledCounter = 0;
}

bool BattlePokemon::MoveIsDisabled() const
{
    return b_moveIsDisabled;
}

BattlePokemon::pokemonMove* BattlePokemon::GetDisabledMove() const
{
    return disabledMove;
}

int BattlePokemon::GetBadlyPoisonCounter() const
{
    return m_badlyPoisonCounter;
}

void BattlePokemon::ResetBadlyPoisonCounter()
{
    m_badlyPoisonCounter = 1;
}

void BattlePokemon::IncrementBadlyPoisonCounter()
{
    ++m_badlyPoisonCounter;
}

bool BattlePokemon::IsFlinched() const
{
    return b_isFlinched;
}

void BattlePokemon::SetIsFlinched(bool flinched)
{
    b_isFlinched = flinched;
}

bool BattlePokemon::HasUsedMinimize() const
{
    return b_hasUsedMinimize;
}

void BattlePokemon::SetUsedMinimize(bool minimize)
{
    b_hasUsedMinimize = minimize;
}

bool BattlePokemon::IsRaging() const
{
    return b_isRaging;
}

void BattlePokemon::SetRaging(bool rage)
{
    b_isRaging = rage;
}

bool BattlePokemon::HasUsedMimic() const
{
    return b_usedMimic;
}

void BattlePokemon::SetUsedMimic(bool mimic)
{
    b_usedMimic = mimic;
}

int BattlePokemon::GetMimicPP() const
{
    return m_mimicPP;
}

void BattlePokemon::SetMimicPP(int pp)
{
    m_mimicPP = pp;
}

bool BattlePokemon::HasFocusEnergy() const
{
    return b_hasFocusEnergy;
}

void BattlePokemon::SetFocusEnergy(bool focus)
{
    b_hasFocusEnergy = focus;
}

bool BattlePokemon::IsBiding() const
{
    return b_isBiding;
}

void BattlePokemon::SetBide(bool bide)
{
    b_isBiding = bide;
}

void BattlePokemon::SetBideTurnCount(int turnCount)
{
    m_bideTurnCount = turnCount;
}

int BattlePokemon::GetBideTurnCount() const
{
    return m_bideTurnCount;
}

void BattlePokemon::IncrementBideCounter()
{
    m_bideCounter++;
}

int BattlePokemon::GetBideCounter() const
{
    return m_bideCounter;
}

void BattlePokemon::ResetBideCounter()
{
    m_bideCounter = 0;
}

int BattlePokemon::GetBideDamage() const
{
    return m_bideDamageTaken;
}

void BattlePokemon::AddBideDamage(int bideDamage)
{
    m_bideDamageTaken += bideDamage;
}

void BattlePokemon::ResetBideDamage()
{
    m_bideDamageTaken = 0;
}

bool BattlePokemon::IsTransformed() const
{
    return b_isTransformed;
}

void BattlePokemon::SetTransformation(BattlePokemon* pokemon)
{
    m_detransformData.BackupOriginalPokemonData(this);

    mp_pokemon = pokemon->mp_pokemon;

    m_attack_iv = pokemon->GetAttackIV();
    m_defense_iv = pokemon->GetDefenseIV();
    m_specialattack_iv = pokemon->GetSpecialAttackIV();
    m_specialdefense_iv = pokemon->GetSpecialDefenseIV();
    m_speed_iv = pokemon->GetSpeedIV();

    m_attack_ev = pokemon->GetAttackEV();
    m_defense_ev = pokemon->GetDefenseEV();
    m_specialattack_ev = pokemon->GetSpecialAttackEV();
    m_specialdefense_ev = pokemon->GetSpecialDefenseEV();
    m_speed_ev = pokemon->GetSpeedEV();

    m_ev_total = pokemon->GetTotalEVs();

    m_type1 = pokemon->GetTypeOne();
    m_type1e = pokemon->GetTypeOneEnum();
    m_type2 = pokemon->GetTypeTwo();
    m_type2e = pokemon->GetTypeTwoEnum();

    m_attackstage = pokemon->GetAttackStage();
    m_defensestage = pokemon->GetDefenseStage();
    m_specialattackstage = pokemon->GetSpecialAttackStage();
    m_specialdefensestage = pokemon->GetSpecialDefenseStage();
    m_speedstage = pokemon->GetSpeedStage();
    m_evasionstage = pokemon->GetEvasionStage();
    m_accuracystage = pokemon->GetAccuracyStage();
    m_criticalhitstage = pokemon->GetCriticalHitStage();

    b_transformBurnPenalty = (GetStatus() == Status::Burned);
    b_transformParalysisPenalty = (GetStatus() == Status::Paralyzed);

    m_moveCount = pokemon->GetMoveCount();

    for (size_t i = 0; i < m_array_moves.size(); ++i)
    {
        m_array_moves[i].SetMovePointer(pokemon->GetMove(i + 1)->GetMovePointer());
        m_array_moves[i].m_currentPP = 5;
        m_array_moves[i].m_maxPP = 5;
        m_array_moves[i].b_isDisabled = false;
        m_array_moves[i].b_isMimicked = false;
    }

    b_isTransformed = true;
}

void BattlePokemon::Detransform()
{
    mp_pokemon = m_detransformData.mp_pokemon;

    m_attack_iv = m_detransformData.m_attack_iv;
    m_defense_iv = m_detransformData.m_defense_iv;
    m_specialattack_iv = m_detransformData.m_specialattack_iv;
    m_specialdefense_iv = m_detransformData.m_specialdefense_iv;
    m_speed_iv = m_detransformData.m_speed_iv;

    m_attack_ev = m_detransformData.m_attack_ev;
    m_defense_ev = m_detransformData.m_defense_ev;
    m_specialattack_ev = m_detransformData.m_specialattack_ev;
    m_specialdefense_ev = m_detransformData.m_specialdefense_ev;
    m_speed_ev = m_detransformData.m_speed_ev;

    m_ev_total = m_detransformData.m_ev_total;

    m_type1 = m_detransformData.m_type1;
    m_type1e = m_detransformData.m_type1e;
    m_type2 = m_detransformData.m_type2;
    m_type2e = m_detransformData.m_type2e;

    m_attackstage = m_detransformData.m_attackstage;
    m_defensestage = m_detransformData.m_defensestage;
    m_specialattackstage = m_detransformData.m_specialattackstage;
    m_specialdefensestage = m_detransformData.m_specialdefensestage;
    m_speedstage = m_detransformData.m_speedstage;
    m_evasionstage = m_detransformData.m_evasionstage;
    m_accuracystage = m_detransformData.m_accuracystage;
    m_criticalhitstage = m_detransformData.m_criticalhitstage;

    b_transformBurnPenalty = false;
    b_transformParalysisPenalty = false;

    m_moveCount = m_detransformData.m_moveCount;

    for (size_t i = 0; i < m_array_moves.size(); ++i)
    {
        m_array_moves[i].SetMovePointer(m_detransformData.m_array_moves[i].GetMovePointer());
        m_array_moves[i].m_currentPP = m_detransformData.m_array_moves[i].m_currentPP;
        m_array_moves[i].m_maxPP = m_detransformData.m_array_moves[i].m_maxPP;
        m_array_moves[i].b_isDisabled = m_detransformData.m_array_moves[i].b_isDisabled;
        m_array_moves[i].b_isMimicked = m_detransformData.m_array_moves[i].b_isMimicked;
    }

    this->b_isTransformed = false;
}

bool BattlePokemon::IsConverted() const
{
    return b_isConverted;
}

void BattlePokemon::SetConversion(BattlePokemon::pokemonMove* move)
{
    b_isConverted = true;

    m_beforeConversionType = m_type1;
    m_beforeConversionType_e = m_type1e;

    m_type1 = move->GetMoveType();
    m_type1e = move->GetMoveTypeEnum();
}

void BattlePokemon::Deconvert()
{
    b_isConverted = false;

    m_type1 = m_beforeConversionType;
    m_type1e = m_beforeConversionType_e;
}

bool BattlePokemon::HasSubstitute() const
{
    return b_hasSubstitute;
}

void BattlePokemon::SetSubstitute(bool sub)
{
    b_hasSubstitute = sub;
}

void BattlePokemon::SetSubstituteHP(int hp)
{
    m_substituteHealth = hp;
}

int BattlePokemon::GetSubstituteHP() const
{
    return m_substituteHealth;
}

void BattlePokemon::DamageSubstitute(int damage)
{
    m_substituteHealth -= damage;

    if (m_substituteHealth < 0)
    {
        m_substituteHealth = 0;
    }
}

BattlePokemon::pokemonMove* BattlePokemon::Struggle()
{
    m_struggle.SetMovePointer(Database::GetInstance().GetPointerToMovedexNumber(164));
    m_struggle.m_currentPP = 1;
    m_struggle.m_maxPP = 1;

    return &m_struggle;
}

void BattlePokemon::ResetStatsOnSwitch()
{
    if (IsTransformed())
    {
        Detransform();
    }

    if (IsConverted())
    {
        Deconvert();
    }

    SetAccuracyStage(0);
    SetAttackStage(0);
    SetCriticalHitStage(0);
    SetDefenseStage(0);
    SetEvasionStage(0);
    SetSpecialAttackStage(0);
    SetSpecialDefenseStage(0);
    SetSpeedStage(0);

    SetConfusedStatus(false);
    SetSemiInvulnerableDig(false);
    SetSemiInvulnerableFly(false);
    ResetBadlyPoisonCounter();
    SetSeededStatus(false);
    SetBound(false);
    ResetBoundCounter();
    SetBoundTurnCount(0);
    SetUsedMinimize(false);
    SetFocusEnergy(false);
    SetSubstitute(false);
    SetSubstituteHP(0);
    lastUsedMove = nullptr;

    if (HasUsedMimic())
    {
        for (size_t i = 1; i < 5; ++i)
        {
            if (GetMove(i)->b_isMimicked)
            {
                GetMove(i)->SetMovePointer(Database::GetInstance().GetPointerToMovedexNumber(101));
                GetMove(i)->m_currentPP = GetMimicPP();
                GetMove(i)->m_maxPP = GetMove(i)->GetPP();

                SetUsedMimic(false);
                GetMove(i)->b_isMimicked = false;
                break;
            }
        }
    }

    if (MoveIsDisabled())
    {
        for (size_t i = 1; i < 5; ++i)
        {
            if (GetMove(i)->b_isDisabled)
            {
                GetMove(i)->b_isDisabled = false;
                SetDisabledStatus(false);
                ResetDisabledCounter();
                break;
            }
        }
    }
}

void BattlePokemon::ResetStatsAndMoves()
{
    mp_pokemon = nullptr;

    m_name = "";
    m_nickname = "";
    b_hasNickname = false;

    m_level = 50;

    //m_maxHP = 0;
    m_currentHP = 0;
    //m_attack = 0;
    //m_defense = 0;
    //m_specialattack = 0;
    //m_specialdefense = 0;
    //m_speed = 0;

    m_hp_iv = 0;
    m_attack_iv = 0;
    m_defense_iv = 0;
    m_specialattack_iv = 0;
    m_specialdefense_iv = 0;
    m_speed_iv = 0;

    m_hp_ev = 0;
    m_attack_ev = 0;
    m_defense_ev = 0;
    m_specialattack_ev = 0;
    m_specialdefense_ev = 0;
    m_speed_iv = 0;

    m_moveCount = 0;

    m_ev_total = 0;

    for (size_t i = 0; i < m_array_moves.size(); ++i)
    {
        m_array_moves[i].SetMovePointer(nullptr);
        m_array_moves[i].m_currentPP = 0;
        m_array_moves[i].m_maxPP = 0;

        m_array_moves[i].b_isDisabled = false;
        m_array_moves[i].b_isMimicked = false;
    }
}

void BattlePokemon::ResetValues()
{
    if (b_isTransformed)
    {
        Detransform();
    }

    if (b_isConverted)
    {
        Deconvert();
    }

    m_currentHP = GetMaxHP();

    m_attackstage = 0;
    m_defensestage = 0;
    m_specialattackstage = 0;
    m_specialdefensestage = 0;
    m_speedstage = 0;
    m_evasionstage = 0;
    m_accuracystage = 0;
    m_criticalhitstage = 0;

    b_isFainted = false;

    b_isConfused = false;
    b_isSeeded = false;
    b_isBound = false;
    b_isSemiInvulnerableFly = false;
    b_isSemiInvulnerableDig = false;
    b_isRecharging = false;
    b_isCharging = false;
    b_isFlinched = false;
    b_hasUsedMinimize = false;
    b_isThrashing = false;
    b_moveIsDisabled = false;
    b_hasFocusEnergy = false;
    b_isBiding = false;
    b_isTransformed = false;
    b_isConverted = false;
    b_hasSubstitute = false;
    m_substituteHealth = 0;

    lastUsedMove = nullptr;
    disabledMove = nullptr;

    m_boundMove = "";

    currentStatus = Status::Normal;

    for (auto& i : m_array_moves)
    {
        if (i.IsActive())
        {
            i.m_currentPP = i.m_maxPP;
            i.b_isDisabled = false;
        }
    }

    if (this->HasUsedMimic())
    {
        for (size_t i = 1; i < 5; ++i)
        {
            if (this->GetMove(i)->b_isMimicked)
            {
                this->GetMove(i)->SetMovePointer(Database::GetInstance().GetPointerToMovedexNumber(101));
                this->GetMove(i)->m_currentPP = this->GetMimicPP();
                this->GetMove(i)->m_maxPP = this->GetMove(i)->GetPP();

                this->SetUsedMimic(false);
                this->GetMove(i)->b_isMimicked = false;
                break;
            }
        }
    }
}