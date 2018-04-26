#include "stdafx.h"
#include "C6SDevState.h"
#include "CGame.h"
#include "ASSETS.h"
#include "6SWeapons.h"
#include "CBaseNPC.h"

#define GRAVITY 12.0f
#define TERMINAL 200.0f
#define FONT_SIZE 24
#define PLAYER_JUMPSPEED -290.0f
#define PLAYER_MOVESPEED 200.0f

int Msg(lua_State* L) {
	printf("Lua exposed!!!\n");
	return 0;
}
void C6SDevState::Init(CGame* pGame) {
	/* Initialize physics engine */
	m_Physics.InitPhysics(pGame, GRAVITY, false);

	if (!IsAlreadyInitialised()) {
		/* Single-time initialisation */
		
		/* Load map */ 
		m_Map.InitializeFromFile(pGame, "./Map/LargerTest.6map");

		/* Init map collision spaces */
		m_Physics.ParseWorldMap(&m_Map);

		//  player
		m_Player.Initialize(&m_Physics, ASSET_PLAYER_TEXTURE, ASSET_PLAYER_HAND, 10, 100, 64, 32, PLAYER_JUMPSPEED, PLAYER_MOVESPEED, m_Map.GetSpawn(pGame));

		// Weapons
		C6SKnife* knife = new C6SKnife();
		knife->Init(&m_Physics, Vector2i(1000,1000));
		m_EntMgr.AddWeapon(knife);

		C6SAk47* ak = new C6SAk47();
		ak->Init(&m_Physics, Vector2f(100, 1000), pGame);
		m_EntMgr.AddWeapon(ak);

		C6SSawnOff* sawn = new C6SSawnOff();
		sawn->Init(&m_Physics, Vector2i(400, 950), pGame);
		m_EntMgr.AddWeapon(sawn);

		C6SRevolver* rev = new C6SRevolver();
		rev->Init(&m_Physics, Vector2i(900, 1000), pGame);
		m_EntMgr.AddWeapon(rev);

		C6STMP* tmp = new C6STMP();
		tmp->Init(&m_Physics, Vector2i(650, 1000), pGame);
		m_EntMgr.AddWeapon(tmp);

		// Gore
		m_Gore.Init(ASSET_FLOOR_BLOOD_TEXTURE, &m_Physics);

		// init lua SDK
		m_LuaLink.InitLua();
		m_LuaLink.SetSDKInformation("6Shots", 0, 1);
		m_LuaLink.RegisterCFunc("testmsg", Msg, "Description");
		char* val;
		if (m_LuaLink.CallExtScriptFunctionStr("C:\\LUA_TEST.lua", "getGamemodeName", val)) {
			printf("CallExtScriptFunctionStr - Recieved string value %s\n", val);
			free(val);
		}
		else {
			printf("CallExtScriptFunctionStr failed\n");
		}

		// Init lua scripting craponi

	
		CBaseNPC* npc = new CBaseNPC();
		npc->Initialize(&m_Physics, ASSET_NPC_TEXTURE, ASSET_PLAYER_HAND, 10, 25, 64, 32, PLAYER_JUMPSPEED, PLAYER_MOVESPEED, m_Map.GetSpawn(pGame));
		m_EntMgr.AddNPC(npc);

		m_LuaLink.PrintSDK();
	}

	/* Reset elapsed time */
	pGame->m_Time.ResetFrameTime();

	/* Reset map data */
	m_Map.Reset();	
	m_Map.ResetSpawns(); 

	/* Player pos */
	m_Player.SetPosition(m_Map.GetSpawn(pGame));

	m_gameSettings.Init(GRAVITY, TERMINAL);
}

void C6SDevState::Cleanup(CGame* pGame){
	m_LuaLink.DestroyLua();
}

void C6SDevState::Draw(CGame* pGame){
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

void C6SDevState::Update(CGame* pGame){
	float fLastFrameTime = pGame->m_Time.LastFrameTime();
	m_Player.Update(fLastFrameTime, pGame, &m_Map, &m_ProjectileManager, &m_Physics);
	m_ProjectileManager.UpdateProjectiles(fLastFrameTime);
	m_EntMgr.Update(fLastFrameTime, pGame, &m_Physics);
	m_Gore.Update(pGame, &m_Physics, fLastFrameTime);
	m_Physics.Step(fLastFrameTime);
}

bool bHeldKey = false;
void C6SDevState::HandleInput(CGame* pGame){
	// This is a hack to fix world to screen stuff
	CWindowManager* pWnd = &pGame->m_WindowManager;
	pWnd->MoveCamera(m_Player.GetPosition2f()); 

	m_Player.HandleInput(pGame, &m_EntMgr, true);

	/* Test - gore */
	if (pGame->m_Input.ActionKeyPressed(EAction::Action_Sprint)) {
		if (!bHeldKey) {
			Vector2i vMouseScreen = pGame->m_Input.GetMousePosition();
			Vector2i vMouseWorld = pGame->m_WindowManager.ScreenToWorld(vMouseScreen);
			m_Gore.CreateBloodSplatter(&m_Physics, vMouseWorld, 0.2f, 4, Vector2f(pGame->m_Random.RandomFloat(-500.0f, 500.0f), pGame->m_Random.RandomFloat(-500.0f, 500.0f)));
			m_Gore.CreateBloodSplatter(&m_Physics, vMouseWorld, 0.2f, 4, Vector2f(pGame->m_Random.RandomFloat(-500.0f, 500.0f), pGame->m_Random.RandomFloat(-500.0f, 500.0f)));
			m_Gore.CreateBloodSplatter(&m_Physics, vMouseWorld, 0.2f, 4, Vector2f(pGame->m_Random.RandomFloat(-500.0f, 500.0f), pGame->m_Random.RandomFloat(-500.0f, 500.0f)));
			m_Gore.CreateBloodSplatter(&m_Physics, vMouseWorld, 0.2f, 4, Vector2f(pGame->m_Random.RandomFloat(-500.0f, 500.0f), pGame->m_Random.RandomFloat(-500.0f, 500.0f)));
			m_Gore.CreateBloodSplatter(&m_Physics, vMouseWorld, 0.2f, 4, Vector2f(pGame->m_Random.RandomFloat(-500.0f, 500.0f), pGame->m_Random.RandomFloat(-500.0f, 500.0f)));
			m_Gore.CreateBloodSplatter(&m_Physics, vMouseWorld, 0.2f, 4, Vector2f(pGame->m_Random.RandomFloat(-500.0f, 500.0f), pGame->m_Random.RandomFloat(-500.0f, 500.0f)));
			m_Gore.CreateBloodSplatter(&m_Physics, vMouseWorld, 0.2f, 4, Vector2f(pGame->m_Random.RandomFloat(-500.0f, 500.0f), pGame->m_Random.RandomFloat(-500.0f, 500.0f)));
		}bHeldKey = true;
	}
	else {
		bHeldKey = false;
	}

	pWnd->RestoreDefaultCamera();
}

void C6SDevState::PauseState(){

}
void C6SDevState::ResumeState(){

}