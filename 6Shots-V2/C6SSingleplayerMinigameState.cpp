#include "stdafx.h"
#include "C6SSingleplayerMinigameState.h"
#include "CGame.h"
#include "ASSETS.h"
#include "6SWeapons.h"
#include "CBaseNPC.h"

#define GRAVITY 12.0f
#define TERMINAL 200.0f
#define FONT_SIZE 24
#define PLAYER_JUMPSPEED -290.0f
#define PLAYER_MOVESPEED 200.0f

void LuaSetMap(lua_State* l) {
	const char* szMapName = lua_tostring(l, 1);
	
	//return 0;
}

// if theres something here worth keeping its not something that i've found
void C6SSingleplayerMinigameState::Init(CGame* pGame, char* szMinigame) {
	// Check for double init
	if (IsAlreadyInitialised()) {
		printf("ERROR - Unsupported 2nd load of singleplayer minigame state, please kill me life is pain\n");
		exit(-1);
	}

	// Initialize physics engine 
	m_Physics.InitPhysics(pGame, GRAVITY, false);

	// default local player
	m_Player.Initialize( &m_Physics, ASSET_PLAYER_TEXTURE, ASSET_PLAYER_HAND, 10, 100, 64, 32, PLAYER_JUMPSPEED, PLAYER_MOVESPEED, Vector2i(0,0) );

	// Init game gore engine
	if (!m_Gore.IsInit()) {
		m_Gore.Init(ASSET_FLOOR_BLOOD_TEXTURE, &m_Physics);
	}

	// init lua SDK
	m_LuaLink.InitLua();
	m_LuaLink.SetSDKInformation("6Shots", 0, 1);
	//m_LuaLink.RegisterCFunc("testmsg", Msg, "Description");

	// Print SDK to console 
	m_LuaLink.PrintSDK();

	// initialise singleplayer gamemode
	m_LuaLink.CallExtScriptFunction(szMinigame, (char*)m_LuaLink.GAMEMODE_INIT_FUNCTION);	
	
	// Reset elapsed game time 
	pGame->m_Time.ResetFrameTime();

	// Init game settings
	m_gameSettings.Init(GRAVITY, TERMINAL);
}

void C6SSingleplayerMinigameState::Cleanup(CGame* pGame) {
	m_LuaLink.DestroyLua();
}

void C6SSingleplayerMinigameState::Draw(CGame* pGame){
	CWindowManager* pWnd = &pGame->m_WindowManager;
	m_Map.DrawBackground(pGame, m_Player.GetPosition2f());

	/* Centre Camera on Player */
	pWnd->MoveCamera(m_Player.GetPosition2f());

	m_Map.DrawLevel(pGame);
	m_Gore.Render(pGame);
	m_Player.Draw(pGame);
	m_EntMgr.Draw(pGame);
	m_ProjectileManager.DrawProjectiles(pGame);

	/* Debug draw if enabled */
	m_Physics.Draw();

	/* Restore camera */
	pWnd->RestoreDefaultCamera();
}

void C6SSingleplayerMinigameState::Update(CGame* pGame){
	float fLastFrameTime = pGame->m_Time.LastFrameTime();
	m_Player.Update(fLastFrameTime, pGame, &m_Map, &m_ProjectileManager, &m_Physics);
	m_ProjectileManager.UpdateProjectiles(fLastFrameTime);
	m_EntMgr.Update(fLastFrameTime, pGame, &m_Physics);
	m_Gore.Update(pGame, &m_Physics, fLastFrameTime);
	m_Physics.Step(fLastFrameTime);
}

void C6SSingleplayerMinigameState::HandleInput(CGame* pGame){
	// This is a hack to fix world to screen stuff
	CWindowManager* pWnd = &pGame->m_WindowManager;
	pWnd->MoveCamera(m_Player.GetPosition2f());

	m_Player.HandleInput(pGame, &m_EntMgr, true);

	pWnd->RestoreDefaultCamera();
}
