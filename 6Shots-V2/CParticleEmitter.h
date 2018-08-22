#ifndef __CPARTICLEEMITTER_H__
#define __CPARTICLEEMITTER_H__

#include "CBaseEntity.h"
#include "CWorldPhysics.h"

class CGame;
class CParticleEmitter {
	struct SParticle {
		float m_fRemainingLife;
		sf::Vector2f m_vPosition;
	};
	
	bool m_bTemporary;	// Whether this should die
	float m_fDieTime;	// Time this emitter destroys

	int m_iEmitRate;	// Particles per second
	float m_fEmitAngle; // Angle to spit out particles
	float m_fEmitAngleDeviation; 
	int m_iLastEmitSecond;

	sf::Vector2f m_vPosition;
	CBaseEntity* m_pAttachEnt;
	/* Particle vars */
	float m_fParticleStartSize;
	float m_fParticleEndSize;
	sf::Color m_emitColorStart;
	sf::Color m_emitColorEnd;
	float m_fParticleSpeed;

	// 
	std::vector<SParticle> m_vParticles;
public:
	CParticleEmitter(sf::Vector2f position, int rate, float speed, float angle, float angleDeviation, float startSize, float endSize,
		float curTime, sf::Color startColor, sf::Color endColor, float lifespan = -1.0f, CBaseEntity* pAttachEnt = NULL);
	void Draw(CGame* pGame);
	void Update(float curTime, CWorldPhysics* pPhysics);
	void Attach(CBaseEntity* pAttachEnt);
};

#endif