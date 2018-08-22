#ifndef __CPROJECTILEMANAGER_H__
#define __CPROJECTILEMANAGER_H__

#include "CProjectile.h"
class CGame;

class CProjectileManager : public b2ContactListener {
	std::vector<CProjectile> m_Projectiles;
	bool m_bInit;
	int m_iLastProjectileID;

public:
	struct SProjectileEvent {
		int iProjectileDamage;
		int iPlayerID;
		int iProjectileID;
		sf::Vector2f vVelocity;
		sf::Vector2f vPosition;
	};

private:

	std::vector<SProjectileEvent> m_ProjectileEvents;
	std::vector<SProjectileEvent> m_ProjectileEventsInternal; 

public:
	CProjectileManager() {
		m_bInit = false;
		m_iLastProjectileID = 0;
	}

	void Init(CWorldPhysics* pPhysics);
	void Update(CWorldPhysics* pPhysics, bool bRecordCollisions);

	/* Projectile functions */
	int AddProjectile(CWorldPhysics* pPhysics, bool bGravity, int iDamage, int iRange, CBaseEntity* pOwner, sf::Vector2f vLocation, sf::Vector2f vVelocity); /* Returns projectile's ID */
	void AddProjectileWithID(CWorldPhysics* pPhysics, bool bGravity, int iDamage, int iRange, CBaseEntity* pOwner, sf::Vector2f vLocation, sf::Vector2f vVelocity, int projectileID);
	void UpdateProjectiles(float delta);
	void DrawProjectiles(CGame* pGame);
	int GetLastProjectileID();
	bool GetProjectileByID(int iID, CProjectile** projectile);
	void RemoveProjectileByID(int id);
	void RemoveAllProjectiles(CWorldPhysics* pPhysics);

	/* b2ContactListener Callbacks */
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);

	/* Collision handling */
	bool HasCollisionEvents();
	bool NextProjectileEvent(SProjectileEvent& pProjectileEvent);
};

#endif