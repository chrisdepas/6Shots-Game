#ifndef __C6SDEVSTATE_H__
#define __C6SDEVSTATE_H__

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
#include "CBaseNPC.h"
 
/* Testing stage */
class C6SDevState : public CGameState {
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
	
	void C6SDevState::Init(CGame* pGame);
	void C6SDevState::Cleanup(CGame* pGame);

	void C6SDevState::Draw(CGame* pGame);
	void C6SDevState::Update(CGame* pGame);
	void C6SDevState::HandleInput(CGame* pGame);

	void C6SDevState::PauseState();
	void C6SDevState::ResumeState();

	static C6SDevState* Instance() {
		static C6SDevState devState;
		return &devState;
	}
};

#endif

