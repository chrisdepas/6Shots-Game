#ifndef __CGAMESTATEHANDLER_H__
#define __CGAMESTATEHANDLER_H__

class CGameState;
class CGame;

class CGameStateHandler
{
	/* Current game state */
	CGameState* m_State;
public:
	CGameStateHandler();

	void Cleanup(CGame* pGame);

	/* Returns the current game state*/
	CGameState* GetCurrentState();

	/* Changes to specified state */
	void ChangeState(CGameState* State, CGame* pGame);

	bool IsCurrentState(CGameState* pState);

	void Draw(CGame* pGame);
	void Update(CGame* pGame, float fFrameTime);
	void HandleInput(CGame* pGame);
};

#endif