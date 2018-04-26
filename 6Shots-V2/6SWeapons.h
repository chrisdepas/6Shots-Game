#ifndef __6SWEAPONS_H__
#define __6SWEAPONS_H__

#include "CBaseMeleeWeapon.h"
#include "CBaseProjectileWeapon.h"

enum E6SWeaponID {
	C6SWEAPONID_UNK = 0,	/* Keep this first - unknown weapon  */
	C6SWEAPONID_KNIFE,		/* Knife */
	C6SWEAPONID_REVOLVER,	/* Revolver */
	C6SWEAPONID_AK47,		/* AK47 */
	C6SWEAPONID_SAWNOFF,	/* Sawnoff Shotgun*/
	C6SWEAPONID_TMP,		/* TMP */
	C6SWEAPONID_BADID,		/* Keep this last - last valid ID non inclusive */
};

class C6SKnife : public CBaseMeleeWeapon {
public:
	void Init(CWorldPhysics* pWorldPhysics, Vector2i pos);
};

class C6SRevolver : public CBaseProjectileWeapon {
public:
	void Init(CWorldPhysics* pWorldPhysics, Vector2i pos, CGame* pGame);

	/* Doesn't create casings on shot */
	bool CanCreateCasing() { return false; };

	CSpentCasingEffect* CreateCasingEffect(CWorldPhysics* pPhysics, Vector2i vPos, Vector2f vEjectDirection, float fEjectRotation) { 
		CDebugLogger::LogError("[REVOLVER] CreateCasingEffect called on caseless weapon!\n"); 
		return new CSpentCasingEffect(); 
	};
};

class C6SAk47 : public CBaseProjectileWeapon {
public:
	void Init(CWorldPhysics* pPhysics, Vector2f pos, CGame* pGame);
	CSpentCasingEffect* CreateCasingEffect(CWorldPhysics* pPhysics, Vector2i vPos, Vector2f vEjectDirection, float fEjectRotation);
};

class C6SSawnOff : public CBaseProjectileWeapon {
public:
	void Init(CWorldPhysics* pPhysics, Vector2i pos, CGame* pGame);

	/* Doesn't create casings on shot */
	bool CanCreateCasing() { return false; };

	CSpentCasingEffect* CreateCasingEffect(CWorldPhysics* pPhysics, Vector2i vPos, Vector2f vEjectDirection, float fEjectRotation) {
		CDebugLogger::LogError("[SHOTGUN] CreateCasingEffect called on caseless weapon!\n");
		return new CSpentCasingEffect();
	};
};

class C6STMP : public CBaseProjectileWeapon {
public:
	void Init(CWorldPhysics* pPhysics, Vector2i pos, CGame* pGame);
	CSpentCasingEffect* CreateCasingEffect(CWorldPhysics* pPhysics, Vector2i vPos, Vector2f vEjectDirection, float fEjectRotation);
};
#endif