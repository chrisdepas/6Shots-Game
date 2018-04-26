#ifndef __C6SSINGLEPLAYERMINIGAMESTATE_H__
#define __C6SSINGLEPLAYERMINIGAMESTATE_H__

/*
Christopher De Pasquale
Early 2016/Late 2015
Noticed added feb 2017
*/

#include "CGameState.h" 
#include "C6SMap.h"
#include "C6SPlayer.h"
#include "CGlobal.h"
#include "CProjectileManager.h"
#include "CEntityManager.h"
#include "CWorldPhysics.h"
#include "CGoreEngine.h"
#include <Box2d/Box2D.h>
#include "CLuaLink.h"

/* Testing stage */
class C6SSingleplayerMinigameState : public CGameState {
	CProjectileManager m_ProjectileManager;
	C6SMap m_Map;
	C6SPlayer m_Player;
	CGlobal m_gameSettings;
	CEntityManager m_EntMgr;
	CWorldPhysics m_Physics;
	CGoreEngine m_Gore;

	// lua 
	CLuaLink m_LuaLink;

public:

	// Load LUA Gamemode
	void C6SSingleplayerMinigameState::Init(CGame* pGame) { printf("C6SSingleplayerMinigameState default init called.\n"); }
	void C6SSingleplayerMinigameState::Init(CGame* pGame, char* szMinigamePath);
	void C6SSingleplayerMinigameState::Cleanup(CGame* pGame);

	void C6SSingleplayerMinigameState::Draw(CGame* pGame);
	void C6SSingleplayerMinigameState::Update(CGame* pGame);
	void C6SSingleplayerMinigameState::HandleInput(CGame* pGame);

	void C6SSingleplayerMinigameState::PauseState();
	void C6SSingleplayerMinigameState::ResumeState();

	static C6SSingleplayerMinigameState* Instance()
	{
		static C6SSingleplayerMinigameState state;
		return &state;
	}
};

#endif

