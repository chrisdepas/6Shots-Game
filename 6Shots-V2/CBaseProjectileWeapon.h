#ifndef __CBASEPROJECTILEWEAPON_H__
#define __CBASEPROJECTILEWEAPON_H__

/*
	Christopher De Pasquale
	Late 2016
	Noticed added feb 2017

*/

#include "C6SBaseNetworkedWeapon.h"
#include "CProjectileManager.h"
#include "CEntityManager.h"
class CBaseProjectileWeapon : public C6SBaseNetworkedWeapon {
protected:
	int m_iPrimaryClip;
	int m_iPrimaryClipMax;
	int m_iWeaponSoundID;
	int m_iDryFireSoundID;

	int m_iBulletsPerShot;

	int m_iBulletDamage;

	float m_fRecoilMin; // Recoil minimum value in RADIANS/second
	float m_fRecoilMax; // Recoil maximum value in RADIANS/second
	float m_fRecoilDecayRate; // Decay rate of recoil in RADIANS/second

	float m_fFireDelay; // Delay between shots
	float m_fNextAttackTime;
	bool m_bIsAutomatic;

	Vector2i m_vFireOffset;		// Offset from hold position to spawn projectile
	Vector2i m_vCasingOffset;	// Offset from hold position to eject casing 
	int m_iFireSpeed;
	float m_fFirespeedVar;

	float m_fSpreadMax; // max spread angle in RADIANS (Actual spread is 2x this value, for +/-)
public:

	CBaseProjectileWeapon(); 
	
	/* Overload ammo */
	int GetClipAmmo() override;
	int GetMaxAmmo() override;

	void DefaultDraw(CGame* pGame) override;

	float GetRandomSpread(CGame* pGame) override;
	float GetRandomRecoil(CGame* pGame) override;
	int GetProjectileDamage() override;

	Vector2f GetProjectileFirePosition();
	Vector2f GetProjectileVelocity(float fRot);

	
	void Attack(CGame* pGame, CEntityManager* pEntMgr, CWorldPhysics* pPhysics) override;


	bool CanFire(CGame* pGame);

	/* Override update */
	void Update(CWorldPhysics* pPhysics, CGame* pGame, CProjectileManager* pProj, float fElapsedTime);
	/* Override networkUpdate */
	void NetworkedUpdate(CWorldPhysics* pPhysics, CGame* pGame, CProjectileManager* pProj, CEntityManager* pEntMgr, float fElapsedTime);

	/* Override casing */
	bool CanCreateCasing() { return true; };
	/* Override casing position */
	Vector2f GetCasingCreateOffset();
	/* Override casing direction */
	Vector2f GetCasingEjectDirection();
};

#endif