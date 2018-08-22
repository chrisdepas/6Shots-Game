#ifndef __C6SPLAYER_H__
#define __C6SPLAYER_H__

#include "CBaseCombatEntity.h"
#include "CInput.h"
#include "CPlayerHand.h"
#include "CGlobal.h"
#include "CBaseWeapon.h"
#include "CAnimatedSprite.h"
#include "CEntityManager.h"
#include "CPlayerPhysics.h"

class CProjectileManager;
class C6SMap;

/* Time between polling position of hand 
	to calculate velocity */
#define HAND_POLL_TIME 0.01f

/* Health */
#define DEFAULT_HP 100

class C6SPlayer : public CBaseCombatEntity {
protected:
	CPlayerPhysics m_PlayerPhysics;
	CAnimatedSprite m_PlayerSprite;

	float m_fMoveSpeed; 
	float m_fJumpSpeed;

	bool m_bMoveLeft;
	bool m_bMoveRight;
	bool m_bJump;
	bool m_bInAir;
	sf::Vector2f m_vVelocity;

	int m_height;
	int m_width;

	C6SBaseNetworkedWeapon* m_pWeapon;
	CPlayerHand m_Hand;
		
	sf::Vector2f m_vHandVelocity;
	sf::Vector2i m_vLastHandPoll;
	sf::Vector2f m_vDeathPosition;
	float m_fLastHandPollTime;

public:
	sf::Vector2f GetHandPosition();
	float GetHandRotation();

	void SetPosition(sf::Vector2f p);
	void SetPosition(sf::Vector2i p);

	void Initialize(CWorldPhysics* pPhysics, char* playerSprite, char* handTexture, int handSize,
		int playerReach, int height, int width, float jumpSpeed, float moveSpeed, sf::Vector2i location, int iNetworkID = 0);

	
	bool IsWeaponEquipped();
	void EquipWeapon(C6SBaseNetworkedWeapon* pWeapon, CEntityManager* pEntMgr);
	void DropWeapon(CEntityManager* pEntMgr);
	int GetWeaponProjectileDamage();

	/* Update */
	void Update(float fElapsedTime, CGame* pGame, CProjectileManager* pProj, CWorldPhysics* pPhysics);
	void UpdateNetworked(float fElapsedTime, CGame* pGame, CProjectileManager* pProj, CWorldPhysics* pPhysics, CEntityManager* pEntMgr);

	/* Handle input */
	void HandleInput(CGame* pGame, CEntityManager* pEntMgr, bool bInputEnabled);
	void HandleInputNetworked(CGame* pGame, CEntityManager* pEntMgr, bool bInputEnabled, float fMaxPickupRange);
	
	void Draw(CGame* pGame);

	sf::Vector2i HandLocation();

	/* Networked death function */
	void NetworkedOnDeath(CGame* pGame, CEntityManager* pEntMgr);

	/* Undoes death changes only */
	void NetworkedRespawn(CGame* pGame);

	/* Networked weapon functions */
	bool NetworkedEquipWeapon(C6SBaseNetworkedWeapon* pWeapon, CEntityManager* pEntMgr);
	bool NetworkedDropWeapon(CEntityManager* pEntMgr, sf::Vector2f vDropPosition, sf::Vector2f vDropVelocity, float fDropRotation);

	/* */
	bool NetworkedAttack(CGame* pGame, CEntityManager* pEntMgr, CWorldPhysics* pPhysics);
};
#endif