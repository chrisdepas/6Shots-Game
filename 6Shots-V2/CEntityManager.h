#ifndef __CENTITYMANAGER_H__
#define __CENTITYMANAGER_H__
/*
	Christopher De Pasquale
	Mid 2016
*/

#include "CBaseEntity.h"
#include "CBaseWeapon.h"
#include "C6SBaseNetworkedWeapon.h"
#include "C6SMap.h"
#include "CSpentCasingEffect.h" // Might change this up to be CPhysicalEffect if i make any more types 
#include <vector>
class CGame;
class CBaseNPC;

class CEntityManager {
	// Objects
	std::vector<CBaseNPC*> m_NPCs;
	std::vector<C6SBaseNetworkedWeapon*> m_Weapons;
	std::vector<CSpentCasingEffect*> m_Casings;

	void CleanupCasings();
public:
	//
	void AddNPC(CBaseNPC* pEntity);

	/* Ent ID functions */
	int GetNextEntityID();
	bool EntityIDExists(int iEntID);
	
	/* Update entities */
	void Update(float fElapsedTime, CGame* pGame, CWorldPhysics* pPhysics);

	/* Draw entities */
	void Draw(CGame* pGame);

	/* Cleanup and wipe entities */
	void DestroyAllEntities();

	/* Weapon functions */
	void AddWeapon(C6SBaseNetworkedWeapon* pWeapon);
	void RemoveWeapon(C6SBaseNetworkedWeapon* pWeapon);
	C6SBaseNetworkedWeapon* ClosestWeapon(Vector2i pos, float maxDist);
	C6SBaseNetworkedWeapon* ClosestWeapon(Vector2f pos, float maxDist);
	C6SBaseNetworkedWeapon* GetWeaponByID(int iWeaponID);

	/* Spent casings */
	void AddCasingEffect(CSpentCasingEffect* effect);
};

#endif