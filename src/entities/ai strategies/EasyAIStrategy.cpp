#include "../../battle/RandomEngine.h"
#include "../BattlePokemon.h"
#include "../Player.h"
#include "EasyAIStrategy.h"

#include <deque>

EasyAIStrategy::EasyAIStrategy(RandomEngine& rng)
    : m_rng(rng) {}

BattlePokemon::pokemonMove* EasyAIStrategy::ChooseMove(Player& self, BattlePokemon& selfMon)
{
    std::deque<BattlePokemon::pokemonMove*> viableMoves;

    for (size_t i = 1; i <= 4; ++i)
    {
        auto* move = selfMon.GetMove(i);
        if (move->mp_move && !move->b_isDisabled && move->m_currentPP > 0)
        {
            viableMoves.push_back(move);
        }
    }

    if (viableMoves.empty())
        return selfMon.Struggle(); // fallback

    std::uniform_int_distribution<int> dist(0, static_cast<int>(viableMoves.size()) - 1);
    int choice = dist(m_rng.GetGenerator());

    return viableMoves[choice];
}

void EasyAIStrategy::ChooseSwitch(Player& self, BattlePokemon& selfMon)
{
    std::deque<BattlePokemon*> viablePokemon;

    for (size_t i = 1; i <= 6; ++i)
    {
        if (!self.GetBelt(i)->IsFainted() && self.GetBelt(i) != &selfMon)
            viablePokemon.push_back(self.GetBelt(i));
    }

    if (!viablePokemon.empty())
    {
        self.SetIsSwitching(true);
        self.SetPokemonToSwitchTo(viablePokemon[0]);
    }
    else
    {
        //std::cout << "Well this isn't supposed to happen. (EasyAIStrategy.cpp error line 43)\n";
    }
    
}

bool EasyAIStrategy::WantsToSwitch(Player& self, BattlePokemon& selfMon)
{
    return false;
}
