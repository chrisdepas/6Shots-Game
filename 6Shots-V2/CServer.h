#ifndef __CSERVER_H__
#define __CSERVER_H__

/*
	Christopher De Pasquale
	Early 2016/Late 2015
	Noticed added feb 2017

*/

#define DEFAULT_SERVER_PORT 55021
#define VERSION 1
#include <SFML\Network.hpp>
#include "CRandom.h"
#include <string> 
#include "INetworkListener.h"

#define DEFAULT_MAX_CLIENTS 4
class C6SPacket;
class CGame;

class CServer {
public:
	/* A client on the server */
	struct SClient {
		int id;// Non-secret ID
		int uuid;// Secret ID
		sf::IpAddress ip; 
		int port;
		std::string username;
		bool isReady;
		bool Send(C6SPacket& packet);
	};

private:
	int m_LastID;
	unsigned int m_iMaxClients;
	sf::UdpSocket m_ServerSocket;
	bool m_bListening;
	std::vector<SClient> m_ConnectedClients;
	std::string m_Password;
	bool m_bPasswordSet;

	/* Handler for client ticks */
	INetworkListener* m_pNetListener;

	/* Host data */
	int m_iHostID;	// Non-secret host ID 
	std::string m_sHostUsername; // Host username

	SClient* GetClientFromUUID(int uuid);

	/* Add a new client */
	void AddClient(int id, int UUID, sf::IpAddress ip, int port, std::string& username);

	/* Generate a secret ID - used to identify player */
	int GenerateUUID(CRandom* random);

	/* Generaete a non-secret ID */
	int GenerateID();
	 
	// Send to all clients
	void BroadcastPacket(CGame* pGame, C6SPacket& packet);
	/* Send to all clients except one */
	void BroadcastPacket(CGame* pGame, C6SPacket& packet, int excludeID);

	// Handler functions for packets
	void OnConnectionRequest(CGame* pGame, C6SPacket& packet, sf::IpAddress addr, unsigned short port);
	void OnClientChatMessage(CGame* pGame, C6SPacket& packet, sf::IpAddress addr, unsigned short port);
	void OnClientTick(CGame* pGame, C6SPacket& packet, sf::IpAddress addr, unsigned short port);
	void OnClientGameReady(CGame* pGame, C6SPacket& packet, sf::IpAddress addr, unsigned short port);
	void OnClientInputEvent(CGame* pGame, C6SPacket& packet, sf::IpAddress addr, unsigned short port);

	bool ShouldAcceptClient(std::string password);
	char* GetRejectReason(std::string password);

public: 
	CServer() {
		m_iHostID = 0;
		m_sHostUsername = "";
		m_LastID = 0;
		m_Password = "";
		m_pNetListener = 0;
		m_bPasswordSet = false;
		m_iMaxClients = DEFAULT_MAX_CLIENTS;
		m_bListening = false;
	}

	/* Get Non-unique ID */
	int GetLocalHostID();

	// Register/deregister listener for network events
	void SetNetworkListener(INetworkListener* pListener);
	void RemoveNetworkListener(INetworkListener* pListener);

	/* Send client tick to server */
	void SendHostTick(CGame* pGame, Vector2f vPlayerPos, Vector2f vHandPos, float fHandRotation, bool bFacingLeft, bool bWalking);

	/* Create a weapon */
	void SendWeaponSpawn(CGame* pGame, Vector2f vPos, float fRotation, int entityID, int weaponID);

	/* Weapon Pickup */
	void SendClientWeaponPickup(CGame* pGame, int iClientID, int iWeaponEntityID);
	void SendHostWeaponPickup(CGame* pGame, int iWeaponEntityID);

	/* Weapon Drop */
	void SendClientWeaponDrop(CGame* pGame, int clientID, Vector2f vPosition, Vector2f vVelocity, float fDropRotation);
	void SendHostWeaponDrop(CGame* pGame, Vector2f vPosition, Vector2f vVelocity, float fDropRotation);

	/* Weapon Shoot */
	void SendClientWeaponShoot(CGame* pGame, int clientID, Vector2f vPosition, Vector2f vVelocity, int projectileID);
	void SendHostWeaponShoot(CGame* pGame, Vector2f vPosition, Vector2f vVelocity, int projectileID);

	/* Weapon Dry Fire */
	void SendClientWeaponDryFire(CGame* pGame, int clientID);
	void SendHostWeaponDryFire(CGame* pGame);

	/* Player Damage */
	void SendClientDamage(CGame* pGame, int clientID, Vector2f vPosition, Vector2f vVelocity, int iDamage);
	void SendHostDamage(CGame* pGame, Vector2f vPosition, Vector2f vVelocity, int iDamage);

	/* Player death */
	void SendClientDeath(CGame* pGame, int clientID, Vector2f vPosition, Vector2f vVelocity);
	void SendHostDeath(CGame* pGame, Vector2f vPosition, Vector2f vVelocity);

	/* Player heal */
	void SendClientSetHitpoints(CGame* pGame, int clientID, int hp);
	void SendHostSetHitpoints(CGame* pGame, int hp);

	/* Launch the game */
	void SendGameLaunch(CGame* pGame);

	/* Start round */
	void SendRoundStart(CGame* pGame);

	/* Reset game round */
	void SendRoundReset(CGame* pGame);

	/* Send a chat message to connected clients */
	void SendChatMessage(std::string& message, CGame* pGame);

	/* Send a broadcast message to clients */
	void SendBroadcastMessage(CGame* pGame, std::string& message, float fTime);

	/* Start listening and accepting clients */
	bool Start(std::string username);
	
	/* Shutdown server & stop listening */
	void Stop();

	/* Get Username */
	std::string& GetHostUsername() { return m_sHostUsername; }

	bool IsListening(); 
	void Update(CGame* pGame);
	void SetMaxClients(int maxclients);
	void SetPassword(char* password);
	void RemovePassword();

	/* Expose clients */
	std::vector<SClient>* GetConnectedClients();

	/* Check if all clients are Game-Ready */
	bool AllClientsReady();
};
#endif