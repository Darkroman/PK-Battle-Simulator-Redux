#pragma once

struct BattleContext;
class IBattleMenuUI;
class WinChecker;
class TurnProcessor;
class PostTurnEffectProcessor;

class TurnManager
{
public:
	TurnManager(BattleContext& context, IBattleMenuUI& battleMenuUI, WinChecker& winChecker, TurnProcessor& turnProcessor, PostTurnEffectProcessor& postTurnEffectProcessor);

	bool RunBattleLoop();
	void ResetValues();

private:
	BattleContext& m_context;
	IBattleMenuUI& m_battleMenuUI;
	WinChecker& m_winChecker;
	TurnProcessor& m_turnProcessor;
	PostTurnEffectProcessor& m_postTurnProcessor;
};