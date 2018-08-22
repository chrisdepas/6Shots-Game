#include "stdafx.h"
#include "CServer.h"
#include "C6SPacket.h"
#include "CRandom.h"
#include "CGame.h"
#include "C6SNetwork.h"

bool CServer::SClient::Send(C6SPacket& packet) {
	assert(0 && "implement me");
	return false;
}  

void CServer::SendChatMessage(std::string& message, CGame* pGame) {
	IEventListener::SChatEvent chatEvent;
	chatEvent.user = &std::string("ME");
	chatEvent.message = &message; 
	/* Display chat */
	pGame->m_EventManager.RaiseEvent(pGame, "ChatMessage", &chatEvent, 0); 

	/* Broadcast chat message to all connected clients */
	C6SPacket response = C6SNetwork::CServerChatMessage::Create(m_iHostID, message);
	BroadcastPacket(pGame, response);
}
void CServer::SendBroadcastMessage(CGame* pGame, std::string& message, float fTime) {
	C6SPacket msg = C6SNetwork::CServerBroadcastMessage::Create(fTime, message);
	BroadcastPacket(pGame, msg);
}

bool CServer::Start(std::string username) {
	if (m_bListening)
		return true;
	 
	sf::Socket::Status e = m_ServerSocket.bind(DEFAULT_SERVER_PORT);
	if (e != sf::Socket::Done)
	{
		printf("Error - Unable to bind Socket to port %i\n", DEFAULT_SERVER_PORT);
		return false;
	}
	m_ServerSocket.setBlocking(false);

	// Init host client data
	m_iHostID = GenerateID();
	m_sHostUsername = username;

	m_bListening = true;
	return m_bListening;
}
void CServer::Stop() {
	m_ServerSocket.unbind();
	m_bListening = false;
} 
bool CServer::IsListening() {
	return m_bListening;
}

void CServer::SetNetworkListener(INetworkListener* pListener) {
	m_pNetListener = pListener;
}
void CServer::RemoveNetworkListener(INetworkListener* pListener) {
	SetNetworkListener(0);
}

void CServer::SendHostTick(CGame* pGame, sf::Vector2f vPlayerPos, sf::Vector2f vHandPos, float fHandRotation, bool bFacingLeft, bool bWalking) {
	C6SPacket tick = C6SNetwork::CServerPlayerUpdate::Create(this->m_iHostID, vPlayerPos, vHandPos, fHandRotation, bFacingLeft, bWalking);
	BroadcastPacket(pGame, tick);
} 

void CServer::SendWeaponSpawn(CGame* pGame, sf::Vector2f vPos, float fRotation, int entityID, int weaponID) {
	C6SPacket spwn = C6SNetwork::CServerEntityCreate::Create(entityID, vPos, fRotation, weaponID);
	BroadcastPacket(pGame, spwn);
}

void CServer::SendClientWeaponPickup(CGame* pGame, int clientID, int iWeaponEntityID) {
	C6SPacket spwn = C6SNetwork::CServerPlayerEvent::CreateAsPickupEvent(clientID, iWeaponEntityID);
	BroadcastPacket(pGame, spwn);
}
void CServer::SendHostWeaponPickup(CGame* pGame, int iWeaponEntityID) {
	SendClientWeaponPickup(pGame, this->m_iHostID, iWeaponEntityID);
}

void CServer::SendClientWeaponDrop(CGame* pGame, int clientID, sf::Vector2f vPosition, sf::Vector2f vVelocity, float fDropRotation) {
	C6SPacket drop = C6SNetwork::CServerPlayerEvent::CreateAsThrowEvent(clientID, vVelocity, vPosition, fDropRotation);
	BroadcastPacket(pGame, drop);
}
void CServer::SendHostWeaponDrop(CGame* pGame, sf::Vector2f vPosition, sf::Vector2f vVelocity, float fDropRotation) {
	SendClientWeaponDrop(pGame, this->m_iHostID, vPosition, vVelocity, fDropRotation);
}

void CServer::SendClientWeaponShoot(CGame* pGame, int clientID, sf::Vector2f vPosition, sf::Vector2f vVelocity, int projectileID) {
	C6SPacket shoot = C6SNetwork::CServerPlayerEvent::CreateAsShootEvent(clientID, vPosition, vVelocity, projectileID);
	BroadcastPacket(pGame, shoot); 
}
void CServer::SendHostWeaponShoot(CGame* pGame, sf::Vector2f vPosition, sf::Vector2f vVelocity, int projectileID) {
	SendClientWeaponShoot(pGame, this->m_iHostID, vPosition, vVelocity, projectileID);
}

void CServer::SendClientWeaponDryFire(CGame* pGame, int clientID) {
	C6SPacket dry = C6SNetwork::CServerPlayerEvent::CreateAsDryFireEvent(clientID);
	BroadcastPacket(pGame, dry);
}

void CServer::SendHostWeaponDryFire(CGame* pGame) {
	SendClientWeaponDryFire(pGame, this->m_iHostID);
}
 
void CServer::SendClientDamage(CGame* pGame, int clientID,  sf::Vector2f vPosition, sf::Vector2f vVelocity, int iDamage) {
	C6SPacket damage = C6SNetwork::CServerPlayerEvent::CreateAsDamageEvent(clientID, vPosition, vVelocity, iDamage);
	BroadcastPacket(pGame, damage);
}
void CServer::SendHostDamage(CGame* pGame, sf::Vector2f vPosition, sf::Vector2f vVelocity, int iDamage) {
	SendClientDamage(pGame, this->m_iHostID, vPosition, vVelocity, iDamage);
}

void CServer::SendClientDeath(CGame* pGame, int clientID, sf::Vector2f vPosition, sf::Vector2f vVelocity) {
	C6SPacket death = C6SNetwork::CServerPlayerEvent::CreateAsDeathEvent(clientID, vPosition, vVelocity);
	BroadcastPacket(pGame, death);
}
void CServer::SendHostDeath(CGame* pGame, sf::Vector2f vPosition, sf::Vector2f vVelocity) {
	SendClientDeath(pGame, this->m_iHostID, vPosition, vVelocity);
}

void CServer::SendClientSetHitpoints(CGame* pGame, int clientID, int iHp) {
	C6SPacket hpPacket = C6SNetwork::CServerPlayerEvent::CreateAsHitpointsChange(clientID, iHp);
	BroadcastPacket(pGame, hpPacket);
}

void CServer::SendHostSetHitpoints(CGame* pGame, int iHp) {
	SendClientSetHitpoints(pGame, this->m_iHostID, iHp);
}

void CServer::SendGameLaunch(CGame* pGame) {
	/* Tell clients game is starting */
	C6SPacket packetLaunchGame = C6SNetwork::CServerLaunchGame::Create();
	BroadcastPacket(pGame, packetLaunchGame);
}

void CServer::SendRoundStart(CGame* pGame) {
	/* Tell clients round is starting */
	C6SPacket packetRound = C6SNetwork::CServerRoundStart::Create();
	BroadcastPacket(pGame, packetRound);
}

void CServer::SendRoundReset(CGame* pGame) {
	/* Tell clients round is resetting */
	C6SPacket packetRound = C6SNetwork::CServerRoundReset::Create();
	BroadcastPacket(pGame, packetRound);
}

void CServer::OnConnectionRequest(CGame* pGame, C6SPacket& packet, sf::IpAddress addr, unsigned short port) {
	C6SNetwork::CConnectionRequest in(packet);
	if (!in.IsValid())
		return;

	if (ShouldAcceptClient(in.GetPassword())) {
		/* Generate Unique ID and UUID */
		int uuid = GenerateUUID(&pGame->m_Random);
		int id = GenerateID();
		std::string username = in.GetUsername();
		 
		/* Send accept response */
		C6SPacket response = C6SNetwork::CConnectionResponse::Create(true, uuid, id);
		m_ServerSocket.send(response, addr, port);
	
		/* Send lobby status message */ 
		C6SPacket newPlayerUpdate = C6SNetwork::CServerPlayerLobbyUpdate::Create(id, false, username);
		BroadcastPacket(pGame, newPlayerUpdate);

		/* Send full lobby update to new player */
		for (unsigned int i = 0; i < m_ConnectedClients.size(); i++) {
			C6SPacket player = C6SNetwork::CServerPlayerLobbyUpdate::Create(m_ConnectedClients[i].id, false, m_ConnectedClients[i].username);
			m_ServerSocket.send(player, addr, port);
		}
		/* Send host player to client */
		C6SPacket host = C6SNetwork::CServerPlayerLobbyUpdate::Create(m_iHostID, false, m_sHostUsername);
		m_ServerSocket.send(host, addr, port);

		/* Add new player */
		AddClient(id, uuid, addr, port, username);

		/* Raise accept event */
		pGame->m_EventManager.RaiseEvent(pGame, "NewPlayer", &in.GetUsername(), 0);
	}
	else {
		/* Send reject response */
		C6SPacket response = C6SNetwork::CConnectionResponse::Create(false, 0, 0, GetRejectReason(in.GetPassword()));
		m_ServerSocket.send(response, addr, port);
		/* Raise reject event */
		pGame->m_EventManager.RaiseEvent(pGame, "NewPlayerRejected", &in.GetUsername(), 0);
	}
}

void CServer::OnClientChatMessage(CGame* pGame, C6SPacket& packet, sf::IpAddress addr, unsigned short port) {
	C6SNetwork::CClientChatMessage in(packet);
	if (!in.IsValid())
		return;

	int uuid = in.GetUUID();
	SClient* pClient = GetClientFromUUID(uuid);
	if (!pClient)
		return;

	std::string message = in.GetChatMessage();

	/* Create chat event */
	IEventListener::SChatEvent chatEvent;
	chatEvent.user = &pClient->username;
	chatEvent.message = &message; 

	/* Display chat */
	pGame->m_EventManager.RaiseEvent(pGame, "ChatMessage", &chatEvent, 0);

	/* Broadcast chat message to all connected clients (including sender) */
	C6SPacket response = C6SNetwork::CServerChatMessage::Create(pClient->id, in.GetChatMessage());
	BroadcastPacket(pGame, response);
}

void CServer::OnClientTick(CGame* pGame, C6SPacket& packet, sf::IpAddress addr, unsigned short port) {
	C6SNetwork::CClientPlayerTick in(packet);
	if (!in.IsValid())
		return;

	int uuid = in.GetUUID();
	SClient* pClient = GetClientFromUUID(uuid);
	if (!pClient)
		return;

	/* Broadcast update to all clients (excluding sender) */
	C6SPacket response = C6SNetwork::CServerPlayerUpdate::Create(pClient->id, in.GetPlayerPosition(), in.GetHandPosition(), in.GetAimAngle(), in.IsFacingLeft(), in.IsWalking());
	BroadcastPacket(pGame, response, pClient->id);

	/* Weapon fired - handle */
/*	if (in.IsAttackPressed()) {
		// todo
	}*/

	/* Run tick handler */
	if (m_pNetListener) {
		m_pNetListener->OnTick(pGame, pClient->id, in.GetPlayerPosition(), in.GetHandPosition(), in.GetAimAngle(), in.IsFacingLeft(), in.IsWalking());
	}
}

void CServer::OnClientGameReady(CGame* pGame, C6SPacket& packet, sf::IpAddress addr, unsigned short port) {
	C6SNetwork::CClientGameReady in(packet);
	if (!in.IsValid())
		return;

	int uuid = in.GetUUID();
	SClient* pClient = GetClientFromUUID(uuid);
	if (!pClient)
		return;
	pClient->isReady = true;
}

void CServer::OnClientInputEvent(CGame* pGame, C6SPacket& packet, sf::IpAddress addr, unsigned short port) {
	C6SNetwork::CClientPlayerInputEvent in(packet);
	if (!in.IsValid())
		return;

	int uuid = in.GetUUID();
	SClient* pClient = GetClientFromUUID(uuid);
	if (!pClient)
		return;

	if (in.IsThrow()) {
		if (m_pNetListener) {
			m_pNetListener->OnPlayerRightMousePress(pGame, pClient->id, in.GetThrowVelocity(), in.GetThrowRotation());
		}
	}

	if (in.IsMousePress()) {
		if (m_pNetListener) {
			m_pNetListener->OnPlayerMousePress(pGame, pClient->id);
		}
	}
}

void CServer::AddClient(int _id, int _UUID, sf::IpAddress _ip, int _port, std::string& username) {
	SClient cl;
	cl.id = _id;
	cl.ip = _ip;
	cl.uuid = _UUID;
	cl.port = _port;
	cl.username = username;
	cl.isReady = false;
	m_ConnectedClients.push_back(cl);
}
int CServer::GenerateUUID(CRandom* random) {
	bool success = false;
	bool unique = true;

	int uuid = random->RandomInt();
	while (!success) {
		uuid++;
		unique = true;
		for (unsigned int i = 0; i < m_ConnectedClients.size(); i++) {
			if (m_ConnectedClients[i].uuid == uuid) {
				unique = false;
				break;
			}
		}
		success = unique;
	}
	return uuid;
}
void CServer::Update(CGame* pGame) {
	if (!IsListening())
		return;
	C6SPacket packet; 
	sf::IpAddress addr;
	unsigned short port;
	sf::Socket::Status status = m_ServerSocket.receive(packet, addr, port);
	int dataLenf = packet.getDataSize();

	while (status == sf::Socket::Done) {  
		int dataLen = packet.getDataSize();  

		/* Run packet handler */
		int id = packet.GetID();
		switch (id) { 
		case C6SPacket::ID_CLIENT_CONNECTION_REQUEST:
			OnConnectionRequest(pGame, packet, addr, port);
			break;
		case C6SPacket::ID_CLIENT_CHAT_MESSAGE:
			OnClientChatMessage(pGame, packet, addr, port);
			break;
		case C6SPacket::ID_CLIENT_TICK:
			OnClientTick(pGame, packet, addr, port);
			break;
		case C6SPacket::ID_CLIENT_GAME_READY:
			OnClientGameReady(pGame, packet, addr, port);
			break;
		case C6SPacket::ID_CLIENT_PLAYER_INPUT_EVENT:
			OnClientInputEvent(pGame, packet, addr, port);
			break;
		default:
			break;
		}
		 
		status = m_ServerSocket.receive(packet, addr, port);
	}
}

void CServer::SetMaxClients(int maxclients) {
	if (maxclients < 0)
		return;
	m_iMaxClients = maxclients;
}

void CServer::SetPassword(char* password) {
	m_Password = password;
	m_bPasswordSet = true;
}
void CServer::RemovePassword() {
	m_Password = "";
	m_bPasswordSet = false;
}
int CServer::GenerateID() {
	m_LastID++;
	return m_LastID;
}
int CServer::GetLocalHostID() {
	return m_iHostID;
}
bool CServer::ShouldAcceptClient(std::string password) {
	if (m_bPasswordSet) {
		return (password.compare(m_Password) == 0);
	}
	return m_ConnectedClients.size() < m_iMaxClients;
}
char* CServer::GetRejectReason(std::string password) {
	if (m_bPasswordSet && password.compare(m_Password) != 0) {
		return "Bad Password";
	} else if (m_ConnectedClients.size() >= m_iMaxClients) {
		return "Server is full";
	} else {
		return "Unknown";
	}
}
CServer::SClient* CServer::GetClientFromUUID(int UUID) {
	for (unsigned int i = 0; i < m_ConnectedClients.size(); i++) {
		if (m_ConnectedClients[i].uuid == UUID)
			return &m_ConnectedClients[i];
	}
	return NULL;
}

void CServer::BroadcastPacket(CGame* pGame, C6SPacket& packet) {
	for (unsigned int i = 0; i < m_ConnectedClients.size(); i++) {
		m_ServerSocket.send(packet, m_ConnectedClients[i].ip, m_ConnectedClients[i].port);
	}
}

void CServer::BroadcastPacket(CGame* pGame, C6SPacket& packet, int ignoreID) {
	for (unsigned int i = 0; i < m_ConnectedClients.size(); i++) {
		if (m_ConnectedClients[i].id != ignoreID)
			m_ServerSocket.send(packet, m_ConnectedClients[i].ip, m_ConnectedClients[i].port);
	}
}

std::vector<CServer::SClient>* CServer::GetConnectedClients() {
	return &(m_ConnectedClients);
}

bool CServer::AllClientsReady() {
	for (unsigned int i = 0; i < m_ConnectedClients.size(); i++) {
		if (!m_ConnectedClients[i].isReady)
			return false;
	}
	return true;
}