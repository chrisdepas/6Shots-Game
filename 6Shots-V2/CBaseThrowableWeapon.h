#ifndef __CBASETHROWABLEWEAPON_H__
#define __CBASETHROWABLEWEAPON_H__

#include "C6SBaseNetworkedWeapon.h"
#include "CProjectileManager.h"
class CBaseThrowableWeapon : public C6SBaseNetworkedWeapon {
protected:
	//
	
public:

	CBaseThrowableWeapon();
	virtual void Attack(CGame* pGame);

	// Overloaded
	int GetClipAmmo();
	int GetMaxAmmo();

	float GetRandomSpread(CGame* pGame);
	Vector2f GetProjectileFirePosition();
	Vector2f GetProjectileVelocity(float fRot);

	bool CanFire(CGame* pGame);

	/* Override update */
	void Update(CWorldPhysics* pPhysics, CGame* pGame, CProjectileManager* pProj, float fElapsedTime);
	/* Override networkUpdate */
	void NetworkedUpdate(CWorldPhysics* pPhysics, CGame* pGame, CProjectileManager* pProj, float fElapsedTime);
};

#endif