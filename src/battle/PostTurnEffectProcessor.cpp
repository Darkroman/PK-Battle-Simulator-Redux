#include <algorithm>

#include "PostTurnEffectProcessor.h"

#include "BattleContext.h"
#include "RandomEngine.h"
#include "BattleCalculations.h"
#include "../ui/interfaces/IStatusEffectUI.h"
#include "StatusEffectProcessor.h"
#include "WinChecker.h"

PostTurnEffectProcessor::PostTurnEffectProcessor(BattleContext& context, BattleCalculations& calculations, IStatusEffectUI& statusEffectUI, StatusEffectProcessor& statusProcessor, WinChecker& winChecker)
    : m_context(context)
    , m_calculations(calculations)
    , m_statusEffectUI(statusEffectUI)
    , m_statusProcessor(statusProcessor)
    , m_winChecker(winChecker)
{}

void PostTurnEffectProcessor::DeterminePostFaintSwitchOrder()
{
    int playerOneSpeed = m_calculations.CalculatePokemonSpeed(*m_context.playerOneCurrentPokemon);
    int playerTwoSpeed = m_calculations.CalculatePokemonSpeed(*m_context.playerTwoCurrentPokemon);

    if (playerOneSpeed > playerTwoSpeed)
    {
        m_context.attackingPlayer = m_context.playerOne;
        m_context.defendingPlayer = m_context.playerTwo;
        m_context.attackingPokemon = m_context.playerOneCurrentPokemon;
        m_context.defendingPokemon = m_context.playerTwoCurrentPokemon;
    }
    else if (playerTwoSpeed > playerOneSpeed)
    {
        m_context.attackingPlayer = m_context.playerTwo;
        m_context.defendingPlayer = m_context.playerOne;
        m_context.attackingPokemon = m_context.playerTwoCurrentPokemon;
        m_context.defendingPokemon = m_context.playerOneCurrentPokemon;
    }
    else
    {
        m_calculations.RandomizePostTurnOrder();
    }
}

void PostTurnEffectProcessor::ProcessAllPostTurnEffects(bool& winCondition)
{
    if ((m_context.attackingPokemon->HasPendingPostTurnEffect() ||
        m_context.defendingPokemon->HasPendingPostTurnEffect() ||
        m_context.attackingPlayer->HasPendingPostTurnEffect() ||
        m_context.defendingPlayer->HasPendingPostTurnEffect()))
    {
        m_statusEffectUI.NewLine();
    }

    m_calculations.RandomizePostTurnOrder();

    CheckSeededStatuses();

    winCondition = m_winChecker.CheckWinCondition(*m_context.defendingPlayer, *m_context.attackingPlayer);
    if (winCondition) { return; }

    winCondition = m_winChecker.CheckWinCondition(*m_context.attackingPlayer, *m_context.defendingPlayer);
    if (winCondition) { return; }

    m_calculations.RandomizePostTurnOrder();

    CheckDamagingStatuses();

    winCondition = m_winChecker.CheckWinCondition(*m_context.defendingPlayer, *m_context.attackingPlayer);
    if (winCondition) { return; }

    winCondition = m_winChecker.CheckWinCondition(*m_context.attackingPlayer, *m_context.defendingPlayer);
    if (winCondition) { return; }

    m_calculations.RandomizePostTurnOrder();
    
    CheckBoundStatuses();

    winCondition = m_winChecker.CheckWinCondition(*m_context.defendingPlayer, *m_context.attackingPlayer);
    if (winCondition) { return; }

    winCondition = m_winChecker.CheckWinCondition(*m_context.attackingPlayer, *m_context.defendingPlayer);
    if (winCondition) { return; }

    m_calculations.RandomizePostTurnOrder();

    CheckDisabledStatus();

    CheckFieldEffects();

    DeterminePostFaintSwitchOrder();

    winCondition = m_winChecker.CheckWinOrSwitch(*m_context.defendingPlayer, *m_context.attackingPlayer, *m_context.attackingPokemon);
    if (winCondition) { return; }

    winCondition = m_winChecker.CheckWinOrSwitch(*m_context.attackingPlayer, *m_context.defendingPlayer, *m_context.defendingPokemon);
    if (winCondition) { return; }
}

void PostTurnEffectProcessor::CheckSeededStatuses()
{
    if (!m_context.attackingPokemon->IsSeeded() && !m_context.defendingPokemon->IsSeeded())
    {
        return;
    }

    if (m_context.attackingPokemon->IsSeeded() && !m_context.attackingPokemon->IsFainted() && !m_context.defendingPokemon->IsFainted())
    {
        int damage = std::max(1, m_context.attackingPokemon->GetMaxHP() / 8);

        int before = m_context.attackingPokemon->GetCurrentHP();
        m_context.attackingPokemon->DamageCurrentHP(damage);

        int drained = before - m_context.attackingPokemon->GetCurrentHP();

        m_context.defendingPokemon->HealCurrentHP(drained);

        m_statusEffectUI.DisplayLeechSeedSappedMsg(m_context.attackingPlayer->GetPlayerNameView(), m_context.attackingPokemon->GetNameView());

        m_statusProcessor.CheckFaintCondition(*m_context.attackingPlayer, *m_context.defendingPlayer, *m_context.attackingPokemon, *m_context.defendingPokemon);
    }

    if (m_context.attackingPlayer->GetPokemonCount() == m_context.attackingPlayer->GetFaintedCount())
    {
        return;
    }

    if (!m_context.defendingPokemon->IsSeeded() || m_context.defendingPokemon->IsFainted())
    {
        return;
    }

    int damage = std::max(1, m_context.defendingPokemon->GetMaxHP() / 8);

    int before = m_context.defendingPokemon->GetCurrentHP();
    m_context.defendingPokemon->DamageCurrentHP(damage);

    int drained = before - m_context.defendingPokemon->GetCurrentHP();

    m_context.attackingPokemon->HealCurrentHP(drained);

    m_statusEffectUI.DisplayLeechSeedSappedMsg(m_context.defendingPlayer->GetPlayerNameView(), m_context.defendingPokemon->GetNameView());

    m_statusProcessor.CheckFaintCondition(*m_context.defendingPlayer, *m_context.attackingPlayer, *m_context.defendingPokemon, *m_context.attackingPokemon);
}

void PostTurnEffectProcessor::CheckDamagingStatuses()
{
    if (!m_context.attackingPokemon->IsFainted())
    {
        switch (m_context.attackingPokemon->currentStatus)
        {
        case Status::Burned:
            BurnedStatus(*m_context.attackingPlayer, *m_context.attackingPokemon);
            break;

        case Status::Poisoned:
            PoisonedStatus(*m_context.attackingPlayer, *m_context.attackingPokemon);
            break;

        case Status::Badly_Poisoned:
            BadlyPoisonedStatus(*m_context.attackingPlayer, *m_context.attackingPokemon);
            break;

        default:
            break;
        }
    }

    m_statusProcessor.CheckFaintCondition(*m_context.attackingPlayer, *m_context.defendingPlayer, *m_context.attackingPokemon, *m_context.defendingPokemon);

    if (m_context.attackingPlayer->GetPokemonCount() == m_context.attackingPlayer->GetFaintedCount())
    {
        return;
    }

    if (!m_context.defendingPokemon->IsFainted())
    {
        switch (m_context.defendingPokemon->currentStatus)
        {
        case Status::Burned:
            BurnedStatus(*m_context.defendingPlayer, *m_context.defendingPokemon);
            break;

        case Status::Poisoned:
            PoisonedStatus(*m_context.defendingPlayer, *m_context.defendingPokemon);
            break;

        case Status::Badly_Poisoned:
            BadlyPoisonedStatus(*m_context.defendingPlayer, *m_context.defendingPokemon);
            break;

        default:
            break;
        }
    }
    m_statusProcessor.CheckFaintCondition(*m_context.defendingPlayer, *m_context.attackingPlayer, *m_context.defendingPokemon, *m_context.attackingPokemon);
}

void PostTurnEffectProcessor::BurnedStatus(Player& player, BattlePokemon& pokemon)
{
    int burnDamage{ std::max(1, pokemon.GetMaxHP() / 16) };

    pokemon.DamageCurrentHP(burnDamage);

    m_statusEffectUI.DisplayDamagedByStatusPostTurn("burn", player.GetPlayerNameView(), pokemon.GetNameView());
}

void PostTurnEffectProcessor::PoisonedStatus(Player& player, BattlePokemon& pokemon)
{
    int poisonDamage{ std::max(1, pokemon.GetMaxHP() / 8) };

    pokemon.DamageCurrentHP(poisonDamage);

    m_statusEffectUI.DisplayDamagedByStatusPostTurn("poison", player.GetPlayerNameView(), pokemon.GetNameView());
}

void PostTurnEffectProcessor::BadlyPoisonedStatus(Player& player, BattlePokemon& pokemon)
{
    int poisonDamage{ std::max(1, pokemon.GetMaxHP() / 16 * pokemon.GetBadlyPoisonCounter() + 15) };

    pokemon.DamageCurrentHP(poisonDamage);

    m_statusEffectUI.DisplayDamagedByStatusPostTurn("poison", player.GetPlayerNameView(), pokemon.GetNameView());

    pokemon.IncrementBadlyPoisonCounter();
}

void PostTurnEffectProcessor::CheckBoundStatuses()
{
    if (!m_context.attackingPokemon->IsBound() && !m_context.defendingPokemon->IsBound())
    {
        return;
    }

    if (m_context.attackingPokemon->IsBound() && !m_context.attackingPokemon->IsFainted())
    {
        if (m_context.attackingPokemon->GetBoundCounter() >= m_context.attackingPokemon->GetBoundTurnCount())
        {
            m_context.attackingPokemon->SetBound(false);
            m_context.attackingPlayer->SetCanSwitch(true);
            m_context.attackingPokemon->ResetBoundCounter();
            m_context.attackingPokemon->SetBoundTurnCount(0);

            m_statusEffectUI.DisplayFreedFromBoundMsg(m_context.attackingPlayer->GetPlayerNameView(),
                m_context.attackingPokemon->GetNameView(),
                m_context.attackingPokemon->GetBoundMoveName());
        }
        else
        {
            m_context.attackingPokemon->IncrementBoundCounter();
            int boundDamage{ std::max(1, m_context.attackingPokemon->GetMaxHP() / 8) };

            m_context.attackingPokemon->DamageCurrentHP(boundDamage);

            m_statusEffectUI.DisplayHurtByBoundMsg(m_context.attackingPlayer->GetPlayerNameView(),
                m_context.attackingPokemon->GetNameView(),
                m_context.attackingPokemon->GetBoundMoveName());

            m_statusProcessor.CheckFaintCondition(*m_context.attackingPlayer, *m_context.defendingPlayer,
                *m_context.attackingPokemon, *m_context.defendingPokemon);
        }
    }

    if (m_context.attackingPlayer->GetPokemonCount() == m_context.attackingPlayer->GetFaintedCount())
    {
        return;
    }

    if (!m_context.defendingPokemon->IsBound() || m_context.defendingPokemon->IsFainted() || m_context.defendingPokemon->GetCurrentHP() <= 0)
    {
        return;
    }

    if (m_context.defendingPokemon->GetBoundCounter() >= m_context.defendingPokemon->GetBoundTurnCount())
    {

        m_context.defendingPokemon->SetBound(false);
        m_context.defendingPlayer->SetCanSwitch(true);
        m_context.defendingPokemon->ResetBoundCounter();
        m_context.defendingPokemon->SetBoundTurnCount(0);

        m_statusEffectUI.DisplayFreedFromBoundMsg(m_context.defendingPlayer->GetPlayerNameView(),
            m_context.defendingPokemon->GetNameView(),
            m_context.defendingPokemon->GetBoundMoveName());
    }
    else
    {
        m_context.defendingPokemon->IncrementBoundCounter();
        int boundDamage{ std::max(1, m_context.defendingPokemon->GetMaxHP() / 8) };

        m_context.defendingPokemon->DamageCurrentHP(boundDamage);

        m_statusEffectUI.DisplayHurtByBoundMsg(m_context.defendingPlayer->GetPlayerNameView(),
            m_context.defendingPokemon->GetNameView(),
            m_context.defendingPokemon->GetBoundMoveName());

        m_statusProcessor.CheckFaintCondition(*m_context.defendingPlayer, *m_context.attackingPlayer,
            *m_context.defendingPokemon, *m_context.attackingPokemon);
    }
}

void PostTurnEffectProcessor::CheckDisabledStatus()
{
    if (!m_context.attackingPokemon->MoveIsDisabled() && !m_context.defendingPokemon->MoveIsDisabled())
    {
        return;
    };

    if (m_context.attackingPokemon->MoveIsDisabled() && !m_context.attackingPokemon->IsFainted())
    {
        m_context.attackingPokemon->IncrementDisabledCounter();

        if (m_context.attackingPokemon->GetDisabledCounter() == 4)
        {
            m_statusEffectUI.DisplayMoveNoLongerDisabledMsg(m_context.attackingPlayer->GetPlayerNameView(), m_context.attackingPokemon->GetNameView(), m_context.attackingPokemon->GetDisabledMove()->GetName());
            m_context.attackingPokemon->SetDisabledStatus(false);
            m_context.attackingPokemon->ResetDisabledCounter();
        }
    }

    if (m_context.defendingPokemon->MoveIsDisabled() && !m_context.defendingPokemon->IsFainted())
    {
        m_context.defendingPokemon->IncrementDisabledCounter();

        if (m_context.defendingPokemon->GetDisabledCounter() == 4)
        {
            m_statusEffectUI.DisplayMoveNoLongerDisabledMsg(m_context.defendingPlayer->GetPlayerNameView(), m_context.defendingPokemon->GetNameView(), m_context.defendingPokemon->GetDisabledMove()->GetName());
            m_context.defendingPokemon->SetDisabledStatus(false);
            m_context.defendingPokemon->ResetDisabledCounter();
        }
    }
}

void PostTurnEffectProcessor::CheckFieldEffects()
{
    constexpr int reflectTurnCount = 4;
    constexpr int lightscreenTurnCount = 4;
    constexpr int mistTurnCount = 4;

    if (!m_context.defendingPlayer->HasReflect() && !m_context.attackingPlayer->HasReflect()
        && !m_context.attackingPlayer->HasLightScreen() && !m_context.defendingPlayer->HasLightScreen()
        && !m_context.attackingPlayer->HasMist() && !m_context.defendingPlayer->HasMist())
    {
        return;
    }

    m_calculations.RandomizePostTurnOrder();

    // Reflect
    if (m_context.attackingPlayer->HasReflect())
    {
        if (m_context.attackingPlayer->GetReflectCounter() >= reflectTurnCount)
        {
            m_statusEffectUI.DisplayFieldEffectFadedMsg(m_context.attackingPlayer->GetPlayerNameView(), "reflect");
            m_context.attackingPlayer->SetReflect(false);
            m_context.attackingPlayer->ResetReflectCounter();
        }
        else
        {
            m_context.attackingPlayer->IncrementReflectCounter();
        }
    }

    if (m_context.defendingPlayer->HasReflect())
    {
        if (m_context.defendingPlayer->GetReflectCounter() >= reflectTurnCount)
        {
            m_statusEffectUI.DisplayFieldEffectFadedMsg(m_context.defendingPlayer->GetPlayerNameView(), "reflect");
            m_context.defendingPlayer->SetReflect(false);
            m_context.defendingPlayer->ResetReflectCounter();
        }
        else
        {
            m_context.defendingPlayer->IncrementReflectCounter();
        }
    }

    m_calculations.RandomizePostTurnOrder();

    // Light Screen
    if (m_context.attackingPlayer->HasLightScreen())
    {
        if (m_context.attackingPlayer->GetLightScreenCounter() >= lightscreenTurnCount)
        {
            m_statusEffectUI.DisplayFieldEffectFadedMsg(m_context.attackingPlayer->GetPlayerNameView(), "light screen");
            m_context.attackingPlayer->SetLightScreen(false);
            m_context.attackingPlayer->ResetLightScreenCounter();
        }
        else
        {
            m_context.attackingPlayer->IncrementLightScreenCounter();
        }
    }

    if (m_context.defendingPlayer->HasLightScreen())
    {
        if (m_context.defendingPlayer->GetLightScreenCounter() >= lightscreenTurnCount)
        {
            m_statusEffectUI.DisplayFieldEffectFadedMsg(m_context.defendingPlayer->GetPlayerNameView(), "light screen");
            m_context.defendingPlayer->SetLightScreen(false);
            m_context.defendingPlayer->ResetLightScreenCounter();
        }
        else
        {
            m_context.defendingPlayer->IncrementLightScreenCounter();
        }
    }

    m_calculations.RandomizePostTurnOrder();

    // Mist
    if (m_context.attackingPlayer->HasMist())
    {
        if (m_context.attackingPlayer->GetMistCounter() >= mistTurnCount)
        {
            m_statusEffectUI.DisplayNoLongerProtectedMist(m_context.attackingPlayer->GetPlayerNameView());
            m_context.attackingPlayer->SetMist(false);
            m_context.attackingPlayer->ResetMistCounter();
        }
        else
        {
            m_context.attackingPlayer->IncrementMistCounter();
        }
    }

    if (m_context.defendingPlayer->HasMist())
    {
        if (m_context.defendingPlayer->GetMistCounter() >= mistTurnCount)
        {
            m_statusEffectUI.DisplayNoLongerProtectedMist(m_context.defendingPlayer->GetPlayerNameView());
            m_context.defendingPlayer->SetMist(false);
            m_context.defendingPlayer->ResetMistCounter();
        }
        else
        {
            m_context.defendingPlayer->IncrementMistCounter();
        }
    }
}