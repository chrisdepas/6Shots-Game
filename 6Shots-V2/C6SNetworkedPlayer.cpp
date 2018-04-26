#include "stdafx.h"
#include "C6SNetworkedPlayer.h"
#include "CEntityManager.h"
#include "C6SBaseNetworkedWeapon.h"
#include "CBaseProjectileWeapon.h"

void C6SNetworkedPlayer::Initialize(int iClientID, CWorldPhysics* pPhysics, char* playerSprite, char* handTexture, int handSize, int height, int width, Vector2i location) {
	// Physics
	m_PlayerPhysics.InitPhysics(pPhysics, 0, 0, width, height - 6, true, iClientID);

	// Weapon
	m_pHeldWeapon = 0;

	// ID
	m_iClientID = iClientID;

	// Player
	m_bAffectedByGravity = true;
	m_vLocation = Vector2f((float)location.X, (float)location.Y);
	//m_TestTexture.loadFromFile("./Assets/NetworkPlayerTest.png");
	if (!m_PlayerSprite.Load(playerSprite)) {
		CDebugLogger::LogFatal("Fatal Error - Unable to load player sprite from %s\n", playerSprite);
	}
	m_PlayerSprite.SetAnimation("idle");
	m_bWalking = false;
	m_vSize.X = width;
	m_vSize.Y = height;

	// Hand
	Vector2i playerCentre = location;
	playerCentre.Y -= height / 2; 
	m_Hand.Init(handTexture, handSize, 1000, playerCentre);
}

void C6SNetworkedPlayer::NetworkUpdate(Vector2f vLocation, Vector2f vHandlocation, float fHandRotation, bool bLeftFacing, bool bWalking) {
	/* Update player position */
	m_vLocation = vLocation;
	Vector2i playerCentre((int)m_vLocation.X, (int)(m_vLocation.Y - (float)m_vSize.Y / 2.0f));

	/* Update physics */
	Vector2f vPhysicsPos = Vector2f(vLocation.X, vLocation.Y - m_vSize.Y / 2.0f);
	m_PlayerPhysics.SetPosition(vPhysicsPos);

	/* Update animation */
	if (bWalking) {
		if (!m_bWalking) {
			m_PlayerSprite.SetAnimation("walk");
			m_bWalking = true;
		} 
	} else {  
		if (m_bWalking) {
			m_PlayerSprite.SetAnimation("idle"); 
			m_bWalking = false;
		}
	}

	/* Update flags */
	SetBaseFlag(FL_FACING_LEFT, bLeftFacing);
	SetBaseFlag(FL_WALKING, bWalking);

	/* Update player hand */
	m_Hand.UpdatePosition(vHandlocation, fHandRotation);
	m_Hand.SetBaseFlag(FL_FACING_LEFT, (vHandlocation.X < playerCentre.X) );

	/* Update weapon */
	if (m_pHeldWeapon != NULL) {
		m_pHeldWeapon->SetPosition(vHandlocation);
		m_pHeldWeapon->SetTrueRotation(m_Hand.GetTrueRotation());
		m_pHeldWeapon->SetBaseFlag(FL_FACING_LEFT, (vHandlocation.X < playerCentre.X)); 
	}
}

void C6SNetworkedPlayer::Update(float fElapsedTime, CGame* pGame, C6SMap* map, CProjectileManager* pProj, CWorldPhysics* pPhysics) {
	m_PlayerSprite.Update(fElapsedTime, !GetBaseFlag(FL_FACING_LEFT));

	//Vector2i c((int)m_vLocation.X, (int)(m_vLocation.Y - (float)m_vSize.Y / 2.0f));
//	Vector2i ray = m_Hand.GetPosition2i() - c;
	//m_Hand. = (270.0f - atan2f((float)ray.X, (float)ray.Y) * 57.2958f);
//	m_fTrueRotation = atan2f((float)ray.X, (float)ray.Y);
}
void C6SNetworkedPlayer::Draw(CGame* pGame) {
	Vector2i c((int)m_vLocation.X, (int)(m_vLocation.Y - (float)m_vSize.Y / 2.0f));
	
//	pGame->m_Drawing.DrawSpriteCentred(pGame->GetWindowMgr(), c, m_vSize.X, m_vSize.Y, &m_TestTexture);

	m_PlayerSprite.Draw(pGame, c, !GetBaseFlag(FL_FACING_LEFT));

	m_Hand.Draw(pGame, 0.0f);
	if (m_pHeldWeapon != NULL) {
		m_pHeldWeapon->DefaultDraw(pGame);
	}
}

void C6SNetworkedPlayer::NetworkedOnDeath(CGame* pGame) {
	m_PlayerPhysics.SetPosition(Vector2f(-1000.0f, -1000.0f));
	m_PlayerPhysics.DisablePhysics();
}
void C6SNetworkedPlayer::NetworkedRespawn(CGame* pGame) {
	m_PlayerPhysics.SetPosition(m_vLocation);
	m_PlayerPhysics.EnablePhysics();
}

bool C6SNetworkedPlayer::NetworkedPickupWeapon(C6SBaseNetworkedWeapon* pWeapon, CEntityManager* pEntMgr) {
	if (m_pHeldWeapon == NULL) {
		m_pHeldWeapon = pWeapon;
		pEntMgr->RemoveWeapon(pWeapon);
		m_pHeldWeapon->OnPickup();
		return true;
	}
	return false;
}

bool C6SNetworkedPlayer::NetworkedDropWeapon(CEntityManager* pEntMgr, Vector2f vDropVelocity, Vector2f vDropPosition) {
	if (m_pHeldWeapon != NULL) {
		pEntMgr->AddWeapon(m_pHeldWeapon);
		m_pHeldWeapon->SetPosition(vDropPosition);
		m_pHeldWeapon->OnDrop(vDropVelocity);
		m_pHeldWeapon = NULL;
		return true;
	}
	return false;
}
bool C6SNetworkedPlayer::HasWeapon() {
	return m_pHeldWeapon != NULL;
}
bool C6SNetworkedPlayer::ShootWeapon(CGame* pGame, CEntityManager* pEntMgr, CWorldPhysics* pWorldPhysics) {
	if (HasWeapon()) {
		m_pHeldWeapon->Attack(pGame, pEntMgr, pWorldPhysics);
		return true;
	}
	return false;
}

bool C6SNetworkedPlayer::CanShootWeapon() {
	if (HasWeapon()) {
		return m_pHeldWeapon->GetClipAmmo() > 0;
	}
	return false;
}

Vector2f C6SNetworkedPlayer::GetShootPosition() {
	CBaseProjectileWeapon* proj = (CBaseProjectileWeapon*)m_pHeldWeapon;
	return proj->GetProjectileFirePosition();
}

Vector2f C6SNetworkedPlayer::GetShootVelocity() {
	CBaseProjectileWeapon* proj = (CBaseProjectileWeapon*)m_pHeldWeapon;
	return proj->GetProjectileVelocity(m_pHeldWeapon->GetTrueRotation());
}
int C6SNetworkedPlayer::GetWeaponDamage() {
	if (HasWeapon()) {
		return m_pHeldWeapon->GetProjectileDamage();
	}
	return 0;
}

Vector2f C6SNetworkedPlayer::GetHandPosition() { 
	return m_Hand.GetPosition2f(); 
}

int C6SNetworkedPlayer::GetClientID() {
	return m_iClientID;
} 