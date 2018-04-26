#include "stdafx.h"
#include "CSpentCasingEffect.h"
#include "CGame.h"
#include "CDebugLogger.h"

void CSpentCasingEffect::InitPhysics(CWorldPhysics* pWorldPhysics, Vector2i physicsPosition, Vector2f physicsVelocity, float fRotation, float fAngularRotation, Vector2i vSize) {
	/* Initialise physics to ignore all collisions except walls */
	m_Physics.InitPhysics(pWorldPhysics, physicsPosition.X, physicsPosition.Y, vSize.X, vSize.Y,
		CEntityPhysics::CATEGORY_PHYSICAL_EFFECT, ~(CEntityPhysics::CATEGORY_DEFAULT) );

	m_Physics.SetRotationalVelocity(fAngularRotation);
	m_Physics.SetVelocity(physicsVelocity);
}
void CSpentCasingEffect::Init(CWorldPhysics* pPhysics, char* szTexture, Vector2i vSize, Vector2i vPosition,
	Vector2f vVelocity, float fRotation, float fAngularRotation) {
	if (!m_Texture.loadFromFile(szTexture)) {
		CDebugLogger::LogWarning("[SPENTCASINGEFFECT] Unable to load texture %s, casing not loaded\n", szTexture);
		return;
	}
	m_fRotation = fRotation;
	m_fDrawRotation = m_fRotation * 57.2958f;

	m_vLocation = Vector2f((float)vPosition.X, (float)vPosition.Y);
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
		/* Update physics position */
		Vector2f pos = m_Physics.GetPosition();
		m_vLocation = pos;

		/* Update physics rotation */
		float rot = m_Physics.GetRotation();
		m_fRotation = rot;
		m_fDrawRotation = m_fRotation * 57.2958f;
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

	pGame->m_Drawing.DrawSpriteCentred(pGame->GetWindowMgr(), (int)(m_vLocation.X), (int)(m_vLocation.Y), m_vSize.X, m_vSize.Y, fAlpha, m_fDrawRotation, &m_Texture);
}