#include "stdafx.h"
#include "CSpentCasingEffect.h"
#include "CGame.h"
#include "CDebugLogger.h"

void CSpentCasingEffect::InitPhysics(CWorldPhysics* pWorldPhysics, sf::Vector2i physicsPosition, sf::Vector2f physicsVelocity, float fRotation, float fAngularRotation, sf::Vector2i vSize) {
	/* Initialise physics to ignore all collisions except walls */
	m_Physics.InitPhysics(pWorldPhysics, physicsPosition.x, physicsPosition.y, vSize.x, vSize.y,
		CEntityPhysics::CATEGORY_PHYSICAL_EFFECT, ~(CEntityPhysics::CATEGORY_DEFAULT) );

	m_Physics.SetRotationalVelocity(fAngularRotation);
	m_Physics.SetVelocity(physicsVelocity);
}
void CSpentCasingEffect::Init(CWorldPhysics* pPhysics, char* szTexture, sf::Vector2i vSize, sf::Vector2i vPosition,
	sf::Vector2f vVelocity, float fRotation, float fAngularRotation) {
	if (!m_Texture.loadFromFile(szTexture)) {
		CDebugLogger::LogWarning("[SPENTCASINGEFFECT] Unable to load texture %s, casing not loaded\n", szTexture);
		return;
	}
	m_fRotation = fRotation;
	m_fDrawRotation = m_fRotation * 57.2958f;

	m_vLocation = sf::Vector2f(vPosition);
	m_vSize = vSize;
	InitPhysics(pPhysics, vPosition, vVelocity, fRotation, fAngularRotation, vSize);
	m_bInit = true;

	m_fLifeTime = CASING_LIFETIME;
}
void CSpentCasingEffect::Update(float fTimeDelta, CWorldPhysics* pWorldPhysics) {
	if (!m_bInit || m_bDestroyed)
		return;
	m_fLifeTime -= fTimeDelta; 

	/* Remove physics on death */
	if (m_fLifeTime < 0 && !m_bDestroyed) {
		m_Physics.DestroyPhysics(pWorldPhysics);
		m_bDestroyed = true;
		return;
	}

	if (m_Physics.IsActive()) {
		/* Sync effect to physics object */
		m_vLocation = m_Physics.GetPosition();
		m_fRotation = m_Physics.GetRotationRadian();
		m_fDrawRotation = m_Physics.GetRotationDegree();
	}
}

bool CSpentCasingEffect::ShouldRemove() {
	if (!m_bInit)
		return true;
	return m_bDestroyed;
}

void CSpentCasingEffect::Draw(CGame* pGame) {
	if (!m_bInit)
		return;

	/* Calculate fade alpha */
	float fAlpha = 1.0f;
	if (m_fLifeTime < FADE_START) {
		fAlpha = m_fLifeTime / FADE_START;
	}

	pGame->m_Drawing.DrawSpriteCentred(pGame->GetWindowMgr(), (int)m_vLocation.x, (int)m_vLocation.y, m_vSize.x, m_vSize.y, fAlpha, m_fDrawRotation, &m_Texture);
}