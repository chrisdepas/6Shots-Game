#include "stdafx.h"
#include "C6SMultiplayerGameState.h"
#include "CGame.h"
#include "ASSETS.h"
#include "6SWeapons.h"
#include "CBaseNPC.h"
#include "C6SNetwork.h"


void C6SMultiplayerGameState::Init(CGame* pGame) {
	/* Default to waiting state */
	m_eCurState = MULTIGAMESTATE_WAITING_FOR_CLIENTS;
	 
	/* Initialize physics engine */
	m_Physics.InitPhysics(pGame, PHYSICS_GRAVITY, PHYSICS_DEBUG);

	if (!IsAlreadyInitialised()) {
		/* Single-time initialisation */
		
		/* Load map */
		m_Map.InitializeFromFile(pGame, ASSET_MAP_SPACEGRAVE);

		/* Init map collision spaces */
		m_Physics.ParseWorldMap(&m_Map);

		/* Init blood/gore engine */
		m_Gore.Init(ASSET_FLOOR_BLOOD_TEXTURE, &m_Physics);

		/* init local player */
		m_iLocalPlayerID = pGame->m_Client.IsConnected() ? pGame->m_Client.GetLocalClientID() : pGame->m_Server.GetLocalHostID();
		m_Player.Initialize(&m_Physics, ASSET_PLAYER_TEXTURE, ASSET_PLAYER_HAND, PLAYER_HANDSIZE, PLAYER_REACH, PLAYER_HEIGHT, PLAYER_WIDTH,
			PLAYER_JUMPSPEED, PLAYER_MOVESPEED, m_Map.GetSpawn(pGame), m_iLocalPlayerID);

		/** Init Connected players **/
		if (pGame->m_Client.IsConnected()) {
			std::vector<CClient::SConnectedPlayer>* players = pGame->m_Client.GetPlayerList();
			int spawnOffset = 0;
			/* Init connected players */
			for (std::vector<CClient::SConnectedPlayer>::iterator it = players->begin();
				it != players->end(); it++) {
				/* Create new networked player */
				C6SNetworkedPlayer* newPlayer = new C6SNetworkedPlayer;
				newPlayer->Initialize(it->id, &m_Physics, ASSET_PLAYER_TEXTURE, ASSET_PLAYER_HAND, PLAYER_HANDSIZE, PLAYER_HEIGHT, 
					PLAYER_WIDTH, Vector2i(100 + spawnOffset, m_Map.GetSize().Y));

				spawnOffset += 100;
				/* Add player*/
				m_vPlayers.push_back(newPlayer);
			}
		} else if (pGame->m_Server.IsListening()) {
			std::vector<CServer::SClient>* clients = pGame->m_Server.GetConnectedClients();
			int spawnOffset = 0;
			/* Init connected clients */
			for (std::vector<CServer::SClient>::iterator it = clients->begin();
				it != clients->end(); it++) {
				/* Create new networked player */
				C6SNetworkedPlayer* newPlayer = new C6SNetworkedPlayer;
				newPlayer->Initialize(it->id, &m_Physics, ASSET_PLAYER_TEXTURE, ASSET_PLAYER_HAND, PLAYER_HANDSIZE, PLAYER_HEIGHT,
					PLAYER_WIDTH, Vector2i(100 + spawnOffset, m_Map.GetSize().Y));

				spawnOffset += 100;
				/* Add player*/
				m_vPlayers.push_back(newPlayer);
			}
		}
	}

	/* Init GUI */
	m_pGui = new tgui::Gui(*pGame->m_WindowManager.GetWindow());
	Vector2i scrSize = pGame->m_WindowManager.GetScreenDimensions(); 
	tgui::Theme::Ptr theme = std::make_shared<tgui::Theme>("widgets/Black.txt");

	/* Chat Box */
	tgui::EditBox::Ptr chatMsg = theme->load("EditBox");
	chatMsg->setSize((float)scrSize.X, 30.0f);
	chatMsg->setPosition(0.0f, (float)scrSize.Y - 30.0f); 
	chatMsg->setText("");
	chatMsg->setTextSize(24);
	chatMsg->connect("ReturnKeyPressed", &C6SMultiplayerGameState::SendChatMessage, this, pGame);
	chatMsg->hide();
	chatMsg->disable();
	m_pGui->add(chatMsg, "ChatMsgBox");

	/* Show GUI */
	pGame->m_WindowManager.SetGUI(m_pGui);
	pGame->m_WindowManager.SetGUIActive(false);

	/* Install state as network listener */
	if (pGame->m_Server.IsListening()) {
		pGame->m_Server.SetNetworkListener(this);
	}
	else if (pGame->m_Client.IsActive()) {
		pGame->m_Client.SetNetworkListener(this);
	}

	/* Register events */
	pGame->m_EventManager.RegisterListener(this, "ChatMessage");

	/* Initialise projectile manager */
	m_Projectiles.Init(&m_Physics);

	/* Init tick delay */
	m_TickRate.Init(TICKRATE);

	/* Reset elapsed time */
	pGame->m_Time.ResetFrameTime();
	  
	/* Reset map data */
	m_Map.Reset();	
	m_Map.ResetSpawns(); 

	/* Set player pos */
	m_Player.SetPosition(m_Map.GetSpawn(pGame)); 

	/* Reset player health */
	m_Player.SetHP(PLAYER_HITPOINTS);
	for (unsigned int i = 0; i < m_vPlayers.size(); i++) {
		m_vPlayers[i]->SetHP(PLAYER_HITPOINTS);
	}

	/* Finally, send GameReady */
	if (pGame->m_Client.IsConnected()) {
		pGame->m_Client.SendReady();
	}
}

void C6SMultiplayerGameState::OnChatMessage(IEventListener::SChatEvent* pChatEvent) {
	/* Get chat color */
	sf::Color chatColor = sf::Color::White;

	/* Format and display chat message */
	std::string chatLine = *pChatEvent->user + ": " + *pChatEvent->message;
	m_ChatDisplay.AddMessage((char*)chatLine.c_str());
}
void C6SMultiplayerGameState::SendChatMessage(CGame* pGame) {
	tgui::EditBox::Ptr editChat = (tgui::EditBox::Ptr)m_pGui->get<tgui::EditBox>(sf::String("ChatMsgBox"));
	printf("Chat: %s\n", editChat->getText().toAnsiString().c_str());

	std::string msg = editChat->getText();

	if (msg[0] == '\\') {
		/* convert to lowercase */
		for (unsigned int i = 0; i < msg.length(); i++) {
			msg[0] = (char)tolower(msg[i]);
		}

		/* Find corresponding concmd */
		int iCmdLength = msg.length() - 1;
		const char* szCmd = msg.c_str() + 1;
		int eCmd = -1; 
		if (iCmdLength == 4 && strncmp(szCmd, "kill", 4) == 0) {
			eCmd = C6SNetwork::EConsoleCommands::COMMAND_KILL;
		}
		else {
			printf("Unknown console command '%s'\n", szCmd);
			return;
		}

		if (pGame->m_Server.IsListening()) {
			/* Run console command */
			HandleConsoleCommand(pGame, m_iLocalPlayerID, (C6SNetwork::EConsoleCommands)eCmd);
		}
		else if (pGame->m_Client.IsConnected()) {
			/* Request console cmd */
			pGame->m_Client.SendConsoleCommand(eCmd);
		}
	}
	else {
		/* Broadcast chat on network */
		if (pGame->m_Server.IsListening()) {
			pGame->m_Server.SendChatMessage(msg, pGame);
		}
		else if (pGame->m_Client.IsConnected()) {
			pGame->m_Client.SendChatMessage(msg);
		}
	}
	/* Clear message box */
	editChat->setText("");

	/* Hide message box */
	editChat->hide();
	editChat->disable();

	/* Disable GUI input handling */
	pGame->m_WindowManager.SetGUIActive(false);
}
void C6SMultiplayerGameState::OnTick(CGame* pGame, int iClientID, Vector2f vPosition, Vector2f vHandPosition, float fHandRotation, bool bLeftFacing, bool bWalking) {
	if (m_eCurState != MULTIGAMESTATE_ROUND && m_eCurState != MULTIGAMESTATE_ROUNDWIN) {
		/* Ignore ticks unless we are during a round / end of round win */
		return;
	}

	/* Handle ticks */	
	for (unsigned int i = 0; i < m_vPlayers.size(); i++) {
		if (m_vPlayers[i]->GetClientID() == iClientID) {
			m_vPlayers[i]->NetworkUpdate(vPosition, vHandPosition, fHandRotation, bLeftFacing, bWalking);
		} 
	}
}
void C6SMultiplayerGameState::OnRemoteEntityCreate(CGame* pGame, int iEntityID, int iType, Vector2f vCreatePos, float fCreateAngle, int iInstanceID, int iClientGiveID) {

	if (pGame->m_Client.IsConnected()) {
		// Ensure ID isn't taken
		if (m_EntMgr.EntityIDExists(iEntityID)) {
			CDebugLogger::LogWarning("[CLIENT] OnRemoteEntityCreate received duplicate ent ID, ignoring\n");
		}

		// Create specified item
		C6SBaseNetworkedWeapon* newWep = NULL;
		switch (iInstanceID) {
			case C6SWEAPONID_AK47:
				{
				C6SAk47* ak = new C6SAk47();
				ak->Init(&m_Physics, vCreatePos, pGame);
				newWep = ak;
				break;
				} 
			case C6SWEAPONID_KNIFE:
				newWep = new C6SKnife();
				break;

			case C6SWEAPONID_REVOLVER:
				{
				C6SRevolver* newWepRevolver = new C6SRevolver();
				newWepRevolver->Init(&m_Physics, Vector2i(vCreatePos.X, vCreatePos.Y), pGame);
				newWep = newWepRevolver;
				break;
				}

			case C6SWEAPONID_SAWNOFF:

				newWep = new C6SSawnOff();
				break;
			case C6SWEAPONID_TMP:
				newWep = new C6STMP();
				break;
			default:
				break;
		}

		if (!newWep) {
			CDebugLogger::LogWarning("[CLIENT] OnRemoteEntityCreate failed to create ent for instance ID %i, ignoring\n", iInstanceID);
			return;
		}

		// Set rotation & ent id 
		newWep->SetTrueRotation(fCreateAngle);
		newWep->SetEntityID(iEntityID);

		// Add to weapons list
		m_EntMgr.AddWeapon(newWep);

		// TODO: Give to specified player 
	}
	else if (pGame->m_Server.IsListening()) {
		/* We are server - Server shouldn't see this
		(Server sends remote ent create only) */
		CDebugLogger::LogWarning("[SERVER] OnRemoteEntityCreate unexpectedly called while server is listening!\n");
	}
}

void C6SMultiplayerGameState::OnRoundStart(CGame* pGame) {
	if (pGame->m_Client.IsActive()) {
		/* Set state to round start */
		m_eCurState = MULTIGAMESTATE_ROUND;
	}
	else if (pGame->m_Server.IsListening()) {
		/* Client-only call */
		CDebugLogger::LogWarning("[SERVER] OnRoundStart unexpectedly called while server is listening!\n");
	}
}

void C6SMultiplayerGameState::OnPlayerRightMousePress(CGame* pGame, int iClientID, Vector2f vThrowVelocity, float fThrowRotation) {
	if (m_eCurState != MULTIGAMESTATE_ROUND && m_eCurState != MULTIGAMESTATE_ROUNDWIN) {
		/* Ignore input unless we are during a round / end of round win */
		return;
	}

	if (pGame->m_Client.IsActive()) {
		/* Server-only call */
		CDebugLogger::LogWarning("[CLIENT] OnPlayerMousePress unexpectedly called while client is active!\n");
	}
	else if (pGame->m_Server.IsListening()) {
		/* Handle player mousepress */
		for (unsigned int i = 0; i < m_vPlayers.size(); i++) {
			if (m_vPlayers[i]->GetClientID() == iClientID) {
				/* Check if player has weapon to drop */
				if (!m_vPlayers[i]->HasWeapon())
					return;

				/* Attempt server weapon drop */
				Vector2f vThrowPosition = m_vPlayers[i]->GetHandPosition();
				if (!m_vPlayers[i]->NetworkedDropWeapon(&m_EntMgr, vThrowVelocity, vThrowPosition)) {
					CDebugLogger::LogError("[SERVER] Client with ID %i failed to drop weapon during OnPlayerRightMousePress\n", iClientID);
					return;
				}

				/* Send network player weapon drop event */
				pGame->m_Server.SendClientWeaponDrop(pGame, iClientID, vThrowPosition, vThrowVelocity, fThrowRotation);
			}
		}
	}
}

void C6SMultiplayerGameState::OnPlayerMousePress(CGame* pGame, int iClientID) {
	if (m_eCurState != MULTIGAMESTATE_ROUND && m_eCurState != MULTIGAMESTATE_ROUNDWIN) {
		/* Ignore input unless we are during a round / end of round win */
		return;
	}

	if (pGame->m_Client.IsActive()) {
		/* Server-only call */
		CDebugLogger::LogWarning("[CLIENT] OnPlayerMousePress unexpectedly called while client is active!\n");
	}
	else if (pGame->m_Server.IsListening()) {
		/* Handle player mousepress */
		for (unsigned int i = 0; i < m_vPlayers.size(); i++) {

			if (m_vPlayers[i]->GetClientID() == iClientID) {
				/* Check if player wants to shoot weapon */
				if (m_vPlayers[i]->HasWeapon()) { 

					if (m_vPlayers[i]->CanShootWeapon()) {
						/* Attempt to shoot weapon for client */
						Vector2f vShootPosition = m_vPlayers[i]->GetShootPosition();
						Vector2f vShootVelocity = m_vPlayers[i]->GetShootVelocity();
						/* Create bullet projectile */
						int newProjectileID = m_Projectiles.AddProjectile(&m_Physics, false, m_vPlayers[i]->GetWeaponDamage(), 100, 0, vShootPosition, vShootVelocity);
						m_vPlayers[i]->ShootWeapon(pGame, &m_EntMgr, &m_Physics);
						pGame->m_Server.SendClientWeaponShoot(pGame, m_vPlayers[i]->GetClientID(), vShootPosition, vShootVelocity, newProjectileID);
					}
					else {
						/* Send dry fire */
						pGame->m_Server.SendClientWeaponDryFire(pGame, m_vPlayers[i]->GetClientID());
					}
				} 
				else {
					/* Attempt to pick up weapon on client */
					C6SBaseNetworkedWeapon* pickup = m_EntMgr.ClosestWeapon(m_vPlayers[i]->GetHandPosition(), PLAYER_PICKUP_RANGE);
					if (pickup) {
						/* Player can pick up weapon, equip on this machine & send pickup event */
						pGame->m_Server.SendClientWeaponPickup(pGame, m_vPlayers[i]->GetClientID(), pickup->GetEntityID());
						if (!m_vPlayers[i]->NetworkedPickupWeapon(pickup, &m_EntMgr)) {
							CDebugLogger::LogError("[CLIENT] Unable to give weapon with ID %i to player during OnPlayerPickupWeapon\n", pickup->GetEntityID());
							return;
						}
					}
				}
			}
		}
	}
}

void C6SMultiplayerGameState::OnPlayerPickupWeapon(CGame* pGame, int clientID, int entityID) {
	if (pGame->m_Client.IsActive()) {
		/* Handle player weapon pickups */
		for (unsigned int i = 0; i < m_vPlayers.size(); i++) {

			/* Handle other player picking up weapon */
			if (m_vPlayers[i]->GetClientID() == clientID) {
				/* Find weapon */
				C6SBaseNetworkedWeapon* pWeapon = m_EntMgr.GetWeaponByID(entityID);
				if (!pWeapon) {
					CDebugLogger::LogError("[CLIENT] Unable to find weapon with ID %i during OnPlayerPickupWeapon\n", entityID);
					return;
				}
				/* Found client, give weapon */
				if (!m_vPlayers[i]->NetworkedPickupWeapon(pWeapon, &m_EntMgr)) {
					CDebugLogger::LogError("[CLIENT] Unable to give weapon with ID %i to player during OnPlayerPickupWeapon\n", entityID);
					return;
				}
			}

			/* Handle local player picking up weapon */
			if (pGame->m_Client.GetLocalClientID() == clientID) {
				/* Find weapon */
				C6SBaseNetworkedWeapon* pWeapon = m_EntMgr.GetWeaponByID(entityID);
				if (!pWeapon) {
					CDebugLogger::LogError("[CLIENT] Unable to find weapon with ID %i during OnPlayerPickupWeapon\n", entityID);
					return;
				}

				/* Equip on client */
				if (!m_Player.NetworkedEquipWeapon(pWeapon, &m_EntMgr)) {
					CDebugLogger::LogError("[CLIENT] Local player unable to equip weapon during OnPlayerPickupWeapon\n", entityID);
					return;
				}
			}
		}
	}
	else if (pGame->m_Server.IsListening()) {
		/* Client-only call */
		CDebugLogger::LogWarning("[SERVER] OnPlayerMousePress unexpectedly called while server is active!\n");
	}
}

void C6SMultiplayerGameState::OnPlayerThrowWeapon(CGame* pGame, int clientID, Vector2f vPosition, Vector2f vVelocity, float fThrowRotation) {
	if (pGame->m_Client.IsActive()) {
		/* Handle player weapon drops */
		for (unsigned int i = 0; i < m_vPlayers.size(); i++) {
			if (m_vPlayers[i]->GetClientID() == clientID) {
				/* Found client, drop weapon */
				if (!m_vPlayers[i]->NetworkedDropWeapon(&m_EntMgr, vVelocity, vPosition)) {
					CDebugLogger::LogError("[CLIENT] Client with ID %i failed to drop weapon during OnPlayerThrowWeapon\n", clientID);
					return;
				}
			}
		}

		/* Handle local player weapon drop */
		if (pGame->m_Client.GetLocalClientID() == clientID) {
			/* Equip on client */
			if (!m_Player.NetworkedDropWeapon(&m_EntMgr, vPosition, vVelocity, fThrowRotation)) {
				CDebugLogger::LogError("[CLIENT] Local client failed to drop weapon during OnPlayerThrowWeapon\n", clientID);
				return;
			}
		}
	}
	else if (pGame->m_Server.IsListening()) {
		/* Client-only call */
		CDebugLogger::LogWarning("[SERVER] OnPlayerThrowWeapon unexpectedly called while server is active!\n");
	}
}

void C6SMultiplayerGameState::OnPlayerShootWeapon(CGame* pGame, int clientID, Vector2f vPosition, Vector2f vVelocity, int projectileID) {
	if (pGame->m_Client.IsActive()) {
		/* Handle player weapon shooting */
		for (unsigned int i = 0; i < m_vPlayers.size(); i++) {
			if (m_vPlayers[i]->GetClientID() == clientID) {
				/* Found client, shoot weapon */
				m_Projectiles.AddProjectileWithID(&m_Physics, false, m_vPlayers[i]->GetWeaponDamage(), 100, 0, vPosition, vVelocity, projectileID);
				m_vPlayers[i]->ShootWeapon(pGame, &m_EntMgr, &m_Physics);
			}
		}

		/* Handle local player weapon shoot */
		if (pGame->m_Client.GetLocalClientID() == clientID) {
			/* Found client, shoot weapon */
			m_Projectiles.AddProjectileWithID(&m_Physics, false, m_Player.GetWeaponProjectileDamage(), 100, 0, vPosition, vVelocity, projectileID);
			m_Player.NetworkedAttack(pGame, &m_EntMgr, &m_Physics); 
		}
	}
	else if (pGame->m_Server.IsListening()) {
		/* Client-only call */
		CDebugLogger::LogWarning("[SERVER] OnPlayerThrowWeapon unexpectedly called while server is active!\n");
	}
}
void C6SMultiplayerGameState::OnPlayerDryFire(CGame* pGame, int clientID) {
	if (pGame->m_Client.IsActive()) {
		/* Handle player weapon shooting */
		for (unsigned int i = 0; i < m_vPlayers.size(); i++) {
			if (m_vPlayers[i]->GetClientID() == clientID) {
				/* Found client, shoot weapon */
			//	m_vPlayers[i]->ShootWeapon(pGame);
			}
		}

		/* Handle local player weapon shoot */
		if (pGame->m_Client.GetLocalClientID() == clientID) {
			/* Found client, shoot weapon */
		//	m_Player.NetworkedAttack(pGame);
		}
	}
	else if (pGame->m_Server.IsListening()) {
		/* Client-only call */
		CDebugLogger::LogWarning("[SERVER] OnPlayerDryFire unexpectedly called while server is active!\n");
	}
}

/* Applies damage to player and calls HandlePlayerDamage */
void C6SMultiplayerGameState::OnPlayerDamage(CGame* pGame, int iClientID, Vector2f vDamagePosition, Vector2f vDamageVelocity, int iDamageAmount) {
	if (pGame->m_Client.IsActive()) {
		if (iClientID == m_iLocalPlayerID) {
			/* Apply damage to local player */ 
			m_Player.TakeDamage(iDamageAmount);
		}
		else {
			/* Apply damage to player */
			for (unsigned int i = 0; i < m_vPlayers.size(); i++) {
				if (m_vPlayers[i]->GetClientID() == iClientID) {
					m_vPlayers[i]->TakeDamage(iDamageAmount);
					break;
				}
			}
		}

		/* Handle damage */
		HandlePlayerDamage(pGame, iClientID, vDamagePosition, vDamageVelocity, iDamageAmount);
	}
	else {
		/* Client-only call */
		CDebugLogger::LogWarning("[SERVER] OnPlayerDamage unexpectedly called on non-client instance!\n");
	}
}

/* Applies death to player and calls HandlePlayerDeath */
void C6SMultiplayerGameState::OnPlayerDeath(CGame* pGame, int iClientID, Vector2f vDamagePosition, Vector2f vDamageVelocity) {
	if (pGame->m_Client.IsActive()) {
		if (iClientID == m_iLocalPlayerID) {
			/* Kill local player */
			m_Player.Kill();
		}
		else {
			/* Kill player */
			for (unsigned int i = 0; i < m_vPlayers.size(); i++) {
				if (m_vPlayers[i]->GetClientID() == iClientID) {
					m_vPlayers[i]->Kill();
					break;
				}
			}
		}

		/* Handle death */
		HandlePlayerDeath(pGame, iClientID, vDamagePosition, vDamageVelocity);
	}
	else {
		/* Client-only call */
		CDebugLogger::LogWarning("[SERVER] OnPlayerDeath unexpectedly called on non-client instance!\n");
	}
}

void C6SMultiplayerGameState::OnPlayerCommand(CGame* pGame, int iClientID, int eConCmd) {
	/* Handle player console commands */
	HandleConsoleCommand(pGame, iClientID, (C6SNetwork::EConsoleCommands)eConCmd);
}

void C6SMultiplayerGameState::OnBroadcastMessage(CGame* pGame, float fTime, std::string& sMessage) {
	/* TODO: DISPLAY THIS PROPERLY */
	m_ChatDisplay.AddMessage((char*)sMessage.c_str());
}

void C6SMultiplayerGameState::OnRoundReset(CGame* pGame) {
	if (pGame->m_Client.IsActive()) {
		/* Put local weapon int ent mgr pool */
		if (m_Player.IsWeaponEquipped()) {
			m_Player.DropWeapon(&m_EntMgr);
		}

		/* Put player weapons into entity mgr pool*/
		for (unsigned int i = 0; i < m_vPlayers.size(); i++) {
			if (m_vPlayers[i]->HasWeapon())
				/* CNetworkedPlayer DropWeapon doesn't send packets */
				m_vPlayers[i]->NetworkedDropWeapon(&m_EntMgr, Vector2f::Null(), Vector2f::Null());
		}

		/* Despawn all entities */
		m_EntMgr.DestroyAllEntities();

		/* Revive & respawn local player */
		m_Player.SetHP(PLAYER_HITPOINTS);
		m_Player.SetPosition(m_Map.GetSpawn(pGame));
		m_Player.NetworkedRespawn(pGame);

		/* Revive connected players */
		for (unsigned int i = 0; i < m_vPlayers.size(); i++) {
			m_vPlayers[i]->SetHP(PLAYER_HITPOINTS);
			m_vPlayers[i]->NetworkedRespawn(pGame);
		}

		/* Despawn blood */
		m_Gore.ClearBloodEffects(&m_Physics);

		/* Despawn projectiles */
		m_Projectiles.RemoveAllProjectiles(&m_Physics);

		/* Exit winstate */
		m_bInWinState = false;
	}
	else {
		//
		CDebugLogger::LogWarning("ServerResetGamemode unexpectedly called on non-server instance.\n");
	}
}

void C6SMultiplayerGameState::HandlePlayerDamage(CGame* pGame, int playerID, Vector2f& vPosition, Vector2f& vVelocity, int iDamageAmount) {
	if (playerID == m_iLocalPlayerID) {
		m_Gore.ApplyDamageEffect(pGame);
	}
	else { 
		 
	}

	m_Gore.CreateBloodSplatter(&m_Physics, Vector2i(vPosition.X, vPosition.Y), 0.2f, 4, Vector2f(pGame->m_Random.RandomFloat(-500.0f, 500.0f), pGame->m_Random.RandomFloat(-500.0f, 500.0f)));
	m_Gore.CreateBloodSplatter(&m_Physics, Vector2i(vPosition.X, vPosition.Y), 0.2f, 4, Vector2f(pGame->m_Random.RandomFloat(-500.0f, 500.0f), pGame->m_Random.RandomFloat(-500.0f, 500.0f)));
	m_Gore.CreateBloodSplatter(&m_Physics, Vector2i(vPosition.X, vPosition.Y), 0.2f, 4, Vector2f(pGame->m_Random.RandomFloat(-500.0f, 500.0f), pGame->m_Random.RandomFloat(-500.0f, 500.0f)));
}
void C6SMultiplayerGameState::HandlePlayerDeath(CGame* pGame, int playerID, Vector2f& vPosition, Vector2f& vVelocity) {
	if (playerID == m_iLocalPlayerID) {
		m_Player.NetworkedOnDeath(pGame, &m_EntMgr);
	}
	else {
		for (unsigned int i = 0; i < m_vPlayers.size(); i++) {
			if (m_vPlayers[i]->GetClientID() == playerID) {
				/* Remove physics */
				m_vPlayers[i]->NetworkedOnDeath(pGame);
				break;
			}
		}
	}


	m_Gore.CreateBloodSplatter(&m_Physics, Vector2i(vPosition.X, vPosition.Y), 0.2f, 4, Vector2f(pGame->m_Random.RandomFloat(-500.0f, 500.0f), pGame->m_Random.RandomFloat(-500.0f, 500.0f)));
	m_Gore.CreateBloodSplatter(&m_Physics, Vector2i(vPosition.X, vPosition.Y), 0.2f, 4, Vector2f(pGame->m_Random.RandomFloat(-500.0f, 500.0f), pGame->m_Random.RandomFloat(-500.0f, 500.0f)));
	m_Gore.CreateBloodSplatter(&m_Physics, Vector2i(vPosition.X, vPosition.Y), 0.2f, 4, Vector2f(pGame->m_Random.RandomFloat(-500.0f, 500.0f), pGame->m_Random.RandomFloat(-500.0f, 500.0f)));
	m_Gore.CreateBloodSplatter(&m_Physics, Vector2i(vPosition.X, vPosition.Y), 0.2f, 4, Vector2f(pGame->m_Random.RandomFloat(-500.0f, 500.0f), pGame->m_Random.RandomFloat(-500.0f, 500.0f)));
	m_Gore.CreateBloodSplatter(&m_Physics, Vector2i(vPosition.X, vPosition.Y), 0.2f, 4, Vector2f(pGame->m_Random.RandomFloat(-500.0f, 500.0f), pGame->m_Random.RandomFloat(-500.0f, 500.0f)));
	m_Gore.CreateBloodSplatter(&m_Physics, Vector2i(vPosition.X, vPosition.Y), 0.2f, 4, Vector2f(pGame->m_Random.RandomFloat(-500.0f, 500.0f), pGame->m_Random.RandomFloat(-500.0f, 500.0f)));
	m_Gore.CreateBloodSplatter(&m_Physics, Vector2i(vPosition.X, vPosition.Y), 0.2f, 4, Vector2f(pGame->m_Random.RandomFloat(-500.0f, 500.0f), pGame->m_Random.RandomFloat(-500.0f, 500.0f)));
	m_Gore.CreateBloodSplatter(&m_Physics, Vector2i(vPosition.X, vPosition.Y), 0.2f, 4, Vector2f(pGame->m_Random.RandomFloat(-500.0f, 500.0f), pGame->m_Random.RandomFloat(-500.0f, 500.0f)));
	m_Gore.CreateBloodSplatter(&m_Physics, Vector2i(vPosition.X, vPosition.Y), 0.2f, 4, Vector2f(pGame->m_Random.RandomFloat(-500.0f, 500.0f), pGame->m_Random.RandomFloat(-500.0f, 500.0f)));
	m_Gore.CreateBloodSplatter(&m_Physics, Vector2i(vPosition.X, vPosition.Y), 0.2f, 4, Vector2f(pGame->m_Random.RandomFloat(-500.0f, 500.0f), pGame->m_Random.RandomFloat(-500.0f, 500.0f)));
	m_Gore.CreateBloodSplatter(&m_Physics, Vector2i(vPosition.X, vPosition.Y), 0.2f, 4, Vector2f(pGame->m_Random.RandomFloat(-500.0f, 500.0f), pGame->m_Random.RandomFloat(-500.0f, 500.0f)));
	m_Gore.CreateBloodSplatter(&m_Physics, Vector2i(vPosition.X, vPosition.Y), 0.2f, 4, Vector2f(pGame->m_Random.RandomFloat(-500.0f, 500.0f), pGame->m_Random.RandomFloat(-500.0f, 500.0f)));
}

void C6SMultiplayerGameState::HandleConsoleCommand(CGame* pGame, int playerID, int eCmd) {
	/* */
	if (pGame->m_Server.IsListening()) {
		switch (eCmd) {
		case C6SNetwork::EConsoleCommands::COMMAND_KILL:
			if (playerID == m_iLocalPlayerID) {
				/* Neck */
				m_Player.Kill();
				/* Network death to clients */
				pGame->m_Server.SendClientDeath(pGame, m_iLocalPlayerID, m_Player.GetHandPosition(), Vector2f::Null());
				/* Handle death */
				HandlePlayerDeath(pGame, m_iLocalPlayerID, m_Player.GetHandPosition(), Vector2f::Null());
			}
			else {
				for (unsigned int i = 0; i < m_vPlayers.size(); i++) {
					if (m_vPlayers[i]->GetClientID() == playerID) {
						/* Kill */
						m_vPlayers[i]->Kill();
						/* Network death to clients */
						pGame->m_Server.SendClientDeath(pGame, playerID, m_Player.GetHandPosition(), Vector2f::Null());
						/* Handle death */
						HandlePlayerDeath(pGame, playerID, m_Player.GetHandPosition(), Vector2f::Null());
						break;
					}
				}
			}
			break;
		default:
			break;
		}
	}
	else {
		CDebugLogger::LogWarning("HandleConsoleCommand unexpectedly called on non-server instance!\n");
	}
}

/* Quickdraw game-mode: Spawn weapons for all players */
void C6SMultiplayerGameState::SpawnPlayerRevolvers(CGame* pGame) {
	for (unsigned int i = 0; i < m_vPlayers.size(); i++) {
		/* Create weapon */
		C6SRevolver* newRevolver = new C6SRevolver();
		Vector2f playerPos = m_vPlayers[i]->GetHandPosition();
		newRevolver->Init(&m_Physics, Vector2i(playerPos.X, playerPos.Y), pGame);
		newRevolver->SetEntityID(m_EntMgr.GetNextEntityID());
		newRevolver->SetTrueRotation(0.0f);

		/* Add & send weapon */
		m_EntMgr.AddWeapon(newRevolver);
		pGame->m_Server.SendWeaponSpawn(pGame, newRevolver->GetPosition2f(), newRevolver->GetTrueRotation(), newRevolver->GetEntityID(), C6SWEAPONID_REVOLVER);
	}

	/* Create weapon */
	C6SRevolver* newRevolver = new C6SRevolver();
	Vector2f playerPos = m_Player.GetHandPosition();  
	newRevolver->Init(&m_Physics, Vector2i(playerPos.X, playerPos.Y), pGame);
	newRevolver->SetEntityID(m_EntMgr.GetNextEntityID());
	newRevolver->SetTrueRotation(0.0f);

	/* Add & send weapon */
	m_EntMgr.AddWeapon(newRevolver);
	pGame->m_Server.SendWeaponSpawn(pGame, newRevolver->GetPosition2f(), newRevolver->GetTrueRotation(), newRevolver->GetEntityID(), C6SWEAPONID_REVOLVER);
}

void C6SMultiplayerGameState::ServerBroadcastPrint(CGame* pGame, char* szMessage, float fTime) {
	/* Send message to clients */
	std::string msg = szMessage;
	pGame->m_Server.SendBroadcastMessage(pGame, msg, fTime);

	/* Display message */
	OnBroadcastMessage(pGame, fTime, msg);
}

/* Reset players for Quickdraw gamemode */
void C6SMultiplayerGameState::ServerResetGamemode(CGame* pGame) {
	if (pGame->m_Server.IsListening()) {
		/* Send client reset command to clients  */
		pGame->m_Server.SendRoundReset(pGame);

		/* Put local weapon int ent mgr pool */
		if (m_Player.IsWeaponEquipped()) {
			m_Player.DropWeapon(&m_EntMgr);
		} 

		/* Put player weapons into entity mgr pool*/
		for (unsigned int i = 0; i < m_vPlayers.size(); i++) {
			if (m_vPlayers[i]->HasWeapon())
				/* CNetworkedPlayer DropWeapon doesn't send packets */
				m_vPlayers[i]->NetworkedDropWeapon(&m_EntMgr, Vector2f::Null(), Vector2f::Null());
		}

		/* Despawn all entities */
		m_EntMgr.DestroyAllEntities();

		/* Despawn projectiles */
		m_Projectiles.RemoveAllProjectiles(&m_Physics);

		/* Revive & respawn local player */
		m_Player.SetHP(PLAYER_HITPOINTS);
		m_Player.SetPosition(m_Map.GetSpawn(pGame));
		m_Player.NetworkedRespawn(pGame);

		/* Revive connected players */
		for (unsigned int i = 0; i < m_vPlayers.size(); i++) {
			m_vPlayers[i]->SetHP(PLAYER_HITPOINTS);
		}

		/* Leave win state */
		m_bInWinState = false;

		/* Despawn blood */
		m_Gore.ClearBloodEffects(&m_Physics);

		/* Start Gamemode again */
		m_Scheduler.ScheduleEvent(1.0f, &C6SMultiplayerGameState::ServerBroadcastPrint, this, pGame, (char*)"5", 1.0f);
		m_Scheduler.ScheduleEvent(2.0f, &C6SMultiplayerGameState::ServerBroadcastPrint, this, pGame, (char*)"4", 1.0f);
		m_Scheduler.ScheduleEvent(3.0f, &C6SMultiplayerGameState::ServerBroadcastPrint, this, pGame, (char*)"3", 1.0f);
		m_Scheduler.ScheduleEvent(4.0f, &C6SMultiplayerGameState::ServerBroadcastPrint, this, pGame, (char*)"2", 1.0f);
		m_Scheduler.ScheduleEvent(5.0f, &C6SMultiplayerGameState::ServerBroadcastPrint, this, pGame, (char*)"1", 1.0f);
		m_Scheduler.ScheduleEvent(6.0f, &C6SMultiplayerGameState::ServerBroadcastPrint, this, pGame, (char*)"DRAW", 1.0f);
		m_Scheduler.ScheduleEvent(6.0f, &C6SMultiplayerGameState::SpawnPlayerRevolvers, this, pGame);
	}
	else {
		//
		CDebugLogger::LogWarning("ServerResetGamemode unexpectedly called on non-server instance.\n");
	}
}
void C6SMultiplayerGameState::OnEvent(CGame* pGame, std::string evtName, void* data, size_t len) {
	if (!pGame->m_StateHandler.IsCurrentState(this))
		return;

	/* Handle event */
	if (evtName.compare("ChatMessage") == 0) {
		IEventListener::SChatEvent* pChat = (IEventListener::SChatEvent*)data;
		OnChatMessage(pChat);
	}
}
void C6SMultiplayerGameState::Cleanup(CGame* pGame) {

}

void C6SMultiplayerGameState::Draw(CGame* pGame){
	CWindowManager* pWnd = &pGame->m_WindowManager;
	m_Map.DrawBackground(pGame, m_Player.GetPosition2f());

	/* Centre Camera on Player */
	pWnd->MoveCamera(m_Player.GetPosition2f());

	/* Draw game level */
	m_Map.DrawLevel(pGame);

	/* Draw local player */
	if (!m_Player.IsDead())
		m_Player.Draw(pGame);

	/* Draw connected players */
	for (unsigned int i = 0; i < m_vPlayers.size(); i++) {
		if (!m_vPlayers[i]->IsDead())
			m_vPlayers[i]->Draw(pGame);
	}

	/* Draw gore*/
	m_Gore.Render(pGame);

	/* Draw all weapons */
	m_EntMgr.Draw(pGame);

	/* Debug draw if enabled */
	m_Physics.Draw();

	/* Draw projectiles */
	m_Projectiles.DrawProjectiles(pGame);

	/* Restore camera */
	pWnd->RestoreDefaultCamera();

	/* Draw gore overlay */
	m_Gore.OverlayRender(pGame);

	/* Draw GUI */
	m_pGui->draw();
	m_ChatDisplay.Draw(pGame);

	/* HP Indicator */
	char szHealthBuf[32]; 
	sprintf_s(szHealthBuf, 32, "HP: %i\n", m_Player.GetHP());
	pGame->m_Drawing.DrawText(pGame->GetWindowMgr(), szHealthBuf, pGame->m_WindowManager.GetScreenDimensions().X - 120, 
		pGame->m_WindowManager.GetScreenDimensions().Y - 30, 24, 255, 255, 255, 255);
}

void C6SMultiplayerGameState::Update(CGame* pGame){
	/* Elapsed frame time */
	float fLastFrameTime = pGame->m_Time.LastFrameTime();

	/* Server networking */
	if (pGame->m_Server.IsListening()) {
		/* Update networking */
		pGame->m_Server.Update(pGame);

		/* Send server update to clients */
		if (m_TickRate.ShouldSendTick()) {
			pGame->m_Server.SendHostTick(pGame, m_Player.GetPosition2f(), m_Player.GetHandPosition(), m_Player.GetHandRotation(),
				m_Player.GetBaseFlag(CBaseEntity::FL_FACING_LEFT), m_Player.GetBaseFlag(CBaseEntity::FL_WALKING));
		}
		 
		/* Check for win condition */
		if (!m_bInWinState)	{
			int iAlivePlayers = 0;
			for (unsigned int i = 0; i < m_vPlayers.size(); i++) {
				if (!m_vPlayers[i]->IsDead())
					iAlivePlayers++;
			}
			iAlivePlayers += m_Player.IsDead() ? 0 : 1;
			if (iAlivePlayers == 1 && m_vPlayers.size() > 0) {
				/* A player has won */
				m_bInWinState = true;

				/* Schedule round reset seconds */
				m_Scheduler.ScheduleEvent(QUICKDRAW_ROUND_RESET_TIME, 
					&C6SMultiplayerGameState::ServerResetGamemode, this, pGame);

				/* Display winner's name */
				if (!m_Player.IsDead()) {
					std::string winMsg = pGame->m_Server.GetHostUsername() + " is the Winner!";
					ServerBroadcastPrint(pGame, (char*)winMsg.c_str(), 1.0f);
				}
				else {
					for (unsigned int i = 0; i < m_vPlayers.size(); i++) {

						if (!m_vPlayers[i]->IsDead()) {

							/* Get username */
							int winnerID = m_vPlayers[i]->GetClientID();
							std::vector<CServer::SClient>* players = pGame->m_Server.GetConnectedClients();
							std::string winMsg = "Unknown player";
							for (std::vector<CServer::SClient>::iterator it = players->begin();
								it != players->end(); it++) {

								if (it->id == winnerID) {
									winMsg = it->username;
									break;
								}
							}

							/* Print */
							winMsg = winMsg + " is the Winner!";
							ServerBroadcastPrint(pGame, (char*)winMsg.c_str(), 1.0f);
							break;
						}

					}
				}
			}
			else if (iAlivePlayers == 0) {
				/* Stalemate */
				m_bInWinState = true;

				/* Display stalemate */
				ServerBroadcastPrint(pGame, (char*)"STALEMATE!", 1.0f);

				/* Schedule round reset */
				m_Scheduler.ScheduleEvent(QUICKDRAW_ROUND_RESET_TIME, 
					&C6SMultiplayerGameState::ServerResetGamemode, this, pGame);
			}
		}

		/* Move out of waiting state */
		if (m_eCurState == MULTIGAMESTATE_WAITING_FOR_CLIENTS) {
			if (pGame->m_Server.AllClientsReady()) {
				/* Move out of Waiting state when all clients connect */
				m_eCurState = MULTIGAMESTATE_ROUND;

				/////////// TEST - CREATE AN AK47

				/* Create weapon */
				/*C6SAk47* newWep = new C6SAk47();
				Vector2i spawn = this->m_Map.GetSpawn(pGame);
				newWep->Init(&m_Physics, Vector2f((float)spawn.X, (float)spawn.Y), pGame);
				newWep->SetEntityID(m_EntMgr.GetNextEntityID());
				newWep->SetTrueRotation(0.0f);

				/* Add & send weapon */
				//m_EntMgr.AddWeapon(newWep);
				//pGame->m_Server.SendWeaponSpawn(pGame, newWep->GetPosition2f(), newWep->GetTrueRotation(), newWep->GetEntityID(), C6SWEAPONID_AK47);
				 
				/* Create weapon */
				/*C6SRevolver* newRevolver = new C6SRevolver();
				Vector2i spawn2 = this->m_Map.GetSpawn(pGame);
				newRevolver->Init(&m_Physics, Vector2i((float)spawn2.X, (float)spawn2.Y), pGame);
				newRevolver->SetEntityID(m_EntMgr.GetNextEntityID());
				newRevolver->SetTrueRotation(0.0f);

				/* Add & send weapon */
				//m_EntMgr.AddWeapon(newRevolver);
				//pGame->m_Server.SendWeaponSpawn(pGame, newRevolver->GetPosition2f(), newRevolver->GetTrueRotation(), newRevolver->GetEntityID(), C6SWEAPONID_REVOLVER);

				///////// 

				m_Scheduler.ScheduleEvent(1.0f, &C6SMultiplayerGameState::ServerBroadcastPrint, this, pGame, (char*)"5", 1.0f);
				m_Scheduler.ScheduleEvent(2.0f, &C6SMultiplayerGameState::ServerBroadcastPrint, this, pGame, (char*)"4", 1.0f);
				m_Scheduler.ScheduleEvent(3.0f, &C6SMultiplayerGameState::ServerBroadcastPrint, this, pGame, (char*)"3", 1.0f);
				m_Scheduler.ScheduleEvent(4.0f, &C6SMultiplayerGameState::ServerBroadcastPrint, this, pGame, (char*)"2", 1.0f);
				m_Scheduler.ScheduleEvent(5.0f, &C6SMultiplayerGameState::ServerBroadcastPrint, this, pGame, (char*)"1", 1.0f);
				m_Scheduler.ScheduleEvent(6.0f, &C6SMultiplayerGameState::ServerBroadcastPrint, this, pGame, (char*)"DRAW", 1.0f);
				m_Scheduler.ScheduleEvent(6.0f, &C6SMultiplayerGameState::SpawnPlayerRevolvers, this, pGame);

				/* Start round */
				pGame->m_Server.SendRoundStart(pGame);
			}
		}

		/* Update projectile collision logic & record collisions */
		m_Projectiles.Update(&m_Physics, true); 

		/* Handle projectile collisions */
		if (m_Projectiles.HasCollisionEvents()) {
			CProjectileManager::SProjectileEvent e;
			while (m_Projectiles.NextProjectileEvent(e)) {
				printf("Projectile hit client %i for %i damage\n", e.iPlayerID, e.iProjectileDamage); 
				if (e.iPlayerID == m_iLocalPlayerID) {
					/* Apply damage to local player */
					m_Player.TakeDamage(e.iProjectileDamage);

					if (m_Player.IsDead()) {
						printf("Sending client death");
						/* Network death to clients */
						pGame->m_Server.SendClientDeath(pGame, e.iPlayerID, e.vPosition, e.vVelocity);
						/* Handle death */
						HandlePlayerDeath(pGame, e.iPlayerID, e.vPosition, e.vVelocity);
					}
					else {
						printf("Sending client damage");
						/* Network damage to clients */
						pGame->m_Server.SendClientDamage(pGame, e.iPlayerID, e.vPosition, e.vVelocity, e.iProjectileDamage);
						/* Handle damage */
						HandlePlayerDamage(pGame, e.iPlayerID, e.vPosition, e.vVelocity, e.iProjectileDamage);
					}
				}
				else {

					/* Apply damage to non-local player */
					for (unsigned int i = 0; i < m_vPlayers.size(); i++) {
						if (m_vPlayers[i]->GetClientID() == e.iPlayerID) {
							m_vPlayers[i]->TakeDamage(e.iProjectileDamage);
							if (m_vPlayers[i]->IsDead()) { 
								printf("Sending client death");
								/* Network death to clients */
								pGame->m_Server.SendClientDeath(pGame, e.iPlayerID, e.vPosition, e.vVelocity);
								/* Handle death */
								HandlePlayerDeath(pGame, e.iPlayerID, e.vPosition, e.vVelocity);
							}
							else {
								printf("Sending client damage");
								/* Network damage to clients */
								pGame->m_Server.SendClientDamage(pGame, e.iPlayerID, e.vPosition, e.vVelocity, e.iProjectileDamage);
								/* Handle damage */
								HandlePlayerDamage(pGame, e.iPlayerID, e.vPosition, e.vVelocity, e.iProjectileDamage);
							}
						}
					}

				}
			}
		}
	} 

	/* Update client connection */
	if (pGame->m_Client.IsActive()) {
		pGame->m_Client.Update(pGame);

		/* Send player tick */
		if (m_TickRate.ShouldSendTick()) {
			pGame->m_Client.SendTick(m_Player.GetPosition2f(), m_Player.GetHandPosition(), m_Player.GetHandRotation(), 
				m_Player.GetBaseFlag(CBaseEntity::FL_FACING_LEFT), m_Player.GetBaseFlag(CBaseEntity::FL_WALKING));
		}

		/* Update projectile collision logic & discard collisions */
		m_Projectiles.Update(&m_Physics, false);
	}

	/* Update scheduler */
	m_Scheduler.Update(fLastFrameTime);
	 
	/* Update networked players */
	for (unsigned int i = 0; i < m_vPlayers.size(); i++) {
		m_vPlayers[i]->Update(fLastFrameTime, pGame, &m_Map, &m_Projectiles, &m_Physics);
	}

	/* Update weapons */
	m_EntMgr.Update(fLastFrameTime, pGame, &m_Physics);

	/* Update weapon projectiles */
	m_Projectiles.UpdateProjectiles(fLastFrameTime);

	/* Update physics/localplayer */
	m_Player.UpdateNetworked(fLastFrameTime, pGame, &m_Map, &m_Projectiles, &m_Physics, &m_EntMgr);
	m_Physics.Step(fLastFrameTime);

	/* Update gore */
	m_Gore.Update(pGame, &m_Physics, fLastFrameTime);

	/* Update chat */
	m_ChatDisplay.Update(fLastFrameTime);
}
 
void C6SMultiplayerGameState::HandleInput(CGame* pGame) {
	bool bGUIEnabled = pGame->m_WindowManager.IsGUIActive();
	// This is a hack to fix world to screen stuff
	CWindowManager* pWnd = &pGame->m_WindowManager;
	pWnd->MoveCamera(m_Player.GetPosition2f());

	if (pGame->m_Input.ActionKeyPressed(EAction::Action_Show_Chat)) {
		tgui::EditBox::Ptr chatBox = (tgui::EditBox::Ptr)m_pGui->get<tgui::EditBox>("ChatMsgBox", false);
		if (chatBox && !chatBox->isEnabled()) {
			/* Clear input of show chat key */
			pGame->m_Input.FlushInput(pGame->GetWindowMgr());

			/* Show & enable */
			chatBox->enable(); 
			chatBox->show(); 

			/* Enable GUI */
			pGame->m_WindowManager.SetGUIActive(true);
			chatBox->focus();

			/* Show chat window */ 
			m_ChatDisplay.Show();
		}
	}

	/* Player input */
	if (!m_Player.IsDead())
		m_Player.HandleInputNetworked(pGame, &m_EntMgr, !bGUIEnabled, PLAYER_PICKUP_RANGE);

	pWnd->RestoreDefaultCamera();
}

void C6SMultiplayerGameState::PauseState(){

}
void C6SMultiplayerGameState::ResumeState(){

}
