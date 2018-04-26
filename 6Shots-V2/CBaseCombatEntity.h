#ifndef __CBASECOMBATENTITY_H__
#define __CBASECOMBATENTITY_H__

#define DEFAULT_MAX_HP 100
#define DEFAULT_OVERHEAL_MAX_MULT 1.0f

#include "CBaseEntity.h"
class CBaseCombatEntity : public CBaseEntity {
protected:
	int m_iHP = DEFAULT_MAX_HP;
	int m_iMaxHP = DEFAULT_MAX_HP;
	int m_iScore = 0;
	
public:

	/* Health */
	int GetHP();
	void SetHP(int hp);
	void SetMaxHP(int iHPMax);
	void Heal(int heal);
	void TakeDamage(int damage);
	void Kill();
	bool IsDead();
	
	/* Score */
	void AddScore(int iScoreAmt = 1);
	int GetScore();
};
#endif