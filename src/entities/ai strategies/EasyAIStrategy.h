#pragma once

#include "IAIStrategy.h"

class RandomEngine;

class EasyAIStrategy : public IAIStrategy
{
public:
    explicit EasyAIStrategy(RandomEngine& rng);

    BattlePokemon::pokemonMove& ChooseMove(Player& self, BattlePokemon& selfMon) override;
    BattlePokemon* ChooseSwitch(Player& self, BattlePokemon& selfMon) override;
    bool WantsToSwitch(Player& self, BattlePokemon& selfMon) override;

private:
    RandomEngine& m_rng;
};