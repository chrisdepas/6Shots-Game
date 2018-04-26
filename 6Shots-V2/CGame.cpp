#include "stdafx.h"
#include "CGame.h"
#include "C6SMenuState.h"
#include <Windows.h>
#include "Util.h"
#include <string>

void CGame::Initialise() {
	m_bInitialised = true;
	 
	/* Load settings & save to disk */
	m_Settings.LoadFromFile(SETTINGS_FILE_PATH, &m_IniReader);
	m_Settings.SaveToFile(SETTINGS_FILE_PATH, &m_IniWriter);

	/* Create a window and initialise drawing */
	m_WindowManager.Create(&m_Settings, (char*)CGame::GAME_WINDOW_TITLE);
	m_Drawing.Initialise(); 

	/* Set initial state to MenuState */
	m_StateHandler.ChangeState(C6SMenuState::Instance(), this);

//	m_Random.Init();

	/* Set up input defaults */
	m_Input.Initialise();
}
bool CGame::ShouldQuit()
{
	return m_WindowManager.WindowClosed();
}
void CGame::Draw()
{
	m_WindowManager.Begin();

	m_StateHandler.Draw(this);

	m_WindowManager.Present();
}
void CGame::HandleInput()
{
	m_StateHandler.HandleInput(this);
}
void CGame::Step()
{
	m_WindowManager.HandleEvents(&m_Input);
	m_StateHandler.Update(this);
}

void CGame::ErrorQuit(char* err) {
	MessageBoxA(0, err, "FATAL ERROR", 0);
	exit(-1);
}

void CGame::OnSettingsChange() {
	/* Output new settings */
	m_Settings.SaveToFile(SETTINGS_FILE_PATH, &m_IniWriter);

	/* Create window with new settings */
	m_WindowManager.ReCreate(&m_Settings, (char*)CGame::GAME_WINDOW_TITLE);
}
void CGame::Quit() {
	exit(0);
}

CWindowManager* CGame::GetWindowMgr() {
	return &m_WindowManager;
}