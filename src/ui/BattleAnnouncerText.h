#pragma once

#include "interfaces/IBattleAnnouncerUI.h"

class BattleAnnouncerText : public IBattleAnnouncerUI
{
public:
    void ThrowOutFirstPokemon(const BattleContext&) const override;
    void DisplayFightingPokemon(const BattleContext&) const override;
    bool AnnounceWinner(const BattleContext&) override;

    void NewLine() const override;
    void DisplayTurnNumber(int) const override;

protected:

private:

};
