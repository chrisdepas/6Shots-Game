#include "stdafx.h"
#include "CGame.h"
#include "C6SMenuState.h"
#include "C6SSettingsState.h"
#include "GUIFactory.h"

#define FONT_SIZE 24

void C6SSettingsState::ReturnToMenu(CGame* pGame) {
	// Disable GUI handling
	pGame->m_WindowManager.SetGUI(NULL);
	pGame->m_WindowManager.SetGUIActive(false);

	// Check for settings delta
	if (m_TempSettings != pGame->m_Settings) {
		CDebugLogger::LogInfo("Settings changed, applying\n");
		pGame->OnSettingsChange();
	} else {
		CDebugLogger::LogInfo("Settings unchanged\n");
	}

	pGame->m_StateHandler.ChangeState(C6SMenuState::Instance(), pGame);
}

void C6SSettingsState::Cancel(CGame* pGame) {
	// Disable GUI handling
	pGame->m_WindowManager.SetGUI(NULL);
	pGame->m_WindowManager.SetGUIActive(false);

	// Restore old settings
	pGame->m_Settings = m_TempSettings;

	// Return to menu
	pGame->m_StateHandler.ChangeState(C6SMenuState::Instance(), pGame);
}

void C6SSettingsState::FullscreenToggled(CGame* pGame) {
	tgui::Panel::Ptr videoPanel = m_pGui->get<tgui::Panel>("VideoPanel");
	if (!videoPanel) {
		CDebugLogger::LogError("Unable to find Video Panel!\n");
	}

	tgui::CheckBox::Ptr pChk = videoPanel->get<tgui::CheckBox>("Fullscreen");
	if (!videoPanel) {
		CDebugLogger::LogError("Unable to find Fullscreen in Video Panel!\n");
	}

	pGame->m_Settings.SetFullscreen(pChk->isChecked());
}

void C6SSettingsState::MusicVolumeChanged(CGame* pGame) {
	tgui::Panel::Ptr audioPanel = m_pGui->get<tgui::Panel>("AudioPanel");
	if (!audioPanel) {
		CDebugLogger::LogError("Unable to find Audio Panel!\n");
		return;
	}
	tgui::Slider::Ptr pSld = audioPanel->get<tgui::Slider>("MusicVol");
	if (!pSld) {
		CDebugLogger::LogError("Unable to find MusicVolume in Video Panel!\n");
		return;
	}
	pGame->m_Settings.SetMusicVolume((pSld->getValue() / 100.0f));
}

void C6SSettingsState::SoundVolumeChanged(CGame* pGame) {
	tgui::Panel::Ptr audioPanel = m_pGui->get<tgui::Panel>("AudioPanel");
	if (!audioPanel) {
		CDebugLogger::LogError("Unable to find Audio Panel!\n");
		return;
	}
	tgui::Slider::Ptr pSld = audioPanel->get<tgui::Slider>("SoundVol");
	if (!pSld) {
		CDebugLogger::LogError("Unable to find SoundVolume in Video Panel!\n");
		return;
	}
	pGame->m_Settings.SetSoundVolume((pSld->getValue() / 100.0f));
}
void C6SSettingsState::ResolutionDoubleClicked(CGame* pGame) {
	tgui::Panel::Ptr videoPanel = m_pGui->get<tgui::Panel>("VideoPanel");
	if (!videoPanel) {
		CDebugLogger::LogError("Unable to find Video Panel!\n");
	}
	tgui::ListBox::Ptr pList = videoPanel->get<tgui::ListBox>("Resolution");
	if (!pList) {
		CDebugLogger::LogError("Unable to find Resolution in Video Panel!\n");
		return;
	}
	int id = pList->getSelectedItemIndex();
	if (m_vResolutions.size() > (unsigned int)id) {
		printf("Resolution changed to (%i, %i)\n", m_vResolutions[id].x, m_vResolutions[id].y);
		pGame->m_Settings.SetResolution(m_vResolutions[id].x, m_vResolutions[id].y);
	}
}


void SettingsTabClicked(tgui::Gui* pGUI) {
	tgui::Tabs::Ptr tab = pGUI->get<tgui::Tabs>("TabMenu");
	int selectedTab = tab->getSelectedIndex();

	switch (selectedTab) {
	case 0:
		pGUI->get("VideoPanel")->setVisible(true);
		pGUI->get("AudioPanel")->setVisible(false);
		pGUI->get("MiscPanel")->setVisible(false);
		break;
	case 1:
		pGUI->get("VideoPanel")->setVisible(false);
		pGUI->get("AudioPanel")->setVisible(true);
		pGUI->get("MiscPanel")->setVisible(false);
		break;
	case 2:
		pGUI->get("VideoPanel")->setVisible(false);
		pGUI->get("AudioPanel")->setVisible(false);
		pGUI->get("MiscPanel")->setVisible(true);
		break;
	default:
		break;
	}
}


void C6SSettingsState::GetResolutions(std::vector<std::string>& vResolutionStrings) {	
	// Get all valid resolutions
	std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();

	// Get maximum bits per pixel for each videomode
	int bppMax = std::max_element(modes.begin(), modes.end(),
		[](const sf::VideoMode& l, const sf::VideoMode& r) {
		return l.bitsPerPixel < r.bitsPerPixel;
	})->bitsPerPixel;

	// Remove videomodes with less than max bits per pixel
	modes.erase(std::remove_if(modes.begin(), modes.end(), [bppMax](const sf::VideoMode& v) {
		return v.bitsPerPixel != bppMax;
	}), modes.end());

	// Create list of resolutions & resolution strings from videomodes
	for (unsigned int i = 0; i < modes.size(); i++) {
		m_vResolutions.push_back(sf::Vector2i(modes[ i ].width, modes[ i ].height));
		vResolutionStrings.push_back(std::to_string(modes[ i ].width) + " x " + std::to_string(modes[ i ].height));
	}
}

void C6SSettingsState::Init(CGame* pGame) {
	// Local copy of old settings
	m_TempSettings = pGame->m_Settings;

	if (IsAlreadyInitialised()) {
		m_pGui->setView(pGame->m_WindowManager.GetGUIView());

	} else {
		std::vector<std::string> renderedResolutions;
		GetResolutions(renderedResolutions);

		/* Create GUI if it doesn't exist */
		m_pGui = new tgui::Gui(*pGame->m_WindowManager.GetWindow());
		try {
			// Create Tabs
			tgui::Tabs::Ptr tabs = GUIFactory::Tabs("Video", "Audio", "Misc");
			tabs->setPosition(150, 150);
			tabs->connect("TabSelected", SettingsTabClicked, m_pGui);
			m_pGui->add(tabs, "TabMenu");

			// Create panels for each tab, with a Left/Right box layout 
			tgui::Panel::Ptr videoPanel = tgui::Panel::create();
			videoPanel->setSize(400, 300);
			videoPanel->setPosition(tabs->getPosition().x, tabs->getPosition().y + tabs->getSize().y);
			m_pGui->add(videoPanel, "VideoPanel");

			tgui::Panel::Ptr audioPanel = tgui::Panel::copy(videoPanel);
			m_pGui->add(audioPanel, "AudioPanel");

			tgui::Panel::Ptr miscPanel = tgui::Panel::copy(videoPanel);
			m_pGui->add(miscPanel, "MiscPanel");

			// Hide panels for other tabs, select first tab
			miscPanel->setVisible(false);
			audioPanel->setVisible(false);
			tabs->select(0);

			const tgui::Layout2d PanelTopLeft = videoPanel->getPosition();
			const tgui::Layout2d PanelSize = videoPanel->getSize();
			

			// Create titles for each tab
			tgui::Layout2d videoPanelCursor = tgui::Layout2d(SETTINGS_PADDING, SETTINGS_PADDING);
			tgui::Layout2d audioPanelCursor = videoPanelCursor;
			tgui::Layout2d miscPanelCursor = videoPanelCursor;


			// Add Video Panel Widgets

			// Fullscreen
			GUILayout::Checkbox(videoPanel, videoPanelCursor, "Fullscreen", "Fullscreen", false, &C6SSettingsState::FullscreenToggled, this, pGame);
			
			// Resolution
			GUILayout::ListBoxVertical(videoPanel, videoPanelCursor, "Resolution", "Resolution", renderedResolutions, -1, &C6SSettingsState::ResolutionDoubleClicked, this, pGame);
			
			 
			// Add Audio Panel widgets

			// Music Volume
			tgui::Label::Ptr lblMusicVolume = std::make_shared<tgui::Label>();
			lblMusicVolume->setText("Music Volume");
			lblMusicVolume->setTextSize(SETTINGS_TEXT_SIZE);
			lblMusicVolume->setPosition(audioPanelCursor);
			audioPanel->add(lblMusicVolume);
			audioPanelCursor.y = audioPanelCursor.y + lblMusicVolume->getFullSize().y + SETTINGS_SPACING;

			tgui::Slider::Ptr sldMusicVolume = tgui::Slider::create(0.0f, 100.0f);
			sldMusicVolume->setSize(100, 15);
			sldMusicVolume->setPosition(audioPanelCursor);
			sldMusicVolume->connect("ValueChanged", &C6SSettingsState::MusicVolumeChanged, this, pGame);
			audioPanel->add(sldMusicVolume, "MusicVol");
			audioPanelCursor.y = audioPanelCursor.y + sldMusicVolume->getFullSize().y + SETTINGS_SPACING;

			/* Save/return button */
			tgui::Button::Ptr btnPlay = tgui::Button::create("Save and Return");
			btnPlay->setSize(200, 30);
			btnPlay->setPosition(pGame->m_WindowManager.GetScreenCentre2f().x - 300.0f, PanelSize.y + PanelTopLeft.y);
			btnPlay->setTextSize(SETTINGS_TEXT_SIZE);
			btnPlay->connect("pressed", &C6SSettingsState::ReturnToMenu, this, pGame);
			m_pGui->add(btnPlay);

			/* Cancel button */
			tgui::Button::Ptr btnCancel = tgui::Button::create("Cancel");
			btnCancel->setSize(200, 30);
			btnCancel->setPosition(pGame->m_WindowManager.GetScreenCentre2f().x + 100.0f, PanelSize.y + PanelTopLeft.y);
			btnCancel->setTextSize(SETTINGS_TEXT_SIZE);
			btnCancel->connect("pressed", &C6SSettingsState::Cancel, this, pGame);
			m_pGui->add(btnCancel);
		}
		catch (std::exception e) { 
			pGame->ErrorQuit(std::string("Unable to create settings GUI. Reason: ") + e.what());
		}
	}

	/* Copy settings values into GUI elements */


	// Video Settings
	tgui::Panel::Ptr videoPanel = m_pGui->get<tgui::Panel>("VideoPanel");

	// Fullscreen
	tgui::CheckBox::Ptr pChkFullscreen = videoPanel->get<tgui::CheckBox>("Fullscreen");
	pChkFullscreen->setChecked(pGame->m_Settings.m_bFullscreen);

	// Resolution 
	for (unsigned int i = 0; i < m_vResolutions.size(); i++) {
		/* Check if resolution matches */
		if (pGame->m_Settings.m_vScreenSize.x == m_vResolutions[i].x
			&& pGame->m_Settings.m_vScreenSize.y == m_vResolutions[i].y) {

			/* Set to current res */
			tgui::ListBox::Ptr pResList = videoPanel->get<tgui::ListBox>("Resolution");
			if (pResList->getItemCount() > i) {
				pResList->setSelectedItemByIndex(i);
				break;
			}
		}
	}


	// Audio Settings
	tgui::Panel::Ptr audioPanel = m_pGui->get<tgui::Panel>("AudioPanel");

	// MusicVolume
	tgui::Slider::Ptr pSldMusic = audioPanel->get<tgui::Slider>("MusicVol");
	float musicSliderVal = pGame->m_Settings.m_fMusicVolume * 100.0f;
	pSldMusic->setValue(musicSliderVal);

	// Enable GUI input handling 
	pGame->m_WindowManager.SetGUI(m_pGui);
	pGame->m_WindowManager.SetGUIActive(true);
}

void C6SSettingsState::Cleanup(CGame* pGame) {}
void C6SSettingsState::Update(CGame* pGame, float fFrameTime) {}
void C6SSettingsState::HandleInput(CGame* pGame) {}
void C6SSettingsState::PauseState() {}
void C6SSettingsState::ResumeState() {}

void C6SSettingsState::Draw(CGame* pGame){
	pGame->m_Drawing.DrawTextCentredX(pGame->GetWindowMgr(), "Settings", 
		pGame->m_WindowManager.GetScreenCentre().x, 80, 64, 255, 255, 255, 255);

	m_pGui->draw();
}