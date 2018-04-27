#include "stdafx.h"
#include "C6SMenuState.h"
#include "C6SDevState.h"
#include "C6SMultiplayerState.h"
#include "C6SMapMakerState.h"
#include "C6SDevTestGM.h"
#include "C6SSettingsState.h"
#include "CGame.h"
#include "ASSETS.h"

#define FONT_SIZE 24
C6SMenuState C6SMenuState::m_MenuState;

void PlayGame(CGame* pGame) {
	pGame->m_WindowManager.SetGUI(NULL);
	pGame->m_WindowManager.SetGUIActive(false);

	pGame->m_StateHandler.ChangeState(C6SDevState::Instance(), pGame); 
}

void LaunchMapMaker(CGame* pGame) { 
	pGame->m_WindowManager.SetGUI(NULL);
	pGame->m_WindowManager.SetGUIActive(false);

	pGame->m_StateHandler.ChangeState(C6SMapMakerState::Instance(), pGame);
}
void LaunchDevGM(CGame* pGame) {
	pGame->m_WindowManager.SetGUI(NULL);
	pGame->m_WindowManager.SetGUIActive(false);

	pGame->m_StateHandler.ChangeState(C6SDevTestGM::Instance(), pGame);
}
void QuitGame(CGame* pGame) { 
	pGame->Quit();
}
 
void MultiPlayer(CGame* pGame) {
	pGame->m_WindowManager.SetGUI(NULL); 
	pGame->m_WindowManager.SetGUIActive(false);

	pGame->m_StateHandler.ChangeState(C6SMultiplayerState::Instance(), pGame);
}

void Settings(CGame* pGame) {
	pGame->m_WindowManager.SetGUI(NULL);
	pGame->m_WindowManager.SetGUIActive(false);

	pGame->m_StateHandler.ChangeState(C6SSettingsState::Instance(), pGame);
} 

void C6SMenuState::Init(CGame* pGame) { 
	if (!IsAlreadyInitialised()) {
		m_pGui = new tgui::Gui(*pGame->m_WindowManager.GetWindow());
		
		try {
			tgui::Theme::Ptr theme = std::make_shared<tgui::Theme>("widgets/Black.txt");
			tgui::Layout yOffset = 300;

			// Play button
			tgui::Button::Ptr btnPlay = theme->load("Button");
			btnPlay->setSize(200, 30);
			btnPlay->setPosition((float)pGame->m_WindowManager.GetScreenCentre().X - 100.0f, yOffset);
			btnPlay->setText("Singleplayer");
			btnPlay->setTextSize(24);
			btnPlay->connect("pressed", &PlayGame, pGame);
			m_pGui->add(btnPlay);
			yOffset += 40;

			// Mutliplayer button
			tgui::Button::Ptr btnMP = theme->load("Button");
			btnMP->setSize(200, 30);
			btnMP->setPosition((float)pGame->m_WindowManager.GetScreenCentre().X - 100.0f, yOffset);
			btnMP->setText("Multiplayer");
			btnMP->setTextSize(24);
			btnMP->connect("pressed", &MultiPlayer, pGame);
			m_pGui->add(btnMP);
			yOffset += 40;

			// Map maker button
			tgui::Button::Ptr btnMaker = theme->load("Button");
			btnMaker->setSize(200, 30);
			btnMaker->setPosition((float)pGame->m_WindowManager.GetScreenCentre().X - 100.0f, yOffset);
			btnMaker->setText("Map Maker");
			btnMaker->setTextSize(24);
			btnMaker->connect("pressed", &LaunchMapMaker, pGame);
			m_pGui->add(btnMaker);
			yOffset += 40;

			// Settings button
			tgui::Button::Ptr btnSettings = theme->load("Button");
			btnSettings->setSize(200, 30);
			btnSettings->setPosition((float)pGame->m_WindowManager.GetScreenCentre().X - 100.0f, yOffset);
			btnSettings->setText("Settings");
			btnSettings->setTextSize(24);
			btnSettings->connect("pressed", &Settings, pGame);
			m_pGui->add(btnSettings);
			yOffset += 40;

			// Dev GM
			/*tgui::Button::Ptr btnDevGM = theme->load("Button");
			btnDevGM->setSize(200, 30);
			btnDevGM->setPosition((float)pGame->m_WindowManager.GetScreenCentre().X - 100.0f, yOffset);
			btnDevGM->setText("Dev GameMode");
			btnDevGM->setTextSize(24);
			btnDevGM->connect("pressed", &LaunchDevGM, pGame);
			m_pGui->add(btnDevGM);
			yOffset += 40;*/

			// Quit button
			tgui::Button::Ptr btnQuit = theme->load("Button");
			btnQuit->setSize(200, 30);
			btnQuit->setPosition((float)pGame->m_WindowManager.GetScreenCentre().X - 100.0f, yOffset);
			btnQuit->setText("Quit");
			btnQuit->setTextSize(24);
			btnQuit->connect("pressed", &QuitGame, pGame);
			m_pGui->add(btnQuit);
			yOffset += 40;			
		}
		catch (...) {
			pGame->ErrorQuit("Unable to create main menu");
		}
	}

	/* Title music */
	pGame->m_SoundManager.PlayMusic(TITLE_MUSIC, &pGame->m_Settings);

	/* Activate GUI */
	pGame->m_WindowManager.SetGUI(m_pGui);
	pGame->m_WindowManager.SetGUIActive(true);
}

void C6SMenuState::Cleanup(CGame* pGame) {}
 
void C6SMenuState::Draw(CGame* pGame){
	// Title
	pGame->m_Drawing.DrawTextCentredX(pGame->GetWindowMgr(), "6Shots", pGame->m_WindowManager.GetScreenCentre().X, 32, 64, 255, 255, 255, 255);
	pGame->m_Drawing.DrawTextCentredX(pGame->GetWindowMgr(), "Version 0.2", pGame->m_WindowManager.GetScreenCentre().X, 100, 24, 255, 255, 255, 255);

	// Menu text
	pGame->m_Drawing.DrawTextCentredX(pGame->GetWindowMgr(), "Main Menu", pGame->m_WindowManager.GetScreenCentre().X, 220, 32, 255, 255, 255, 255);

	m_pGui->draw();
}

void C6SMenuState::Update(CGame* pGame) {

}

void C6SMenuState::HandleInput(CGame* pGame) {

}

void C6SMenuState::PauseState(){

}
void C6SMenuState::ResumeState(){

}