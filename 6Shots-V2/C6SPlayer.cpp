#include "stdafx.h"
#include "C6SPlayer.h"
#include "Actions.h"
#include "C6SMap.h"
#include "CDebugLogger.h"
#include "CBaseProjectileWeapon.h"

Vector2f C6SPlayer::GetHandPosition() {
	return m_Hand.GetPosition2f();
}
float C6SPlayer::GetHandRotation() {
	return m_Hand.GetTrueRotation();
}
void C6SPlayer::SetPosition(Vector2f p) {
	m_vLocation = p;
	m_PlayerPhysics.SetPosition(p);
}

void C6SPlayer::SetPosition(Vector2i p) { 
	m_vLocation.X = (float)p.X;
	m_vLocation.Y = (float)p.Y;
	m_PlayerPhysics.SetPosition(p);
}

void C6SPlayer::Initialize(CWorldPhysics* pPhysics, char* playerSprite, char* handTexture, int handSize, int playerReach, int height, int width, float jumpSpeed, float moveSpeed, Vector2i location, int NetworkID) {
	// Input defaults
	m_bMoveLeft = m_bMoveRight = m_bJump = m_bInAir = false;
	m_vVelocity.X = m_vVelocity.Y = 0.0f;
	m_fJumpSpeed = jumpSpeed; 
	m_fMoveSpeed = moveSpeed;
	
	// Physics
	m_PlayerPhysics.InitPhysics(pPhysics, 0, 0, width, height-6, true, NetworkID);

	// Weapon
	m_pWeapon = 0;

	// Player
	m_bAffectedByGravity = true;
	m_vLocation = Vector2f((float)location.X, (float)location.Y);
	m_height = height;
	m_width = width;
	m_PlayerSprite.Load(playerSprite);
	m_PlayerSprite.SetAnimation("idle");

	// Hand
	Vector2i playerCentre = location;
	playerCentre.Y -= height / 2;
	m_Hand.Init(handTexture, handSize, playerReach, playerCentre);
}

void C6SPlayer::Update(float fElapsedTime, CGame* pGame, C6SMap* map, CProjectileManager* pProj, CWorldPhysics* pPhysics) {
	if (IsDead())
		return; 

	float fCurTime = pGame->m_Time.Time();

	// Update animation
	m_PlayerSprite.Update(fElapsedTime, !GetBaseFlag(FL_FACING_LEFT));

	// Update position
	Vector2f physPos = m_PlayerPhysics.GetPosition();
	m_vLocation.X = physPos.X;
	m_vLocation.Y = physPos.Y + m_height / 2.0f;
	
	// Update InAir
	m_bInAir = m_PlayerPhysics.InAir(); 

	if (!m_bInAir && m_bJump) {
		m_PlayerPhysics.SetVerticalVelocity(m_fJumpSpeed);
	}

	if (m_bMoveLeft) {
		SetBaseFlag(FL_FACING_LEFT, true);
		m_PlayerPhysics.SetHorizontalVelocity(-1.0f * m_fMoveSpeed);

	} else if (m_bMoveRight) {
		SetBaseFlag(FL_FACING_LEFT, false);
		m_PlayerPhysics.SetHorizontalVelocity(m_fMoveSpeed);

	} else {
		m_PlayerPhysics.SetHorizontalVelocity(0.0f);

	}


	if (m_pWeapon) {
		m_pWeapon->SetTrueRotation(m_Hand.GetTrueRotation());
		m_pWeapon->SetDrawRotation(m_Hand.GetDrawRotation());

		m_pWeapon->Update(pPhysics, pGame, pProj, fElapsedTime);
	}

	/* Calculate hand velocity */
	if (fCurTime - m_fLastHandPollTime > HAND_POLL_TIME) {
		Vector2i handDelta = m_Hand.GetPosition2i() - m_vLastHandPoll;
		m_vHandVelocity.X = handDelta.X / (fCurTime - m_fLastHandPollTime);
		m_vHandVelocity.Y = handDelta.Y / (fCurTime - m_fLastHandPollTime);
		
		m_fLastHandPollTime = fCurTime;
		m_vLastHandPoll = m_Hand.GetPosition2i();
	}
}
void C6SPlayer::UpdateNetworked(float fElapsedTime, CGame* pGame, C6SMap* map, CProjectileManager* pProj, CWorldPhysics* pPhysics, CEntityManager* pEntMgr) {
	/* Don't update on death */
	if (IsDead())
		return;

	float fCurTime = pGame->m_Time.Time();

	// Update animation
	m_PlayerSprite.Update(fElapsedTime, !GetBaseFlag(FL_FACING_LEFT));

	// Update position
	Vector2f physPos = m_PlayerPhysics.GetPosition();
	m_vLocation.X = physPos.X;
	m_vLocation.Y = physPos.Y + m_height / 2.0f;

	// Update InAir
	m_bInAir = m_PlayerPhysics.InAir();

	if (!m_bInAir && m_bJump) {
		m_PlayerPhysics.SetVerticalVelocity(m_fJumpSpeed);
	}

	if (m_bMoveLeft) {
		SetBaseFlag(FL_FACING_LEFT, true);
		m_PlayerPhysics.SetHorizontalVelocity(-1.0f * m_fMoveSpeed);
	}
	else if (m_bMoveRight) {
		SetBaseFlag(FL_FACING_LEFT, false);
		m_PlayerPhysics.SetHorizontalVelocity(m_fMoveSpeed);
	}
	else {
		m_PlayerPhysics.SetHorizontalVelocity(0.0f);
	}


	/* Update weapon */
	if (m_pWeapon) {
		m_pWeapon->SetTrueRotation(m_Hand.GetTrueRotation());
		m_pWeapon->SetDrawRotation(m_Hand.GetDrawRotation());

		m_pWeapon->NetworkedUpdate(pPhysics, pGame, pProj, pEntMgr, fElapsedTime);
	}

	/* Calculate hand velocity */
	if (fCurTime - m_fLastHandPollTime > HAND_POLL_TIME) {
		Vector2i handDelta = m_Hand.GetPosition2i() - m_vLastHandPoll;
		m_vHandVelocity.X = handDelta.X / (fCurTime - m_fLastHandPollTime);
		m_vHandVelocity.Y = handDelta.Y / (fCurTime - m_fLastHandPollTime);

		m_fLastHandPollTime = fCurTime;
		m_vLastHandPoll = m_Hand.GetPosition2i();
	}
}
bool C6SPlayer::IsWeaponEquipped() {
	return m_pWeapon != NULL;
}
Vector2i C6SPlayer::HandLocation() {
	return m_Hand.GetPosition2i();
}
void C6SPlayer::DropWeapon(CEntityManager* pEntMgr) {
	if (IsWeaponEquipped()) {
		pEntMgr->AddWeapon(m_pWeapon);
		m_pWeapon->OnDrop(m_vHandVelocity);		
		m_pWeapon = 0;
	}
}
void C6SPlayer::EquipWeapon(C6SBaseNetworkedWeapon* pWep, CEntityManager* pEntMgr) {
	if (!IsWeaponEquipped()) {
		m_pWeapon = pWep;
		pEntMgr->RemoveWeapon(pWep);
		m_pWeapon->OnPickup();
	}
}
int C6SPlayer::GetWeaponProjectileDamage() {
	return m_pWeapon->GetProjectileDamage();
}

/* TODO: FIX SPAGHETTI CODE! */
void C6SPlayer::HandleInput(CGame* pGame, CEntityManager* pEntMgr, bool bInputEnabled) {
	/* Don't handle input on death */
	if (IsDead())
		return;

	CInput::SCursorEvent curEvent;
	while (pGame->m_Input.GetNextCursorEvent(curEvent)) {
		/* Ignore input */
		if (!bInputEnabled)
			continue;
		if (curEvent.m_button == sf::Mouse::Left)  {
			if (curEvent.m_bRelease) {
				/* Attack button released */
				if (IsWeaponEquipped()) {
					m_pWeapon->AttackRelease();
				}
			} else {
				/* Attack button pressed */
				if (!IsWeaponEquipped()) { 
					// Try take weapon
					if (pEntMgr) {
						C6SBaseNetworkedWeapon* pickup = pEntMgr->ClosestWeapon(HandLocation(), 15.0f);
						if (pickup) {
							EquipWeapon(pickup, pEntMgr);
						}
					} 
				} else {
					// Weapon is equipped, fire
					m_pWeapon->AttackPress();
				}
			}
		} else if (curEvent.m_button == sf::Mouse::Right) {
			if (curEvent.m_bRelease) {
				// Release Right Mouse 
			} else {
				// Press Right Mouse 
				if (IsWeaponEquipped()) {
					if (pEntMgr)	
						DropWeapon(pEntMgr);
				}
			}
			break;
		}
	}

	/* Movement keys */
	m_bJump			= !bInputEnabled ? false : pGame->m_Input.ActionKeyPressed(EAction::Action_Jump);
	m_bMoveLeft		= !bInputEnabled ? false : pGame->m_Input.ActionKeyPressed(EAction::Action_Move_Left);
	m_bMoveRight	= !bInputEnabled ? false : pGame->m_Input.ActionKeyPressed(EAction::Action_Move_Right);
	if (m_bMoveLeft && m_bMoveRight) {
		m_bMoveLeft = m_bMoveRight = false;
	}

	/* Walk animation */
	if (m_bMoveLeft || m_bMoveRight) {
		SetBaseFlag(FL_WALKING, true);
		m_PlayerSprite.SetAnimation("walk");
	} else {
		SetBaseFlag(FL_WALKING, false);
		m_PlayerSprite.SetAnimation("idle");
	}

	/* Get body centre */
	Vector2i centre((int)m_vLocation.X, (int)(m_vLocation.Y - (float)m_height / 2.0f));

	/* Mouse in-world position */
	Vector2i vMouseScreen = pGame->m_Input.GetMousePosition();
	Vector2i vMouseWorld = pGame->m_WindowManager.ScreenToWorld(vMouseScreen);

	/* Get hand direction & weapon recoil */
	bool bFacingLeft = (vMouseWorld.X < centre.X);
	m_Hand.SetBaseFlag(FL_FACING_LEFT, bFacingLeft);
	float recoil = 0.0f;
	if (IsWeaponEquipped()) {
		recoil = m_pWeapon->GetRecoilRotation();
		m_pWeapon->SetBaseFlag(FL_FACING_LEFT, bFacingLeft);
	}

	m_Hand.UpdatePosition(vMouseWorld, centre, recoil);
}

void C6SPlayer::HandleInputNetworked(CGame* pGame, CEntityManager* pEntMgr, bool bInputEnabled, float fMaxPickupRange) {
	CInput::SCursorEvent curEvent;
	while (pGame->m_Input.GetNextCursorEvent(curEvent)) {
		/* Ignore input */
		if (!bInputEnabled)
			continue;

		if (curEvent.m_button == sf::Mouse::Left)  {
			if (curEvent.m_bRelease) {
				/* Attack button released */
				if (IsWeaponEquipped()) {
					m_pWeapon->AttackRelease();
				}
			}
			else {
				/* Attack button pressed */
				if (!IsWeaponEquipped()) {
					if (pGame->m_Client.IsConnected()) {
						/* Trying to pickup weapon on client */
						pGame->m_Client.SendMousepressEvent();
					}
					else if (pGame->m_Server.IsListening()) {
						/* Trying to pickup weapon on server */
						C6SBaseNetworkedWeapon* pickup = pEntMgr->ClosestWeapon(HandLocation(), fMaxPickupRange);
						if (pickup) {
							/* Pickup localy & send */
							EquipWeapon(pickup, pEntMgr);
							pGame->m_Server.SendHostWeaponPickup(pGame, pickup->GetEntityID());
						}
					}
					else {
						/* Called when offline */
						CDebugLogger::LogWarning("[PLAYER] HandleInputNetworked called when client/server are disconnected!\n");
					}
				}
				else {
					// Weapon is equipped, fire
					m_pWeapon->AttackPress();
				}
			}
		}
		else if (curEvent.m_button == sf::Mouse::Right) {
			if (curEvent.m_bRelease) {
				// Release Right Mouse 
			}
			else {
				// Press Right Mouse  
				if (IsWeaponEquipped()) {
					/* Drop weapon */
					if (pGame->m_Client.IsConnected()) {
						/* Try to drop weapon on client */
						pGame->m_Client.SendRightMousepressEvent(m_vHandVelocity);
					}
					else if (pGame->m_Server.IsListening()) {
						/* Drop weapon on server */
						pEntMgr->AddWeapon(m_pWeapon);
						pGame->m_Server.SendHostWeaponDrop(pGame, m_pWeapon->GetPosition2f(), m_vHandVelocity, m_pWeapon->GetTrueRotation());			
						m_pWeapon->OnDrop(m_vHandVelocity);
						m_pWeapon = 0;
					}
					else {
						/* Called when offline */
						CDebugLogger::LogWarning("[PLAYER] HandleInputNetworked called when client/server are disconnected!\n");
					}
				}
				else {

				}
			}
			break;
		} 
	}

	/* Movement keys */
	m_bJump = !bInputEnabled ? false : pGame->m_Input.ActionKeyPressed(EAction::Action_Jump);
	m_bMoveLeft = !bInputEnabled ? false : pGame->m_Input.ActionKeyPressed(EAction::Action_Move_Left);
	m_bMoveRight = !bInputEnabled ? false : pGame->m_Input.ActionKeyPressed(EAction::Action_Move_Right);
	if (m_bMoveLeft && m_bMoveRight) {
		m_bMoveLeft = m_bMoveRight = false;
	}

	/* Walk animation */
	if (m_bMoveLeft || m_bMoveRight) {
		SetBaseFlag(FL_WALKING, true);
		m_PlayerSprite.SetAnimation("walk");
	}
	else {
		SetBaseFlag(FL_WALKING, false);
		m_PlayerSprite.SetAnimation("idle");
	}

	/* Get body centre */
	Vector2i centre((int)m_vLocation.X, (int)(m_vLocation.Y - (float)m_height / 2.0f));

	/* Mouse in-world position */
	Vector2i vMouseScreen = pGame->m_Input.GetMousePosition();
	Vector2i vMouseWorld = pGame->m_WindowManager.ScreenToWorld(vMouseScreen);
	 
	/* Get hand direction & weapon recoil */
	bool bFacingLeft = (vMouseWorld.X < centre.X);
	m_Hand.SetBaseFlag(FL_FACING_LEFT, bFacingLeft);
	float recoil = 0.0f;
	if (IsWeaponEquipped()) {
		recoil = m_pWeapon->GetRecoilRotation();
		m_pWeapon->SetBaseFlag(FL_FACING_LEFT, bFacingLeft);
	}

	m_Hand.UpdatePosition(vMouseWorld, centre, recoil);
}
void C6SPlayer::Draw(CGame* pGame) {
	Vector2i c((int)m_vLocation.X, (int)(m_vLocation.Y - (float)m_height / 2.0f));
	m_PlayerSprite.Draw(pGame, c, !GetBaseFlag(FL_FACING_LEFT));
	float fWeaponRecoil = 0.0f;
	if (IsWeaponEquipped()) {
		fWeaponRecoil = m_pWeapon->GetRecoilRotation();
	}
	m_Hand.Draw(pGame, fWeaponRecoil * 57.2958f);
	if (IsWeaponEquipped()) {
		Vector2i handPos = m_Hand.GetPosition2i();
		m_pWeapon->SetPosition(handPos); 
		m_pWeapon->DefaultDraw(pGame);
	}
}
void C6SPlayer::NetworkedOnDeath(CGame* pGame, CEntityManager* pEntMgr) {
	/* Stop firing and drop equipped weapon */
	if (IsWeaponEquipped()) {
		m_pWeapon->AttackRelease();

		/* Drop weapon */
		if (pGame->m_Client.IsConnected()) {
			/* Try to drop weapon on client */
			pGame->m_Client.SendRightMousepressEvent(m_vHandVelocity);
		}
		else if (pGame->m_Server.IsListening()) {
			/* Drop weapon on server */
			pEntMgr->AddWeapon(m_pWeapon);
			pGame->m_Server.SendHostWeaponDrop(pGame, m_pWeapon->GetPosition2f(), m_vHandVelocity, m_pWeapon->GetTrueRotation() );
			m_pWeapon->OnDrop(m_vHandVelocity);
			m_pWeapon = 0;
		}
		else {
			/* Called when offline */
			CDebugLogger::LogWarning("[PLAYER] NetworkedOnDeath called when client/server are disconnected!\n");
		}
	}
	//m_PlayerPhysics.DisableCollisions();

	m_vDeathPosition = m_vLocation;
	m_PlayerPhysics.SetPosition(Vector2f(-1000.0f, -1000.0f));

	/* Disable physics */
	m_PlayerPhysics.DisablePhysics();
}

void C6SPlayer::NetworkedRespawn(CGame* pGame) {
	/* Re-enable physics simulation & collisions */
	m_PlayerPhysics.EnablePhysics();
	m_bInAir = false;
	m_PlayerPhysics.ResetGroundContact();

	/* After teleporting from floor, 'jumpstart' physics engine*/
	m_PlayerPhysics.SetVerticalVelocity(10.0f);
}
bool C6SPlayer::NetworkedEquipWeapon(C6SBaseNetworkedWeapon* pWeapon, CEntityManager* pEntMgr) {
	if (!IsWeaponEquipped()) {
		m_pWeapon = pWeapon;
		pEntMgr->RemoveWeapon(pWeapon);
		m_pWeapon->OnPickup();
		return true;
	}
	return false;
}
bool C6SPlayer::NetworkedDropWeapon(CEntityManager* pEntMgr, Vector2f vDropPosition, Vector2f vDropVelocitiy, float fThrowRotation) {
	if (IsWeaponEquipped()) {
		pEntMgr->AddWeapon(m_pWeapon);
		m_pWeapon->OnDrop(vDropVelocitiy);
		m_pWeapon->SetPosition(vDropPosition);
		m_pWeapon->SetTrueRotation(fThrowRotation);
		m_pWeapon = 0;
		return true;
	}
	return false;
}

bool C6SPlayer::NetworkedAttack(CGame* pGame, CEntityManager* pEntMgr, CWorldPhysics* pPhysics) {
	if (IsWeaponEquipped()) {
		m_pWeapon->Attack(pGame, pEntMgr, pPhysics);
	
		return true;
	}
	return false;
}