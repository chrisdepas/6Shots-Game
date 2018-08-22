#ifndef __C6SSETTINGSSTATE_H__
#define __C6SSETTINGSSTATE_H__

#include "CGameState.h"

/* Main Menu */
class C6SSettingsState : public CGameState {
	tgui::Gui* m_pGui;
	CSettings m_TempSettings;

	const int SETTINGS_TEXT_SIZE = 24;
	const int SETTINGS_SUB_TEXT_SIZE = 14;
	const int SETTINGS_SPACING = 12;

	const int SETTINGS_TITLE_SIZE = 32;
	const int SETTINGS_TITLE_SPACING = 18;
	const float SETTINGS_PADDING = 8.0f;

	/* Resolutions */
	std::vector<sf::Vector2i> m_vResolutions;

	// Polulate m_vResolutions and arg vector with resolution vector2i & 
	// resolution strings respectively
	void GetResolutions(std::vector<std::string>& vResolutionStrings);

public:
	void C6SSettingsState::Init(CGame* pGame);
	void C6SSettingsState::Cleanup(CGame* pGame);

	void C6SSettingsState::Draw(CGame* pGame);
	void C6SSettingsState::Update(CGame* pGame, float fFrameTime);
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

