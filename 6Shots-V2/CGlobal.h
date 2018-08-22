#ifndef __CGLOBAL_H__
#define __CGLOBAL_H__
/*
	Chris De Pasquale
	Created		Sometime in 2016
	Last Edit	16/05/2018

*/
class CGlobal {
	
	float m_fGravity;
	float m_fTerminalVelocity;

public:
	void Init(float gravity, float terminal);
	float GetGravity();
	void SetGravity(float gravity);

	float GetTerminalVelocity();
};

#endif
