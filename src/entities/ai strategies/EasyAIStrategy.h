#pragma once

#include "IAIStrategy.h"

class RandomEngine;

class EasyAIStrategy : public IAIStrategy
{
public:
    explicit EasyAIStrategy(RandomEngine& rng);

    BattlePokemon::pokemonMove* ChooseMove(AIPlayer& self, BattlePokemon& selfMon) override;
    void ChooseSwitch(Player& self, BattlePokemon& selfMon) override;
    bool WantsToSwitch(Player& self, BattlePokemon& selfMon) override;

private:
    RandomEngine& m_rng;
};