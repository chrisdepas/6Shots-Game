#ifndef __CGAMESTATE_H__
#define __CGAMESTATE_H__

class CGame;
class CGameState;

/*	A state in the game, such as main menu
Handled by CGameStateHandler
*/
class CGameState
{
protected:
	CGameState(){ m_bAlreadyInitialised = false; }
	CGameState(CGameState const&);
	void operator=(CGameState const&);
	bool m_bAlreadyInitialised = false;

public:
	void OnInitialised() {
		m_bAlreadyInitialised = true;
	}
	bool IsAlreadyInitialised() {
		return m_bAlreadyInitialised;
	}

	/* Initialisation & Destruction */
	virtual void Init(CGame* pGame) = 0;
	virtual void Cleanup(CGame* pGame) = 0;

	virtual void Draw(CGame* pGame) = 0;
	virtual void Update(CGame* pGame, float fFrameTime) = 0;
	virtual void HandleInput(CGame* pGame) = 0;

	/* Pause/Resume */
	virtual void PauseState() = 0;
	virtual void ResumeState() = 0;

	/* Change game state */
	void ChangeState(CGameState* pState, CGame* pGame);
};


#endif