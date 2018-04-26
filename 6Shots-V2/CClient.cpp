#include "stdafx.h"
#include "CClient.h"
#include "C6SNetwork.h"
#include "C6SPacket.h"
#include "CGame.h"
#include "C6SMultiplayerGameState.h"

void CClient::OnConnectionResponse(C6SPacket& packet, sf::IpAddress addr, unsigned short port, CGame* pGame) {
	if (IsConnected())
		return;
	C6SNetwork::CConnectionResponse in(packet);
	if (!in.IsValid())
		return;

	if (in.ConnectionAccepted()) {

		/* Save connection data */
		m_Connection.Init(in.GetUUID(), in.GetID(), addr, port);
		m_bConnected = true;

		printf("Connection to server accepted, UUID is %i.\n", m_Connection.uuid);
		pGame->m_EventManager.RaiseEvent(pGame, "ConnectionAccepted", 0, 0);
	} else {
		m_bConnected = false;
		printf("Connection to server refused.\n");
		pGame->m_EventManager.RaiseEvent(pGame, "ConnectionRefused", &in.GetFailReason(), 0);
	}
	
}
void CClient::OnPlayerLobbyUpdate(C6SPacket& packet, sf::IpAddress addr, unsigned short port, CGame* pGame) {
	if (!IsConnected())
		return;
	C6SNetwork::CServerPlayerLobbyUpdate in(packet);
	if (!in.IsValid())
		return;
	 
	int id = in.GetClientID();
	if (in.PlayerLeaving()) {
		/* Remove player from players list */
		std::vector< SConnectedPlayer >::iterator it;
		for (it = m_Players.begin(); it != m_Players.end();) {
			if (it->id == id) {
				m_Players.erase(it);
				return;
			} 
		}
	} else {
		std::string playerName = in.GetPlayerName();
		/* add player to list */
		m_Players.push_back(SConnectedPlayer(playerName, id));
		/* Raise accept event */ 
		pGame->m_EventManager.RaiseEvent(pGame, "NewPlayer", &playerName, 0);
	}

}
void CClient::OnServerChatMessage(C6SPacket& packet, sf::IpAddress addr, unsigned short port, CGame* pGame) {
	if (!IsConnected())
		return;
	C6SNetwork::CServerChatMessage in(packet);
	if (!in.IsValid())
		return;

	int id = in.GetClientID();

	/* Message data */
	std::string message = in.GetMessage();
	std::string user = "ME";

	IEventListener::SChatEvent chatEvent;
	SConnectedPlayer* pPlayer = GetPlayerByID(in.GetClientID());
	if (pPlayer) {
		/* Another user send chat */
		user = pPlayer->username;
		chatEvent.user = &user;
		chatEvent.message = &message;
		/* Display chat */
		pGame->m_EventManager.RaiseEvent(pGame, "ChatMessage", &chatEvent, 0);
	}
	else if (in.GetClientID() == m_Connection.id) {
		/* Local user sent chat */
		chatEvent.user = &user;
		chatEvent.message = &message;
		/* Display chat */ 
		pGame->m_EventManager.RaiseEvent(pGame, "ChatMessage", &chatEvent, 0);
	}
	else {
		/* Unknown ID recieved */
		printf("Recieved chat message from unknown user (ID=%i)\n", in.GetClientID());
		std::string unconnected = "Unconnected";
		chatEvent.user = &unconnected;
		chatEvent.message = &message;
		/* Display chat */
		pGame->m_EventManager.RaiseEvent(pGame, "ChatMessage", &chatEvent, 0);
		return;
	}

	
}

void CClient::OnServerLaunchGame(C6SPacket& packet, sf::IpAddress addr, unsigned short port, CGame* pGame) {
	if (!IsConnected())
		return;
	C6SNetwork::CServerLaunchGame in(packet);
	if (!in.IsValid())
		return;

	/* Raise game launch event */
	pGame->m_EventManager.RaiseEvent(pGame, "ServerLaunchGame", 0, 0);
}

void CClient::OnServerPlayerUpdate(C6SPacket& packet, sf::IpAddress addr, unsigned short port, CGame* pGame) {
	if (!IsConnected())
		return;
	C6SNetwork::CServerPlayerUpdate in(packet);
	if (!in.IsValid())
		return;
	 
	int id = in.GetID();

	/* Run tick handler */
	SConnectedPlayer* pPlayer = GetPlayerByID(id);
	if (pPlayer) {
		if (m_pNetListener)
			m_pNetListener->OnTick(pGame, id, in.GetPosition(), in.GetHandPosition(), in.GetAimAngle(), in.IsFacingLeft(), in.IsWalking());
	}
}

void CClient::OnServerEntityCreate(C6SPacket& packet, sf::IpAddress addr, unsigned short port, CGame* pGame) {
	if (!IsConnected())
		return;

	C6SNetwork::CServerEntityCreate in(packet);
	if (!in.IsValid())
		return;

	/* Run entityCreate handler */
	int id = in.GetEntityID();
	if (m_pNetListener)
		m_pNetListener->OnRemoteEntityCreate(pGame, id, in.GetEntityType(), in.GetCreatePosition(), 
		in.GetCreateAngle(), in.GetEntityInstanceID(), in.GetClientGiveID());
}
void CClient::OnServerRoundStart(C6SPacket& packet, sf::IpAddress addr, unsigned short port, CGame* pGame) {
	if (!IsConnected())
		return;

	C6SNetwork::CServerRoundStart in(packet);
	if (!in.IsValid())
		return;

	/* Run handler */
	if (m_pNetListener)
		m_pNetListener->OnRoundStart(pGame);
}
void CClient::OnServerPlayerEvent(C6SPacket& packet, sf::IpAddress addr, unsigned short port, CGame* pGame) {
	if (!IsConnected())
		return;

	C6SNetwork::CServerPlayerEvent in(packet);
	if (!in.IsValid())
		return;
	 
	if (in.IsPickupEvent()) {
		/* Run weapon pickup handler */
		int entid = in.GetPickupID();
		int cliid = in.GetClientID();
		 
		if (m_pNetListener)
			m_pNetListener->OnPlayerPickupWeapon(pGame, cliid, entid);
	} else if (in.IsThrowEvent()) { 
		/* Run throw weapon handler */ 
		if (m_pNetListener)
			m_pNetListener->OnPlayerThrowWeapon(pGame, in.GetClientID(), in.GetEventPosition(), in.GetEventVelocity(), in.GetThrowRotation()); 
	} else if (in.IsShootEvent()) {
		/* Run shoot weapon handler */
		if (m_pNetListener)
			m_pNetListener->OnPlayerShootWeapon(pGame, in.GetClientID(), in.GetEventPosition(), in.GetEventVelocity(), in.GetProjectileID());
	} else if (in.IsDryFireEvent()) {
		if (m_pNetListener)
			m_pNetListener->OnPlayerDryFire(pGame, in.GetClientID());
	} else if (in.IsDamageEvent()) {
		if (m_pNetListener)
			m_pNetListener->OnPlayerDamage(pGame, in.GetClientID(), in.GetEventPosition(), in.GetEventVelocity(), in.GetDamageAmount());
	} if (in.IsDeathEvent()) {
		if (m_pNetListener)
			m_pNetListener->OnPlayerDeath(pGame, in.GetClientID(), in.GetEventPosition(), in.GetEventVelocity());
	}
} 

void CClient::OnServerBroadcastMessage(C6SPacket& packet, sf::IpAddress addr, unsigned short port, CGame* pGame) {
	if (!IsConnected())
		return;

	C6SNetwork::CServerBroadcastMessage in(packet);
	if (!in.IsValid())
		return;

	/* Run handler */
	if (m_pNetListener)
		m_pNetListener->OnBroadcastMessage(pGame, in.GetBroadcastTime(), in.GetMessage());
}

void CClient::OnServerRoundReset(C6SPacket& packet, sf::IpAddress addr, unsigned short port, CGame* pGame) {
	if (!IsConnected())
		return;

	C6SNetwork::CServerRoundReset in(packet);
	if (!in.IsValid())
		return;

	/* Run handler */
	if (m_pNetListener)
		m_pNetListener->OnRoundReset(pGame); 
}

void CClient::SendServerPacket(C6SPacket& packet) {
	if (!IsConnected())
		return;
	m_ClientSocket.send(packet, sf::IpAddress(m_Connection.ip), m_Connection.port);
}

CClient::SConnectedPlayer* CClient::GetPlayerByID(int id) {
	for (unsigned int i = 0; i < m_Players.size(); i++) {
		if (m_Players[i].id == id)
			return &m_Players[i];
	}
	return NULL;
}

void CClient::SetNetworkListener(INetworkListener* pListener) {
	m_pNetListener = pListener;
}
void CClient::RemoveNetworkListener(INetworkListener* pListener) {
	SetNetworkListener(0);
}

void CClient::SendTick(Vector2f vPlayerPos, Vector2f vHandPos, float fHandRotation, bool bFacingLeft, bool bWalking) {
	if (!IsConnected())
		return;

	C6SPacket packet = C6SNetwork::CClientPlayerTick::Create(m_Connection.uuid, vPlayerPos, vHandPos, fHandRotation, bFacingLeft, bWalking);
	SendServerPacket(packet);
}

void CClient::SendMousepressEvent() {
	if (!IsConnected())
		return;
	C6SPacket p = C6SNetwork::CClientPlayerInputEvent::Create(m_Connection.uuid, C6SNetwork::EVENT_MOUSEPRESS);
	SendServerPacket(p);
}

void CClient::SendRightMousepressEvent(Vector2f vThrowVelocity) {
	if (!IsConnected())
		return;
	C6SPacket p = C6SNetwork::CClientPlayerInputEvent::Create(m_Connection.uuid, C6SNetwork::EVENT_THROW, vThrowVelocity);
	SendServerPacket(p);
}

void CClient::SendReady() {
	if (!IsConnected())
		return;

	C6SPacket packet = C6SNetwork::CClientGameReady::Create(m_Connection.uuid);
	SendServerPacket(packet);
}

void CClient::SendChatMessage(std::string szMessage) { 
	if (!IsConnected())
		return;

	C6SPacket msg = C6SNetwork::CClientChatMessage::Create(m_Connection.uuid, szMessage);
	SendServerPacket(msg);
	 
	return;
}

void CClient::SendConsoleCommand(int eCmd) {
	if (!IsConnected())
		return;

	C6SPacket cmd = C6SNetwork::CClientConCmd::Create(m_Connection.uuid, eCmd);
	SendServerPacket(cmd);

	return;
}
bool CClient::Start(char* szConnectIP, char* username, char* password, int connectPort) {
	if (m_bConnected) 
		return true;

	if (!m_bActive) {
		sf::Socket::Status e = m_ClientSocket.bind(sf::Socket::AnyPort);
		if (e != sf::Socket::Done)
		{
			printf("ERROR: Unable to bind socket");
			return false;
		}
	}

	m_bActive = true;

	/* Send a connection request */
	C6SPacket req = C6SNetwork::CConnectionRequest::Create(username, password);
	m_ClientSocket.setBlocking(false);
	m_ClientSocket.send(req, sf::IpAddress(szConnectIP), connectPort);

	/* Save username */
	m_sUsername = username;

	return true;
}

void CClient::Update(CGame* pGame) {
	if (!IsActive())
		return;

	C6SPacket packet;
	sf::IpAddress addr;
	unsigned short port;
	int dataLen;

	/* Get first packet */
	sf::Socket::Status status = m_ClientSocket.receive(packet, addr, port);

	while (status == sf::Socket::Done) {
		/* Data size */
		dataLen = packet.getDataSize();

		/* Find & run handler */
		switch (packet.GetID()) {
		case C6SPacket::ID_SERVER_CONNECTION_RESPONSE:
			OnConnectionResponse(packet, addr, port, pGame);
			break;
		case C6SPacket::ID_SERVER_PLAYER_LOBBY_UPDATE:
			OnPlayerLobbyUpdate(packet, addr, port, pGame);
			break;
		case C6SPacket::ID_SERVER_CHAT_MESSAGE:
			OnServerChatMessage(packet, addr, port, pGame);
			break;
		case C6SPacket::ID_SERVER_LAUNCH_GAME:
			OnServerLaunchGame(packet, addr, port, pGame);
			break;
		case C6SPacket::ID_SERVER_PLAYER_UPDATE:
			OnServerPlayerUpdate(packet, addr, port, pGame);
			break;
		case C6SPacket::ID_SERVER_ENTITY_CREATE:
			OnServerEntityCreate(packet, addr, port, pGame);
			break;
		case C6SPacket::ID_SERVER_ROUND_START:
			OnServerRoundStart(packet, addr, port, pGame);
			break;
		case C6SPacket::ID_SERVER_PLAYER_EVENT:
			OnServerPlayerEvent(packet, addr, port, pGame);
			break;
		case C6SPacket::ID_SERVER_MESSAGE:
			OnServerBroadcastMessage(packet, addr, port, pGame);
			break;
		case C6SPacket::ID_SERVER_ROUND_RESTART:
			OnServerRoundReset(packet, addr, port, pGame);
			break;
		default:
			break;
		}

		/* Get next packet */
		status = m_ClientSocket.receive(packet, addr, port);
	}

}

std::vector<CClient::SConnectedPlayer>* CClient::GetPlayerList() {
	return &(m_Players);
}

std::string& CClient::GetUsername() {
	return m_sUsername;
}

int CClient::GetLocalClientID() {
	return m_Connection.id;
}