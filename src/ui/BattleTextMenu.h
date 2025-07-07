#pragma once

#include "interfaces/IBattleMenuUI.h"

struct BattleContext;

class BattleTextMenu : public IBattleMenuUI
{
public:
    explicit BattleTextMenu(BattleContext&);

    void ThrowOutFirstPokemon() override;
    void DisplayFightingPokemon() override;
    void PlayerOneMakeSelection() override;
    void PlayerTwoMakeSelection() override;
    bool SwitchPokemonOption(Player* currentPlayer, BattlePokemon* currentPokemon) override;
    bool AnnounceWinner() override;

protected:
    void MakeASelectionLoop(Player* player, BattlePokemon* currentPokemon) override;
    bool Fight(Player* player, BattlePokemon* currentPokemon) override;

private:
    BattleContext& m_context;

    void Forfeit(Player* player);
    bool CheckPPCountForStruggle(BattlePokemon* pokemon);
};
