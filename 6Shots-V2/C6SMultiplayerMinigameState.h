#ifndef __C6SMULTIPLAYERMINIGAMESTATE_H__
#define __C6SMULTIPLAYERMINIGAMESTATE_H__

/*
	Christopher De Pasquale
	24 / 03 / 2017
	
*/
#include "CGameState.h"
#include <TGUI\TGUI.hpp>
#include "IEventListener.h"
#include "C6SMap.h"
#include "CWorldPhysics.h"
#include "C6SPlayer.h"
#include "C6SNetworkedPlayer.h"
#include "CTickRate.h"
#include "CChatList.h"
#include "INetworkListener.h"
#include "C6SBaseNetworkedWeapon.h"
#include "CEventScheduler.h"
#include "CLuaLink.h"
#include "CNetVarFactory.h"

class C6SMultiplayerMinigameState {
	tgui::Gui* m_pGui;
	C6SMap m_Map;
	C6SPlayer m_Player;
	CWorldPhysics m_Physics;
	CTickRate m_TickRate;

	/* Event scheduler interface */
	CEventScheduler m_Scheduler;

	/* Chat list */
	CChatList m_ChatList;

	/* Networked entities */
	CEntityManager m_EntMgr;

	/* Lua interface */
	CLuaLink m_Lua;

	/* Networked Players List */
	std::vector<C6SNetworkedPlayer*> m_vPlayers;

	/* Network Constants */
	const int TICKRATE = 60; // Ticks per second 

	/* In win stage */
	bool m_bInWinState = false;

	/* Local player networked ID */
	int m_iLocalPlayerID;

	/* Player consts */
	const float PLAYER_PICKUP_RANGE = 20.0f;
	const int PLAYER_HITPOINTS = 100;
	const float PLAYER_JUMPSPEED = -290.0f;
	const float PLAYER_MOVESPEED = 200.0f;
	const int PLAYER_HANDSIZE = 10;
	const int PLAYER_REACH = 25;
	const int PLAYER_HEIGHT = 64;
	const int PLAYER_WIDTH = 32;

	/* Time before round resets on Quickdraw */
	const float QUICKDRAW_ROUND_RESET_TIME = 6.0f;

	/* Physics constants */
	const bool PHYSICS_DEBUG = true;
	const float PHYSICS_GRAVITY = 12.0f;

	/* States multiplayer can be in */
	enum EMultiplayerGameState {
		MULTIGAMESTATE_WAITING_FOR_CLIENTS, /* Wait for all clients to connect */
		MULTIGAMESTATE_ROUND_WAITING,		/* Round begin countdown, weapons spawned here, unused for now */
		MULTIGAMESTATE_ROUND,				/* Round playtime */
		MULTIGAMESTATE_ROUNDWIN,			/* End of a round, winner exists */
		MULTIGAMESTATE_GAMEOVER,			/* All rounds completed */
	};

	/* Current state of multiplayer game */
	EMultiplayerGameState m_eCurState;

	/* Handlers for player damage/death */
	void HandlePlayerDamage(CGame* pGame, int playerID, Vector2f& vPosition, Vector2f& vVelocity, int iDamageAmount);
	void HandlePlayerDeath(CGame* pGame, int playerID, Vector2f& vPosition, Vector2f& vVelocity);

	/* ServerPrintMessage */
	void ServerBroadcastPrint(CGame* pGame, char* szMessage, float fTime);

	/* Link LUA and game */
	bool InitLuaInterface(CGame* pGame);

public:
	void Init(CGame* pGame);
	void Cleanup(CGame* pGame);

	void Draw(CGame* pGame);
	void Update(CGame* pGame);
	void HandleInput(CGame* pGame);

	void PauseState();
	void ResumeState();

	void OnChatMessage(IEventListener::SChatEvent* pChatEvent);
	void SendChatMessage(CGame* pGame);

	void OnEvent(CGame* pGame, std::string evtName, void* data, size_t len);

	/* Network listen callbacks */
	void OnTick(CGame* pGame, int iClientID, Vector2f vPosition, Vector2f vHandPosition, float fHandRotation, bool bLeftFacing, bool bWalking);
	void OnRemoteEntityCreate(CGame* pGame, int iEntityID, int iType, Vector2f vCreatePos, float fCreateAngle, int iInstanceID, int iClientGiveID);
	void OnRoundStart(CGame* pGame);
	void OnPlayerMousePress(CGame* pGame, int clientID);
	void OnPlayerThrow(CGame* pGame, int clientID);
	void OnPlayerPickupWeapon(CGame* pGame, int clientID, int entityID);
	void OnPlayerThrowWeapon(CGame* pGame, int clientID, Vector2f vPosition, Vector2f vVelocity, float fThrowRotation);
	void OnPlayerRightMousePress(CGame* pGame, int iClientID, Vector2f vThrowVelocity, float fThrowRotation);
	void OnPlayerShootWeapon(CGame* pGame, int clientID, Vector2f vPosition, Vector2f vVelocity, int projectileID);
	void OnPlayerDryFire(CGame* pGame, int clientID);
	void OnPlayerDamage(CGame* pGame, int iClientID, Vector2f vDamagePosition, Vector2f vDamageVelocity, int iDamageAmount);
	void OnPlayerDeath(CGame* pGame, int iClientID, Vector2f vDeathDamagePosition, Vector2f vDamageVelocity);
	void OnPlayerCommand(CGame* pGame, int iClientID, int eConCmd);
	void OnBroadcastMessage(CGame* pGame, float fTime, std::string& sMessage);
	void OnRoundReset(CGame* pGame);

	static C6SMultiplayerMinigameState* Instance()
	{
		static C6SMultiplayerMinigameState m_State;
		return &m_State;
	}
};

#endif