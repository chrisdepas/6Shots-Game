#ifndef __CBASEENTITY_H__
#define __CBASEENTITY_H__

#include "SFML\Graphics.hpp"
#include "Vector.h"
class CGame;

class CBaseEntity {
	
protected:
	bool m_bFacingRight;
	bool m_bAffectedByGravity;
	Vector2f m_vVelocity;
	Vector2f m_vLocation;
	Vector2i m_vSize;
	float m_fScale;
	sf::Texture m_texture;
	int m_Flags;
public:

	enum EBaseFlags {
		FL_FACING_LEFT =	(1 << 0), 
		FL_WALKING =		(1 << 1),
	};

	Vector2f GetVelocity();
	Vector2f GetPosition2f();
	Vector2i GetPosition2i();
	CBaseEntity();

	void SetBaseFlag(EBaseFlags f, bool b);
	bool GetBaseFlag(EBaseFlags f);

	void SetVelocity(Vector2f v);
	void SetPosition(Vector2f p);
	void SetPosition(Vector2i p);
	void DefaultDraw(CGame* pGame);

	virtual void Draw(CGame* pGame){}
};
#endif