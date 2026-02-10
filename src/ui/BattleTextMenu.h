#pragma once

#include "interfaces/IBattleMenuUI.h"

class BattleTextMenu : public IBattleMenuUI
{
public:
    void ThrowOutFirstPokemon(const BattleContext&) const override;
    void DisplayFightingPokemon(const BattleContext&) const override;
    BattleAction PlayerOneMakeSelection(const BattleContext&) override;
    BattleAction PlayerTwoMakeSelection(const BattleContext&) override;
    void SwitchOutMsg(const Player&, const BattlePokemon&) const override;
    void PlayerChoosesMsg(const Player&, const BattlePokemon&) const override;
    bool AnnounceWinner(const BattleContext&) override;

    BattleAction MakeASelectionLoop(Player&, BattlePokemon&) override;
    BattleAction Fight(Player&, BattlePokemon&) override;
    BattleAction SwitchPokemonOption(Player&, BattlePokemon&) override;
    BattleAction Forfeit(Player& player) override;

    void NewLine() const override;
    void DisplayTurnNumber(int) const override;

    BattlePokemon::pokemonMove* GetChosenMove() const override;
    BattlePokemon* GetChosenPokemon() const override;

protected:

private:
    //BattleContext& m_context;

    bool CheckPPCountForStruggle(BattlePokemon& pokemon);
};
