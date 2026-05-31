#include <ranges>
#include <charconv>

#include "BattlePokemon.h"

#include "../data/Pokemon.h"
#include "../data/Move.h"
#include "../data/Database.h"
#include "../common/InputValidation.h"

constexpr int EV_TOTAL_ALLOWED = 510;
constexpr int MAX_STAT_EV = 252;

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
        m_array_moves[i].SetMovePointer(pokemon->GetMove(i + 1).GetMovePointer());
        m_array_moves[i].m_currentPP = pokemon->GetMove(i + 1).m_currentPP;
        m_array_moves[i].m_maxPP = pokemon->GetMove(i + 1).m_maxPP;
        m_array_moves[i].b_isDisabled = pokemon->GetMove(i + 1).b_isDisabled;
        m_array_moves[i].b_isMimicked = pokemon->GetMove(i + 1).b_isMimicked;
    }
}

const Pokemon* BattlePokemon::GetPokemonDatabasePointer() const
{
    return mp_pokemon;
}

SetPokemonOutcome BattlePokemon::SetPokemon(std::string_view pkname)
{
    SetPokemonOutcome outcome{};

    size_t result{};
    auto& db = Database::GetInstance();

    if (IsDigits(pkname))
    {
        try 
        {
            std::from_chars(pkname.data(), pkname.data() + pkname.size(), result);

            if (result == 0)
            {
                outcome.result = SetPokemonResult::Exit;
                return outcome;
            }

            if (result > 151)
            {
                outcome.result = SetPokemonResult::InvalidRange;
                return outcome;
            }

            result--;
        }
        catch (...)
        {
            outcome.result = SetPokemonResult::InvalidRange;
            return outcome;
        }
    }
    else
    {
        auto pokedexView = db.GetPokedexView();
        bool found{ false };

        for (auto [index, pokemon] : std::views::enumerate(pokedexView))
        {
            if (pokemon.GetName() == pkname)
            {
                result = index;
                found = true;
                break;
            }
        }

        if (!found)
        {
            outcome.pokemonName = pkname;
            outcome.result = SetPokemonResult::InvalidPokemon;
            return outcome;
        }
    }

    ResetStatsAndMoves();

    mp_pokemon = db.GetPointerToPokedexNumber(result);
    m_name = mp_pokemon->GetName();
    m_type1 = mp_pokemon->GetFirstType();
    m_type1e = mp_pokemon->GetFirstTypeEnum();
    m_type2 = mp_pokemon->GetSecondType();
    m_type2e = mp_pokemon->GetSecondTypeEnum();
    b_hasNickname = false;

    ResetValues();

    UpdateStats();

    outcome.pokemonName = m_name;
    outcome.result = SetPokemonResult::Success;
    return outcome;
}

void BattlePokemon::SetPokemon(Pokemon* customPkmn)
{
    ResetStatsAndMoves();

    mp_pokemon = customPkmn;
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

SetMoveOutcome BattlePokemon::SetMove(size_t moveslot, std::string_view movename)
{
    --moveslot;

    SetMoveOutcome outcome{};

    if (moveslot > 3)
    {
        outcome.result = SetMoveResult::InvalidSlot;
        return outcome;
    }

    size_t result{};
    auto& db = Database::GetInstance();

    if (IsDigits(movename))
    {
        try
        {
            std::from_chars(movename.data(), movename.data() + movename.size(), result);

            if (result == 0)
            {
                outcome.result = SetMoveResult::Exit;
                return outcome;
            }

            if (result > 165)
            {
                outcome.result = SetMoveResult::InvalidRange;
                return outcome;
            }

            result--;
        }
        catch (...)
        {
            outcome.result = SetMoveResult::InvalidRange;
            return outcome;
        }
    }
    else
    {
        auto movedexView = db.GetMovedexView();
        bool found{ false };
        
        for (auto [index, move] : std::views::enumerate(movedexView))
        {
            if (move.GetName() == movename)
            {
                result = index;
                found = true;
                break;
            }
        }

        if (!found)
        {
            outcome.moveName = movename;
            outcome.result = SetMoveResult::InvalidMove;
            return outcome;
        }
    }

    auto movePtr = db.GetPointerToMovedexNumber(result);
    if (!mp_pokemon->CheckPokemonMoveList(result))
    {
        outcome.moveName = movePtr->GetName();
        outcome.result = SetMoveResult::NotLearnable;
        return outcome;
    }

    for (const auto& i : m_array_moves)
    {
        if (movePtr == i.GetMovePointer())
        {
            outcome.moveName = movePtr->GetName();
            outcome.result = SetMoveResult::DuplicateMove;
            return outcome;
        }
    }

    m_array_moves[moveslot].SetMovePointer(movePtr);
    m_array_moves[moveslot].m_currentPP = m_array_moves[moveslot].GetPP();
    m_array_moves[moveslot].m_maxPP = m_array_moves[moveslot].GetPP();

    outcome.moveName = m_array_moves[moveslot].GetName();
    outcome.result = SetMoveResult::Success;
    return outcome;
}

std::span<pokemonMove> BattlePokemon::GetMoveArray()
{
    return m_array_moves;
}

std::span<const pokemonMove> BattlePokemon::GetMoveArray() const
{
    return m_array_moves;
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

void BattlePokemon::SetLevel(unsigned int input)
{
    m_level = input;
    UpdateStats();
}

void BattlePokemon::SetHPIV(unsigned int input)
{
    m_hp_iv = input;
    UpdateStats();
}

void BattlePokemon::SetAttackIV(unsigned int input)
{
    m_attack_iv = input;
    UpdateStats();
}

void BattlePokemon::SetDefenseIV(unsigned int input)
{
    m_defense_iv = input;
    UpdateStats();
}

void BattlePokemon::SetSpecialAttackIV(unsigned int input)
{
    m_specialattack_iv = input;
    UpdateStats();
}

void BattlePokemon::SetSpecialDefenseIV(unsigned int input)
{
    m_specialdefense_iv = input;
    UpdateStats();
}

void BattlePokemon::SetSpeedIV(unsigned int input)
{
    m_speed_iv = input;
    UpdateStats();
}

SetEVResult BattlePokemon::SetHPEV(unsigned int input)
{
    if (input < 0 || input > MAX_STAT_EV)
    {
        return SetEVResult::InvalidValue;
    }

    int ev_value{ static_cast<int>(input) };
    ev_value -= m_hp_ev;

    int ev_total_temp{ 0 };
    ev_total_temp = m_ev_total;
    ev_total_temp += ev_value;

    if (ExceedsMaxAllowedEVs(ev_total_temp))
    {
        return SetEVResult::ExceedsTotal;
    }

    m_hp_ev = input;
    m_ev_total += ev_value;
    UpdateStats();

    return SetEVResult::Success;
}

SetEVResult BattlePokemon::SetAttackEV(unsigned int input)
{
    if (input < 0 || input > MAX_STAT_EV)
    {
        return SetEVResult::InvalidValue;
    }

    int ev_value{ static_cast<int>(input) };
    ev_value -= m_attack_ev;

    int ev_total_temp{ 0 };
    ev_total_temp = m_ev_total;
    ev_total_temp += ev_value;

    if (ExceedsMaxAllowedEVs(ev_total_temp))
    {
        return SetEVResult::ExceedsTotal;
    }

    m_attack_ev = input;
    m_ev_total += ev_value;
    UpdateStats();

    return SetEVResult::Success;
}

SetEVResult BattlePokemon::SetDefenseEV(unsigned int input)
{
    if (input < 0 || input > MAX_STAT_EV)
    {
        return SetEVResult::InvalidValue;
    }

    int ev_value{ static_cast<int>(input) };
    ev_value -= m_defense_ev;

    int ev_total_temp{ 0 };
    ev_total_temp = m_ev_total;
    ev_total_temp += ev_value;

    if (ExceedsMaxAllowedEVs(ev_total_temp))
    {
        return SetEVResult::ExceedsTotal;
    }

    m_defense_ev = input;
    m_ev_total += ev_value;
    UpdateStats();

    return SetEVResult::Success;
}

SetEVResult BattlePokemon::SetSpecialAttackEV(unsigned int input)
{
    if (input < 0 || input > MAX_STAT_EV)
    {
        return SetEVResult::InvalidValue;
    }

    int ev_value{ static_cast<int>(input) };
    ev_value -= m_specialattack_ev;

    int ev_total_temp{ 0 };
    ev_total_temp = m_ev_total;
    ev_total_temp += ev_value;

    if (ExceedsMaxAllowedEVs(ev_total_temp))
    {
        return SetEVResult::ExceedsTotal;
    }

    m_specialattack_ev = input;
    m_ev_total += ev_value;
    UpdateStats();

    return SetEVResult::Success;
}

SetEVResult BattlePokemon::SetSpecialDefenseEV(unsigned int input)
{
    if (input < 0 || input > MAX_STAT_EV)
    {
        return SetEVResult::InvalidValue;
    }

    int ev_value{ static_cast<int>(input) };
    ev_value -= m_specialdefense_ev;

    int ev_total_temp{ 0 };
    ev_total_temp = m_ev_total;
    ev_total_temp += ev_value;

    if (ExceedsMaxAllowedEVs(ev_total_temp))
    {
        return SetEVResult::ExceedsTotal;
    }

    m_specialdefense_ev = input;
    m_ev_total += ev_value;
    UpdateStats();

    return SetEVResult::Success;
}

SetEVResult BattlePokemon::SetSpeedEV(unsigned int input)
{
    if (input < 0 || input > MAX_STAT_EV)
    {
        return SetEVResult::InvalidValue;
    }

    int ev_value{ static_cast<int>(input) };
    ev_value -= m_speed_ev;

    int ev_total_temp{ 0 };
    ev_total_temp = m_ev_total;
    ev_total_temp += ev_value;

    if (ExceedsMaxAllowedEVs(ev_total_temp))
    {
        return SetEVResult::ExceedsTotal;
    }

    m_speed_ev = input;
    m_ev_total += ev_value;
    UpdateStats();

    return SetEVResult::Success;
}

unsigned int BattlePokemon::GetHPEV() const
{
    return m_hp_ev;
}

unsigned int BattlePokemon::GetAttackEV() const
{
    return m_attack_ev;
}

unsigned int BattlePokemon::GetDefenseEV() const
{
    return m_defense_ev;
}

unsigned int BattlePokemon::GetSpecialAttackEV() const
{
    return m_specialattack_ev;
}

unsigned int BattlePokemon::GetSpecialDefenseEV() const
{
    return m_specialdefense_ev;
}

unsigned int BattlePokemon::GetSpeedEV() const
{
    return m_speed_ev;
}

unsigned int BattlePokemon::GetHPIV() const
{
    return m_hp_iv;
}

unsigned int BattlePokemon::GetAttackIV() const
{
    return m_attack_iv;
}

unsigned int BattlePokemon::GetDefenseIV() const
{
    return m_defense_iv;
}

unsigned int BattlePokemon::GetSpecialAttackIV() const
{
    return m_specialattack_iv;
}

unsigned int BattlePokemon::GetSpecialDefenseIV() const
{
    return m_specialdefense_iv;
}

unsigned int BattlePokemon::GetSpeedIV() const
{
    return m_speed_iv;
}

bool BattlePokemon::ExceedsMaxAllowedEVs(unsigned int ev_total_temp)
{
    if (ev_total_temp > EV_TOTAL_ALLOWED)
    {
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

unsigned int BattlePokemon::GetLevel() const
{
    return m_level;
}

pokemonMove& BattlePokemon::GetMove(size_t moveslot)
{
    --moveslot;
    return m_array_moves[moveslot];
}

const pokemonMove& BattlePokemon::GetMove(size_t moveslot) const
{
    --moveslot;
    return m_array_moves[moveslot];
}

void BattlePokemon::DeleteMove(size_t moveslot)
{
    --moveslot;

    m_array_moves[moveslot].ResetMove();
}

void BattlePokemon::SwapMoves(size_t firstSlot, size_t secondSlot)
{
    --firstSlot;
    --secondSlot;

    std::swap(m_array_moves[firstSlot], m_array_moves[secondSlot]);
}

void BattlePokemon::ReorderMoves(size_t slotToMove, size_t targetSlot)
{
    --slotToMove;
    --targetSlot;

    if (slotToMove > targetSlot)
    {
        std::rotate(m_array_moves.begin() + targetSlot, m_array_moves.begin() + slotToMove, m_array_moves.begin() + slotToMove + 1);
    }
    else
    {
        std::rotate(m_array_moves.begin() + slotToMove, m_array_moves.begin() + slotToMove + 1, m_array_moves.begin() + targetSlot + 1);
    }
}

unsigned int BattlePokemon::GetPP(size_t moveslot) const
{
    --moveslot;
    return m_array_moves[moveslot].m_currentPP;
}

unsigned int BattlePokemon::GetMaxPP(size_t moveslot) const
{
    --moveslot;
    return m_array_moves[moveslot].m_maxPP;
}

bool BattlePokemon::WillPerformStruggle() const
{
    int inactiveCount{ 0 };

    for (const auto& move : m_array_moves)
    {
        if (!move.IsActive())
        {
            ++inactiveCount;
        }
    }

    if (inactiveCount >= m_array_moves.size())
    {
        return true;
    }
    return false;
}

bool BattlePokemon::HasPokemon() const
{
    return (mp_pokemon == nullptr) ? false : true;
}

bool BattlePokemon::HasMove(size_t moveslot)
{
    --moveslot;
    return (m_array_moves[moveslot].HasMove()) ? true : false;
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

pokemonMove* BattlePokemon::GetLastUsedMove() const
{
    return lastUsedMove;
}

void BattlePokemon::SetLastUsedMove(pokemonMove* move)
{
    lastUsedMove = move;
}

unsigned int BattlePokemon::GetTotalEVs() const
{
    return m_ev_total;
}

unsigned int BattlePokemon::GetCurrentHP() const
{
    return m_currentHP;
}

unsigned int BattlePokemon::GetMaxHP() const
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

unsigned int BattlePokemon::GetAttack() const
{
    if (b_isTransformed && b_transformBurnPenalty && currentStatus != Status::Burned)
    {
        return (((m_attack_iv + 2 * mp_pokemon->GetBaseAttack() + (m_attack_ev / 4)) * m_level / 100) + 5) / 2;
    }

    return (((m_attack_iv + 2 * mp_pokemon->GetBaseAttack() + (m_attack_ev / 4)) * m_level / 100) + 5);
}

unsigned int BattlePokemon::GetDefense() const
{
    return  (((m_defense_iv + 2 * mp_pokemon->GetBaseDefense() + (m_defense_ev / 4)) * m_level / 100) + 5);
}

unsigned int BattlePokemon::GetSpecialAttack() const
{
    return (((m_specialattack_iv + 2 * mp_pokemon->GetBaseSpecialAttack() + (m_specialattack_ev / 4)) * m_level / 100) + 5);
}

unsigned int BattlePokemon::GetSpecialDefense() const
{
    return (((m_specialdefense_iv + 2 * mp_pokemon->GetBaseSpecialDefense() + (m_specialdefense_ev / 4)) * m_level / 100) + 5);
}

unsigned int BattlePokemon::GetSpeed() const
{
    if (b_isTransformed && b_transformParalysisPenalty && currentStatus != Status::Paralyzed)
    {
        return (((m_speed_iv + 2 * mp_pokemon->GetBaseSpeed() + (m_speed_ev / 4)) * m_level / 100) + 5) / 2;
    }

    return (((m_speed_iv + 2 * mp_pokemon->GetBaseSpeed() + (m_speed_ev / 4)) * m_level / 100) + 5);
}

void BattlePokemon::DamageCurrentHP(unsigned int damage)
{
    damage = std::min(m_currentHP, damage);

    m_currentHP -= damage;
}

void BattlePokemon::HealCurrentHP(unsigned int amount)
{
    unsigned int max = GetMaxHP() - m_currentHP;

    amount = std::min(max, amount);

    m_currentHP += amount;
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

size_t BattlePokemon::GetAttackStage() const
{
    return m_attackstage;
}

size_t BattlePokemon::GetDefenseStage() const
{
    return m_defensestage;
}

size_t BattlePokemon::GetSpecialAttackStage() const
{
    return m_specialattackstage;
}

size_t BattlePokemon::GetSpecialDefenseStage() const
{
    return m_specialdefensestage;
}

size_t BattlePokemon::GetSpeedStage() const
{
    return m_speedstage;
}

size_t BattlePokemon::GetEvasionStage() const
{
    return m_evasionstage;
}

size_t BattlePokemon::GetAccuracyStage() const
{
    return m_accuracystage;
}

size_t BattlePokemon::GetCriticalHitStage() const
{
    return m_criticalhitstage;
}

void BattlePokemon::SetAttackStage(size_t num)
{
    m_attackstage = num;
}

void BattlePokemon::SetDefenseStage(size_t num)
{
    m_defensestage = num;
}

void BattlePokemon::SetSpecialAttackStage(size_t num)
{
    m_specialattackstage = num;
}

void BattlePokemon::SetSpecialDefenseStage(size_t num)
{
    m_specialdefensestage = num;
}

void BattlePokemon::SetSpeedStage(size_t num)
{
    m_speedstage = num;
}

void BattlePokemon::SetEvasionStage(size_t num)
{
    m_evasionstage = num;
}

void BattlePokemon::SetAccuracyStage(size_t num)
{
    m_accuracystage = num;
}

void BattlePokemon::SetCriticalHitStage(size_t num)
{
    m_criticalhitstage = num;
}

unsigned int BattlePokemon::GetMoveCount() const
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

void BattlePokemon::SetConfusedTurnCount(unsigned int count)
{
    m_confusedTurnCount = count;
}

unsigned int BattlePokemon::GetConfusedTurnCount() const
{
    return m_confusedTurnCount;
}

void BattlePokemon::IncrementConfusedCounter()
{
    ++m_confusedCounter;
}

unsigned int BattlePokemon::GetConfusedCounter() const
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

void BattlePokemon::SetBoundTurnCount(unsigned int number)
{
    m_boundTurnCount = number;
}

unsigned int BattlePokemon::GetBoundTurnCount() const
{
    return m_boundTurnCount;
}

void BattlePokemon::IncrementBoundCounter()
{
    m_boundCounter++;
}

unsigned int BattlePokemon::GetBoundCounter() const
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

void BattlePokemon::SetSleepTurnCount(unsigned int number)
{
    m_sleepTurnCount = number;
}

unsigned int BattlePokemon::GetSleepTurnCount() const
{
    return m_sleepTurnCount;
}

void BattlePokemon::IncrementSleepCounter()
{
    m_sleepCounter++;
}

unsigned int BattlePokemon::GetSleepCounter() const
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

void BattlePokemon::SetThrashTurnCount(unsigned int number)
{
    m_thrashTurnCount = number;
}

unsigned int BattlePokemon::GetThrashTurnCount() const
{
    return m_thrashTurnCount;
}

void BattlePokemon::IncrementThrashCounter()
{
    m_thrashCounter++;
}

unsigned int BattlePokemon::GetThrashCounter() const
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

unsigned int BattlePokemon::GetDisabledCounter() const
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

pokemonMove* BattlePokemon::GetDisabledMove() const
{
    return disabledMove;
}

bool BattlePokemon::HasPendingPostTurnEffect() const
{
    return IsSeeded() ||
        currentStatus == Status::Burned ||
        currentStatus == Status::Poisoned ||
        currentStatus == Status::Badly_Poisoned ||
        IsBound() ||
        (GetDisabledCounter() == 4);
}

unsigned int BattlePokemon::GetBadlyPoisonCounter() const
{
    return m_badlyPoisonCounter;
}

void BattlePokemon::ResetBadlyPoisonCounter()
{
    m_badlyPoisonCounter = 1;
}

void BattlePokemon::IncrementBadlyPoisonCounter()
{
    if (m_badlyPoisonCounter < 15)
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

void BattlePokemon::SetBideTurnCount(unsigned int turnCount)
{
    m_bideTurnCount = turnCount;
}

unsigned int BattlePokemon::GetBideTurnCount() const
{
    return m_bideTurnCount;
}

void BattlePokemon::IncrementBideCounter()
{
    m_bideCounter++;
}

unsigned int BattlePokemon::GetBideCounter() const
{
    return m_bideCounter;
}

void BattlePokemon::ResetBideCounter()
{
    m_bideCounter = 0;
}

unsigned int BattlePokemon::GetBideDamage() const
{
    return m_bideDamageTaken;
}

void BattlePokemon::AddBideDamage(unsigned int bideDamage)
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
        m_array_moves[i].SetMovePointer(pokemon->GetMove(i + 1).GetMovePointer());
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

void BattlePokemon::SetConversion(pokemonMove* move)
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

void BattlePokemon::SetSubstituteHP(unsigned int hp)
{
    m_substituteHealth = hp;
}

unsigned int BattlePokemon::GetSubstituteHP() const
{
    return m_substituteHealth;
}

void BattlePokemon::DamageSubstitute(unsigned int damage)
{
    damage = std::min(m_substituteHealth, damage);

    m_substituteHealth -= damage;
}

void BattlePokemon::SetMetronome(const Move& move)
{
    metronomeMove.SetMovePointer(&move);
    metronomeMove.m_currentPP = 1;
    metronomeMove.m_maxPP = 1;
}

void BattlePokemon::SetMirrorMove(const Move& move)
{
    mirrorMove.SetMovePointer(&move);
    mirrorMove.m_currentPP = 1;
    mirrorMove.m_maxPP = 1;
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

    SetAttackStage(6);
    SetDefenseStage(6);
    SetSpecialAttackStage(6);
    SetSpecialDefenseStage(6);
    SetSpeedStage(6);
    SetAccuracyStage(6);
    SetEvasionStage(6);
    SetCriticalHitStage(0);

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
            if (GetMove(i).b_isMimicked)
            {
                GetMove(i).SetMovePointer(Database::GetInstance().GetPointerToMovedexNumber(101));
                GetMove(i).m_currentPP = GetMimicPP();
                GetMove(i).m_maxPP = GetMove(i).GetPP();

                SetUsedMimic(false);
                GetMove(i).b_isMimicked = false;
                break;
            }
        }
    }

    if (MoveIsDisabled())
    {
        for (size_t i = 1; i < 5; ++i)
        {
            if (GetMove(i).b_isDisabled)
            {
                GetMove(i).b_isDisabled = false;
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
    m_speed_ev = 0;

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

    m_attackstage = 6;
    m_defensestage = 6;
    m_specialattackstage = 6;
    m_specialdefensestage = 6;
    m_speedstage = 6;
    m_evasionstage = 6;
    m_accuracystage = 6;
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
        if (i.HasMove())
        {
            i.m_currentPP = i.m_maxPP;
            i.b_isDisabled = false;
        }
    }

    if (this->HasUsedMimic())
    {
        for (size_t i = 1; i < 5; ++i)
        {
            if (this->GetMove(i).b_isMimicked)
            {
                this->GetMove(i).SetMovePointer(Database::GetInstance().GetPointerToMovedexNumber(101));
                this->GetMove(i).m_currentPP = this->GetMimicPP();
                this->GetMove(i).m_maxPP = this->GetMove(i).GetPP();

                this->SetUsedMimic(false);
                this->GetMove(i).b_isMimicked = false;
                break;
            }
        }
    }
}