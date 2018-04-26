#ifndef __C6SDEVTESTGM_H__
#define __C6SDEVTESTGM_H__
#include "CGameState.h"
#include <TGUI\TGUI.hpp>

/* Main Menu */
class C6SDevTestGM : public CGameState {
	tgui::Gui* m_pGui;

	const int TEXT_SIZE = 24;
	const int TEXT_SPACING = 12;

	/* Resolutions */
	std::vector<sf::Vector2i> m_vResolutions;

public:
	void C6SDevTestGM::Init(CGame* pGame);
	void C6SDevTestGM::Cleanup(CGame* pGame);

	void C6SDevTestGM::Draw(CGame* pGame);
	void C6SDevTestGM::Update(CGame* pGame);
	void C6SDevTestGM::HandleInput(CGame* pGame);

	void C6SDevTestGM::PauseState();
	void C6SDevTestGM::ResumeState();

	void C6SDevTestGM::LoadGMPressed(CGame* pGame);

	void C6SDevTestGM::ReturnToMenu(CGame* pGame);

	void C6SDevTestGM::Cancel(CGame* pGame);

	static C6SDevTestGM* Instance()
	{
		static C6SDevTestGM state;
		return &state;
	}
};

#endif

