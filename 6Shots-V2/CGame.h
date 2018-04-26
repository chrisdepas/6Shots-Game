#ifndef __CGAME_H__
#define __CGAME_H__

#include "CWindowManager.h"
#include "CGameStateHandler.h"
#include "CDrawingManager.h"
#include "CTime.h"
#include "CServer.h"
#include "CRandom.h"
#include "CClient.h"
#include "CEventManager.h"
#include "CSoundManager.h"
#include "CDebugLogger.h"
class CGame
{
	bool				m_bInitialised;

	const char*			GAME_WINDOW_TITLE = "ChrisGame";
public:

	CWindowManager		m_WindowManager;
	CInput				m_Input;
	CSettings			m_Settings;
	CGameStateHandler	m_StateHandler;
	CDrawingManager		m_Drawing;
	CTime				m_Time;
	CRandom				m_Random;
	CEventManager		m_EventManager;

	// Networking
	CServer				m_Server;
	CClient				m_Client;
	
	// Sound
	CSoundManager		m_SoundManager;
		
	// Ini
	CIniReader			m_IniReader;
	CIniWriter			m_IniWriter;

	//CTextureManager		m_TextureManager;
	
	

	CGame() {}
	~CGame() {}
	
	CWindowManager* GetWindowMgr();
	void Initialise();
	bool ShouldQuit();
	void Draw();
	void HandleInput();
	void Step();
	void ErrorQuit(char* szMessage);
	void OnSettingsChange();
	void Quit();
};

#endif