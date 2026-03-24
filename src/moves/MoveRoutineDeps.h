#pragma once

struct BattleContext;
class BattleCalculations;
class StatusEffectProcessor;
class TurnProcessor;
class IMoveResultsUI;
class IBattleMenuUI;
class IStatusEffectUI;
class RandomEngine;
class SwitchExecutor;

struct MoveRoutineDeps {
	BattleContext& context;
	BattleCalculations& calculations;
	StatusEffectProcessor& statusProcessor;
	IMoveResultsUI& resultsUI;
	RandomEngine& rng;
	SwitchExecutor& switchExecutor;

	MoveRoutineDeps
	(BattleContext& ctx,
		BattleCalculations& calc,
		StatusEffectProcessor& statusProc,
		IMoveResultsUI& results,
		RandomEngine& randomEngine,
		SwitchExecutor& switchExec)

		: context(ctx)
		, calculations(calc)
		, statusProcessor(statusProc)
		, resultsUI(results)
		, rng(randomEngine)
		, switchExecutor(switchExec)
	{
	}
};