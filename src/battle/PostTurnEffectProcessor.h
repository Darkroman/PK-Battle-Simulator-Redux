#pragma once

struct BattleContext;
class BattleCalculations;
class IStatusEffectUI;
class StatusEffectProcessor;
class WinChecker;
class Player;
class BattlePokemon;

class PostTurnEffectProcessor
{
public:
    explicit PostTurnEffectProcessor(BattleContext&, BattleCalculations&, IStatusEffectUI&, StatusEffectProcessor&, WinChecker&);

    void DeterminePostTurnDamageOrder();
    void ProcessAllPostTurnEffects(bool&);

private:
    void CheckSeededStatuses();
    void CheckDamagingStatuses();
    void BurnedStatus(Player& player, BattlePokemon& pokemon);
    void PoisonedStatus(Player& player, BattlePokemon& pokemon);
    void BadlyPoisonedStatus(Player& player, BattlePokemon& pokemon);
    void CheckBoundStatuses();
    void CheckDisabledStatus();
    void CheckFieldEffects();

    BattleContext& m_context;
    BattleCalculations& m_calculations;
    IStatusEffectUI& m_statusEffectUI;
    StatusEffectProcessor& m_statusProcessor;
    WinChecker& m_winChecker;
};