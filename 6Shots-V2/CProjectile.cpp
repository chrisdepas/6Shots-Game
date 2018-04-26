#include "stdafx.h"
#include "CProjectile.h"
#include "CGame.h"
#include "CGlobal.h"

bool CProjectile::ShouldHit(CBaseEntity* pEntity) {
	return pEntity != m_pOwner;
}

void CProjectile::Draw(CGame* pGame) {
	Vector2f d; d.X = m_vStartPosition.X - m_vLocation.X; d.Y = m_vStartPosition.Y - m_vLocation.Y;
	Vector2f front = m_vLocation - m_vStartPosition;
	front.LimitLength(30);
//	pGame->m_Drawing.DrawRectangleCentred(pGame->GetWindowMgr(), GetPosition2i(), 3, 3, 100, 100, 0, 255);
	pGame->m_Drawing.DrawLine(pGame->GetWindowMgr(), Vector2i(m_vLocation.X+front.X, m_vLocation.Y+front.Y), Vector2i(m_vLocation.X, m_vLocation.Y), 255, 240, 0, 255);
}

void CProjectile::UpdatePosition() {
	m_vLocation = m_BulletPhysics.GetPosition();
}

CProjectile::CProjectile(int iDamage, int iRange, int iID, CBaseEntity* pOwner, Vector2f vLocation, Vector2f vVelocity) {
	m_iDamage = iDamage;
	m_iRange = iRange;
	m_pOwner = pOwner;
	m_vLocation = vLocation;
	m_vStartPosition = vLocation;
	m_vVelocity = vVelocity;

	m_iID = iID;
}

void CProjectile::Init(CWorldPhysics* pPhysics, Vector2f vLocation, Vector2f vVelocity) {
	m_BulletPhysics.InitPhysics(pPhysics, (int)vLocation.X, (int)vLocation.Y, 1, 1, CEntityPhysics::CATEGORY_BULLET,
		CEntityPhysics::CATEGORY_PLAYER_PICKUP | CEntityPhysics::CATEGORY_BULLET | CEntityPhysics::CATEGORY_BULLET, m_iID);

	m_BulletPhysics.SetVelocity(vVelocity);
}
int CProjectile::GetID() { return m_iID; }
