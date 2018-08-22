#ifndef __CPROP_H__
#define __CPROP_H__

/*	Christopher De Pasquale
		1 Feb 2017
*/
#include "CBaseCombatEntity.h"
#include "CEntityPhysics.h"

class CBaseProp : public CBaseCombatEntity {
	CEntityPhysics m_Physics; 

	/* Base class, Don't implement */
	CBaseProp();

	
	bool m_bIsInForeground;
	bool m_bDefaultForegroundProp;

	/* Weight */
	int m_iWeight;

	/* As a weapon */
	bool m_bCanBePickedUp;
	int m_iMaxThrowDamage;
	int m_iMaxHitDamage;

public:

	/* Initialise */
	virtual void Init(CWorldPhysics* pPhysics, CGame* pGame) = 0;

	/* Update */
	virtual void Update(CWorldPhysics* pPhysics, CGame* pGame);

};

#endif