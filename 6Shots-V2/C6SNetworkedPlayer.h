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
	sf::Vector2i m_vSize;

	bool m_bWalking;

public:

	/* Init networked player */
	void Initialize(int m_iClientID, CWorldPhysics* pPhysics, char* playerSprite, char* handTexture, int handSize, int height, int width, sf::Vector2i location);
	
	/* Update networked player & hand */
	void NetworkUpdate(sf::Vector2f vLocation, sf::Vector2f vHand, float fHandRotation, bool bLeftFacing, bool bWalking);
	
	/* Update player */
	void Update(float fElapsedTime, CGame* pGame, CProjectileManager* pProj, CWorldPhysics* pPhysics);

	/* Networked input handling */
	void HandleInput(CGame* pGame, CEntityManager* pEntMgr, bool bInputEnabled);

	/* Draw networked player */
	void Draw(CGame* pGame);

	/* Networked death and respawn */
	void NetworkedRespawn(CGame* pGame);
	void NetworkedOnDeath(CGame* pGame);

	/* Networked weapon functions */
	bool NetworkedPickupWeapon(C6SBaseNetworkedWeapon* pWeapon, CEntityManager* pEntMgr);
	bool NetworkedDropWeapon(CEntityManager* pEntMgr, sf::Vector2f vDropVelocity, sf::Vector2f vDropPosition);
	bool HasWeapon();

	/* Weapon functions */
	bool ShootWeapon(CGame* pGame, CEntityManager* pEntMgr, CWorldPhysics* pWorldPhysics);
	bool CanShootWeapon();
	sf::Vector2f GetShootPosition();
	sf::Vector2f GetShootVelocity();
	int GetWeaponDamage();

	sf::Vector2f GetHandPosition();
	
	int GetClientID();
};

#endif