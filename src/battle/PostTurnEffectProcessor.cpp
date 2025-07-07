#include "PostTurnEffectProcessor.h"
#include "WinChecker.h"

#include "BattleContext.h"
#include "BattleStatusManager.h"
#include "RandomEngine.h"
#include "../entities/Player.h"
#include "../entities/BattlePokemon.h"
#include <cmath>
#include <iostream>


PostTurnEffectProcessor::PostTurnEffectProcessor(BattleContext& context, BattleStatusManager& statusManager, WinChecker& winChecker, RandomEngine& rng)
    : m_context(context)
    , m_statusManager(statusManager)
    , m_winChecker(winChecker)
    , m_rng(rng)
{}

void PostTurnEffectProcessor::ProcessAllPostTurnEffects(bool& winCondition)
{
    CheckSeededStatuses();

    winCondition = m_winChecker.CheckWinCondition(m_context.defendingPlayer, m_context.attackingPlayer);
    if (winCondition) { return; }

    winCondition = m_winChecker.CheckWinCondition(m_context.attackingPlayer, m_context.defendingPlayer);
    if (winCondition) { return; }

    CheckDamagingStatuses();

    winCondition = m_winChecker.CheckWinCondition(m_context.defendingPlayer, m_context.attackingPlayer);
    if (winCondition) { return; }

    winCondition = m_winChecker.CheckWinCondition(m_context.attackingPlayer, m_context.defendingPlayer);
    if (winCondition) { return; }
    
    CheckBoundStatuses();

    winCondition = m_winChecker.CheckWinOrSwitch(m_context.defendingPlayer, m_context.attackingPlayer, m_context.attackingPokemon);
    if (winCondition) { return; }

    winCondition = m_winChecker.CheckWinOrSwitch(m_context.attackingPlayer, m_context.defendingPlayer, m_context.defendingPokemon);
    if (winCondition) { return; }

    CheckDisabledStatus();

    CheckFieldEffects();
}

void PostTurnEffectProcessor::CheckSeededStatuses()
{
    if (!m_context.attackingPokemon->IsSeeded() && !m_context.defendingPokemon->IsSeeded())
    {
        return;
    }
        
    double leechedHealth{ 0 };

    if (m_context.attackingPokemon->IsSeeded() && (!m_context.attackingPokemon->IsFainted() || m_context.attackingPokemon->GetCurrentHP() > 0) && !m_context.defendingPokemon->IsFainted())
    {
        leechedHealth = std::floor(m_context.attackingPokemon->GetMaxHP() / 8.0);

        m_context.attackingPokemon->DamageCurrentHP(static_cast<int>(leechedHealth));
        m_context.defendingPokemon->HealCurrentHP(static_cast<int>(leechedHealth));

        std::cout << m_context.attackingPlayer->GetPlayerNameView() << "'s " << m_context.attackingPokemon->GetNameView() << "'s health is sapped by Leech Seed.\n";

        m_statusManager.CheckFaintCondition(m_context.attackingPlayer, m_context.defendingPlayer, m_context.attackingPokemon, m_context.defendingPokemon);
    }

    if (!m_context.defendingPokemon->IsSeeded() || m_context.defendingPokemon->IsFainted() || m_context.defendingPokemon->GetCurrentHP() <= 0)
    {
        return;
    }

    leechedHealth = std::floor(m_context.defendingPokemon->GetMaxHP() / 8.0);

    m_context.defendingPokemon->DamageCurrentHP(static_cast<int>(leechedHealth));
    m_context.attackingPokemon->HealCurrentHP(static_cast<int>(leechedHealth));

    std::cout << m_context.defendingPlayer->GetPlayerNameView() << "'s " << m_context.defendingPokemon->GetNameView() << "'s health is sapped by Leech Seed.\n";

    m_statusManager.CheckFaintCondition(m_context.defendingPlayer, m_context.attackingPlayer, m_context.defendingPokemon, m_context.attackingPokemon);
}

void PostTurnEffectProcessor::CheckDamagingStatuses()
{
    if (!m_context.attackingPokemon->IsFainted() || m_context.attackingPokemon->GetCurrentHP() > 0)
    {
        switch (m_context.attackingPokemon->currentStatus)
        {
        case Status::Burned:
            BurnedStatus(m_context.attackingPlayer, m_context.attackingPokemon);
            break;

        case Status::Poisoned:
            PoisonedStatus(m_context.attackingPlayer, m_context.attackingPokemon);
            break;

        case Status::Badly_Poisoned:
            BadlyPoisonedStatus(m_context.attackingPlayer, m_context.attackingPokemon);
            break;

        default:
            break;
        }
    }

    m_statusManager.CheckFaintCondition(m_context.attackingPlayer, m_context.defendingPlayer, m_context.attackingPokemon, m_context.defendingPokemon);

    if (!m_context.defendingPokemon->IsFainted() || m_context.defendingPokemon->GetCurrentHP() > 0)
    {
        switch (m_context.defendingPokemon->currentStatus)
        {
        case Status::Burned:
            BurnedStatus(m_context.defendingPlayer, m_context.defendingPokemon);
            break;

        case Status::Poisoned:
            PoisonedStatus(m_context.defendingPlayer, m_context.defendingPokemon);
            break;

        case Status::Badly_Poisoned:
            BadlyPoisonedStatus(m_context.defendingPlayer, m_context.defendingPokemon);
            break;

        default:
            return;
        }
    }
    m_statusManager.CheckFaintCondition(m_context.defendingPlayer, m_context.attackingPlayer, m_context.defendingPokemon, m_context.attackingPokemon);
}

void PostTurnEffectProcessor::BurnedStatus(Player* player, BattlePokemon* pokemon)
{
    double burnDamage = std::floor(pokemon->GetMaxHP() / 16.0);
    pokemon->DamageCurrentHP(static_cast<int>(burnDamage));

    std::cout << player->GetPlayerNameView() << "'s " << pokemon->GetNameView() << " was damaged by burn.\n";
}

void PostTurnEffectProcessor::PoisonedStatus(Player* player, BattlePokemon* pokemon)
{
    double poisonDamage = std::floor(pokemon->GetMaxHP() / 8.0);
    pokemon->DamageCurrentHP(static_cast<int>(poisonDamage));

    std::cout << player->GetPlayerNameView() << "'s " << pokemon->GetNameView() << " was damaged by poison.\n";
}

void PostTurnEffectProcessor::BadlyPoisonedStatus(Player* player, BattlePokemon* pokemon)
{
    double poisonDamage = std::floor(pokemon->GetMaxHP() / 16.0) * pokemon->GetBadlyPoisonCounter();
    pokemon->DamageCurrentHP(static_cast<int>(poisonDamage));

    std::cout << player->GetPlayerNameView() << "'s " << pokemon->GetNameView() << " was damaged by poison.\n";

    pokemon->IncrementBadlyPoisonCounter();
}

void PostTurnEffectProcessor::CheckBoundStatuses()
{
    if (!m_context.attackingPokemon->IsBound() && !m_context.defendingPokemon->IsBound())
    {
        return;
    }

    double boundDamage{};

    if (m_context.attackingPokemon->IsBound() && (!m_context.attackingPokemon->IsFainted() || m_context.attackingPokemon->GetCurrentHP() > 0))
    {
        if (m_context.attackingPokemon->GetBoundCounter() >= m_context.attackingPokemon->GetBoundTurnCount())
        {
            m_context.attackingPokemon->SetBound(false);
            m_context.attackingPlayer->SetCanSwitch(true);
            m_context.attackingPokemon->ResetBoundCounter();
            m_context.attackingPokemon->SetBoundTurnCount(0);

            std::cout << m_context.attackingPlayer->GetPlayerNameView() << "'s "
                << m_context.attackingPokemon->GetNameView() << " was freed from "
                << m_context.attackingPokemon->GetBoundMoveName() << "!\n";
        }
        else
        {
            m_context.attackingPokemon->IncrementBoundCounter();
            boundDamage = std::floor(m_context.attackingPokemon->GetMaxHP() / 8.0);

            m_context.attackingPokemon->DamageCurrentHP(static_cast<int>(boundDamage));

            std::cout << m_context.attackingPlayer->GetPlayerNameView() << "'s "
                << m_context.attackingPokemon->GetNameView() << " was hurt by "
                << m_context.attackingPokemon->GetBoundMoveName() << "!\n";

            m_statusManager.CheckFaintCondition(m_context.attackingPlayer, m_context.defendingPlayer,
                m_context.attackingPokemon, m_context.defendingPokemon);
        }
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

        std::cout << m_context.defendingPlayer->GetPlayerNameView() << "'s "
            << m_context.defendingPokemon->GetNameView() << " was freed from "
            << m_context.defendingPokemon->GetBoundMoveName() << "!\n";
    }
    else
    {
        m_context.defendingPokemon->IncrementBoundCounter();
        boundDamage = std::floor(m_context.defendingPokemon->GetMaxHP() / 8.0);

        m_context.defendingPokemon->DamageCurrentHP(static_cast<int>(boundDamage));

        std::cout << m_context.defendingPlayer->GetPlayerNameView() << "'s "
            << m_context.defendingPokemon->GetNameView() << " was hurt by "
            << m_context.defendingPokemon->GetBoundMoveName() << "!\n";

        m_statusManager.CheckFaintCondition(m_context.defendingPlayer, m_context.attackingPlayer,
            m_context.defendingPokemon, m_context.attackingPokemon);
    }
}

void PostTurnEffectProcessor::CheckDisabledStatus()
{
    if (!m_context.attackingPokemon->MoveIsDisabled() && !m_context.defendingPokemon->MoveIsDisabled())
    {
        return;
    };

    if (m_context.attackingPokemon->MoveIsDisabled())
    {
        m_context.attackingPokemon->IncrementDisabledCounter();

        if (m_context.attackingPokemon->GetDisabledCounter() == 4)
        {
            std::cout << m_context.attackingPlayer->GetPlayerNameView() << "'s "
                << m_context.attackingPokemon->GetNameView() << "'s "
                << m_context.attackingPokemon->GetDisabledMove()->mp_move->GetName()
                << " is no longer disabled!\n";
            m_context.attackingPokemon->SetDisabledStatus(false);
        }
    }

    if (m_context.defendingPokemon->MoveIsDisabled())
    {
        m_context.defendingPokemon->IncrementDisabledCounter();

        if (m_context.defendingPokemon->GetDisabledCounter() == 4)
        {
            std::cout << m_context.defendingPlayer->GetPlayerNameView() << "'s "
                << m_context.defendingPokemon->GetNameView() << "'s "
                << m_context.defendingPokemon->GetDisabledMove()->mp_move->GetName()
                << " is no longer disabled!\n";
            m_context.defendingPokemon->SetDisabledStatus(false);
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

    // Reflect
    if (m_context.defendingPlayer->HasReflect())
    {
        if (m_context.defendingPlayer->GetReflectCounter() >= reflectTurnCount)
        {
            std::cout << m_context.defendingPlayer->GetPlayerNameView() << "'s team's Reflect wore off!\n";
            m_context.defendingPlayer->SetReflect(false);
            m_context.defendingPlayer->ResetReflectCounter();
        }
        else
        {
            m_context.defendingPlayer->IncrementReflectCounter();
        }
    }

    if (m_context.attackingPlayer->HasReflect())
    {
        if (m_context.attackingPlayer->GetReflectCounter() >= reflectTurnCount)
        {
            std::cout << m_context.attackingPlayer->GetPlayerNameView() << "'s team's Reflect wore off!\n";
            m_context.attackingPlayer->SetReflect(false);
            m_context.attackingPlayer->ResetReflectCounter();
        }
        else
        {
            m_context.attackingPlayer->IncrementReflectCounter();
        }
    }

    // Light Screen
    if (m_context.attackingPlayer->HasLightScreen())
    {
        if (m_context.attackingPlayer->GetLightScreenCounter() >= lightscreenTurnCount)
        {
            std::cout << m_context.attackingPlayer->GetPlayerNameView() << "'s team's Light Screen wore off!\n";
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
            std::cout << m_context.defendingPlayer->GetPlayerNameView() << "'s team's Light Screen wore off!\n";
            m_context.defendingPlayer->SetLightScreen(false);
            m_context.defendingPlayer->ResetLightScreenCounter();
        }
        else
        {
            m_context.defendingPlayer->IncrementLightScreenCounter();
        }
    }

    // Mist
    if (m_context.attackingPlayer->HasMist())
    {
        if (m_context.attackingPlayer->GetMistCounter() >= mistTurnCount)
        {
            std::cout << m_context.attackingPlayer->GetPlayerNameView() << "'s team is no longer protected by mist!\n";
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
            std::cout << m_context.defendingPlayer->GetPlayerNameView() << "'s team is no longer protected by mist!\n";
            m_context.defendingPlayer->SetMist(false);
            m_context.defendingPlayer->ResetMistCounter();
        }
        else
        {
            m_context.defendingPlayer->IncrementMistCounter();
        }
    }
}