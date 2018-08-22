#include "stdafx.h"
#include "CGame.h"
#include "C6SMenuState.h"
#include "Util.h"

void CmdExitGame(CDebugConsole* pConsole, CGame* pGame) {
	pConsole->AddConsoleText("Closing game...");
	pGame->m_WindowManager.CloseWindow();
}

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

	/* Setup console */
	m_Console.Init(this); 

	m_Console.RegisterConsoleFunction("quit", "Exit game", CmdExitGame, this);
}

bool CGame::ShouldQuit() {
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
void CGame::Step() {
	float fFrameTime = m_Time.LastFrameTime();
	m_WindowManager.HandleEvents(&m_Input);
	m_StateHandler.Update(this, fFrameTime);
	m_Scheduler.Update(fFrameTime);
}
#include <Windows.h>
void CGame::ErrorQuit(const std::string& sMsg) {
	MessageBoxA(NULL, sMsg.c_str(), "Fatal Error", NULL);
	printf("FATAL ERROR - %s\n", sMsg.c_str());
	exit(-1);
}

void CGame::OnSettingsChange() {
	// Output new settings 
	m_Settings.SaveToFile(SETTINGS_FILE_PATH, &m_IniWriter);

	// Create window with new settings
	m_WindowManager.ReCreate(&m_Settings, (char*)CGame::GAME_WINDOW_TITLE);
}
void CGame::Quit() {
	exit(0);
}

CWindowManager* CGame::GetWindowMgr() {
	return &m_WindowManager;
}