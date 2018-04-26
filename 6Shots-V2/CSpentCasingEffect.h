#ifndef __CSPENTCASINGEFFECT_H__
#define __CSPENTCASINGEFFECT_H__

#include "CWorldPhysics.h"
#include "CEntityPhysics.h"
#include <vector>
class CGame;

class CSpentCasingEffect {
	float m_fLifeTime;
	bool m_bInit;
	bool m_bDestroyed;
	sf::Texture m_Texture;
	CEntityPhysics m_Physics;
	Vector2f m_vLocation;
	Vector2i m_vSize;
	float m_fRotation;
	float m_fDrawRotation;

	/* Lifetime of casings in world */
	const float CASING_LIFETIME = 8.0f;
	/* Time when casing image starts fading */ 
	const float FADE_START = 1.0f;

	/* Initialise physics - use this in overloaded init */
	void InitPhysics(CWorldPhysics* pWorldPhysics, Vector2i physicsPosition, 
		Vector2f physicsVelocity, float fRotation, float fAngularRotation, Vector2i vSize); 
public:
	CSpentCasingEffect() {
		m_fLifeTime = CASING_LIFETIME;
		m_bInit = false;
		m_bDestroyed = false;
	}

	/* Overloaded = operator to allow std::erase usage */
	CSpentCasingEffect& operator=(CSpentCasingEffect other) {
		m_bDestroyed = other.m_bDestroyed;
		m_bInit = other.m_bInit;
		m_bDestroyed = other.m_bDestroyed;
		m_Texture = other.m_Texture;
		m_Physics = other.m_Physics;
		m_vLocation = other.m_vLocation;
		m_vSize = other.m_vSize;
		m_fRotation = other.m_fRotation;
		m_fDrawRotation = other.m_fDrawRotation;
		return *this;
	}

	/*  */
	void Init(CWorldPhysics* pPhysics, char* szTexture, Vector2i vSize, Vector2i vPosition, 
		Vector2f vVelocity, float fRotation, float fAngularRotation);

	/* Update physics, destroys physics when time is up */
	void Update(float fTimeDelta, CWorldPhysics* pWorldPhysics);

	/* Returns true if casing should be removed from world */
	bool ShouldRemove();

	/* Default drawing */
	void Draw(CGame* pGame);
};

#endif