#include "stdafx.h"
#include "C6SMenuState.h"
#include "C6SDevState.h"
#include "C6SMultiplayerState.h"
#include "C6SMapMakerState.h"
#include "C6SSettingsState.h"
#include "C6SWeaponMakerState.h"
#include "CGame.h"
#include "ASSETS.h"
#include "GUIFactory.h"

#define FONT_SIZE 24
C6SMenuState C6SMenuState::m_MenuState;

void PlayGame(CGame* pGame) {
	pGame->m_WindowManager.RemoveGUI();
	pGame->m_StateHandler.ChangeState(C6SDevState::Instance(), pGame); 
}

void LaunchMapMaker(CGame* pGame) {
	pGame->m_WindowManager.RemoveGUI();
	pGame->m_StateHandler.ChangeState(C6SMapMakerState::Instance(), pGame);
}
void LaunchWeaponMaker(CGame* pGame) {
	pGame->m_WindowManager.RemoveGUI();
	pGame->m_StateHandler.ChangeState(C6SWeaponMakerState::Instance(), pGame);
}
void QuitGame(CGame* pGame) { 
	pGame->Quit();
}
 
void MultiPlayer(CGame* pGame) {
	pGame->m_WindowManager.RemoveGUI();
	pGame->m_StateHandler.ChangeState(C6SMultiplayerState::Instance(), pGame);
}

void Settings(CGame* pGame) {
	pGame->m_WindowManager.RemoveGUI();
	pGame->m_StateHandler.ChangeState(C6SSettingsState::Instance(), pGame);
} 

void C6SMenuState::Init(CGame* pGame) { 
	if (IsAlreadyInitialised()) {
		// Set view to correct size and position
		// Fixes case where screen has been resized after GUI is initialized
		m_pGui->setView(pGame->m_WindowManager.GetGUIView());

	} else {
		m_pGui = new tgui::Gui(*pGame->m_WindowManager.GetWindow());
		
		try {
			sf::Vector2f scrCentre = pGame->m_WindowManager.GetScreenCentre2f();
			tgui::Layout2d btnPos(tgui::Layout(scrCentre.x) + tgui::Layout(MENU_BUTTON_OFFSET_X), tgui::Layout(scrCentre.y));

			// Play button (Template)
			tgui::Button::Ptr btnPlay = GUIFactory::ButtonLarge("Singleplayer");
			btnPlay->setPosition(btnPos); btnPos.y = btnPos.y + tgui::bindHeight(btnPlay) + MENU_BUTTON_SPACE;			
			btnPlay->connect("pressed", &PlayGame, pGame);
			m_pGui->add(btnPlay);

			// Mutliplayer button
			tgui::Button::Ptr btnMultiplayer = GUIFactory::ButtonLarge("Multiplayer");
			btnMultiplayer->setPosition((tgui::bindWidth(*m_pGui) / 2) + MENU_BUTTON_OFFSET_X, tgui::bindBottom(btnPlay) + MENU_BUTTON_SPACE);
			btnMultiplayer->connect("pressed", &MultiPlayer, pGame);
			m_pGui->add(btnMultiplayer);

			// Map maker button
			tgui::Button::Ptr btnMakerMap = GUIFactory::ButtonLarge("Create Map");
			btnMakerMap->setPosition((tgui::bindWidth(*m_pGui) / 2) + MENU_BUTTON_OFFSET_X, tgui::bindBottom(btnMultiplayer) + MENU_BUTTON_SPACE);
			btnMakerMap->connect("pressed", &LaunchMapMaker, pGame);
			m_pGui->add(btnMakerMap);

			// Weapon maker button
			tgui::Button::Ptr btnMakerWeapon = GUIFactory::ButtonLarge("Create Weapon");
			btnMakerWeapon->setPosition((tgui::bindWidth(*m_pGui) / 2) + MENU_BUTTON_OFFSET_X, tgui::bindBottom(btnMakerMap) + MENU_BUTTON_SPACE);
			btnMakerWeapon->connect("pressed", &LaunchWeaponMaker, pGame);
			m_pGui->add(btnMakerWeapon);

			// Settings button
			tgui::Button::Ptr btnSettings = GUIFactory::ButtonLarge("Settings");
			btnSettings->setPosition((tgui::bindWidth(*m_pGui) / 2) + MENU_BUTTON_OFFSET_X, tgui::bindBottom(btnMakerWeapon) + MENU_BUTTON_SPACE);
			btnSettings->connect("pressed", &Settings, pGame);
			m_pGui->add(btnSettings);

			// Quit button
			tgui::Button::Ptr btnQuit = GUIFactory::ButtonLarge("Quit");
			btnQuit->setPosition((tgui::bindWidth(*m_pGui) / 2) + MENU_BUTTON_OFFSET_X, tgui::bindBottom(btnSettings) + MENU_BUTTON_SPACE);
			btnQuit->connect("pressed", &QuitGame, pGame);
			m_pGui->add(btnQuit);

			// Test-> 'Open File' dialogue
			tgui::Button::Ptr btnOpenFile = GUIFactory::ButtonLarge("Browse...");
			btnOpenFile->setPosition((tgui::bindWidth(*m_pGui) / 2) + MENU_BUTTON_OFFSET_X, tgui::bindBottom(btnQuit) + MENU_BUTTON_SPACE);
			m_pGui->add(btnOpenFile);
		}
		catch (std::exception e) {
			pGame->ErrorQuit(std::string("Fatal Error: Unable to create main menu. Reason: ") + e.what());
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
	sf::Vector2f scrCentre = pGame->m_WindowManager.GetScreenCentre2f();
	// Title
	pGame->m_Drawing.DrawTextCentredX(pGame->GetWindowMgr(), "6Shots", 
		sf::Vector2f(scrCentre.x, 32.0f), 64, sf::Color::White);

	pGame->m_Drawing.DrawTextCentredX(pGame->GetWindowMgr(), "Version 0.2", 
		sf::Vector2f(scrCentre.x, 100.0f), 24, sf::Color::White);

	// Menu text
	pGame->m_Drawing.DrawTextCentredX(pGame->GetWindowMgr(), "Main Menu", 
		sf::Vector2f(scrCentre.x, 220.0f), 32, sf::Color::White);

	m_pGui->draw();

	if (pGame->m_Console.IsActive()) {
		pGame->m_Console.Draw();
	}
}

void C6SMenuState::Update(CGame* pGame, float fFrameTime) {
	pGame->m_Console.Update(pGame);
}

void C6SMenuState::HandleInput(CGame* pGame) {
	if (pGame->m_Input.ActionKeyPressed(EAction::Action_Console_Toggle)) {
		if (!pGame->m_Console.IsActive()) {
			pGame->m_Console.Show(pGame);
		}
	}

	if (pGame->m_Input.ActionKeyPressed(EAction::Action_Menu_Toggle)) {
		if (pGame->m_Console.IsActive()) {
			pGame->m_Console.Hide(pGame, m_pGui);
		}
	}
}

void C6SMenuState::PauseState(){

}
void C6SMenuState::ResumeState(){

}