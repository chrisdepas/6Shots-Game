#include "stdafx.h"
#include "CWorldPhysics.h"
#include "CPhysicsDebugDraw.h"
#include "CEntityPhysics.h"

CWorldPhysics::CWorldPhysics() {
	m_pWorldPhysics = 0;
	m_vGravity.x = m_vGravity.y = 0;
	m_bInit = 0;
}

float CWorldPhysics::GetMeterToPixelFactor() {
	return METER_TO_PIXEL;
}
float CWorldPhysics::GetPixelToMeterFactor() {
	return PIXEL_TO_METER;
} 

void CWorldPhysics::Draw() {
	if (m_bInit) {
		m_pWorldPhysics->DrawDebugData();
	}
}

void CWorldPhysics::InitPhysics(CGame* pGame, float fGravity, bool bDebugDraw) {
	if (m_bInit) {
		return;
	}
	m_vGravity.x = 0.0f;
	m_vGravity.y = fGravity;

	/* Create world */
	m_pWorldPhysics = new b2World(m_vGravity);

	/* Init debug drawing if enabled */
	if (!bDebugDraw) {
		m_pDebugDraw = NULL;
	} else {
		m_pDebugDraw = new CPhysicsDebugDraw();
		m_pDebugDraw->Init(pGame);
		int32 flags = 0;
		flags |= b2Draw::e_shapeBit;
		flags |= b2Draw::e_jointBit;
		flags |= b2Draw::e_aabbBit;
		flags |= b2Draw::e_pairBit;
		flags |= b2Draw::e_centerOfMassBit;

		m_pDebugDraw->SetFlags(flags);
		m_pWorldPhysics->SetDebugDraw(m_pDebugDraw);
	}

	/* Listen for contacts */
	m_pWorldPhysics->SetContactListener(this);

	m_bInit = true;
}

#define WALL_THICKNESS 10.0f
void CWorldPhysics::ParseWorldMap(C6SBaseMap* map) {
	assert(m_bInit && m_pWorldPhysics);

	sf::Vector2i mapSize = map->GetSize();

	b2BodyDef boundsDef;
	boundsDef.position.Set(0.0f, 0.0f);
	m_pGround = m_pWorldPhysics->CreateBody(&boundsDef);

	/* Left Wall */
	b2PolygonShape leftWallShape;
	leftWallShape.SetAsBox(WALL_THICKNESS, mapSize.y / 2.0f, b2Vec2(-WALL_THICKNESS, mapSize.y / 2.0f), 0.0f);
	/* Scale down to metres */
	for (int i = 0; i < leftWallShape.GetVertexCount(); i++) {
		leftWallShape.m_vertices[i] *= PIXEL_TO_METER;
	}
	b2FixtureDef leftWallFixtureDef;
	leftWallFixtureDef.shape = &leftWallShape;
	m_pGround->CreateFixture(&leftWallFixtureDef);

	/* Right Wall */
	b2PolygonShape rightWallShape;
	rightWallShape.SetAsBox(WALL_THICKNESS, mapSize.y / 2.0f, b2Vec2(mapSize.x + WALL_THICKNESS, mapSize.y / 2.0f), 0.0f);
	/* Scale down to metres */
	for (int i = 0; i < rightWallShape.GetVertexCount(); i++) {
		rightWallShape.m_vertices[i] *= PIXEL_TO_METER;
	}
	b2FixtureDef rightWallFixtureDef;
	rightWallFixtureDef.shape = &rightWallShape;
	m_pGround->CreateFixture(&rightWallFixtureDef);

	/* Top Wall */
	b2PolygonShape topWallShape;
	topWallShape.SetAsBox(mapSize.x / 2.0f, WALL_THICKNESS, b2Vec2(mapSize.x / 2.0f, -WALL_THICKNESS), 0.0f);
	/* Scale down to metres */
	for (int i = 0; i < topWallShape.GetVertexCount(); i++) {
		topWallShape.m_vertices[i] *= PIXEL_TO_METER;
	}
	b2FixtureDef topWallFixtureDef;
	topWallFixtureDef.shape = &topWallShape;
	m_pGround->CreateFixture(&topWallFixtureDef);

	/* Bottom Wall */
	b2PolygonShape bottomWallShape;
	bottomWallShape.SetAsBox(mapSize.x / 2.0f, WALL_THICKNESS, b2Vec2(mapSize.x / 2.0f,  mapSize.y + WALL_THICKNESS), 0.0f);
	/* Scale down to metres */
	for (int i = 0; i < bottomWallShape.GetVertexCount(); i++) {
		bottomWallShape.m_vertices[i] *= PIXEL_TO_METER;
	}
	b2FixtureDef bottomWallFixtureDef;
	bottomWallFixtureDef.shape = &bottomWallShape;
	m_pGround->CreateFixture(&bottomWallFixtureDef);

	/* Create collision polygons out of map shapes */
	std::vector<C6SBaseMap::SMapShape> vCollisionRects = map->ShapeCompose();

	b2PolygonShape rect;
	for (unsigned int i = 0; i < vCollisionRects.size(); i++) {
		rect.SetAsBox(vCollisionRects[i].vSize.x / 2.0f, vCollisionRects[i].vSize.y / 2.0f, 
			b2Vec2((float)vCollisionRects[i].vCentrePos.x, (float)vCollisionRects[i].vCentrePos.y), 0.0f);

		/* Scale down to metres */
		for (int j = 0; j < rect.GetVertexCount(); j++) {
			rect.m_vertices[j] *= PIXEL_TO_METER;
		}
		b2FixtureDef rectFixtureDef;
		rectFixtureDef.shape = &rect; 

		/* Set category bits on ladder */
		if (vCollisionRects[i].eShapeType == SMapTile::TYPE_LADDER)
			rectFixtureDef.filter.categoryBits = CEntityPhysics::CATEGORY_LADDER;

		m_pGround->CreateFixture(&rectFixtureDef);
	}
	
}

b2Body* CWorldPhysics::CreateBody(b2BodyDef* pDef) {
	return m_pWorldPhysics->CreateBody(pDef);
}
void CWorldPhysics::DestroyBody(b2Body* pBody) {
	m_pWorldPhysics->DestroyBody(pBody);
}
 
/* Insert new contact listener */
void CWorldPhysics::SetContactListener(b2ContactListener* listener) {
	/* Check if contact already exists */
	for (unsigned int i = 0; i < m_pListeners.size(); i++) {
		if (m_pListeners[i] == listener)
			return;
	}

	/* Not a duplicate, insert */
	m_pListeners.push_back(listener);
}

void CWorldPhysics::RemoveContactListener(b2ContactListener* listener) {
	/* remove listener */
	m_pListeners.erase( std::remove(m_pListeners.begin(), m_pListeners.end(), listener), m_pListeners.end() );
}

void CWorldPhysics::Step(float fTime) {
	m_pWorldPhysics->Step(fTime, 6, 2);
}

/* Contact begin - inform listeners */
void CWorldPhysics::BeginContact(b2Contact* contact) {
	for (unsigned int i = 0; i < m_pListeners.size(); i++) {
		if (m_pListeners[i])
			m_pListeners[i]->BeginContact(contact);
	}
}

/* Contact end - inform listeners */
void CWorldPhysics::EndContact(b2Contact* contact) {
	for (unsigned int i = 0; i < m_pListeners.size(); i++) {
		if (m_pListeners[i])
			m_pListeners[i]->EndContact(contact);
	}
}