#pragma once

#include "../../battle/BattleAction.h"
#include "../../entities/BattlePokemon.h"

struct BattleContext;
class Player;

class IBattleMenuUI
{
public:
    virtual ~IBattleMenuUI() = default;

    virtual void ThrowOutFirstPokemon(const BattleContext&) const = 0;
    virtual void DisplayFightingPokemon(const BattleContext&) const = 0;
    virtual BattleAction PlayerOneMakeSelection(const BattleContext&) = 0;
    virtual BattleAction PlayerTwoMakeSelection(const BattleContext&) = 0;
    virtual void SwitchOutMsg(const Player&, const BattlePokemon&) const = 0;
    virtual void PlayerChoosesMsg(const Player&, const BattlePokemon&) const = 0;
    virtual bool AnnounceWinner(const BattleContext&) = 0;

    virtual BattleAction MakeASelectionLoop(Player&, BattlePokemon&) = 0;
    virtual BattleAction Fight(Player&, BattlePokemon&) = 0;
    virtual BattleAction SwitchPokemonOption(Player&, BattlePokemon&) = 0;
    virtual BattleAction Forfeit(Player& player) = 0;

    virtual void NewLine() const = 0;
    virtual void DisplayTurnNumber(int) const = 0;

    virtual BattlePokemon::pokemonMove* GetChosenMove() const = 0;
    virtual BattlePokemon* GetChosenPokemon() const = 0;

protected:

    BattlePokemon::pokemonMove* selectedMove{ nullptr };
    BattlePokemon* selectedPokemon{ nullptr };
};