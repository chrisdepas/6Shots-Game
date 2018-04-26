#include "stdafx.h"
#include "CProjectileManager.h"
#include "CGame.h"
#include "CGlobal.h"

void CProjectileManager::Init(CWorldPhysics* pPhysics) {
	if (m_bInit)
		return;
	m_bInit = true; 
	pPhysics->SetContactListener(this);
}

void CProjectileManager::Update(CWorldPhysics* pPhysics, bool bRecordCollisions) {
	CProjectile* pProjectile;
	for (unsigned int i = 0; i < m_ProjectileEventsInternal.size(); i++) {
		/* Remove projectile physics */
		if (GetProjectileByID(m_ProjectileEventsInternal[i].iProjectileID, &pProjectile)) {
			pProjectile->GetPhysics()->DestroyPhysics(pPhysics);
		}
		/* Remove projectile */
		RemoveProjectileByID(m_ProjectileEventsInternal[i].iProjectileID);
		/* Add to projectile events */
		if (bRecordCollisions)
			m_ProjectileEvents.push_back(m_ProjectileEventsInternal[i]);
	}

	/* Erase internal events */
	m_ProjectileEventsInternal.clear();
}

int CProjectileManager::AddProjectile(CWorldPhysics* pPhysics, bool bGravity, int iDamage, int iRange, CBaseEntity* pOwner, Vector2f vLocation, Vector2f vVelocity) {
	CProjectile projectile(iDamage, iRange, ++m_iLastProjectileID, pOwner, vLocation, vVelocity);
	projectile.Init(pPhysics, vLocation, vVelocity);
	m_Projectiles.push_back(projectile);
	return m_iLastProjectileID;
}

void CProjectileManager::AddProjectileWithID(CWorldPhysics* pPhysics, bool bGravity, int iDamage, int iRange, CBaseEntity* pOwner, Vector2f vLocation, Vector2f vVelocity, int projectileID) {
	CProjectile projectile(iDamage, iRange, projectileID, pOwner, vLocation, vVelocity);
	projectile.Init(pPhysics, vLocation, vVelocity);
	m_Projectiles.push_back(projectile);
}

void CProjectileManager::UpdateProjectiles(float delta) {
	for (unsigned int i = 0; i < m_Projectiles.size(); i++) {
		m_Projectiles[i].UpdatePosition();
	}
}

void CProjectileManager::DrawProjectiles(CGame* pGame) {
	for (unsigned int i = 0; i < m_Projectiles.size(); i++) {
		m_Projectiles[i].Draw(pGame);
	}
}
	
bool CProjectileManager::GetProjectileByID(int iID, CProjectile** projectile) {
	for (unsigned int i = 0; i < m_Projectiles.size(); i++) {
		if (m_Projectiles[i].GetID() == iID) {
			*projectile = &m_Projectiles[i];
			return true;
		}
	}
	return false;
}
void CProjectileManager::RemoveProjectileByID(int id) {
	for (unsigned int i = 0; i < m_Projectiles.size(); i++) {
		if (m_Projectiles[i].GetID() == id) {
			m_Projectiles.erase(m_Projectiles.begin() + i);
			return; 
		}
	}
}
void CProjectileManager::RemoveAllProjectiles(CWorldPhysics* pWorldPhysics) {
	/* Delete events */
	m_ProjectileEvents.clear();
	m_ProjectileEventsInternal.clear();

	/* Remove projectile physics from world */
	for (unsigned int i = 0; i < m_Projectiles.size(); i++) {
		CEntityPhysics* pBulletPhysics = m_Projectiles[i].GetPhysics();
		if (pBulletPhysics && pWorldPhysics) {
			pBulletPhysics->DestroyPhysics(pWorldPhysics);
		}
	}

	/* Remove projectiles */
	m_Projectiles.clear();
}
void CProjectileManager::BeginContact(b2Contact* contact) {
	if (!contact)
		return;

	/* Find player feet sensor & other collider */
	b2Fixture* pPlayerFx = NULL;
	b2Fixture* pBulletFx = NULL;
	b2Fixture* fx = contact->GetFixtureA();
	if (fx) {
		if (fx->GetFilterData().categoryBits == CEntityPhysics::CATEGORY_PLAYER) {
			pPlayerFx = fx;
		}
		else if (fx->GetFilterData().categoryBits == CEntityPhysics::CATEGORY_BULLET) {
			pBulletFx = fx;
		}
	}
	fx = contact->GetFixtureB();
	if (fx) {
		if (fx->GetFilterData().categoryBits == CEntityPhysics::CATEGORY_PLAYER) {
			pPlayerFx = fx;
		}
		else if (fx->GetFilterData().categoryBits == CEntityPhysics::CATEGORY_BULLET) {
			pBulletFx = fx;
		}
	}
	 
	if (pPlayerFx && pBulletFx) {
		int projectileID = (int)pBulletFx->GetUserData();
		int playerID = (int)pPlayerFx->GetUserData();
		CProjectile* pProjectileHit;
		b2Body* pBody = pBulletFx->GetBody();
		if (pBody) {
			/* Create collision event */
			SProjectileEvent collision;
			collision.iPlayerID = playerID;
			collision.iProjectileID = projectileID;
			collision.iProjectileDamage = 0;//
			if (GetProjectileByID(projectileID, &pProjectileHit)) {
				collision.iProjectileDamage = pProjectileHit->GetDamage();
			}

			/* get position */
			collision.vPosition.X = CWorldPhysics::GetMeterToPixelFactor() * pBody->GetPosition().x;
			collision.vPosition.Y = CWorldPhysics::GetMeterToPixelFactor() * pBody->GetPosition().y;
			/* get velocity */
			collision.vVelocity.X = CWorldPhysics::GetMeterToPixelFactor() * pBody->GetLinearVelocity().x;
			collision.vVelocity.Y = CWorldPhysics::GetMeterToPixelFactor() * pBody->GetLinearVelocity().y;

			m_ProjectileEventsInternal.push_back(collision);
		}
	}
}

void CProjectileManager::EndContact(b2Contact* contact) {
	/* Unused atm */
}
bool CProjectileManager::HasCollisionEvents() {
	return m_ProjectileEvents.size() > 0;
}
bool CProjectileManager::NextProjectileEvent(SProjectileEvent& pProjectileEvent) {
	if (!HasCollisionEvents())
		return false;
	pProjectileEvent = m_ProjectileEvents.back();
	m_ProjectileEvents.pop_back();
	return true;
} 