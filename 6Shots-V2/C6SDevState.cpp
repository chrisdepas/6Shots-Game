#include "stdafx.h"
#include "C6SDevState.h"
#include "CGame.h"
#include "ASSETS.h"
#include "6SWeapons.h"
#include "CBaseNPC.h"
#include "C6SMenuState.h"
#include "GUIFactory.h"

#define GRAVITY 12.0f
#define TERMINAL 200.0f
#define FONT_SIZE 24
#define PLAYER_JUMPSPEED -290.0f
#define PLAYER_MOVESPEED 200.0f


void ReturnToMainMenuPressed(CGame* pGame) {
	pGame->m_WindowManager.RemoveGUI();
	pGame->m_StateHandler.ChangeState(C6SMenuState::Instance(), pGame);
}

void C6SDevState::Init(CGame* pGame) {
	/* Initialize physics engine */
	m_Physics.InitPhysics(pGame, GRAVITY, false);

	if (IsAlreadyInitialised()) {
		// Set view to correct size and position
		// Fixes case where screen has been resized after GUI is initialized
		m_GUI.setView(pGame->m_WindowManager.GetGUIView());

	} else {
		/* Single-time initialisation */
		
		/* Load map */ 
		m_Map.LoadFromFile(pGame, "publi");

		/* Init map collision spaces */
		m_Physics.ParseWorldMap(&m_Map);

		//  player
		m_Player.Initialize(&m_Physics, ASSET_PLAYER_TEXTURE, ASSET_PLAYER_HAND, 10, 32, 64, 32, PLAYER_JUMPSPEED, PLAYER_MOVESPEED, m_Map.GetSpawn(pGame));

		// Weapons
		C6SKnife* knife = new C6SKnife();
		knife->Init(&m_Physics, sf::Vector2i(100, 300));
		m_EntMgr.AddWeapon(knife);

		C6SAk47* ak = new C6SAk47();
		ak->Init(&m_Physics, sf::Vector2f(100, 300), pGame);
		m_EntMgr.AddWeapon(ak);

		//C6SSawnOff* sawn = new C6SSawnOff();
		//sawn->Init(&m_Physics, Vector2i(400, 950), pGame);
		//m_EntMgr.AddWeapon(sawn);

		C6SRevolver* rev = new C6SRevolver();
		rev->Init(&m_Physics, sf::Vector2i(100, 300), pGame);
		m_EntMgr.AddWeapon(rev);

		C6SRevolver* rev2 = new C6SRevolver();
		rev2->Init(&m_Physics, sf::Vector2i(100, 300), pGame);
		m_EntMgr.AddWeapon(rev2);

		C6STMP* tmp = new C6STMP();
		tmp->Init(&m_Physics, sf::Vector2i(100, 300), pGame);
		m_EntMgr.AddWeapon(tmp);

		// Gore
		m_Gore.Init(ASSET_FLOOR_BLOOD_TEXTURE, &m_Physics);

		// init lua SDK
		/*m_LuaLink.InitLua();
		m_LuaLink.SetSDKInformation("6Shots", 0, 1);
		m_LuaLink.RegisterCFunc("testmsg", Msg, "Description");
		char* val;
		if (m_LuaLink.CallExtScriptFunctionStr("C:\\LUA_TEST.lua", "getGamemodeName", val)) {
			printf("CallExtScriptFunctionStr - Recieved string value %s\n", val);
			free(val);
		}
		else {
			printf("CallExtScriptFunctionStr failed\n");
		}*/

		//
		m_GUI.setTarget(*pGame->m_WindowManager.GetWindow());
		tgui::Layout2d windowSize = tgui::bindSize(m_GUI);
		tgui::Layout2d panelSize = tgui::Layout2d(sf::Vector2f(
			std::max(windowSize.x.getValue() * 0.3f, 100.0f), 
			std::max(windowSize.y.getValue() * 0.3f, 100.0f)
		));

		auto panel = GUIFactory::PanelCentred(m_GUI, sf::Vector2f(0.3f, 0.3f));
		m_GUI.add(panel);

		auto header = GUIFactory::LabelTitle("Menu");
		header->setPosition((tgui::bindWidth(panel) / 2.0f) - (tgui::bindWidth(header) / 2.0f), 0.0f);
		panel->add(header);
		
		auto btnExit = GUIFactory::ButtonMedium("Exit to Main Menu");
		btnExit->connect("pressed", ReturnToMainMenuPressed, pGame);
		btnExit->setSize(tgui::Layout2d(GUIUtil::bindWidthLimit(panel, 200.0f), 24.0f));
		btnExit->setPosition(panelSize.x / 2.0f - (tgui::bindWidth(btnExit)/2.0f), tgui::bindHeight(header) + 24.0f);
		panel->add(btnExit);

		pGame->m_WindowManager.SetGUI(&m_GUI);
		pGame->m_WindowManager.SetGUIActive(false);
		//

		CBaseNPC* npc = new CBaseNPC();
		npc->Initialize(&m_Physics, ASSET_NPC_TEXTURE, ASSET_PLAYER_HAND, 10, 25, 64, 32, PLAYER_JUMPSPEED, PLAYER_MOVESPEED, m_Map.GetSpawn(pGame));
		m_EntMgr.AddNPC(npc);

		//m_LuaLink.PrintSDK();
	}

	/* Reset elapsed time */
	pGame->m_Time.ResetFrameTime();

	/* Reset map data */
	m_Map.ResetState();	
	m_Map.ResetSpawns(); 

	/* Player pos */
	m_Player.SetPosition(m_Map.GetSpawn(pGame));

	m_gameSettings.Init(GRAVITY, TERMINAL);
}

void C6SDevState::Cleanup(CGame* pGame){
//	m_LuaLink.DestroyLua();
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

	pGame->m_Drawing.DrawRectangleCentred(&pGame->m_WindowManager, sf::Vector2f(20, 20), sf::Vector2f(5, 5), sf::Color::Red);
	pGame->m_Drawing.DrawRectangleCentred(&pGame->m_WindowManager, sf::Vector2f(200, 20), sf::Vector2f(50, 50), sf::Color::Green);

	/* Debug draw if enabled */
	m_Physics.Draw();
	
	/* Restore camera */
	pWnd->RestoreDefaultCamera();

	if (pWnd->IsGUIActive())
		m_GUI.draw();
}

void C6SDevState::Update(CGame* pGame, float fFrameTime) {
	m_Player.Update(fFrameTime, pGame, &m_ProjectileManager, &m_Physics);

	m_ProjectileManager.UpdateProjectiles(fFrameTime);
	if (m_ProjectileManager.HasCollisionEvents()) {
		CProjectileManager::SProjectileEvent e;
		while (m_ProjectileManager.NextProjectileEvent(e)) {
			printf("Projectile hit client %i for %i damage\n", e.iPlayerID, e.iProjectileDamage);
		}
	}

	m_EntMgr.Update(fFrameTime, pGame, &m_Physics);
	m_Gore.Update(pGame, &m_Physics, fFrameTime);
	m_Physics.Step(fFrameTime);
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
			sf::Vector2i vMouseScreen = pGame->m_Input.GetMousePosition();
			sf::Vector2i vMouseWorld = pGame->m_WindowManager.ScreenToWorld(vMouseScreen);
			m_Gore.CreateBloodSplatter(&m_Physics, vMouseWorld, 0.2f, 4, sf::Vector2f(pGame->m_Random.RandomFloat(-500.0f, 500.0f), pGame->m_Random.RandomFloat(-500.0f, 500.0f)));
			m_Gore.CreateBloodSplatter(&m_Physics, vMouseWorld, 0.2f, 4, sf::Vector2f(pGame->m_Random.RandomFloat(-500.0f, 500.0f), pGame->m_Random.RandomFloat(-500.0f, 500.0f)));
			m_Gore.CreateBloodSplatter(&m_Physics, vMouseWorld, 0.2f, 4, sf::Vector2f(pGame->m_Random.RandomFloat(-500.0f, 500.0f), pGame->m_Random.RandomFloat(-500.0f, 500.0f)));
			m_Gore.CreateBloodSplatter(&m_Physics, vMouseWorld, 0.2f, 4, sf::Vector2f(pGame->m_Random.RandomFloat(-500.0f, 500.0f), pGame->m_Random.RandomFloat(-500.0f, 500.0f)));
			m_Gore.CreateBloodSplatter(&m_Physics, vMouseWorld, 0.2f, 4, sf::Vector2f(pGame->m_Random.RandomFloat(-500.0f, 500.0f), pGame->m_Random.RandomFloat(-500.0f, 500.0f)));
			m_Gore.CreateBloodSplatter(&m_Physics, vMouseWorld, 0.2f, 4, sf::Vector2f(pGame->m_Random.RandomFloat(-500.0f, 500.0f), pGame->m_Random.RandomFloat(-500.0f, 500.0f)));
			m_Gore.CreateBloodSplatter(&m_Physics, vMouseWorld, 0.2f, 4, sf::Vector2f(pGame->m_Random.RandomFloat(-500.0f, 500.0f), pGame->m_Random.RandomFloat(-500.0f, 500.0f)));
		}bHeldKey = true;
	}
	else {
		bHeldKey = false;
	}

	sf::Event::KeyEvent keyEvent;
	while (pGame->m_Input.GetNextKeyEvent(keyEvent)) {
		if (keyEvent.code == sf::Keyboard::Escape) {
			pGame->m_WindowManager.ToggleGUI();
			printf("Toggle");
		}
	}

	pWnd->RestoreDefaultCamera();
}

void C6SDevState::PauseState(){

}
void C6SDevState::ResumeState(){

}