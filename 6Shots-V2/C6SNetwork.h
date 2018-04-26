#ifndef __C6SNETWORK_H__
#define __C6SNETWORK_H__

/*
		Christopher De Pasquale
Unknown date, notice added 12 December 2016
*/

#define C6SNETWORK_VERSION 1
#include <sfml\Network.hpp>
#include "Vector.h"
class C6SPacket;
class C6SNetwork {

	/* Network version number */
	static const int m_iNetworkVersion = C6SNETWORK_VERSION;
public:

	/* Player events */
	enum EPlayerEvent {
		// CLIENT->SERVER ONLY EVENTS (i.e. input events)
		EVENT_MOUSEPRESS, /* Press mouse (request shoot/pickup) depending on state */

		// CLIENT->SERVER and SERVER->CLIENT EVENTS 
		EVENT_THROW, /* throw item or weapon */

		// SERVER->CLIENT ONLY EVENTS 
		EVENT_PICKUP,			/* Pickup item or weapon */
		EVENT_SHOOT_PROJECTILE, /* Shoot a weapon */
		EVENT_DRYFIRE,			/* Attempt to shoot, but no ammo */
		EVENT_DAMAGE,			/* Client takes damage */
		EVENT_DEATH,			/* Client dies */
		EVENT_SET_HITPOINTS,	/* Set HP*/
		EVENT_TELEPORT,			/* Move client position */
	};

	/* Console commands */
	enum EConsoleCommands {
		COMMAND_KILL,		/* Slay self */
	};

	/* Get network version number */
	static int Version() { return m_iNetworkVersion; }

	/* Base packet class
	*/ 
	class CBasePacket {
	protected:
		bool m_bIsValid;
	public:
		bool IsValid() { return m_bIsValid; }
	};

	/* CONNECTION REQUEST PACKET
	SENT BY:	Client

	Sent to server to attempt to initiate a connection.
	contains username and optional password for server
	*/
	class CConnectionRequest : public CBasePacket {
		int m_iNetworkVersion;
		std::string m_szUsername;
		std::string m_szPassword;
	public:
		CConnectionRequest(C6SPacket& packet);
		std::string GetUsername() { return m_szUsername; }
		std::string GetPassword() { return m_szPassword; }
		static C6SPacket Create(char* username, char* password);
	};

	/* CONNECTION RESPONSE PACKET
	SENT BY:	Server

	Sent to client in response to a connection request.
	Is either a deny with a reason, or an accept packet with new user ID
	*/
	class CConnectionResponse : public CBasePacket {
		bool m_bResult;
		int m_iUUID;
		int m_iID;
		std::string m_FailReason;
	public:
		CConnectionResponse(C6SPacket& packet);
		int GetUUID() { return m_iUUID; }
		int GetID() { return m_iID; }
		std::string GetFailReason() { return m_FailReason;  }
		bool ConnectionAccepted() {	return m_bResult != 0; }
		static C6SPacket Create(bool accepted, int UUID, int ID, char* szFailReason = "Unknown");
	};

	/* CLIENT CHAT MESSAGE
	SENT BY: Client

	Packet is sent to server to indicate that a chat message has been sent
	*/
	class CClientChatMessage : public CBasePacket {
		int m_iUUID;
		std::string m_szMessage;
	public:
		CClientChatMessage(C6SPacket& packet);
		std::string GetChatMessage() { return m_szMessage; }
		int GetUUID() { return m_iUUID; }
		static C6SPacket Create(int uuid, std::string& szMessage);
	};

	/* SERVER CHAT MESSAGE
	SENT BY: Server

	Packet is sent to clients to update chat with a new message */
	class CServerChatMessage : public CBasePacket {
		int m_iClientNumber; // Non-secret UUID which each client has
		std::string m_szMessage;
	public:
		CServerChatMessage(C6SPacket& packet);
		int GetClientID() { return m_iClientNumber; }
		std::string GetMessage() { return m_szMessage; }
		static C6SPacket Create(int clientID, std::string szMessage);
	};

	/* SERVER KICK MESSAGE
	SENT BY: Server

	Packet is sent to clients to inform them that they have been kicked. */
	class CServerKickMessage : public CBasePacket {
		std::string m_szReason;
	public:
		CServerKickMessage(C6SPacket& packet);
		std::string GetReason() { return m_szReason; }
		static C6SPacket Create(char* szReason);
	};

	/* CLIENT DISCONNECT MESSAGE
	SENT BY: Client

	Packet is sent to server to inform server that client has disconnected. */
	class CClientDisconnect : public CBasePacket {
		int m_iUUID;
	public:
		CClientDisconnect(C6SPacket& packet); 
		int GetUUID() { return m_iUUID; }
		static C6SPacket Create(int UUID);
	};

	/* SERVER PLAYER LOBBY UPDATE
	SENT BY: Server

	Packet is sent to client to tell them to add/remove a player from lobby. */
	class CServerPlayerLobbyUpdate : public CBasePacket {
		int m_iClientID; // Non-secret player identifier
		bool m_bLeaving;
		std::string m_sPlayerName;
	public:
		CServerPlayerLobbyUpdate(C6SPacket& packet);
		bool NewPlayer() { return !m_bLeaving; }
		bool PlayerLeaving() { return m_bLeaving; }
		std::string GetPlayerName() { return m_sPlayerName; }
		int GetClientID() { return m_iClientID; }
		static C6SPacket Create(int m_iClientID, bool bLeaving, std::string& szPlayerName);
	};

	/* SERVER LAUNCH GAME
	SENT BY: Server

	Packet is sent to client to tell them game has been launched */
	class CServerLaunchGame : public CBasePacket {
	public:
		CServerLaunchGame(C6SPacket& packet);
		static C6SPacket Create();
	};

	/* CLIENT GAMEREADY
	SENT BY: Client

	Packet is sent to server to tell them game has been launched */
	class CClientGameReady : public CBasePacket {
		int m_iUUID;
	public:
		CClientGameReady(C6SPacket& packet);
		static C6SPacket Create(int uuid);
		int GetUUID();
	};

	/* SERVER ROUND START
	SENT BY: Server

	Packet is sent to client to tell them round has begun */
	class CServerRoundStart : public CBasePacket {
	public:
		CServerRoundStart(C6SPacket& packet);
		static C6SPacket Create();
	};

	/* SERVER ROUND RESET
	SENT BY: Server 
	
	*/
	class CServerRoundReset: public CBasePacket {
	public:
		CServerRoundReset(C6SPacket& packet);
		static C6SPacket Create();
	};

	/* CLIENT PLAYER TICK
	SENT BY: Client

	Packet is sent to server to inform server of player actions  */
	class CClientPlayerTick: public CBasePacket {
		int m_iClientUUID; // Secret player identifier, must match server 
		float m_fPositionX; // Player position 
		float m_fPositionY;
		float m_fHandPositionX; // Hand position 
		float m_fHandPositionY; 
		float m_fAimAngle; // Player aim/hand angle
		bool m_bFacingLeft; // Facing left
		bool m_bWalking;	// Walking animation 
	public:
		CClientPlayerTick(C6SPacket& packet);
		Vector2f GetPlayerPosition(); 
		Vector2f GetHandPosition();
		float GetAimAngle();
		//bool IsAttackPressed();
		int GetUUID();
		bool IsFacingLeft();
		bool IsWalking();
		static C6SPacket Create(int iClientUUID, Vector2f vPosition, Vector2f vHandPosition, float fAimAngle, bool bLeftFacing, bool bWalking);
	};

	/* SERVER PLAYER UPDATE
	SENT BY: Server

	Packet is sent by server to clients to inform them of a player state update (i.e. movement etc) */
	class CServerPlayerUpdate : public CBasePacket {
		int m_iClientID; // Non-secret player identifier
		float m_fPositionX; // Player position X
		float m_fPositionY;
		float m_fHandPositionX;
		float m_fHandPositionY;
		float m_fAimAngle; // Player aim angle
		bool m_bFacingLeft; // Facing left
		bool m_bWalking;	// Walking animation 
	public:
		CServerPlayerUpdate(C6SPacket& packet);
		Vector2f GetPosition();
		Vector2f GetHandPosition();
		float GetAimAngle();
		int GetID();
		bool IsFacingLeft();
		bool IsWalking();
		static C6SPacket Create(int iClientID, Vector2f vPosition, Vector2f vHandPosition, float fAimAngle, bool bLeftFacing, bool bWalking);
	};

	/* CLIENT PLAYER INPUT EVENT
	SENT BY: Client

	Packet is sent to server to inform server of player input action (e.g. click/throw)  */
	class CClientPlayerInputEvent : public CBasePacket {
	public:
		
	private:
		int m_iClientUUID; // Secret player identifier, must match server 
		int m_eType; // Type of player input event 

		/* Optional throw event parameters */
		Vector2f m_vThrowVelocity; 
		float m_fThrowRotation;
	public:
		CClientPlayerInputEvent(C6SPacket& packet);
		int GetUUID();
		bool IsMousePress();
		bool IsThrow();
		Vector2f GetThrowVelocity();
		float GetThrowRotation();
		static C6SPacket Create(int iClientUUID, C6SNetwork::EPlayerEvent eType, Vector2f fThrowVelocity = Vector2f(0.0f, 0.0f), float fThrowRotation = 0.0f);
	};

	/* SERVER PLAYER EVENT
	SENT BY: Server

	Packet is sent by server to clients to inform them of a player state update (i.e. movement etc) */
	class CServerPlayerEvent: public CBasePacket {
		int m_iClientID; // Non-secret player identifier
		int m_eType;
		Vector2f m_vEventVelocity; /* Shoot / throw velocity */
		Vector2f m_vEventPosition; /* Shoot / throw position */
		int m_iDamage;
		int m_iPickupID; 
		float m_fThrowRotation;
	public:
		CServerPlayerEvent(C6SPacket& packet);
		Vector2f GetPosition();
		Vector2f GetHandPosition();
		float GetThrowRotation();

		int GetDamageAmount();
		int GetProjectileID();
		int GetPickupID();
		int GetClientID();
		int GetClientHitpoints();
		bool IsPickupEvent();
		bool IsThrowEvent();
		bool IsShootEvent();
		bool IsDryFireEvent();
		bool IsDamageEvent();
		bool IsDeathEvent();
		bool IsSetHitpointsEvent();
		Vector2f GetEventVelocity();
		Vector2f GetEventPosition(); 
		static C6SPacket CreateAsDeathEvent(int iClientID, Vector2f vDamagePosition, Vector2f vDamageVelocity);
		static C6SPacket CreateAsDamageEvent(int iClientID, Vector2f vDamagePosition, Vector2f vDamageVelocity, int iDamageAmount);
		static C6SPacket CreateAsPickupEvent(int iClientID, int iPickupID);
		static C6SPacket CreateAsThrowEvent(int iClientID, Vector2f vThrowVelocity, Vector2f vThrowPosition, float fThrowRotation);
		static C6SPacket CreateAsShootEvent(int iClientID, Vector2f vShootPosition, Vector2f vShootVelocity, int iProjectileID);
		static C6SPacket CreateAsDryFireEvent(int iClientID);
		static C6SPacket CreateAsHitpointsChange(int iClientID, int iHp);
		static C6SPacket CreateAsTeleport(int iClientID, Vector2f vPosition);
	};

	/* SERVER ENTITY CREATE
	SENT BY: Server

	Packet is sent by server to clients to inform them that entity has been created */
	class CServerEntityCreate : public CBasePacket {
		int m_iEntityID;		/* ID of new entity on server */
		int m_eType;			/* Type of new entity */
		Vector2f m_vEntPosition;/* create position */
		float m_fEntAngle;		/* Create rotation */
		int m_eEntityInstanceID;/* Specific instance of entity, e.g. if eType is WEAPON, eInstanceID might be KNIFE */
		int m_iClientGiveID;	/* Non-secret ID of client that entity has been given to, if entity can be held */
	public:
		CServerEntityCreate(C6SPacket& packet);
		Vector2f GetCreatePosition();
		float GetCreateAngle();
		int GetEntityID();
		int GetEntityType();
		int GetEntityInstanceID();
		int GetClientGiveID();
		static C6SPacket Create(int iEntID, Vector2f vPosition, float fAngle, int eInstanceID, int eType = 0, int iPickupID = 0);
	};

	/* CLIENT CONSOLE COMMAND
	SENT BY: Client

	Packet is sent by client to server to request running a console cmd */
	class CClientConCmd : public CBasePacket {
		int m_iClientUUID;		/* Secret ID of player */
		int m_eType;			/* Type of new entity */
		
	public:
		CClientConCmd(C6SPacket& packet);
		int GetConCmdType();
		int GetClientUUID();
		static C6SPacket Create(int clientUUID, int eType);
	};

	/* SERVER MESSAGE
	SENT BY: Server

	A broadcast message displayed to all clients */
	class CServerBroadcastMessage : public CBasePacket {
		float m_fTime;					/* Secret ID of player */
		std::string m_sMessage;			/* Type of new entity */

	public:
		CServerBroadcastMessage(C6SPacket& packet);
		std::string& GetMessage();
		float GetBroadcastTime();
		static C6SPacket Create(float fTime, std::string sMessage);
	};

	/* SERVER CHANGE MAP
	SENT BY: Server

	Server informs clients of a map change */
	class CServerChangeMap : public CBasePacket {
		std::string m_sMapPath;			/* New map path */
		std::string m_sMapDisplayName;	/* New map display path */

	public:
		CServerChangeMap(C6SPacket& packet);
		std::string& GetFullPath();
		std::string& GetMapDisplayName();
		static C6SPacket Create(std::string sPath, std::string sDisplayName);
	};
};
#endif