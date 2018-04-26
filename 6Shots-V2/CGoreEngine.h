#ifndef __CGOREENGINE_H__
#define __CGOREENGINE_H__

#include <SFML\Graphics.hpp>
#include "Vector.h"
#include <vector>
#include "CEntityPhysics.h"
#include <Box2D\Box2D.h>
class CGame;
class CWorldPhysics;
class CGoreEngine : public b2ContactListener {
public:
	struct SAirBloodSplatterParticle {
		Vector2i vPos;
		float fRemainingTime;
		int iMaxSize;
		int iMinSize;
		float fTotalLifespan;
		SAirBloodSplatterParticle(Vector2i _vPos, float fLifetime, int _iMinSize, int _iMaxSize) { vPos = _vPos; fRemainingTime = fLifetime; fTotalLifespan = fLifetime; iMaxSize = _iMaxSize; iMinSize = _iMinSize; }
	};
	struct SAirBloodSplatter : public CEntityPhysics {
		Vector2f vPos;
		Vector2f vLastParticlePos;
		float fParticleLifetime;
		int iRadius;
		bool bShouldRemove;
		/* Create splatter w/ physics*/
		SAirBloodSplatter(Vector2i _vPos, Vector2f _vVelocity, float _fParticleLifetime, int _iSize, CWorldPhysics* pPhysics);

		/* Update position from physics simulation */
		void UpdatePosition();

		/* Destroy physics */
		void DestroyPhysics(CWorldPhysics* pPhysics);
	};
private:
	bool m_bLoaded = false;

	/* Saved physics */
	CWorldPhysics* m_pPhysics;

	/* Floor blood splatters */
	sf::Texture m_FloorBloodTexture;
	Vector2i m_vFloorBloodTextureSize;
	std::vector<Vector2i> m_vFloorBlood;

	/* Damage effect (Generic red screen flash) */
	const float MAX_DAMAGE_EFFECT_ALPHA = 0.2f;
	float m_fLocalDamageEffectTime = 0.0f;
	float m_fLocalDamageEffectDuration = 0.9f;
	bool m_bLocalDamageEffectActive = false;

	/* Gore */
	sf::Texture m_GoreTexture;
	Vector2i m_vGoreTextureSize;
	std::vector<Vector2i> m_vGore;

	/* B*/
	
	const float m_fDistancePerParticle = 0.1f;
	std::vector<SAirBloodSplatterParticle> m_vSplatterParticles;
	
	/* In-air blood splatter */
	std::vector<SAirBloodSplatter*> m_vAirBloodSplatters;

	/* Box2D Collision Override */
	virtual void BeginContact(b2Contact* contact);

public:
	CGoreEngine() {}
	bool IsInit() { return m_bLoaded; }
	bool Init(char* szFloorBloodTexture, CWorldPhysics* pPhysics);
	void ApplyFloorBlood(Vector2i vfloorPosition);
	void CreateGoreEffect(CWorldPhysics* pPhysics, Vector2i vGorePosition, Vector2f vGoreVelocity);
	void CreateBloodSplatter(CWorldPhysics* pPhysics, Vector2i vPosition, float fParticleLifetime, int iSize, Vector2f vVelocity);
	void ApplyDamageEffect(CGame* pGame, float fDuration = 0.9f);
	void Render(CGame* pGame);
	void OverlayRender(CGame* pGame);
	void Update(CGame* pGame, CWorldPhysics* pPhysics, float fFrameTime);

	void ClearBloodEffects(CWorldPhysics* pWorldPhysics);
};

#endif