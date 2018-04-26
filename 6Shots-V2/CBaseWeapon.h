#ifndef __CBASEWEAPON_H__
#define __CBASEWEAPON_H__

/*
	Christopher De Pasquale
	Sometime in 2016 or late 2015

*/
#include "CBaseEntity.h"
#include "Vector.h"
#include "CProjectileManager.h"
#include "CEntityPhysics.h"
#include "CSpentCasingEffect.h"
#include "CDebugLogger.h"

class CGame;
class CEntityManager;

class CBaseWeapon : public CBaseEntity { 
protected:
	char* m_szName;
	bool m_bIsProjectileWeapon;
	Vector2i m_vHoldOffset;	// Offset for when gun is aimed left 
	bool m_bFiring;
	float m_fTrueRotation;
	float m_fDrawRotation;
	float m_fRecoilRotation; 
	CEntityPhysics m_Physics;
	
public:

	CBaseWeapon();

	virtual void DefaultDraw(CGame* pGame);

	/* Attack effects, doesn't create bullet */
	virtual void Attack(CGame* pGame, CEntityManager* pEntMgr, CWorldPhysics* pPhysics) = 0;

	virtual bool CanCreateCasing() = 0;
	virtual CSpentCasingEffect* CreateCasingEffect(CWorldPhysics* pPhysics, Vector2i vPos, Vector2f vEjectDirection, float fEjectRotation) = 0;
	virtual Vector2f GetCasingCreateOffset() = 0;
	virtual Vector2f GetCasingEjectDirection() = 0;

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


	/* Update locally only */
	virtual void Update(CWorldPhysics* pPhysics, CGame* pGame, CProjectileManager* pProj, float fElapsedTime) = 0;
	/* Update over a network (Includes server/client) */
	virtual void NetworkedUpdate(CWorldPhysics* pPhysics, CGame* pGame, CProjectileManager* pProj, CEntityManager* pEntMgr, float fElapsedTime) = 0;
	 
	void UpdatePhysicsPosition();

	void OnPickup();
	void OnDrop(Vector2f dropVelocity);

	float GetTrueRotation();
	void AttackPress();
	void AttackRelease();

	virtual int GetClipAmmo() { return 0; }
	virtual int GetMaxAmmo() { return 0; }

	Vector2i GetHoldOffset(); 

	void SetTrueRotation(float fRot); // Set rotation
	void SetDrawRotation(float fRot); // Set drawing rotation

	float GetRecoilRotation();
	void AddRecoil(float fRecoilRadian);

	char* GetName();
};

#endif 