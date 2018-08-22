#ifndef __C6SMULTIPLAYERGAMESTATE_H__
#define __C6SMULTIPLAYERGAMESTATE_H__

#include "CGameState.h"
#include "IEventListener.h"
#include "C6SMap.h"
#include "CWorldPhysics.h"
#include "C6SPlayer.h"
#include "C6SNetworkedPlayer.h"
#include "CTickRate.h"
#include "CChatList.h"
#include "INetworkListener.h"
#include "C6SBaseNetworkedWeapon.h"
#include "CGoreEngine.h"
#include "CEventScheduler.h"

/*	Multiplayer Game 
 *  Versus mode state 
 */ 
class C6SMultiplayerGameState : public CGameState, public IEventListener, public INetworkListener {
	tgui::Gui* m_pGui;
	C6SReleaseMap m_Map;
	C6SPlayer m_Player;
	CWorldPhysics m_Physics; 
	CProjectileManager m_Projectiles;
	CGoreEngine m_Gore;
	CTickRate m_TickRate;
	CChatList m_ChatDisplay;
	CEventScheduler m_Scheduler;

	/* Network Constants */
	const int TICKRATE = 60; // Ticks per second 

	/* Networked entities */
	CEntityManager m_EntMgr;

	/* Networked Players */
	std::vector<C6SNetworkedPlayer*> m_vPlayers;

	/* Post-round win stage */
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
	void HandlePlayerDamage(CGame* pGame, int playerID, sf::Vector2f& vPosition, sf::Vector2f& vVelocity, int iDamageAmount);
	void HandlePlayerDeath(CGame* pGame, int playerID, sf::Vector2f& vPosition, sf::Vector2f& vVelocity);

	/* Console command handler */
	void HandleConsoleCommand(CGame* pGame, int playerID, int eCmd);

	/* Spawn weapons */
	void SpawnPlayerRevolvers(CGame* pGame);

	/* ServerPrintMessage */
	void ServerBroadcastPrint(CGame* pGame, char* szMessage, float fTime);

	/* Reset gamemode */
	void ServerResetGamemode(CGame* pGame);

public:
	void C6SMultiplayerGameState::Init(CGame* pGame);
	void C6SMultiplayerGameState::Cleanup(CGame* pGame);

	void C6SMultiplayerGameState::Draw(CGame* pGame);
	void C6SMultiplayerGameState::Update(CGame* pGame, float fFrameTime);
	void C6SMultiplayerGameState::HandleInput(CGame* pGame);

	void C6SMultiplayerGameState::PauseState();
	void C6SMultiplayerGameState::ResumeState();

	void C6SMultiplayerGameState::OnChatMessage(IEventListener::SChatEvent* pChatEvent);
	void C6SMultiplayerGameState::SendChatMessage(CGame* pGame);

	void C6SMultiplayerGameState::OnEvent(CGame* pGame, std::string evtName, void* data, size_t len);

	/* Network listen callbacks */
	void C6SMultiplayerGameState::OnTick(CGame* pGame, int iClientID, sf::Vector2f vPosition, sf::Vector2f vHandPosition, float fHandRotation, bool bLeftFacing, bool bWalking);
	void C6SMultiplayerGameState::OnRemoteEntityCreate(CGame* pGame, int iEntityID, int iType, sf::Vector2f vCreatePos, float fCreateAngle, int iInstanceID, int iClientGiveID);
	void C6SMultiplayerGameState::OnRoundStart(CGame* pGame);
	void C6SMultiplayerGameState::OnPlayerMousePress(CGame* pGame, int clientID);
	void C6SMultiplayerGameState::OnPlayerThrow(CGame* pGame, int clientID);
	void C6SMultiplayerGameState::OnPlayerPickupWeapon(CGame* pGame, int clientID, int entityID);
	void C6SMultiplayerGameState::OnPlayerThrowWeapon(CGame* pGame, int clientID, sf::Vector2f vPosition, sf::Vector2f vVelocity, float fThrowRotation);
	void C6SMultiplayerGameState::OnPlayerRightMousePress(CGame* pGame, int iClientID, sf::Vector2f vThrowVelocity, float fThrowRotation);
	void C6SMultiplayerGameState::OnPlayerShootWeapon(CGame* pGame, int clientID, sf::Vector2f vPosition, sf::Vector2f vVelocity, int projectileID);
	void C6SMultiplayerGameState::OnPlayerDryFire(CGame* pGame, int clientID);
	void C6SMultiplayerGameState::OnPlayerDamage(CGame* pGame, int iClientID, sf::Vector2f vDamagePosition, sf::Vector2f vDamageVelocity, int iDamageAmount);
	void C6SMultiplayerGameState::OnPlayerDeath(CGame* pGame, int iClientID, sf::Vector2f vDeathDamagePosition, sf::Vector2f vDamageVelocity);
	void C6SMultiplayerGameState::OnPlayerCommand(CGame* pGame, int iClientID, int eConCmd);
	void C6SMultiplayerGameState::OnBroadcastMessage(CGame* pGame, float fTime, std::string& sMessage);
	void C6SMultiplayerGameState::OnRoundReset(CGame* pGame);

	static C6SMultiplayerGameState* Instance()
	{
		static C6SMultiplayerGameState m_State;
		return &m_State;
	}
};

#endif