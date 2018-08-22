#ifndef __CBASEMELEEWEAPON_H__
#define __CBASEMELEEWEAPON_H__
#include "C6SBaseNetworkedWeapon.h"

class CBaseMeleeWeapon : public C6SBaseNetworkedWeapon {
	int m_iMeleeDamage;
	int m_iReach;
public:
	/* Override update */
	void Update(CWorldPhysics* pPhysics, CGame* pGame, CProjectileManager* pProj, float fElapsedTime) {}
	/* Override networkUpdate */
	void NetworkedUpdate(CWorldPhysics* pPhysics, CGame* pGame, CProjectileManager* pProj, CEntityManager* pEntMgr, float fElapsedTime) {}

	/* Override attack */
	void Attack(CGame* pGame, CEntityManager* pEntMgr, CWorldPhysics* pPhysics) override {}

	bool CanCreateCasing() { return false; };

	CSpentCasingEffect* CreateCasingEffect(CWorldPhysics* pPhysics, sf::Vector2i vPos, sf::Vector2f vEjectDirection, float fEjectRotation) {
		CDebugLogger::LogError("[MELEEWEAPON] CreateCasingEffect called on non-projectile weapon!\n"); 
		return new CSpentCasingEffect(); 
	};

	sf::Vector2f GetCasingCreateOffset() {
		CDebugLogger::LogError("[MELEEWEAPON] GetCasingCreateOffset called on non-projectile weapon!\n");
		return sf::Vector2f(0.0f, 0.0f);
	}

	sf::Vector2f GetCasingEjectDirection() {
		CDebugLogger::LogError("[MELEEWEAPON] GetCasingEjectDirection called on non-projectile weapon!\n");
		return sf::Vector2f(0.0f, 0.0f);
	}

	float GetRandomRecoil(CGame* pGame) override { return 0.0f; }
	float GetRandomSpread(CGame* pGame) override { return 0.0f; }
	bool IsProjectileWeapon() override { return false; }
};

#endif