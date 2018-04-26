#ifndef __CGLOBAL_H__
#define __CGLOBAL_H__

// global game settings and shit here like gravity
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
