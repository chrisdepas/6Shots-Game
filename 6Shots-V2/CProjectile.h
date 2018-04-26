#ifndef __CPROJECTILE_H__
#define __CPROJECTILE_H__

#include "CBaseEntity.h"
#include "CEntityPhysics.h"
#include "CWorldPhysics.h"
class CGame;
class CGlobal;

class CProjectileManager;

class CProjectile : public CBaseEntity {
	int m_iRange;
	int m_iDamage;
	CBaseEntity* m_pOwner;
	Vector2f m_vStartPosition;
	CEntityPhysics m_BulletPhysics;
	int m_iID;	/* Unique Networked ID */
	
public:

	bool AffectedByGravity();
	bool ShouldHit(CBaseEntity*);
	void Draw(CGame* pGame);

	void UpdatePosition();

	int GetID();
	int GetDamage() { return m_iDamage; }
	int GetRange() { return m_iRange; }

	CEntityPhysics* GetPhysics() { return &m_BulletPhysics; }

	/* Init physics etc */
	void CProjectile::Init(CWorldPhysics* pPhysics, Vector2f vLocation, Vector2f vVelocity);

	CProjectile(int iDamage, int iRange, int iID, CBaseEntity* pOwner, Vector2f vLocation, Vector2f vVelocity);

};

#endif