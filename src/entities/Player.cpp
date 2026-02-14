#include "../data/Database.h"

#include "Player.h"
#include "../ui/views/PokemonTextView.h"
//#include "controllers/IPlayerController.h"
#include "controllers/AIController.h"

Player::Player(std::string_view name) :
    m_name(name) {}

Player::~Player() = default;

bool Player::IsAI() const
{
    if (e_type == ControllerType::AI)
    {
        return true;
    }
    else
    {
        return false;
    }
}

IPlayerController& Player::GetController() const
{
    return *uptr_controller;
}

AIController& Player::GetAIController()
{
    return static_cast<AIController&>(*uptr_controller);
}

void Player::SetController(std::unique_ptr<IPlayerController> controller, ControllerType type)
{
    uptr_controller = std::move(controller);
    e_type = type;
}

const std::array<BattlePokemon, 6>& Player::GetBeltArray() const
{
    return belt;
}

std::array<BattlePokemon, 6> Player::CopyBelt()
{
    return belt;
}

void Player::AssignBelt(std::array<BattlePokemon, 6>& newBelt)
{
    belt = newBelt;

    m_PokemonCount = 0;

    for (size_t i = 0; i < 6; ++i)
    {
        if (belt[i].HasPokemon())
        {
            IncrementPokemonCount();
        }
    }
}

BattlePokemon& Player::GetBelt(size_t beltslot)
{
    --beltslot;
    return belt[beltslot];
}

const BattlePokemon& Player::GetBelt(size_t beltslot) const
{
    --beltslot;
    return belt[beltslot];
}

void Player::SwapPokemon(size_t firstPick, size_t secondPick)
{
    --firstPick;
    --secondPick;
    std::swap(belt[firstPick], belt[secondPick]);
}

void Player::ReorderPokemon(size_t slotToMove, size_t targetSlot)
{
    --slotToMove;
    --targetSlot;

    if (slotToMove > targetSlot)
    {
        std::rotate(belt.begin() + targetSlot, belt.begin() + slotToMove, belt.begin() + slotToMove + 1);
    }
    else
    {
        std::rotate(belt.begin() + slotToMove, belt.begin() + slotToMove + 1, belt.begin() + targetSlot + 1);
    }
}

const std::string& Player::GetPlayerName() const
{
    return m_name;
}

std::string_view Player::GetPlayerNameView() const
{
    return m_name;
}

void Player::SetName(std::string_view input)
{
    m_name = input;
}

void Player::IncrementPokemonCount()
{
    ++m_PokemonCount;
}

void Player::DecrementPokemonCount()
{
    --m_PokemonCount;
}

void Player::SetPokemonCount(int count)
{
    m_PokemonCount = count;
}

int Player::GetPokemonCount() const
{
    return m_PokemonCount;
}

int Player::GetFaintedCount() const
{
    return m_faintedPokemon;
}

void Player::IncrementFaintedCount()
{
    ++m_faintedPokemon;
}

void Player::DecrementFaintedCount()
{
    --m_faintedPokemon;
}

bool Player::IsFirst() const
{
    return b_isFirst;
}

void Player::SetFirst(bool first)
{
    b_isFirst = first;
}

bool Player::CanSwitch() const
{
    return b_canSwitch;
}

void Player::SetCanSwitch(bool canSwitch)
{
    b_canSwitch = canSwitch;
}

bool Player::IsSwitching() const
{
    return b_isSwitching;
}

void Player::SetIsSwitching(bool Switching)
{
    b_isSwitching = Switching;
}

bool Player::HasSwitched() const
{
    return b_hasSwitched;
}

void Player::SetHasSwitched(bool hasSwitched)
{
    b_hasSwitched = hasSwitched;
}

void Player::SetPokemonToSwitchTo(BattlePokemon* pokemon)
{
    pokemonToSwitchTo = pokemon;
}

BattlePokemon* Player::GetPokemonToSwitchTo()
{
    return pokemonToSwitchTo;
}

void Player::SetWinCondition(bool won)
{
    b_hasWon = won;
}

bool Player::HasWon() const
{
    return b_hasWon;
}

void Player::SetForfeit(bool forfeit)
{
    b_hasForfeited = forfeit;
}

bool Player::HasForfeited() const
{
    return b_hasForfeited;
}

void Player::SetMist(bool mist)
{
    b_hasMist = mist;
}

bool Player::HasMist() const
{
    return b_hasMist;
}

void Player::IncrementMistCounter()
{
    m_mistCounter++;
}

int Player::GetMistCounter() const
{
    return m_mistCounter;
}

void Player::ResetMistCounter()
{
    m_mistCounter = 0;
}

void Player::SetLightScreen(bool lightScreen)
{
    b_hasLightScreen = lightScreen;
}

bool Player::HasLightScreen() const
{
    return b_hasLightScreen;
}

void Player::IncrementLightScreenCounter()
{
    m_lightscreenCounter++;
}

int Player::GetLightScreenCounter() const
{
    return m_lightscreenCounter;
}

void Player::ResetLightScreenCounter()
{
    m_lightscreenCounter = 0;
}

void Player::SetReflect(bool reflect)
{
    b_hasReflect = reflect;
}

bool Player::HasReflect() const
{
    return b_hasReflect;
}

void Player::IncrementReflectCounter()
{
    m_reflectCounter++;
}

int Player::GetReflectCounter() const
{
    return m_reflectCounter;
}

void Player::ResetReflectCounter()
{
    m_reflectCounter = 0;
}

void Player::ResetValues()
{
    b_canSwitch = true;
    b_isSwitching = false;
    b_hasWon = false;
    b_hasForfeited = false;
    b_hasMist = false;
    b_hasLightScreen = false;
    b_hasReflect = false;

    m_faintedPokemon = 0;

    pokemonToSwitchTo = nullptr;
}
