#include "stdafx.h"
#include "CProjectile.h"
#include "CGame.h"
#include "CGlobal.h"

bool CProjectile::ShouldHit(CBaseEntity* pEntity) {
	return pEntity != m_pOwner;
}

void CProjectile::Draw(CGame* pGame) {
	sf::Vector2f front = m_vLocation - m_vStartPosition;
	if (front.x == 0 && front.y == 0)
		return;
	vec::LimitLength(front, 30);
	pGame->m_Drawing.DrawLine(pGame->GetWindowMgr(), m_vLocation + front, m_vLocation, sf::Color::Yellow);
}

void CProjectile::UpdatePosition() {
	m_vLocation = m_BulletPhysics.GetPosition();
}

CProjectile::CProjectile(int iDamage, int iRange, int iID, CBaseEntity* pOwner, sf::Vector2f vLocation, sf::Vector2f vVelocity) {
	m_iDamage = iDamage;
	m_iRange = iRange;
	m_pOwner = pOwner;
	m_vLocation = vLocation;
	m_vStartPosition = vLocation;
	m_vVelocity = vVelocity;
	m_iID = iID;
}

void CProjectile::Init(CWorldPhysics* pPhysics, sf::Vector2f vLocation, sf::Vector2f vVelocity) {
	m_BulletPhysics.InitPhysics(pPhysics, (int)vLocation.x, (int)vLocation.y, 1, 1, CEntityPhysics::CATEGORY_BULLET,
		CEntityPhysics::CATEGORY_PLAYER_PICKUP | CEntityPhysics::CATEGORY_BULLET | CEntityPhysics::CATEGORY_BULLET, m_iID);

	m_BulletPhysics.SetVelocity(vVelocity);
}
int CProjectile::GetID() { return m_iID; }
