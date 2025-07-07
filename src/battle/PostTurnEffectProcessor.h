#pragma once

struct BattleContext;
class BattleStatusManager;
class RandomEngine;
class WinChecker;
class Player;
class BattlePokemon;

class PostTurnEffectProcessor
{
public:
    explicit PostTurnEffectProcessor(BattleContext& context, BattleStatusManager& statusManager, WinChecker& winChecker, RandomEngine& rng);

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
    BattleStatusManager& m_statusManager;
    WinChecker& m_winChecker;
    RandomEngine& m_rng;

};