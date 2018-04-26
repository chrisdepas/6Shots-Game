#include "stdafx.h"
#include "CBaseWeapon.h"
#include "CGame.h"

CBaseWeapon::CBaseWeapon() 
{
	m_szName = 0;
	m_bIsProjectileWeapon = false;
	m_vHoldOffset.X = m_vHoldOffset.Y = 0;
	m_bFiring = false;
	m_fRecoilRotation = 0.0f;
}
void CBaseWeapon::OnPickup() {
	m_bFiring = false;
	m_fRecoilRotation = 0.0f;

	/* Disable collision */
	m_Physics.DisablePhysics();
}

float CBaseWeapon::GetTrueRotation() {
	return m_fTrueRotation;
}
void CBaseWeapon::OnDrop(Vector2f dropVelocity) {
	/**/
	float fPhysicsRotation = m_fDrawRotation * 0.017453f;
	m_Physics.SetPosition(m_vLocation);
	m_Physics.SetRotation(fPhysicsRotation);
	m_Physics.SetVelocity(dropVelocity);
	m_Physics.EnablePhysics(); 
}
void CBaseWeapon::UpdatePhysicsPosition() {
	if (m_Physics.IsActive()) {
		Vector2f pos = m_Physics.GetPosition();
		m_vLocation = pos;

		float rot = m_Physics.GetRotation();
		m_fTrueRotation = rot;
		m_fDrawRotation = m_fTrueRotation * 57.2958f;
	}
}
void CBaseWeapon::DefaultDraw(CGame* pGame) {
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

bool CBaseWeapon::IsMeleeWeapon() {
	return !m_bIsProjectileWeapon;
}

bool CBaseWeapon::IsProjectileWeapon() {
	return m_bIsProjectileWeapon;
}

char* CBaseWeapon::GetName() {
	return m_szName;
}

void CBaseWeapon::AttackPress() {
	m_bFiring = true;
}
void CBaseWeapon::AttackRelease() {
	m_bFiring = false;
}

void CBaseWeapon::SetTrueRotation(float fRot) {
	m_fTrueRotation = fRot;
	m_fDrawRotation = (270.0f - m_fTrueRotation * 57.2958f);
}
void CBaseWeapon::SetDrawRotation(float fRot) {
	m_fDrawRotation = fRot;
}
float CBaseWeapon::GetRecoilRotation() {
	if (GetBaseFlag(CBaseEntity::FL_FACING_LEFT))
		return -1*m_fRecoilRotation;
	else
		return m_fRecoilRotation;
}
void CBaseWeapon::AddRecoil(float fRecoilRadian) {
	m_fRecoilRotation -= fRecoilRadian;
}

Vector2i CBaseWeapon::GetHoldOffset() {
	if (GetBaseFlag(CBaseEntity::FL_FACING_LEFT))
		return m_vHoldOffset;
	else 
		return Vector2i(m_vHoldOffset.X, -1*m_vHoldOffset.Y);
	
}