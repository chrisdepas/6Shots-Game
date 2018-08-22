#ifndef __CBASEWEAPON_H__
#define __CBASEWEAPON_H__

/*
	Christopher De Pasquale
	Sometime in 2016 or late 2015

*/
#include "CBaseEntity.h"

#include "CProjectileManager.h"
#include "CEntityPhysics.h"
#include "CSpentCasingEffect.h"
#include "CDebugLogger.h"

class CGame;
class CEntityManager;

class CBaseWeapon : public CBaseEntity { 
protected:
	char* m_szName = NULL;
	bool m_bIsProjectileWeapon = false;
	bool m_bTriggerPrimary = false;
	bool m_bTriggerSecondary = false;

	// Offset for when gun is aimed left 
	sf::Vector2i m_vHoldOffset = sf::Vector2i(0, 0);

	float m_fTrueRotation = 0.0f;
	float m_fDrawRotation = 0.0f;
	float m_fRecoilRotation = 0.0f;
	CEntityPhysics m_Physics;
	
public:

	virtual void DefaultDraw(CGame* pGame);

	/* Attack effects, doesn't create bullet */
	virtual void Attack(CGame* pGame, CEntityManager* pEntMgr, CWorldPhysics* pPhysics) = 0;

	virtual bool CanCreateCasing() = 0;
	virtual CSpentCasingEffect* CreateCasingEffect(CWorldPhysics* pPhysics, sf::Vector2i vPos, sf::Vector2f vEjectDirection, float fEjectRotation) = 0;
	virtual sf::Vector2f GetCasingCreateOffset() = 0;
	virtual sf::Vector2f GetCasingEjectDirection() = 0;

	virtual float GetRandomRecoil(CGame* pGame) {
		CDebugLogger::LogError("[BASEWEAPON] GetRandomRecoil not implemented!\n");
		return 0.0f;
	}
	virtual float GetRandomSpread(CGame* pGame) {
		CDebugLogger::LogError("[BASEWEAPON] GetRandomSpread not implemented!\n");
		return 0.0f;
	}


	virtual bool IsProjectileWeapon();
	virtual bool IsMeleeWeapon();
	virtual int GetProjectileDamage() {
		CDebugLogger::LogError("[BASEWEAPON] GetProjectileDamage not implemented!\n");
		return 0;
	}

//	virtual void Attack2() = 0;
//	virtual bool CanAttack() = 0;
//	virtual bool IsProjectileWeapon() = 0;

	void AttackPress() { m_bTriggerPrimary = true; }
	void AttackRelease()  { m_bTriggerPrimary = false; }
	bool TriggerPrimaryHeld() { return m_bTriggerPrimary; }

	void AltPress() { m_bTriggerSecondary = true; }
	void AltRelease()  { m_bTriggerSecondary = false; }
	bool TriggerSecondaryHeld() { return m_bTriggerSecondary; }


	/* Update locally only */
	virtual void Update(CWorldPhysics* pPhysics, CGame* pGame, CProjectileManager* pProj, float fElapsedTime) = 0;

	/* Update over a network (Includes server/client) */
	virtual void NetworkedUpdate(CWorldPhysics* pPhysics, CGame* pGame, CProjectileManager* pProj, CEntityManager* pEntMgr, float fElapsedTime) = 0;
	 
	void UpdatePhysicsPosition();

	void OnPickup();
	void OnDrop(sf::Vector2f dropVelocity);

	float GetTrueRotation() { return m_fTrueRotation; }

	virtual int GetClipAmmo() { return 0; }
	virtual int GetMaxAmmo() { return 0; }

	sf::Vector2i GetHoldOffset();

	void SetTrueRotation(float fRot); // Set rotation
	void SetDrawRotation(float fRot); // Set drawing rotation

	float GetRecoilRotation();
	void AddRecoil(float fRecoilRadian);

	char* GetName();
};

#endif 