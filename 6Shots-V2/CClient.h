#ifndef __CCLIENT_H__
#define __CCLIENT_H__

/*
	Christopher De Pasquale
	Early 2016/Late 2015
	Noticed added feb 2017

*/

#define DEFAULT_SERVER_PORT 55021
#define VERSION 1
#include <SFML\Network.hpp>
#include "CRandom.h"
#include "Vector.h"
#include "INetworkListener.h"
class C6SPacket;
class CGame;

class CClient {
public:
	/* Connection to server */
	struct SServerConnection {
		int uuid;
		int id;
		sf::IpAddress ip;
		int port;
		bool valid;
		SServerConnection() {
			valid = false;
		}
		bool IsValid() {
			return valid;
		}
		void Init(int _uuid, int _id, sf::IpAddress _ip, int _port) {
			uuid = _uuid;
			id = _id; 
			ip = _ip;
			port = _port;
			valid = true;
		}
	};

	/* Player on server */
	struct SConnectedPlayer {
		std::string username;
		int id;
		SConnectedPlayer(std::string _username, int _id) {
			username = _username;
			id = _id;
		}
	};

private:
	std::vector< SConnectedPlayer > m_Players;
	SServerConnection m_Connection;
	sf::UdpSocket m_ClientSocket;
	bool m_bConnected;
	bool m_bActive;

	INetworkListener* m_pNetListener; 

	std::string m_sUsername;
	  
	/* Packet handlers */
	void OnConnectionResponse(C6SPacket& packet, sf::IpAddress addr, unsigned short port, CGame* pGame);
	void OnPlayerLobbyUpdate(C6SPacket& packet, sf::IpAddress addr, unsigned short port, CGame* pGame);
	void OnServerChatMessage(C6SPacket& packet, sf::IpAddress addr, unsigned short port, CGame* pGame);
	void OnServerLaunchGame(C6SPacket& packet, sf::IpAddress addr, unsigned short port, CGame* pGame);
	void OnServerPlayerUpdate(C6SPacket& packet, sf::IpAddress addr, unsigned short port, CGame* pGame);
	void OnServerEntityCreate(C6SPacket& packet, sf::IpAddress addr, unsigned short port, CGame* pGame);
	void OnServerRoundStart(C6SPacket& packet, sf::IpAddress addr, unsigned short port, CGame* pGame);
	void OnServerPlayerEvent(C6SPacket& packet, sf::IpAddress addr, unsigned short port, CGame* pGame);
	void OnServerBroadcastMessage(C6SPacket& packet, sf::IpAddress addr, unsigned short port, CGame* pGame);
	void OnServerRoundReset(C6SPacket& packet, sf::IpAddress addr, unsigned short port, CGame* pGame);

	/* Send a packet to server */
	void SendServerPacket(C6SPacket& packet);

	SConnectedPlayer* GetPlayerByID(int id);
public:

	CClient() { m_bConnected = m_bActive = false; m_pNetListener = 0; }
	bool IsConnected() { return m_bConnected; }
	bool IsActive() { return m_bActive; }

	/**/
	void SetNetworkListener(INetworkListener* pListener);
	void RemoveNetworkListener(INetworkListener* pListener);

	/* Send client tick to server */
	void SendTick(Vector2f vPlayerPos, Vector2f vHandPos, float fHandRotation, bool bFacingLeft, bool bWalking);

	/* Send client input event to server */
	void SendMousepressEvent();

	/*  */
	void SendRightMousepressEvent(Vector2f vThrowVelocity);

	/* Send ready packet to server */
	void SendReady();

	/* Send a message to the server */
	void SendChatMessage(std::string szMessage); 

	/* Send a concmd */
	void SendConsoleCommand(int eConsoleCmd);

	/* Connect to a server */
	bool Start(char* szConnectIP, char* username, char* password, int connectPort = DEFAULT_SERVER_PORT);

	/* Stop client networking */
	void Stop();
	 
	/* Update client connection */
	void Update(CGame* pGame);

	/* Expose player list */
	std::vector<SConnectedPlayer>* GetPlayerList();

	/* Username */
	std::string& GetUsername();

	int GetLocalClientID();
};

#endif