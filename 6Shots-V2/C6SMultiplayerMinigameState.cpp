#include "stdafx.h"
#include "C6SMultiplayerMinigameState.h"
#include "CGame.h"
#include "ASSETS.h"
#include "6SWeapons.h"
#include "CBaseNPC.h"
#include "C6SNetwork.h"


void C6SMultiplayerMinigameState::Init(CGame* pGame) {

	// Init lua interface & extensions 
	if (!InitLuaInterface(pGame)) {
		CDebugLogger::LogError(" C6SMultiplayerMinigameState::Init ERROR - UNABLE TO INIT LUA INTERFACE.\n");
	}
}

bool C6SMultiplayerMinigameState::InitLuaInterface(CGame* pGame) {
	// init lua state 
	if (!m_Lua.InitLua()) {
		CDebugLogger::LogError("InitLuaInterface: Unable to init lua state\n");
		return false;
	}

	// Register 'this' func 
	// register LUA->game funcs
	//m_Lua.RegisterCFunc("BroadcastMsg",  )
	return true;
}

void C6SMultiplayerMinigameState::SendChatMessage(CGame* pGame) {
	
}
void C6SMultiplayerMinigameState::OnTick(CGame* pGame, int iClientID, Vector2f vPosition, Vector2f vHandPosition, float fHandRotation, bool bLeftFacing, bool bWalking) {

}
void C6SMultiplayerMinigameState::OnRemoteEntityCreate(CGame* pGame, int iEntityID, int iType, Vector2f vCreatePos, float fCreateAngle, int iInstanceID, int iClientGiveID) {

}

void C6SMultiplayerMinigameState::OnRoundStart(CGame* pGame) {
	
}

void C6SMultiplayerMinigameState::OnPlayerRightMousePress(CGame* pGame, int iClientID, Vector2f vThrowVelocity, float fThrowRotation) {

}

void C6SMultiplayerMinigameState::OnPlayerMousePress(CGame* pGame, int iClientID) {
	
}

void C6SMultiplayerMinigameState::OnPlayerPickupWeapon(CGame* pGame, int clientID, int entityID) {
	
}

void C6SMultiplayerMinigameState::OnPlayerThrowWeapon(CGame* pGame, int clientID, Vector2f vPosition, Vector2f vVelocity, float fThrowRotation) {
	
}

void C6SMultiplayerMinigameState::OnPlayerShootWeapon(CGame* pGame, int clientID, Vector2f vPosition, Vector2f vVelocity, int projectileID) {

}
void C6SMultiplayerMinigameState::OnPlayerDryFire(CGame* pGame, int clientID) {

}

/* Applies damage to player and calls HandlePlayerDamage */
void C6SMultiplayerMinigameState::OnPlayerDamage(CGame* pGame, int iClientID, Vector2f vDamagePosition, Vector2f vDamageVelocity, int iDamageAmount) {

}

/* Applies death to player and calls HandlePlayerDeath */
void C6SMultiplayerMinigameState::OnPlayerDeath(CGame* pGame, int iClientID, Vector2f vDamagePosition, Vector2f vDamageVelocity) {

}

void C6SMultiplayerMinigameState::OnPlayerCommand(CGame* pGame, int iClientID, int eConCmd) {

}

void C6SMultiplayerMinigameState::OnBroadcastMessage(CGame* pGame, float fTime, std::string& sMessage) {
	/* TODO: DISPLAY THIS PROPERLY */
	m_ChatList.AddMessage((char*)sMessage.c_str());
}

void C6SMultiplayerMinigameState::OnRoundReset(CGame* pGame) {
	
}

void C6SMultiplayerMinigameState::HandlePlayerDamage(CGame* pGame, int playerID, Vector2f& vPosition, Vector2f& vVelocity, int iDamageAmount) {
	
}
void C6SMultiplayerMinigameState::HandlePlayerDeath(CGame* pGame, int playerID, Vector2f& vPosition, Vector2f& vVelocity) {
	
}

void C6SMultiplayerMinigameState::ServerBroadcastPrint(CGame* pGame, char* szMessage, float fTime) {
	/* Send message to clients */
	std::string msg = szMessage;
	pGame->m_Server.SendBroadcastMessage(pGame, msg, fTime);

	/* Display message */
	OnBroadcastMessage(pGame, fTime, msg);
}

void C6SMultiplayerMinigameState::OnEvent(CGame* pGame, std::string evtName, void* data, size_t len) {
	
}
void C6SMultiplayerMinigameState::Cleanup(CGame* pGame) {

}

void C6SMultiplayerMinigameState::Draw(CGame* pGame){
	// Draw message list 
	m_ChatList.Draw(pGame);


}

void C6SMultiplayerMinigameState::Update(CGame* pGame){
	float fFrameTime = pGame->m_Time.LastFrameTime();

	// Update player chat / server message list 
	m_ChatList.Update( fFrameTime );

	// update task scheduler 
	m_Scheduler.Update(fFrameTime);

}

void C6SMultiplayerMinigameState::HandleInput(CGame* pGame) {

}

void C6SMultiplayerMinigameState::PauseState(){

}
void C6SMultiplayerMinigameState::ResumeState(){

}
