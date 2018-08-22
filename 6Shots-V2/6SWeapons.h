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
	void Init(CWorldPhysics* pWorldPhysics, sf::Vector2i pos);
};

class C6SRevolver : public CBaseProjectileWeapon {
public:
	void Init(CWorldPhysics* pWorldPhysics, sf::Vector2i pos, CGame* pGame);

	/* Doesn't create casings on shot */
	bool CanCreateCasing() { return false; };

	CSpentCasingEffect* CreateCasingEffect(CWorldPhysics* pPhysics, sf::Vector2i vPos, sf::Vector2f vEjectDirection, float fEjectRotation) { 
		CDebugLogger::LogError("[REVOLVER] CreateCasingEffect called on caseless weapon!\n"); 
		return new CSpentCasingEffect(); 
	};
};

class C6SBaseDevPistol : public CBaseProjectileWeapon {
	bool m_bCanCreateCasing = false;
	std::string m_sCasingAsset = "";
	sf::Vector2i m_vCasingSize = sf::Vector2i(20, 10);
	float m_fCasingEjectSpeed = 230.0f;
	float m_fCasingEjectAngularSpeed = 700.0f;
	bool m_bWrittenName = false;

public:
	void Init(CWorldPhysics* pWorldPhysics, sf::Vector2i pos, CGame* pGame);

	char* GetName() { return m_szName; }
	void SetName(std::string sName);

	bool SetGunFireSoundFromFile(std::string sFireSoundFile, CGame* pGame);
	bool SetGunDryFireSoundFromFile(std::string sDryFireSoundFile, CGame* pGame);

	bool SetGunTextureFromFile(std::string sTextureFilePath);
	sf::Vector2i GetGunTextureSize() { return m_vSize; }

	void SetAutomatic(bool bAutomatic) { m_bIsAutomatic = bAutomatic; }
	bool GetAutomatic() { return m_bIsAutomatic; }

	int GetPrimaryClipMax() { return m_iPrimaryClipMax; }
	void SetPrimaryClipMax(int iPrim) { m_iPrimaryClipMax = iPrim; }

	void RefillPrimaryClip() { m_iPrimaryClip = m_iPrimaryClipMax; }
	bool GetInfiniteAmmo() { return m_bInfiniteBullets; }
	void SetInfiniteAmmo(bool bInfinite) { m_bInfiniteBullets = bInfinite; }

	int GetFirespeed() { return m_iFireSpeed; }
	void SetFirespeed(int fs) { m_iFireSpeed = fs; }

	float GetFireDelay() { return m_fFireDelay; }
	void SetFireDelay(float fd) { m_fFireDelay = fd; }

	float GetRecoilMin() { return m_fRecoilMin; }
	float GetRecoilMax() { return m_fRecoilMax; }
	void SetRecoilMin(float fRecoilMin) { m_fRecoilMin = fRecoilMin; }
	void SetRecoilMax(float fRecoilMax) { m_fRecoilMax = fRecoilMax; }

	float GetSpreadMax() { return m_fSpreadMax; }
	void SetSpreadMax(float fSpreadMax) { m_fSpreadMax = fSpreadMax; }

	int GetBulletsPerShot() { return m_iBulletsPerShot; }
	void SetBulletsPerShot(int bps) { m_iBulletsPerShot = bps; }

	int GetBulletDamage() { return m_iBulletDamage; }
	void SetBulletDamage(int iDamage) { m_iBulletDamage = iDamage; }

	bool SetHoldOffsetX(int x) { m_vHoldOffset.x = x; }
	bool SetHoldOffsetY(int y) { m_vHoldOffset.y = y; }
	sf::Vector2i GetHoldOffset() { return m_vHoldOffset; }

	bool SetFireOffsetX(int x) { m_vFireOffset.x = x; }
	bool SetFireOffsetY(int y) { m_vFireOffset.y = y; }
	sf::Vector2i GetFireOffset() { return m_vFireOffset; }

	bool SetCasingOffsetX(int x) { m_vCasingOffset.x = x; }
	bool SetCasingOffsetY(int y) { m_vCasingOffset.y = y; }
	sf::Vector2i GetCasingOffset() { return m_vCasingOffset; }

	void SetCasingTextureFromFile(char* szCasingTexture) { m_sCasingAsset = szCasingTexture; }
	std::string GetCasingTextureString() { return m_sCasingAsset; }
	void SetCasingTypeRifle();
	void SetCasingTypeShotgun();
	void SetCasingTypePistol();

	void SetCanCreateCasing(bool bCreateCasing) { m_bCanCreateCasing = bCreateCasing; }
	bool GetCanCreateCasing() { return m_bCanCreateCasing; }

	void SetCasingSize(sf::Vector2i vCasingSize) { m_vCasingSize = vCasingSize; }
	sf::Vector2i GetCasingSize() { return m_vCasingSize; }

	void SetCasingAngularVelocity(float fAngVel) { m_fCasingEjectAngularSpeed = fAngVel; }
	float GetCasingAngularVelocity() { return m_fCasingEjectAngularSpeed; }

	void SetAffectedByGravity(bool bGravity) { m_bAffectedByGravity = bGravity; };
	bool GetAffectedByGravity() { return m_bAffectedByGravity; };

	CSpentCasingEffect* CreateCasingEffect(CWorldPhysics* pPhysics, sf::Vector2i vPos, sf::Vector2f vEjectDirection, float fEjectRotation);
};

class C6SAk47 : public CBaseProjectileWeapon {
public:
	void Init(CWorldPhysics* pPhysics, sf::Vector2f pos, CGame* pGame);
	CSpentCasingEffect* CreateCasingEffect(CWorldPhysics* pPhysics, sf::Vector2i vPos, sf::Vector2f vEjectDirection, float fEjectRotation);
};

class C6SSawnOff : public CBaseProjectileWeapon {
public:
	void Init(CWorldPhysics* pPhysics, sf::Vector2i pos, CGame* pGame);

	/* Doesn't create casings on shot */
	bool CanCreateCasing() { return false; };

	CSpentCasingEffect* CreateCasingEffect(CWorldPhysics* pPhysics, sf::Vector2i vPos, sf::Vector2f vEjectDirection, float fEjectRotation) {
		CDebugLogger::LogError("[SHOTGUN] CreateCasingEffect called on caseless weapon!\n");
		return new CSpentCasingEffect();
	};
};

class C6STMP : public CBaseProjectileWeapon {
public:
	void Init(CWorldPhysics* pPhysics, sf::Vector2i pos, CGame* pGame);
	CSpentCasingEffect* CreateCasingEffect(CWorldPhysics* pPhysics, sf::Vector2i vPos, sf::Vector2f vEjectDirection, float fEjectRotation);
};
#endif