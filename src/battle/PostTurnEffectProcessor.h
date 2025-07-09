#pragma once

struct BattleContext;
class RandomEngine;
class IStatusEffectUI;
class BattleStatusManager;
class WinChecker;
class Player;
class BattlePokemon;

class PostTurnEffectProcessor
{
public:
    explicit PostTurnEffectProcessor(BattleContext&, RandomEngine&, IStatusEffectUI&, BattleStatusManager&, WinChecker&);

    void ProcessAllPostTurnEffects(bool&);

private:
    void CheckSeededStatuses();
    void CheckDamagingStatuses();
    void BurnedStatus(Player* player, BattlePokemon* pokemon);
    void PoisonedStatus(Player* player, BattlePokemon* pokemon);
    void BadlyPoisonedStatus(Player* player, BattlePokemon* pokemon);
    void CheckBoundStatuses();
    void CheckDisabledStatus();
    void CheckFieldEffects();

    BattleContext& m_context;
    RandomEngine& m_rng;
    IStatusEffectUI& m_statusEffectUI;
    BattleStatusManager& m_statusManager;
    WinChecker& m_winChecker;

};