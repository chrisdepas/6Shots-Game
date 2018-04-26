#include "stdafx.h"
#include "CParticleEmitter.h"

CParticleEmitter::CParticleEmitter(Vector2f position, int rate, float speed, float angle, float angleDeviation, float startSize, float endSize, 
	float curTime, sf::Color startColor, sf::Color endColor, float lifespan, CBaseEntity* pAttachEnt) {
	m_bTemporary = lifespan == -1.0f;
	if (m_bTemporary) {
		m_fDieTime = curTime + lifespan;
	}
	m_iEmitRate = rate;
	m_fParticleSpeed = speed;
	m_fEmitAngle = angle;
	m_fEmitAngleDeviation = angleDeviation;
	m_fParticleStartSize = startSize;
	m_fParticleEndSize = endSize;
	m_emitColorStart = startColor;
	m_emitColorEnd = endColor;
	m_vPosition = position;
	m_pAttachEnt = pAttachEnt;
}
void CParticleEmitter::Draw(CGame* pGame) {
	Vector2f pos;
	for (unsigned int i = 0; i < m_vParticles.size(); i++) {

	}
}
void CParticleEmitter::Update(float fDelta, CWorldPhysics* pPhysics) {
	if (m_pAttachEnt) {
		m_vPosition = m_pAttachEnt->GetPosition2f(); 
	}

	// Update entity positions, delete old ents
	for (unsigned int i = 0; i < m_vParticles.size(); i++) {
		m_vParticles[i].m_fRemainingLife -= fDelta;
		if (m_vParticles[i].m_fRemainingLife <= 0.0f) {
		}
	//	pos = m_vParticles[i].m_vPosition;
	//	pos.X += m_fParticleSpeed *
	}
}