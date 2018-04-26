#ifndef __C6SNETWORKEDWEAPON_H__
#define __C6SNETWORKEDWEAPON_H__

/*
	Christopher De Pasquale
	Sometime in 2016 or late 2015

*/
#include "CBaseCombatEntity.h"
#include "CBaseWeapon.h"
#include "CAnimatedSprite.h"
class C6SBaseNetworkedWeapon : public CBaseWeapon {
	/* Network ID */
	int m_iEntityID;

public:
	int GetEntityID() { return m_iEntityID; }
	void SetEntityID(int entID) { m_iEntityID = entID; }


};

#endif