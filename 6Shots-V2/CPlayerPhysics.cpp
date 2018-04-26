#include "stdafx.h"
#include "CPlayerPhysics.h"
#include "CWorldPhysics.h"


CPlayerPhysics::~CPlayerPhysics() {

}


void CPlayerPhysics::InitPhysics(CWorldPhysics* pWorldPhysics, int initPosX, int initPosY, int width, int height, bool bAffectedByGravity, int IDData) {
	b2BodyDef def;
	if (bAffectedByGravity)
		def.type = b2BodyType::b2_dynamicBody;
	else
		def.type = b2BodyType::b2_kinematicBody;
	def.fixedRotation = true; // Don't rotate 
	def.active = true;

	m_pBody = pWorldPhysics->CreateBody(&def);

	b2PolygonShape box;
	box.SetAsBox(width/2.0f, height/2.0f);
	/* Scale down to metres */
	for (int i = 0; i < box.GetVertexCount(); i++) {
		box.m_vertices[i] *= PIXEL_TO_METER;
	}
	b2Fixture* pPlayerFixture = m_pBody->CreateFixture(&box, 1.0f);

	/* Store ID in root fixture data */
	pPlayerFixture->SetUserData((void*)IDData);

	/* Set jumping collisions */
	m_usJumpCategoryBits = CATEGORY_PLAYER;
	m_usJumpMaskBits = ~(CATEGORY_PLAYER_PICKUP); // Ladder removed for ghetto ladder mechanics

	/* Set filter for collisions */
	b2Filter filter = pPlayerFixture->GetFilterData();
	filter.categoryBits = CATEGORY_PLAYER;
	filter.maskBits = ~(CATEGORY_PLAYER_PICKUP | CATEGORY_LADDER | CATEGORY_PLAYER);
	pPlayerFixture->SetFilterData(filter);

	/* Create feet sensor */
	b2CircleShape circle;
	circle.m_radius = (width / 2.0f) * PIXEL_TO_METER; 
	circle.m_p.x = 0;
	circle.m_p.y = (height / 2.0f) * PIXEL_TO_METER;
	m_pFeetSensor = m_pBody->CreateFixture(&circle, 1.0f);
	m_pFeetSensor->SetSensor(true);
	/* Set user data to thisptr for feet sensor */
	m_pFeetSensor->SetUserData(this);

	/* Set to listen to check for foot collision */
	pWorldPhysics->SetContactListener(this);
}


bool CPlayerPhysics::InAir() {
	return !(IsOnGround());
}

bool CPlayerPhysics::IsOnGround() {
	return m_iContacts > 0;
}

// todo: don't count pickups
void CPlayerPhysics::BeginContact(b2Contact* contact) {
	if (!contact)
		return;

	/* Find player feet sensor & other collider */
	b2Fixture* pFootSensor = NULL;
	b2Fixture* pCollideFixture = NULL;
	b2Fixture* fx = contact->GetFixtureA();
	if (fx) {
		if (fx->GetUserData() == this) {
			pFootSensor = fx;
		}
		else {
			pCollideFixture = fx;
		}
	}
	fx = contact->GetFixtureB();
	if (fx) {
		if (fx->GetUserData() == this) {
			pFootSensor = fx;
		}
		else {
			pCollideFixture = fx;
		}
	}

	if (pFootSensor && pCollideFixture) {
		/* Check if objects should collide */
		if ((m_usJumpMaskBits & pCollideFixture->GetFilterData().categoryBits) != 0 &&
			(m_usJumpCategoryBits & pCollideFixture->GetFilterData().maskBits) != 0) {
			m_iContacts++;
		}
	}
}
// todo: don't count pickups
void CPlayerPhysics::EndContact(b2Contact* contact) {
	if (!contact)
		return;

	/* Find player feet sensor & other collider */
	b2Fixture* pFootSensor = NULL;
	b2Fixture* pCollideFixture = NULL;
	b2Fixture* fx = contact->GetFixtureA();
	if (fx) {
		if (fx->GetUserData() == this) {
			pFootSensor = fx;
		}
		else {
			pCollideFixture = fx;
		}
	}
	fx = contact->GetFixtureB();
	if (fx) {
		if (fx->GetUserData() == this) {
			pFootSensor = fx;
		}
		else {
			pCollideFixture = fx; 
		}
	}

	if (pFootSensor && pCollideFixture) {
		/* Check if objects should collide */
		if ((m_usJumpMaskBits & pCollideFixture->GetFilterData().categoryBits) != 0 &&
			(m_usJumpCategoryBits & pCollideFixture->GetFilterData().maskBits) != 0) {
			m_iContacts--;
		}
	}
}