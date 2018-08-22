#include "stdafx.h"
#include "CGoreEngine.h"
#include "CGame.h"
#include "CWorldPhysics.h"

const sf::Color CGoreEngine::GORE_COLOR = sf::Color(197, 16, 26, 200);

CGoreEngine::SAirBloodSplatter::SAirBloodSplatter(sf::Vector2i _vPos, sf::Vector2f _vVelocity, float _fParticleLifetime, int _iRadius, CWorldPhysics* pPhysics) {
	/* Initialise member data */
	vPos = sf::Vector2f(_vPos);
	vLastParticlePos = vPos;
	iRadius = _iRadius;
	fParticleLifetime = _fParticleLifetime;
	bShouldRemove = false;

	/* Calculate radius of blood */
	float fRadius = (float)_iRadius * PIXEL_TO_METER;

	/* Create physics body */
	b2BodyDef def;
	def.type = b2BodyType::b2_dynamicBody;
	def.fixedRotation = true; 
	def.active = true;
	m_pBody = pPhysics->CreateBody(&def);

	/* Create physics shape */
	b2CircleShape circle;
	circle.m_radius = fRadius;
	b2FixtureDef fixtureDef; 
	fixtureDef.shape = &circle;
	b2Fixture* pPlayerFixture = m_pBody->CreateFixture(&fixtureDef);

	/* Init velocity & location  */
	SetVelocity(_vVelocity);
	SetPosition(_vPos);

	/* Collide with world only */
	b2Filter filter = pPlayerFixture->GetFilterData();
	filter.categoryBits = CATEGORY_BLOOD_AND_GORE;
	filter.maskBits = CATEGORY_DEFAULT;
	pPlayerFixture->SetFilterData(filter);

	/* Store splatter ptr */ 
	pPlayerFixture->SetUserData(this);
}
void CGoreEngine::SAirBloodSplatter::UpdatePosition() {
	/* Get pixel transformed physics position */
	vPos = GetPosition();
}
void CGoreEngine::SAirBloodSplatter::DestroyPhysics(CWorldPhysics* pPhysics) {
	/* Remove physics object from world */
	pPhysics->DestroyBody(this->m_pBody); 
}

/* Box2D Collision Override */
void CGoreEngine::BeginContact(b2Contact* contact) {
	b2Fixture* pFixA = contact->GetFixtureA();
	b2Fixture* pFixB = contact->GetFixtureB();

	/* what was i thinking ? todo: fix below */
	SAirBloodSplatter* pSplatterA = (SAirBloodSplatter*)pFixA->GetUserData();
	SAirBloodSplatter* pSplatterB = (SAirBloodSplatter*)pFixB->GetUserData();
	if (pSplatterA && pSplatterB)
		return;
	SAirBloodSplatter* pSplatter = 0;
	if (pSplatterA && pFixA->GetFilterData().categoryBits == CEntityPhysics::CATEGORY_BLOOD_AND_GORE)
		pSplatter = pSplatterA;
	if (pSplatterB && pFixB->GetFilterData().categoryBits == CEntityPhysics::CATEGORY_BLOOD_AND_GORE)
		pSplatter = pSplatterB;
	if (!pSplatter)
		return;

	/* Already requires destruction */
	if (pSplatter->bShouldRemove == true)
		return;

	pSplatter->bShouldRemove = true;
	/* Remove blood splatter && splatter physics body */

	b2WorldManifold manifold;
	contact->GetWorldManifold(&manifold);

	sf::Vector2i floorPos(
		(int)(manifold.points[0].x*METER_TO_PIXEL), 
		(int)(manifold.points[0].y*METER_TO_PIXEL));
	ApplyFloorBlood(floorPos);
	/* Check if there is floor blood too close */
	

	/* No floor blood, spawn */ 

	/* Free splatter memory */
}

bool CGoreEngine::Init(char* szFloorBloodTexture, CWorldPhysics* pPhysics) {
	/* Load texture data */
	m_bLoaded = m_FloorBloodTexture.loadFromFile(szFloorBloodTexture);
	m_vFloorBloodTextureSize = sf::Vector2i(m_FloorBloodTexture.getSize());

	if (m_bLoaded) {
		/* Set listener if successfully loaded */
		pPhysics->SetContactListener(this);

		/* Save physics */
		m_pPhysics = pPhysics; 
	}

	return m_bLoaded && m_pPhysics;
}
void CGoreEngine::ApplyFloorBlood(sf::Vector2i floorPosition) {
	if (m_bLoaded) { 
		m_vFloorBlood.push_back(floorPosition);
	}
}
void CGoreEngine::CreateGoreEffect(CWorldPhysics* pPhysics, sf::Vector2i vGorePosition, sf::Vector2f vGoreVelocity) {
	//pPhysics->CreateBody()
}

void CGoreEngine::CreateBloodSplatter(CWorldPhysics* pPhysics, sf::Vector2i vPosition, float fLifetime, int iSize, sf::Vector2f vVelocity) {
	m_vAirBloodSplatters.push_back(new SAirBloodSplatter(vPosition, vVelocity, fLifetime, iSize, pPhysics));
}  
void CGoreEngine::ApplyDamageEffect(CGame* pGame, float fDuration) {
	m_fLocalDamageEffectTime = m_fLocalDamageEffectDuration = fDuration;
	m_bLocalDamageEffectActive = true;
}
void CGoreEngine::Render(CGame* pGame) { 
	if (m_bLoaded) {
		CWindowManager* pWnd = pGame->GetWindowMgr();

		/* Floor blood splatter */
		for (unsigned int i = 0; i < m_vFloorBlood.size(); i++) {
			pGame->m_Drawing.DrawSpriteCentred(pWnd, m_vFloorBlood[i], m_vFloorBloodTextureSize, &m_FloorBloodTexture);
		}

		/* Air blood splatters (Head) */
		for (unsigned int i = 0; i < m_vAirBloodSplatters.size(); i++) {
			if (m_vAirBloodSplatters[i]->bShouldRemove)
				continue;
			pGame->m_Drawing.DrawCircleCentred(pWnd, m_vAirBloodSplatters[i]->vPos, (float)m_vAirBloodSplatters[i]->iRadius, GORE_COLOR);
		}

		/* Blood splatter particles (tail) */
		for (unsigned int i = 0; i < m_vSplatterParticles.size(); i++) { 
			int sizeDiff = m_vSplatterParticles[i].iMaxSize - m_vSplatterParticles[i].iMinSize;
			float percentLifetimeRemaining = m_vSplatterParticles[i].fRemainingTime / m_vSplatterParticles[i].fTotalLifespan;
			float drawSize = m_vSplatterParticles[i].iMinSize + (float)sizeDiff * percentLifetimeRemaining;
			pGame->m_Drawing.DrawCircleCentred(pWnd, sf::Vector2f(m_vSplatterParticles[i].vPos), drawSize, GORE_COLOR);
		} 
	}

	
}
void CGoreEngine::OverlayRender(CGame* pGame) {
	if (m_bLoaded) {
		CWindowManager* pWnd = pGame->GetWindowMgr();

		/* Red damage overlay */
		if (m_bLocalDamageEffectActive) {
			float fPercentAlpha = (m_fLocalDamageEffectTime / m_fLocalDamageEffectDuration);
			if (fPercentAlpha > MAX_DAMAGE_EFFECT_ALPHA)
				fPercentAlpha = MAX_DAMAGE_EFFECT_ALPHA;
			pGame->m_Drawing.DrawRectangleCentred(pWnd, 
				pWnd->GetScreenCentre2f(),
				pWnd->GetScreenDimensions2f(), 
				sf::Color(255, 0, 0, (sf::Uint8)(fPercentAlpha*255.0f)));
		}
	}
}

/* Predicate for expired particles */
bool ParticleExpired(CGoreEngine::SAirBloodSplatterParticle& p) {
	return p.fRemainingTime <= 0.0f;
}
bool SplatterRequiresRemoval(CGoreEngine::SAirBloodSplatter* p) {
	return p->bShouldRemove;
}

void CGoreEngine::Update(CGame* pGame, CWorldPhysics* pPhysics, float fFrameTime) {
	/* Air blood */
	for (unsigned int i = 0; i < m_vAirBloodSplatters.size(); i++) {
		/* Ignore removed splatters */
		if (m_vAirBloodSplatters[i]->bShouldRemove)
			continue;

		/* Update position */
		m_vAirBloodSplatters[i]->UpdatePosition();

		/* Spawn particle */
		sf::Vector2f delta = m_vAirBloodSplatters[i]->vPos - m_vAirBloodSplatters[i]->vLastParticlePos;
		if (vec::Length(delta) >= m_fDistancePerParticle) {
			/* Calculate particle data */
			int iMaxRad = (int)(0.90f*m_vAirBloodSplatters[i]->iRadius);
			int iMinRad = (int)(0.5f*m_vAirBloodSplatters[i]->iRadius);
			;
			/* add to particle list */
			m_vSplatterParticles.push_back(CGoreEngine::SAirBloodSplatterParticle(
				sf::Vector2i(m_vAirBloodSplatters[i]->vPos),
				m_vAirBloodSplatters[i]->fParticleLifetime, 
				iMinRad, iMaxRad));
		}
	}

	/* Update particle lifetimes */
	for (unsigned int i = 0; i < m_vSplatterParticles.size(); i++) {
		m_vSplatterParticles[i].fRemainingTime -= fFrameTime;
	}

	/* Delete expired particles */
	m_vSplatterParticles.erase(std::remove_if(m_vSplatterParticles.begin(), m_vSplatterParticles.end(), ParticleExpired), m_vSplatterParticles.end());

	/* Free removed splatter memory & destroy physics */
	for (unsigned int i = 0; i < m_vAirBloodSplatters.size(); i++) {
		if (m_vAirBloodSplatters[i]->bShouldRemove) {
			m_vAirBloodSplatters[i]->DestroyPhysics(pPhysics);
			delete m_vAirBloodSplatters[i];
		}
	}

	/* decrease effect duration */
	if (m_bLocalDamageEffectActive) {
		m_fLocalDamageEffectTime -= fFrameTime;
		if (m_fLocalDamageEffectTime <= 0.0f)
			m_bLocalDamageEffectActive = false;
	}

	/* Erase removed splatters */
	m_vAirBloodSplatters.erase(std::remove_if(m_vAirBloodSplatters.begin(), m_vAirBloodSplatters.end(), SplatterRequiresRemoval), m_vAirBloodSplatters.end());
}  

void CGoreEngine::ClearBloodEffects(CWorldPhysics* pWorldPhysics) {
	/* Remove blood splatters */
	for (unsigned int i = 0; i < m_vAirBloodSplatters.size(); i++) {
		if (m_vAirBloodSplatters[i]) {
			m_vAirBloodSplatters[i]->DestroyPhysics(pWorldPhysics);
			delete m_vAirBloodSplatters[i];
		}
	}
	m_vAirBloodSplatters.clear();

	/* Remove all floor blood */
	m_vFloorBlood.clear();
}