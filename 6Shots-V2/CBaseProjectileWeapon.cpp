#include "stdafx.h"
#include "CBaseProjectileWeapon.h"
#include "CGame.h"

CBaseProjectileWeapon::CBaseProjectileWeapon() {
	m_fRecoilMax = 0.3f;// 10 degrees
	m_fRecoilMin = 0.25f;// 5 degrees
	m_fRecoilDecayRate = 0.174533f*10;// 100 degrees/sec
	m_iBulletsPerShot = 1;
	m_fSpreadMax = 0.05f;
	m_iBulletDamage = 0;
}

void CBaseProjectileWeapon::DefaultDraw(CGame* pGame) {
	Vector2i hold = GetHoldOffset();
	int xOffset = (int)(m_vHoldOffset.X * sin(m_fTrueRotation) + m_vHoldOffset.Y * cos(m_fTrueRotation));
	int yOffset = (int)(m_vHoldOffset.X * cos(m_fTrueRotation) + m_vHoldOffset.Y * sin(m_fTrueRotation));
	if (!GetBaseFlag(FL_FACING_LEFT)) {
		// Facing right, flip
		pGame->m_Drawing.DrawSpriteCentred(pGame->GetWindowMgr(), GetPosition2i().X + xOffset, GetPosition2i().Y + yOffset, m_vSize.X, -m_vSize.Y, 1.0f, m_fDrawRotation, &m_texture);
	}
	else {
		pGame->m_Drawing.DrawSpriteCentred(pGame->GetWindowMgr(), GetPosition2i().X + xOffset, GetPosition2i().Y + yOffset, m_vSize.X, m_vSize.Y, 1.0f, m_fDrawRotation, &m_texture);
	}
}
int CBaseProjectileWeapon::GetClipAmmo() {
	return m_iPrimaryClip;
}
int CBaseProjectileWeapon::GetMaxAmmo() {
	return m_iPrimaryClipMax;
}

Vector2f CBaseProjectileWeapon::GetProjectileFirePosition() {
	Vector2f pos = GetPosition2f();
	pos.X += (m_vFireOffset.X * sin(m_fTrueRotation) + m_vFireOffset.Y * cos(m_fTrueRotation));
	pos.Y += (m_vFireOffset.X * cos(m_fTrueRotation) + m_vFireOffset.Y * sin(m_fTrueRotation));
	return pos;
}

Vector2f CBaseProjectileWeapon::GetProjectileVelocity(float fRotation) {
	Vector2f vel;
	vel.X = (m_iFireSpeed * sin(fRotation));
	vel.Y = (m_iFireSpeed * cos(fRotation));
	return vel;
}
void CBaseProjectileWeapon::Attack(CGame* pGame, CEntityManager* pEntMgr, CWorldPhysics* pPhysics) {
	if (m_iPrimaryClip > 0) {
		m_iPrimaryClip--;

		/* Sound effect */
		pGame->m_SoundManager.PlaySound(m_iWeaponSoundID);

		AddRecoil(GetRandomRecoil(pGame));

		/* Throw casing */
		if (CanCreateCasing()) {
			Vector2f ejectDirection = GetCasingEjectDirection();
			Vector2f ejectPosition = GetCasingCreateOffset();
			pEntMgr->AddCasingEffect(CreateCasingEffect(pPhysics, Vector2i((int)ejectPosition.X, (int)ejectPosition.Y), ejectDirection, 0.0f));
		}
	}
	else {
		pGame->m_SoundManager.PlaySound(m_iDryFireSoundID);
	}
}
float CBaseProjectileWeapon::GetRandomRecoil(CGame* pGame) {
	return pGame->m_Random.RandomFloat(m_fRecoilMin, m_fRecoilMax);
}
float CBaseProjectileWeapon::GetRandomSpread(CGame* pGame) {
	return pGame->m_Random.RandomFloat(-1.f*m_fSpreadMax, m_fSpreadMax);
}
int CBaseProjectileWeapon::GetProjectileDamage() {
	return m_iBulletDamage;
}
void CBaseProjectileWeapon::Update(CWorldPhysics* pPhysics, CGame* pGame,
	CProjectileManager* pProj, float fElapsedTime)  {
	// Decay Recoil
	if (m_fRecoilRotation < 0.0f) {
		m_fRecoilRotation += m_fRecoilDecayRate * fElapsedTime;
		if (m_fRecoilRotation > 0.0f)
			m_fRecoilRotation = 0.0f; 
	}

	// Handle firing 
	if (m_bFiring && CanFire(pGame)) {
		m_fNextAttackTime = pGame->m_Time.Time() + m_fFireDelay;
		
		if (m_iPrimaryClip > 0) {
			// Fire
			for (int i = 0; i < m_iBulletsPerShot; i++)
				pProj->AddProjectile(pPhysics, false, GetProjectileDamage(), 100, 0, GetProjectileFirePosition(), GetProjectileVelocity(m_fTrueRotation+GetRandomSpread(pGame)));
			m_iPrimaryClip--;
			pGame->m_SoundManager.PlaySound(m_iWeaponSoundID);
			if (!m_bIsAutomatic) {
				m_bFiring = false;
			}
			AddRecoil(GetRandomRecoil(pGame));
		} else {
			// Dry Fire
			pGame->m_SoundManager.PlaySound(m_iDryFireSoundID);
			m_bFiring = false;
		}
	}
}
void CBaseProjectileWeapon::NetworkedUpdate(CWorldPhysics* pPhysics, CGame* pGame, 
	CProjectileManager* pProj, CEntityManager* pEntMgr, float fElapsedTime)  {
	// Decay Recoil
	if (m_fRecoilRotation < 0.0f) {
		m_fRecoilRotation += m_fRecoilDecayRate * fElapsedTime;
		if (m_fRecoilRotation > 0.0f)
			m_fRecoilRotation = 0.0f;
	}

	// Handle firing 
	if (m_bFiring && CanFire(pGame)) {
		m_fNextAttackTime = pGame->m_Time.Time() + m_fFireDelay;

		if (m_iPrimaryClip > 0) {
			if (pGame->m_Client.IsConnected()) {
				/* Trying to shoot weapon on client */
				pGame->m_Client.SendMousepressEvent();
				 
				if (!m_bIsAutomatic) {
					m_bFiring = false;
				}

			} else if (pGame->m_Server.IsListening()) {
				/* Server shoots locally & sends */
				for (int i = 0; i < m_iBulletsPerShot; i++) {
					Vector2f vFirePosition = GetProjectileFirePosition();
					Vector2f vFireVelocity = GetProjectileVelocity(m_fTrueRotation + GetRandomSpread(pGame));
					int newProjectileID = pProj->AddProjectile(pPhysics, false, GetProjectileDamage(), 100, 0, vFirePosition, vFireVelocity);
					Attack(pGame, pEntMgr, pPhysics);
					pGame->m_Server.SendHostWeaponShoot(pGame, vFirePosition, vFireVelocity, newProjectileID);

					if (!m_bIsAutomatic) {
						m_bFiring = false;
					}
				} 
			} else {
				/* Called when offline */
				CDebugLogger::LogError("[PLAYER] HandleInputNetworked called when client/server are disconnected!\n");
			}
		} else {
			// Dry Fire
			m_bFiring = false;

			if (pGame->m_Client.IsConnected()) {
				/* server handles logic */
				pGame->m_Client.SendMousepressEvent();
			}
			else if (pGame->m_Server.IsListening()) { 
				/* Server plays local sound & informs clients */
				pGame->m_Server.SendHostWeaponDryFire(pGame);
				pGame->m_SoundManager.PlaySound(m_iDryFireSoundID);
			}
			else {
				/* Called when offline */
				CDebugLogger::LogWarning("[PLAYER] HandleInputNetworked called when client/server are disconnected!\n");
			}
		}
	}
}

bool CBaseProjectileWeapon::CanFire(CGame* pGame) {
	return  pGame->m_Time.Time() >= m_fNextAttackTime;
}

Vector2f CBaseProjectileWeapon::GetCasingCreateOffset() {
	Vector2f pos = GetPosition2f();
	pos.X += (m_vCasingOffset.X * sin(m_fTrueRotation) + m_vCasingOffset.Y * cos(m_fTrueRotation));
	pos.Y += (m_vCasingOffset.X * cos(m_fTrueRotation) + m_vCasingOffset.Y * sin(m_fTrueRotation));
	return pos;
}
Vector2f CBaseProjectileWeapon::GetCasingEjectDirection() {
	if (!GetBaseFlag(FL_FACING_LEFT)) {
		return Vector2f(sin(m_fTrueRotation + 1.5708f), cos(m_fTrueRotation + 1.5708f));
	}
	else {
		return Vector2f(sin(m_fTrueRotation - 1.5708f), cos(m_fTrueRotation - 1.5708f));
	}
}