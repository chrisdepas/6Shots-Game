#ifndef __CPLAYERPHYSICS_H__
#define __CPLAYERPHYSICS_H__

/*
	Christopher De Pasquale
	Late 2016
	Noticed added feb 2017

*/


#include "CEntityPhysics.h"

class CWorldPhysics;

class CPlayerPhysics : public CEntityPhysics, public b2ContactListener {
	b2Fixture* m_pFeetSensor;
	int m_iContacts;

	bool ShouldCollideWithFixture(b2Fixture* pFixture);

	/* Type used in checking collision */
	uint16 m_usJumpMaskBits; /* Mask used for jump collisions */
	uint16 m_usJumpCategoryBits; /* Type of object used for jump collisions */

public:
	CPlayerPhysics() {
		m_iContacts = 0;
		m_pFeetSensor = 0;
	}
	~CPlayerPhysics();

	void InitPhysics(CWorldPhysics* pWorldPhysics, int initPosX, int initPosY, int width, int height, bool bAffectedByGravity = true, int iIDData = 0);
	bool IsOnGround();
	bool InAir();

	/* Box2D Contact callbacks */
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);

	/* Workarounds */
	void ResetGroundContact() { m_iContacts = 0; }

};
#endif