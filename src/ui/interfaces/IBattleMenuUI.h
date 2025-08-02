#pragma once

class Player;
class BattlePokemon;

class IBattleMenuUI
{
public:
    virtual ~IBattleMenuUI() = default;

    virtual void ThrowOutFirstPokemon() = 0;
    virtual void DisplayFightingPokemon() = 0;
    virtual void PlayerOneMakeSelection() = 0;
    virtual void PlayerTwoMakeSelection() = 0;
    virtual bool SwitchPokemonOption(Player* currentPlayer, BattlePokemon* currentPokemon) = 0;
    virtual void SwitchOutMsg(Player*, BattlePokemon*) = 0;
    virtual void PlayerChoosesMsg(Player*, BattlePokemon*) = 0;
    virtual bool AnnounceWinner() = 0;

    virtual void NewLine() = 0;
    virtual void DisplayTurnNumber(int) = 0;

protected:
    virtual void MakeASelectionLoop(Player*, BattlePokemon*) = 0;
    virtual bool Fight(Player*, BattlePokemon*) = 0;
};