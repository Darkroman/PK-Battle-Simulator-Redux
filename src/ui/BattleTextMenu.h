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
    void SwitchOutMsg(Player*, BattlePokemon*) override;
    void PlayerChoosesMsg(Player*, BattlePokemon*) override;
    bool AnnounceWinner() override;

    void NewLine() override;
    void DisplayTurnNumber(int) override;

protected:
    void MakeASelectionLoop(Player* player, BattlePokemon* currentPokemon) override;
    bool Fight(Player* player, BattlePokemon* currentPokemon) override;

private:
    BattleContext& m_context;

    void Forfeit(Player* player);
    bool CheckPPCountForStruggle(BattlePokemon* pokemon);
};
