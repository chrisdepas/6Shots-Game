#include "stdafx.h"
#include "C6SNetwork.h"
#include "C6SPacket.h"
	
/* CONNECTION REQUEST

*/
C6SNetwork::CConnectionRequest::CConnectionRequest(C6SPacket& packet) {
	m_bIsValid = false;

	int id = packet.GetID();
	if (id != C6SPacket::ID_CLIENT_CONNECTION_REQUEST)
		return;

	packet >> m_iNetworkVersion;
	packet >> m_szUsername;
	packet >> m_szPassword;

	m_bIsValid = (m_szUsername.length() != 0);
}
C6SPacket C6SNetwork::CConnectionRequest::Create(char* username, char* password) {
	 C6SPacket packet;
	 packet.SetID(C6SPacket::ID_CLIENT_CONNECTION_REQUEST);
	 packet << C6SNetwork::Version();
	 packet << username;
	 packet << password;
	 return packet;
 }


/*	CONNECTION RESPONSE
	
*/
C6SNetwork::CConnectionResponse::CConnectionResponse(C6SPacket& packet) {
	m_bIsValid = false;
	m_iUUID = -1;
	m_FailReason = "";

	int id = packet.GetID();
	if (id != C6SPacket::ID_SERVER_CONNECTION_RESPONSE)
		return;

	packet >> m_bResult;
	if (m_bResult) {
		/* Accepted -> Packet contains user ID */
		packet >> m_iUUID;
		packet >> m_iID;
		m_bIsValid = (m_iUUID != -1); 
	}
	else {
		/* Declined -> Packet contains fail reason */
		packet >> m_FailReason;
		m_bIsValid = true;
	}
}
C6SPacket C6SNetwork::CConnectionResponse::Create(bool accepted, int UUID, int ID, char* szFailReason) {
	C6SPacket packet;
	packet.SetID(C6SPacket::ID_SERVER_CONNECTION_RESPONSE);
	packet << accepted;
	if (accepted) {
		packet << UUID;
		packet << ID;
	}
	else {
		packet << szFailReason;
	}
	return packet;
}


/* CLIENT CHAT MESSAGE 

*/
C6SNetwork::CClientChatMessage::CClientChatMessage(C6SPacket& packet) {
	m_bIsValid = false;

	int id = packet.GetID();
	if (id != C6SPacket::ID_CLIENT_CHAT_MESSAGE)
		return;

	packet >> m_iUUID;
	packet >> m_szMessage;

	m_bIsValid = (m_szMessage.length() != 0);
}
C6SPacket C6SNetwork::CClientChatMessage::Create(int uuid, std::string& szMessage) {
	C6SPacket packet;
	packet.SetID(C6SPacket::ID_CLIENT_CHAT_MESSAGE);
	packet << uuid;
	packet << szMessage;

	return packet;
}


/* SERVER CHAT MESSAGE

*/
C6SNetwork::CServerChatMessage::CServerChatMessage(C6SPacket& packet) {
	m_bIsValid = false;

	int id = packet.GetID();
	if (id != C6SPacket::ID_SERVER_CHAT_MESSAGE)
		return;

	packet >> m_iClientNumber;
	packet >> m_szMessage;

	m_bIsValid = (m_szMessage.length() != 0);
}
C6SPacket C6SNetwork::CServerChatMessage::Create(int clientID, std::string szMessage) {
	C6SPacket packet;
	packet.SetID(C6SPacket::ID_SERVER_CHAT_MESSAGE);
	packet << clientID;
	packet << szMessage;

	return packet;
}

/* SERVER PLAYER UPDATE

*/
C6SNetwork::CServerPlayerLobbyUpdate::CServerPlayerLobbyUpdate(C6SPacket& packet) {
	m_bIsValid = false;
	m_iClientID = -1;
	int id = packet.GetID();
	if (id != C6SPacket::ID_SERVER_PLAYER_LOBBY_UPDATE)
		return;

	packet >> m_iClientID;
	packet >> m_bLeaving; 
	packet >> m_sPlayerName;

	m_bIsValid = (m_iClientID >= 0);
}
C6SPacket C6SNetwork::CServerPlayerLobbyUpdate::Create(int iClientID, bool bLeaving, std::string& szPlayerName) {
	C6SPacket packet;
	packet.SetID(C6SPacket::ID_SERVER_PLAYER_LOBBY_UPDATE);
	packet << iClientID;
	packet << bLeaving;
	packet << szPlayerName;

	return packet;
}

/* SERVER LAUNCH GAME
 
*/
C6SNetwork::CServerLaunchGame::CServerLaunchGame(C6SPacket& packet) {
	m_bIsValid = false;
	int id = packet.GetID();
	if (id != C6SPacket::ID_SERVER_LAUNCH_GAME)
		return;

	m_bIsValid = true;
}
C6SPacket C6SNetwork::CServerLaunchGame::Create() {
	C6SPacket packet;
	packet.SetID(C6SPacket::ID_SERVER_LAUNCH_GAME);

	return packet;
}

/* CLIENT GAMEREADY

*/
C6SNetwork::CClientGameReady::CClientGameReady(C6SPacket& packet) {
	m_bIsValid = false;
	int id = packet.GetID();
	if (id != C6SPacket::ID_CLIENT_GAME_READY)
		return;
	packet >> m_iUUID;

	m_bIsValid = true;
}
C6SPacket C6SNetwork::CClientGameReady::Create(int iClientUUID) {
	C6SPacket packet;
	packet.SetID(C6SPacket::ID_CLIENT_GAME_READY);

	packet << iClientUUID;

	return packet;
}
int C6SNetwork::CClientGameReady::GetUUID() {
	return m_iUUID;
}

/* SERVER ROUND START

*/
C6SNetwork::CServerRoundStart::CServerRoundStart(C6SPacket& packet) {
	m_bIsValid = false;
	int id = packet.GetID();
	if (id != C6SPacket::ID_SERVER_ROUND_START)
		return;

	m_bIsValid = true;
}
C6SPacket C6SNetwork::CServerRoundStart::Create() {
	C6SPacket packet;
	packet.SetID(C6SPacket::ID_SERVER_ROUND_START);

	return packet;
}

/* SERVER ROUND RESET

*/
C6SNetwork::CServerRoundReset::CServerRoundReset(C6SPacket& packet) {
	m_bIsValid = false;
	int id = packet.GetID();
	if (id != C6SPacket::ID_SERVER_ROUND_RESTART)
		return;

	m_bIsValid = true;
}
C6SPacket C6SNetwork::CServerRoundReset::Create() {
	C6SPacket packet;
	packet.SetID(C6SPacket::ID_SERVER_ROUND_RESTART);

	return packet;
}

/* CLIENT PLAYER TICK

 */
C6SNetwork::CClientPlayerTick::CClientPlayerTick(C6SPacket& packet) {
	int id = packet.GetID();
	if (id != C6SPacket::ID_CLIENT_TICK)
		return;

	packet >> m_iClientUUID;
	packet >> m_fPositionX;
	packet >> m_fPositionY;
	packet >> m_fHandPositionX;
	packet >> m_fHandPositionY;
	packet >> m_fAimAngle;
	packet >> m_bFacingLeft;
	packet >> m_bWalking;

	m_bIsValid = true;
}
Vector2f C6SNetwork::CClientPlayerTick::GetPlayerPosition() {
	return Vector2f(m_fPositionX, m_fPositionY);
}
Vector2f C6SNetwork::CClientPlayerTick::GetHandPosition() {
	return Vector2f(m_fHandPositionX, m_fHandPositionY);
}
float C6SNetwork::CClientPlayerTick::GetAimAngle() {
	return m_fAimAngle;
}
bool C6SNetwork::CClientPlayerTick::IsFacingLeft() {
	return m_bFacingLeft;
}
bool C6SNetwork::CClientPlayerTick::IsWalking() {
	return m_bWalking;
}
int C6SNetwork::CClientPlayerTick::GetUUID() {
	return m_iClientUUID;
}

C6SPacket C6SNetwork::CClientPlayerTick::Create(int iClientUUID, Vector2f vPosition, Vector2f vHandPosition, float fAimAngle, bool bLeftFacing, bool bWalking) {
	C6SPacket packet;
	packet.SetID(C6SPacket::ID_CLIENT_TICK);
	packet << iClientUUID; 
	packet << vPosition.X;
	packet << vPosition.Y;
	packet << vHandPosition.X;
	packet << vHandPosition.Y;
	packet << fAimAngle;
	packet << bLeftFacing;
	packet << bWalking;

	return packet;
}

/* SERVER PLAYER UPDATE

*/

C6SNetwork::CServerPlayerUpdate::CServerPlayerUpdate(C6SPacket& packet) {
	m_bIsValid = false;
	int id = packet.GetID();
	m_iClientID = -1;
	if (id != C6SPacket::ID_SERVER_PLAYER_UPDATE)
		return;

	packet >> m_iClientID;
	packet >> m_fPositionX;
	packet >> m_fPositionY;
	packet >> m_fHandPositionX;
	packet >> m_fHandPositionY;
	packet >> m_fAimAngle;
	packet >> m_bFacingLeft;
	packet >> m_bWalking;

	m_bIsValid = (m_iClientID >= 0);
}
Vector2f C6SNetwork::CServerPlayerUpdate::GetPosition() {
	return Vector2f(m_fPositionX, m_fPositionY);
}
Vector2f C6SNetwork::CServerPlayerUpdate::GetHandPosition() {
	return Vector2f(m_fHandPositionX, m_fHandPositionY);
}

float C6SNetwork::CServerPlayerUpdate::GetAimAngle() {
	return m_fAimAngle;
}

int C6SNetwork::CServerPlayerUpdate::GetID() {
	return m_iClientID;
}
bool C6SNetwork::CServerPlayerUpdate::IsFacingLeft() {
	return m_bFacingLeft;
}
bool C6SNetwork::CServerPlayerUpdate::IsWalking() {
	return m_bWalking;
}

C6SPacket C6SNetwork::CServerPlayerUpdate::Create(int iClientID, Vector2f vPosition, Vector2f vHandPosition, float fAimAngle, bool bLeftFacing, bool bWalking) {
	C6SPacket packet;
	packet.SetID(C6SPacket::ID_SERVER_PLAYER_UPDATE);
	packet << iClientID;
	packet << vPosition.X;
	packet << vPosition.Y;
	packet << vHandPosition.X;
	packet << vHandPosition.Y;
	packet << fAimAngle;
	packet << bLeftFacing;
	packet << bWalking;

	return packet;

}

/*  CLIENT PLAYER INPUT EVENT
1
*/ 

C6SNetwork::CClientPlayerInputEvent::CClientPlayerInputEvent(C6SPacket& packet) {
	m_bIsValid = false;
	int id = packet.GetID();
	m_iClientUUID = -1;
	if (id != C6SPacket::ID_CLIENT_PLAYER_INPUT_EVENT)
		return;

	packet >> m_iClientUUID; /* Client Unique ID */
	packet >> m_eType; /* Input event type */


	switch (m_eType) {
	case EVENT_MOUSEPRESS:
		break;
	case EVENT_THROW:
		packet >> m_vThrowVelocity.X;
		packet >> m_vThrowVelocity.Y;
		break;
	default:
		return;
	}
	
	m_bIsValid = true;
}
 
Vector2f C6SNetwork::CClientPlayerInputEvent::GetThrowVelocity() {
	if (IsThrow())
		return m_vThrowVelocity;
	return Vector2f(0.0f, 0.0f);
}
float C6SNetwork::CClientPlayerInputEvent::GetThrowRotation() {
	if (IsThrow())
		return m_fThrowRotation;
	return 0.0f;
}

int C6SNetwork::CClientPlayerInputEvent::GetUUID() {
	return m_iClientUUID;
}
bool C6SNetwork::CClientPlayerInputEvent::IsMousePress() {
	return m_eType == EVENT_MOUSEPRESS;
}
bool C6SNetwork::CClientPlayerInputEvent::IsThrow() {
	return m_eType == EVENT_THROW;
}

C6SPacket C6SNetwork::CClientPlayerInputEvent::Create(int iClientUUID, C6SNetwork::EPlayerEvent eType, Vector2f fThrowVelocity, float fThrowRotation) {
	C6SPacket packet;
	packet.SetID(C6SPacket::ID_CLIENT_PLAYER_INPUT_EVENT);
	packet << iClientUUID;
	packet << (int)eType;

	/* Throw event stores velocity */
	if (eType == EVENT_THROW) {
		packet << fThrowVelocity.X;
		packet << fThrowVelocity.Y;
		packet << fThrowRotation;
	}

	return packet;
}
 
/*  SERVER PLAYER EVENT

*/

C6SNetwork::CServerPlayerEvent::CServerPlayerEvent(C6SPacket& packet) {
	m_bIsValid = false;
	int id = packet.GetID();
	m_iClientID = -1;
	if (id != C6SPacket::ID_SERVER_PLAYER_EVENT)
		return;

	packet >> m_iClientID; /* Client Unique ID */
	packet >> m_eType; /* Input event type */
	 
	switch (m_eType) { 
	case EVENT_THROW:
		/* Throw position */
		packet >> m_vEventPosition.X;
		packet >> m_vEventPosition.Y;
		/* Throw velocity */
		packet >> m_vEventVelocity.X;
		packet >> m_vEventVelocity.Y;
		/* Projectile ID */
		packet >> m_iPickupID;
		/* Throw rotation */
		packet >> m_fThrowRotation;
		break;
	case EVENT_SHOOT_PROJECTILE:
		/* Shoot position */
		packet >> m_vEventPosition.X;
		packet >> m_vEventPosition.Y;
		/* Shoot velocity */
		packet >> m_vEventVelocity.X;
		packet >> m_vEventVelocity.Y;
		/* Projectile ID */
		packet >> m_iPickupID;
		break;
	case EVENT_PICKUP: 
		/* Pickup weapon ID */
		packet >> m_iPickupID;
		break;
	case EVENT_DAMAGE:
		/* Damage position */
		packet >> m_vEventPosition.X;
		packet >> m_vEventPosition.Y;
		/* Velocity of damage */
		packet >> m_vEventVelocity.X; 
		packet >> m_vEventVelocity.Y;
		/* Damage amount */
		packet >> m_iDamage;
		break;
	case EVENT_DEATH:
		/* Death position */
		packet >> m_vEventPosition.X;
		packet >> m_vEventPosition.Y;
		/* Velocity of death blow */
		packet >> m_vEventVelocity.X;
		packet >> m_vEventVelocity.Y; 
		break;
	case EVENT_DRYFIRE:
		/* No data for dryfire */
		break;
	case EVENT_SET_HITPOINTS:
		/* Net client hitpoints */
		packet >> m_iDamage;
		break;
	default: 
		return;
	}

	m_bIsValid = true;
}

int C6SNetwork::CServerPlayerEvent::GetClientID() {
	return m_iClientID;
}
int C6SNetwork::CServerPlayerEvent::GetClientHitpoints() {
	return m_iDamage;
}
int C6SNetwork::CServerPlayerEvent::GetDamageAmount() {
	return m_iDamage;
}
int C6SNetwork::CServerPlayerEvent::GetProjectileID() {
	return m_iPickupID;
}
int C6SNetwork::CServerPlayerEvent::GetPickupID() {
	return m_iPickupID;
}
bool C6SNetwork::CServerPlayerEvent::IsPickupEvent() { 
	return m_eType == EVENT_PICKUP;
}
bool C6SNetwork::CServerPlayerEvent::IsThrowEvent() {
	return m_eType == EVENT_THROW;
}
bool C6SNetwork::CServerPlayerEvent::IsShootEvent() {
	return m_eType == EVENT_SHOOT_PROJECTILE;
}
bool C6SNetwork::CServerPlayerEvent::IsDryFireEvent() {
	return m_eType == EVENT_DRYFIRE;
}
bool C6SNetwork::CServerPlayerEvent::IsDamageEvent() {
	return m_eType == EVENT_DAMAGE;
}
bool C6SNetwork::CServerPlayerEvent::IsDeathEvent() {
	return m_eType == EVENT_DEATH;
}
bool C6SNetwork::CServerPlayerEvent::IsSetHitpointsEvent() {
	return m_eType == EVENT_DEATH;
}
Vector2f C6SNetwork::CServerPlayerEvent::GetEventPosition() {
	return m_vEventPosition;
}
Vector2f C6SNetwork::CServerPlayerEvent::GetEventVelocity() {
	return m_vEventVelocity;
}
float C6SNetwork::CServerPlayerEvent::GetThrowRotation() {
	return m_fThrowRotation;
}


C6SPacket C6SNetwork::CServerPlayerEvent::CreateAsDeathEvent(int iClientID, Vector2f vDamagePosition, Vector2f vDamageVelocity) {
	C6SPacket packet;
	packet.SetID(C6SPacket::ID_SERVER_PLAYER_EVENT);
	packet << iClientID;
	packet << (int)(EVENT_DEATH);
	packet << vDamagePosition.X;
	packet << vDamagePosition.Y;
	packet << vDamageVelocity.X;
	packet << vDamageVelocity.Y;

	return packet;
}

C6SPacket C6SNetwork::CServerPlayerEvent::CreateAsDamageEvent(int iClientID, Vector2f vDamagePosition, Vector2f vDamageVelocity, int iDamageAmount) {
	C6SPacket packet;
	packet.SetID(C6SPacket::ID_SERVER_PLAYER_EVENT);
	packet << iClientID;
	packet << (int)(EVENT_DAMAGE);
	packet << vDamagePosition.X;
	packet << vDamagePosition.Y;
	packet << vDamageVelocity.X;
	packet << vDamageVelocity.Y;
	packet << iDamageAmount;

	return packet;
}

C6SPacket C6SNetwork::CServerPlayerEvent::CreateAsPickupEvent(int iClientID, int iPickupID) {
	C6SPacket packet;
	packet.SetID(C6SPacket::ID_SERVER_PLAYER_EVENT);
	packet << iClientID;
	packet << (int)(EVENT_PICKUP);
	packet << iPickupID;

	return packet;
}
C6SPacket C6SNetwork::CServerPlayerEvent::CreateAsThrowEvent(int iClientID, Vector2f fThrowVelocity, Vector2f vThrowPosition, float fThrowRotation) {
	C6SPacket packet;
	packet.SetID(C6SPacket::ID_SERVER_PLAYER_EVENT);
	packet << iClientID;
	packet << (int)(EVENT_THROW);
	packet << vThrowPosition.X;
	packet << vThrowPosition.Y;
	packet << fThrowVelocity.X;
	packet << fThrowVelocity.Y;
	packet << fThrowRotation;

	return packet;
}

C6SPacket C6SNetwork::CServerPlayerEvent::CreateAsShootEvent(int iClientID, Vector2f vShootPosition, Vector2f vShootAngle, int iProjectileID) {
	C6SPacket packet;
	packet.SetID(C6SPacket::ID_SERVER_PLAYER_EVENT);
	packet << iClientID;
	packet << (int)(EVENT_SHOOT_PROJECTILE);
	packet << vShootPosition.X;
	packet << vShootPosition.Y;
	packet << vShootAngle.X;
	packet << vShootAngle.Y;
	packet << iProjectileID;

	return packet;
}
C6SPacket C6SNetwork::CServerPlayerEvent::CreateAsDryFireEvent(int iClientID) {
	C6SPacket packet;
	packet.SetID(C6SPacket::ID_SERVER_PLAYER_EVENT);
	packet << iClientID;
	packet << (int)(EVENT_DRYFIRE);

	return packet;
}

C6SPacket C6SNetwork::CServerPlayerEvent::CreateAsHitpointsChange(int iClientID, int iHP) {
	C6SPacket packet;
	packet.SetID(C6SPacket::ID_SERVER_PLAYER_EVENT);
	packet << iClientID;
	packet << (int)(EVENT_SET_HITPOINTS);
	packet << iHP;

	return packet;
}

/*  SERVER ENTITY CREATE

*/
C6SNetwork::CServerEntityCreate::CServerEntityCreate(C6SPacket& packet) {
	m_bIsValid = false;
	int id = packet.GetID();
	if (id != C6SPacket::ID_SERVER_ENTITY_CREATE)
		return;

	packet >> m_iEntityID; /* Client Unique ID */
	packet >> m_eType; /* Input event type */
	packet >> m_vEntPosition.X;
	packet >> m_vEntPosition.Y;
	packet >> m_fEntAngle;
	packet >> m_eEntityInstanceID;
	packet >> m_iClientGiveID;

	m_bIsValid = true;
}

Vector2f C6SNetwork::CServerEntityCreate::GetCreatePosition() {
	return m_vEntPosition;
}

float C6SNetwork::CServerEntityCreate::GetCreateAngle() {
	return m_fEntAngle;
}
int C6SNetwork::CServerEntityCreate::GetEntityID() {
	return m_iEntityID;
}
int C6SNetwork::CServerEntityCreate::GetEntityType() {
	return m_eType;
}
int C6SNetwork::CServerEntityCreate::GetEntityInstanceID() {
	return m_eEntityInstanceID;
}
int C6SNetwork::CServerEntityCreate::GetClientGiveID() {
	return m_iClientGiveID;
}

C6SPacket C6SNetwork::CServerEntityCreate::Create(int iEntID, Vector2f vPosition, float fAngle, int eInstanceID, int eType, int iPickupID) {
	C6SPacket packet;
	packet.SetID(C6SPacket::ID_SERVER_ENTITY_CREATE);

	packet << iEntID;
	packet << eType; 
	packet << vPosition.X;
	packet << vPosition.Y;
	packet << fAngle;
	packet << eInstanceID;
	packet << iPickupID;

	return packet;
}


/*  CLIENT CONSOLE CMD

*/
C6SNetwork::CClientConCmd::CClientConCmd(C6SPacket& packet) {
	m_bIsValid = false;
	int id = packet.GetID();
	if (id != C6SPacket::ID_CLIENT_CONCMD)
		return;

	packet >> m_iClientUUID;	/* Client Unique ID */
	packet >> m_eType;			/* Command type */

	m_bIsValid = true;
}
int C6SNetwork::CClientConCmd::GetConCmdType() {
	return m_eType;
}
int C6SNetwork::CClientConCmd::GetClientUUID() {
	return m_iClientUUID;
}
C6SPacket C6SNetwork::CClientConCmd::Create(int clientUUID, int eType) {
	C6SPacket packet;
	packet.SetID(C6SPacket::ID_SERVER_ENTITY_CREATE);

	packet << clientUUID;
	packet << eType;

	return packet;
}


/*  SERVER MESSAGE

*/
C6SNetwork::CServerBroadcastMessage::CServerBroadcastMessage(C6SPacket& packet) {
	m_bIsValid = false;
	int id = packet.GetID();
	if (id != C6SPacket::ID_SERVER_MESSAGE)
		return;

	packet >> m_fTime;	/* Client Unique ID */
	packet >> m_sMessage;			/* Command type */

	m_bIsValid = true;
}
std::string& C6SNetwork::CServerBroadcastMessage::GetMessage() {
	return m_sMessage; 
}
float C6SNetwork::CServerBroadcastMessage::GetBroadcastTime() {
	return m_fTime;
}
C6SPacket C6SNetwork::CServerBroadcastMessage::Create(float fTime, std::string sMessage) {
	C6SPacket packet;
	packet.SetID(C6SPacket::ID_SERVER_MESSAGE);

	packet << fTime;
	packet << sMessage;

	return packet;
}
