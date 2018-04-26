#include "stdafx.h"
#include "CGame.h"
#include "C6SMenuState.h"
#include "C6SSettingsState.h"

#define FONT_SIZE 24

void C6SSettingsState::ReturnToMenu(CGame* pGame) {
	// Disable GUI handling
	pGame->m_WindowManager.SetGUI(NULL);
	pGame->m_WindowManager.SetGUIActive(false);

	/* Check for settings delta */
	if (m_TempSettings != pGame->m_Settings) {
		CDebugLogger::LogInfo("Settings changed, applying\n");
		/* Apply settings change */
		pGame->OnSettingsChange();
	}
	else {
		CDebugLogger::LogInfo("Settings saved unchanged\n");
	}

	/* */
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
	tgui::CheckBox::Ptr pChk = m_pGui->get<tgui::CheckBox>("Fullscreen");
	pGame->m_Settings.SetFullscreen(pChk->isChecked());
}

void C6SSettingsState::MusicVolumeChanged(CGame* pGame) {
	tgui::Slider::Ptr pSld = m_pGui->get<tgui::Slider>("MusicVol");
	pGame->m_Settings.SetMusicVolume((pSld->getValue() / 100.0f));
}

void C6SSettingsState::SoundVolumeChanged(CGame* pGame) {
	tgui::Slider::Ptr pSld = m_pGui->get<tgui::Slider>("SoundVol");
	pGame->m_Settings.SetSoundVolume((pSld->getValue() / 100.0f));
}
void C6SSettingsState::ResolutionDoubleClicked(CGame* pGame) {
	tgui::ListBox::Ptr pList = m_pGui->get<tgui::ListBox>("Resolution");
	int id = pList->getSelectedItemIndex();
	if (m_vResolutions.size() > (unsigned int)id) {
		printf("Resolution changed to (%i, %i)\n", m_vResolutions[id].x, m_vResolutions[id].y);
		pGame->m_Settings.SetResolution(m_vResolutions[id].x, m_vResolutions[id].y);
	}
}

void C6SSettingsState::Init(CGame* pGame) {
	// Local copy of old settings
	m_TempSettings = pGame->m_Settings;

	
	if (!IsAlreadyInitialised()) {
		/* Get valid resolutions for max bpp */
		std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
		int bppMax = -1;
		for (unsigned int i = 0; i < modes.size(); i++) {
			if ((unsigned int)bppMax > modes[i].bitsPerPixel) {
				bppMax = modes[i].bitsPerPixel; m_vResolutions.clear();
			}
			m_vResolutions.push_back(sf::Vector2i((int)modes[i].width, (int)modes[i].height));
		}
			

		/* Create GUI if it doesn't exist */
		m_pGui = new tgui::Gui(*pGame->m_WindowManager.GetWindow());
		try {
			tgui::Theme::Ptr theme = std::make_shared<tgui::Theme>("widgets/Black.txt");

			tgui::Layout yOffset = 200;

			/* Add Fullscreen checkbox */
			tgui::CheckBox::Ptr chkFullscreen = theme->load("Checkbox");
			chkFullscreen->setSize(25, 30);
			chkFullscreen->setPosition(pGame->m_WindowManager.GetScreenCentre().X - chkFullscreen->getSize().x, yOffset);
			chkFullscreen->setText("Fullscreen");
			chkFullscreen->setTextSize(SETTINGS_TEXT_SIZE);
			chkFullscreen->connect("Checked Unchecked", &C6SSettingsState::FullscreenToggled, this, pGame);
			m_pGui->add(chkFullscreen, "Fullscreen");
			yOffset += 50;

			/* Add resolution */
			tgui::Label::Ptr lblResolution = std::make_shared<tgui::Label>();
			lblResolution->setText("Resolution");
			lblResolution->setTextSize(SETTINGS_TEXT_SIZE);
			lblResolution->setPosition(pGame->m_WindowManager.GetScreenCentre().X - lblResolution->getSize().x, yOffset);
			m_pGui->add(lblResolution);
			yOffset += (float)(SETTINGS_TEXT_SIZE + SETTINGS_SPACING);
			tgui::ListBox::Ptr lstResolution = theme->load("ListBox");
			lstResolution->setSize(200, (float)(SETTINGS_TEXT_SIZE * 4));
			lstResolution->setItemHeight(SETTINGS_TEXT_SIZE);
			lstResolution->setPosition(pGame->m_WindowManager.GetScreenCentre().X - lstResolution->getSize().x, yOffset);
			lstResolution->setTextSize(SETTINGS_TEXT_SIZE);
			for (unsigned int i = 0; i < m_vResolutions.size(); i++) {
				/* Build resolution string*/
				std::string resolution = std::to_string(m_vResolutions[i].x);
				resolution += "x";
				resolution += std::to_string(m_vResolutions[i].y);

				/* Add to listbox*/
				lstResolution->addItem(resolution.c_str());
			}
			lstResolution->connect("DoubleClicked", &C6SSettingsState::ResolutionDoubleClicked, this, pGame);
			m_pGui->add(lstResolution, "Resolution");
			yOffset += (float)(SETTINGS_TEXT_SIZE * 4);
			 
			/* MusicVolume Text */
			tgui::Label::Ptr lblMusicVolume = std::make_shared<tgui::Label>();
			lblMusicVolume->setText("Music Volume");
			lblMusicVolume->setTextSize(SETTINGS_TEXT_SIZE);
			lblMusicVolume->setPosition(pGame->m_WindowManager.GetScreenCentre().X - lblMusicVolume->getSize().x, yOffset);
			m_pGui->add(lblMusicVolume);
			yOffset += 50;
			/* MusicVolume Slider */
			tgui::Slider::Ptr sldMusicVolume = theme->load("Slider");
			sldMusicVolume->setSize(100, 15);
			sldMusicVolume->setPosition(pGame->m_WindowManager.GetScreenCentre().X - sldMusicVolume->getSize().x, yOffset);
			sldMusicVolume->connect("ValueChanged", &C6SSettingsState::MusicVolumeChanged, this, pGame);
			sldMusicVolume->setMinimum(0);
			sldMusicVolume->setMaximum(100); 
			m_pGui->add(sldMusicVolume, "MusicVol");
			yOffset += 50;

			/* Save/return button */
			yOffset += 50;
			tgui::Button::Ptr btnPlay = theme->load("Button");
			btnPlay->setSize(200, 30);
			btnPlay->setPosition((float)pGame->m_WindowManager.GetScreenCentre().X - 300.0f, yOffset);
			btnPlay->setText("Save and Return");
			btnPlay->setTextSize(SETTINGS_TEXT_SIZE);
			btnPlay->connect("pressed", &C6SSettingsState::ReturnToMenu, this, pGame);
			m_pGui->add(btnPlay);

			/* Cancel button */
			tgui::Button::Ptr btnCancel = theme->load("Button");
			btnCancel->setSize(200, 30);
			btnCancel->setPosition((float)pGame->m_WindowManager.GetScreenCentre().X + 100.0f, yOffset);
			btnCancel->setText("Cancel");
			btnCancel->setTextSize(SETTINGS_TEXT_SIZE);
			btnCancel->connect("pressed", &C6SSettingsState::Cancel, this, pGame);
			m_pGui->add(btnCancel);
		}
		catch (...) { 
			pGame->ErrorQuit("Unable to create main menu");
		}

		
	}

	/* Copy settings values into GUI elements */

	// Fullscreen
	tgui::CheckBox::Ptr pChkFullscreen = m_pGui->get<tgui::CheckBox>("Fullscreen");
	if (pGame->m_Settings.m_bFullscreen)
		pChkFullscreen->check();
	else
		pChkFullscreen->uncheck();

	// MusicVolume
	tgui::Slider::Ptr pSldMusic = m_pGui->get<tgui::Slider>("MusicVol");
	int musicSliderVal = (int)(pGame->m_Settings.m_fMusicVolume * 100.0f);
	pSldMusic->setValue(musicSliderVal);

	/* Find res */
	for (unsigned int i = 0; i < m_vResolutions.size(); i++) {
		/* Check if resolution matches */
		if (pGame->m_Settings.m_vScreenSize.X == m_vResolutions[i].x
			&& pGame->m_Settings.m_vScreenSize.Y == m_vResolutions[i].y) {

			/* Set to current res */
			tgui::ListBox::Ptr pResList = m_pGui->get<tgui::ListBox>("Resolution");
			if (pResList->getItemCount() > i) {
				pResList->setSelectedItemByIndex(i);
				break;
			}
		}
	}

	/* Enable GUI input handling */
	pGame->m_WindowManager.SetGUI(m_pGui);
	pGame->m_WindowManager.SetGUIActive(true);
}

void C6SSettingsState::Cleanup(CGame* pGame) {

}

void C6SSettingsState::Draw(CGame* pGame){
	pGame->m_Drawing.DrawTextCentredX(pGame->GetWindowMgr(), "Settings", pGame->m_WindowManager.GetScreenCentre().X, 100, 64, 255, 255, 255, 255);
	m_pGui->draw();
}

void C6SSettingsState::Update(CGame* pGame){

}

void C6SSettingsState::HandleInput(CGame* pGame) {

}

void C6SSettingsState::PauseState(){

}
void C6SSettingsState::ResumeState(){

}