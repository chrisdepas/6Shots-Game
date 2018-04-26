#ifndef __C6SNETWORKEDPLAYER_H__
#define __C6SNETWORKEDPLAYER_H__

#include "CBaseCombatEntity.h"
#include "CPlayerPhysics.h"
#include "CPlayerHand.h"
#include "CBaseWeapon.h"
#include "CAnimatedSprite.h"

class CEntityManager;
class C6SBaseNetworkedWeapon;
class C6SNetworkedPlayer : public CBaseCombatEntity {
	/* Network ID */
	int m_iClientID;
	/* Player Physics */
	CPlayerPhysics m_PlayerPhysics;
	/* Hand */
	CPlayerHand m_Hand;
	/* Held weapon */
	C6SBaseNetworkedWeapon* m_pHeldWeapon;
	/* Texture */
	CAnimatedSprite m_PlayerSprite;
	//sf::Texture m_TestTexture;

	/**/
	Vector2i m_vSize; 

	bool m_bWalking;

public:

	/* Init networked player */
	void Initialize(int m_iClientID, CWorldPhysics* pPhysics, char* playerSprite, char* handTexture, int handSize, int height, int width, Vector2i location);
	
	/* Update networked player & hand */
	void NetworkUpdate(Vector2f vLocation, Vector2f vHand, float fHandRotation, bool bLeftFacing, bool bWalking);
	
	/* Update player */
	void Update(float fElapsedTime, CGame* pGame, C6SMap* map, CProjectileManager* pProj, CWorldPhysics* pPhysics);

	/* Networked input handling */
	void HandleInput(CGame* pGame, CEntityManager* pEntMgr, bool bInputEnabled);

	/* Draw networked player */
	void Draw(CGame* pGame);

	/* Networked death and respawn */
	void NetworkedRespawn(CGame* pGame);
	void NetworkedOnDeath(CGame* pGame);

	/* Networked weapon functions */
	bool NetworkedPickupWeapon(C6SBaseNetworkedWeapon* pWeapon, CEntityManager* pEntMgr);
	bool NetworkedDropWeapon(CEntityManager* pEntMgr, Vector2f vDropVelocity, Vector2f vDropPosition);
	bool HasWeapon();

	/* Weapon functions */
	bool ShootWeapon(CGame* pGame, CEntityManager* pEntMgr, CWorldPhysics* pWorldPhysics);
	bool CanShootWeapon();
	Vector2f GetShootPosition();
	Vector2f GetShootVelocity();
	int GetWeaponDamage();

	Vector2f GetHandPosition();
	
	int GetClientID();
};

#endif