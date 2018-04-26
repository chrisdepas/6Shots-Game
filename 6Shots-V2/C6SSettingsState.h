#ifndef __C6SSETTINGSSTATE_H__
#define __C6SSETTINGSSTATE_H__

#include "CGameState.h"
#include <TGUI\TGUI.hpp>

/* Main Menu */
class C6SSettingsState : public CGameState {
	tgui::Gui* m_pGui;
	CSettings m_TempSettings;

	const int SETTINGS_TEXT_SIZE = 24;
	const int SETTINGS_SPACING = 12;

	/* Resolutions */
	std::vector<sf::Vector2i> m_vResolutions;

public:
	void C6SSettingsState::Init(CGame* pGame);
	void C6SSettingsState::Cleanup(CGame* pGame);

	void C6SSettingsState::Draw(CGame* pGame);
	void C6SSettingsState::Update(CGame* pGame);
	void C6SSettingsState::HandleInput(CGame* pGame);

	void C6SSettingsState::PauseState();
	void C6SSettingsState::ResumeState();

	void C6SSettingsState::FullscreenToggled(CGame* pGame);

	void C6SSettingsState::ReturnToMenu(CGame* pGame);

	void C6SSettingsState::Cancel(CGame* pGame);

	/* GUI Callbacks */
	void C6SSettingsState::MusicVolumeChanged(CGame* pGame);
	void C6SSettingsState::SoundVolumeChanged(CGame* pGame);
	void C6SSettingsState::ResolutionDoubleClicked(CGame* pGame);

	static C6SSettingsState* Instance()
	{
		static C6SSettingsState m_SettingsState;
		return &m_SettingsState;
	}
};

#endif

