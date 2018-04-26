#ifndef __C6SMENUSTATE_H__
#define __C6SMENUSTATE_H__

#include "CGameState.h"
#include <TGUI\TGUI.hpp>

/* Main Menu */
class C6SMenuState : public CGameState {
	static C6SMenuState m_MenuState;
	tgui::Gui* m_pGui;

public:
	void C6SMenuState::Init(CGame* pGame);
	void C6SMenuState::Cleanup(CGame* pGame);

	void C6SMenuState::Draw(CGame* pGame);
	void C6SMenuState::Update(CGame* pGame);
	void C6SMenuState::HandleInput(CGame* pGame);

	void C6SMenuState::PauseState();
	void C6SMenuState::ResumeState();

	static C6SMenuState* Instance()
	{
		return &m_MenuState;
	}
};

#endif

