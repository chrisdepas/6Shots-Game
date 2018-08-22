#include "stdafx.h"
#include "CBaseWeapon.h"
#include "CGame.h"

void CBaseWeapon::OnPickup() {
	// Reset trigger state
	AltRelease();
	AttackRelease();

	// Reset recoil
	m_fRecoilRotation = 0.0f;

	// Disable collisions
	m_Physics.DisablePhysics();
}

void CBaseWeapon::OnDrop(sf::Vector2f dropVelocity) {
	/**/
	float fPhysicsRotation = m_fDrawRotation * 0.017453f;
	m_Physics.SetPosition(m_vLocation);
	m_Physics.SetRotation(fPhysicsRotation);
	m_Physics.SetVelocity(dropVelocity);
	m_Physics.EnablePhysics(); 
}
void CBaseWeapon::UpdatePhysicsPosition() {
	if (!m_Physics.IsActive())
		return;
	m_vLocation = m_Physics.GetPosition();
	m_fTrueRotation = m_Physics.GetRotationRadian();
	m_fDrawRotation = m_Physics.GetRotationDegree();
}
void CBaseWeapon::DefaultDraw(CGame* pGame) {
	int xOffset = (int)(m_vHoldOffset.x * sin(m_fTrueRotation) + m_vHoldOffset.y * cos(m_fTrueRotation));
	int yOffset = (int)(m_vHoldOffset.x * cos(m_fTrueRotation) + m_vHoldOffset.y * sin(m_fTrueRotation));
	if (!GetBaseFlag(FL_FACING_LEFT)) {
		// Facing right, flip
		pGame->m_Drawing.DrawSpriteCentred(pGame->GetWindowMgr(), GetPosition2i().x + xOffset, GetPosition2i().y + yOffset, m_vSize.x, -m_vSize.y, 1.0f, m_fDrawRotation, &m_texture);
	}
	else {
		pGame->m_Drawing.DrawSpriteCentred(pGame->GetWindowMgr(), GetPosition2i().x + xOffset, GetPosition2i().y + yOffset, m_vSize.x, m_vSize.y, 1.0f, m_fDrawRotation, &m_texture);
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

sf::Vector2i CBaseWeapon::GetHoldOffset() {
	if (GetBaseFlag(CBaseEntity::FL_FACING_LEFT))
		return m_vHoldOffset;
	else 
		return sf::Vector2i(m_vHoldOffset.x, -1*m_vHoldOffset.y);
	
}