#include "stdafx.h"
#include "CEntityPhysics.h"
#include "CWorldPhysics.h"
#include <fstream>

/*
	Christopher De Pasquale
	Late 2016
	Noticed added feb 2017

*/

#define COLLIDER_LINE_BUFSIZE 1024 
#define COLLIDER_MAX_VERTICES 120

CEntityPhysics::CEntityPhysics() {
	m_pBody = 0;
}
Vector2f CEntityPhysics::GetPosition() {
	b2Vec2 p = m_pBody->GetPosition();

	// Scale up from metres to pixels
	return Vector2f(p.x * METER_TO_PIXEL,
		p.y * METER_TO_PIXEL);
}
float CEntityPhysics::GetRotation() {
	b2Transform t = m_pBody->GetTransform();

	return t.q.GetAngle();
}

bool CEntityPhysics::IsCategory(EEntityCategory category) {
	b2Fixture* pFixture = m_pBody->GetFixtureList();
	while (pFixture) {
		if ((pFixture->GetFilterData().categoryBits & category) == 0)
			return false;
		pFixture = pFixture->GetNext();
	}
	
	return true;
}

void CEntityPhysics::SetPosition(Vector2i position) {
	// Scale from pixels to meters
	position *= PIXEL_TO_METER;

	m_pBody->SetTransform(b2Vec2((float32)position.X, (float32)position.Y), 0.0f);
}
void CEntityPhysics::SetPosition(Vector2f position) {
	// Scale from pixels to meters
	position *= PIXEL_TO_METER;

	m_pBody->SetTransform(b2Vec2(position.X, position.Y), 0.0f);
}

void CEntityPhysics::SetRotation(float rotationRadian) {
	b2Vec2 pos = m_pBody->GetPosition();
	m_pBody->SetTransform(pos, rotationRadian);
}

void CEntityPhysics::SetRotationalVelocity(float rotationVel) {
	// Scale from pixels/second to meters/second
	rotationVel *= PIXEL_TO_METER;

	m_pBody->SetAngularVelocity(rotationVel);
}

void CEntityPhysics::SetVelocity(Vector2f vel) {
	if (m_pBody) {
		// Scale from pixels/second to meters/second
		vel *= PIXEL_TO_METER;
		m_pBody->SetLinearVelocity(b2Vec2(vel.X, vel.Y));
	}
}

void CEntityPhysics::SetHorizontalVelocity(float vel) {
	if (m_pBody) {
		b2Vec2 velocity = m_pBody->GetLinearVelocity();
		// Scale from pixels/second to meters/second
		velocity.x = vel * PIXEL_TO_METER;

		m_pBody->SetLinearVelocity(velocity);
	}
}
void CEntityPhysics::SetVerticalVelocity(float vel) {
	if (m_pBody) {
		b2Vec2 velocity = m_pBody->GetLinearVelocity();
		// Scale from pixels/second to meters/second
		velocity.y = vel * PIXEL_TO_METER;

		m_pBody->SetLinearVelocity(velocity);
	}
}

void CEntityPhysics::InitPhysics(CWorldPhysics* pWorldPhysics, int initPosX, int initPosY, int width, int height, 
	CEntityPhysics::EEntityCategory ePhysicsCategories, int eIgnoreCollisionCategories) {
	b2BodyDef def;
	def.type = b2BodyType::b2_dynamicBody;
	def.fixedRotation = false;
	def.active = true;

	m_pBody = pWorldPhysics->CreateBody(&def);

	b2PolygonShape box;
	box.SetAsBox(width / 2.0f, height / 2.0f, b2Vec2(0.0f, 0), 0.0f);
	/* Scale down to metres */
	for (int i = 0; i < box.GetVertexCount(); i++) {
		box.m_vertices[i] *= PIXEL_TO_METER;
	}
	b2Fixture* pFixture = m_pBody->CreateFixture(&box, 1.0f);

	/* Bullet */
	if (ePhysicsCategories & CATEGORY_BULLET) {
		m_pBody->SetBullet(true);
	}

	/* Set filter data */
	b2Filter filter = pFixture->GetFilterData();
	filter.categoryBits = ePhysicsCategories;
	filter.maskBits &= ~eIgnoreCollisionCategories;
	pFixture->SetFilterData(filter);

	m_pBody->SetTransform(b2Vec2(((float)initPosX * PIXEL_TO_METER), ((float)initPosY * PIXEL_TO_METER)), 0.0f);
}
void CEntityPhysics::InitPhysics(CWorldPhysics* pWorldPhysics, int initPosX, int initPosY, int width, int height,
	CEntityPhysics::EEntityCategory ePhysicsCategories, int eIgnoreCollisionCategories, int dataID) {

	b2BodyDef def;
	def.type = b2BodyType::b2_dynamicBody;
	def.fixedRotation = false;
	def.active = true;

	m_pBody = pWorldPhysics->CreateBody(&def);

	b2PolygonShape box;
	box.SetAsBox(width / 2.0f, height / 2.0f, b2Vec2(0.0f, 0), 0.0f);
	/* Scale down to metres */
	for (int i = 0; i < box.GetVertexCount(); i++) {
		box.m_vertices[i] *= PIXEL_TO_METER;
	}
	b2Fixture* pFixture = m_pBody->CreateFixture(&box, 1.0f);

	/* Bullet */
	if (ePhysicsCategories & CATEGORY_BULLET) {
		m_pBody->SetBullet(true);
	}

	/* Set filter data */
	b2Filter filter = pFixture->GetFilterData();
	filter.categoryBits = ePhysicsCategories;
	filter.maskBits &= ~eIgnoreCollisionCategories;
	pFixture->SetFilterData(filter);

	/* Store id */
	pFixture->SetUserData((void*)dataID);

	m_pBody->SetTransform(b2Vec2(((float)initPosX * PIXEL_TO_METER), ((float)initPosY * PIXEL_TO_METER)), 0.0f);
}
void CEntityPhysics::InitPhysics(CWorldPhysics* pWorldPhysics, int initPosX, int initPosY, char* szColliderFile,
	CEntityPhysics::EEntityCategory ePhysicsCategories, int eIgnoreCollisionCategories) {

	b2BodyDef def;
	def.type = b2BodyType::b2_dynamicBody;
	def.fixedRotation = false;
	def.active = true; 

	m_pBody = pWorldPhysics->CreateBody(&def);

	b2PolygonShape colliderShape;
	//
	std::ifstream file(szColliderFile);
	if (!file || !file.good()) {
		printf("Unable to init physics from file '%s' - bad file supplied\n", szColliderFile);
		return;
	}
	char lineBuf[COLLIDER_LINE_BUFSIZE];

	/* Read vertices count */
	int vertexCount = -1;
	if (file.getline(lineBuf, COLLIDER_LINE_BUFSIZE)) {
		vertexCount = (int)strtol(lineBuf, 0, 10);
	}
	/* Validate count */
	if (vertexCount <= 0 || vertexCount > COLLIDER_MAX_VERTICES) {
		printf("Unable to init physics from file '%s' - bad or missing vertex count\n", szColliderFile);
		return;
	}

	/* Parse each vertex */
	int curVertex = 0;
	b2Vec2* vertices = (b2Vec2*)malloc(vertexCount * sizeof(b2Vec2));
	while (file.getline(lineBuf, COLLIDER_LINE_BUFSIZE)) {
		if (curVertex >= vertexCount) {
			printf("Unable to init physics from file '%s' - vertice count differs from vertices supplied\n", szColliderFile);
			return;
		}

		// Split over delim:
		int lineLen = strlen(lineBuf);
		char* szX = lineBuf;
		char* szY = 0;
		for (int i = 0; i < lineLen; i++) {
			if (lineBuf[i] == ',') {
				/* Split string */
				if (i + 1 < lineLen) {
					lineBuf[i] = 0;
					szY = (char*)(lineBuf + i + 1);
					break;
				}
			}
		}

		if (szY == NULL) {
			printf("Unable to init physics from file '%s' - line %i is malformed (Expected 'X,Y')\n", szColliderFile, curVertex);
			return;
		}

		/* Read vertice X,Y */
		int vertexX = strtol(szX, 0, 10);
		int vertexY = strtol(szY, 0, 10);

		/* Convert to meters */
		vertices[curVertex].x = ((float)vertexX) * PIXEL_TO_METER;
		vertices[curVertex].y = ((float)vertexY) * PIXEL_TO_METER;

		curVertex++;
	}
	file.close();
	
	/* Check if collider was read correctly */
	if (curVertex != vertexCount) {
		printf("Warning - physics file '%s' contains incorrect number of vertices (Contained %i, expected %i)\n", szColliderFile, curVertex, vertexCount);
	}
	
	/* Create collider */
	colliderShape.Set(vertices, curVertex);
	b2Fixture* pFixture = m_pBody->CreateFixture(&colliderShape, 1.0f);

	/* Bullet */
	if (ePhysicsCategories & CATEGORY_BULLET) {
		m_pBody->SetBullet(true);
	}

	/* Set filter data */
	b2Filter filter = pFixture->GetFilterData();
	filter.categoryBits = ePhysicsCategories;
	filter.maskBits &= ~eIgnoreCollisionCategories;
	pFixture->SetFilterData(filter);

	/* Init position, scaled to meters */
	m_pBody->SetTransform(b2Vec2(((float)initPosX * PIXEL_TO_METER), ((float)initPosY * PIXEL_TO_METER)), 0.0f);
}

/* Simple shape decomp algo 
1. Shrink bounds from image size to min 
2. Select largest rectangle containing no pixels & add it's vertices 
3. repeat */
void InitPhysicsFromImage(CWorldPhysics* pWorldPhysics, int initPosX, int initPosY, sf::Image* pImg,
	CEntityPhysics::EEntityCategory ePhysicsCategory, int eIgnoreCollisionCategories) {

}
void CEntityPhysics::DestroyPhysics(CWorldPhysics* pWorldPhysics) {
	if (m_pBody) {
		pWorldPhysics->DestroyBody(m_pBody);
		m_pBody = 0;
	}
}

void CEntityPhysics::DisablePhysics() {
	if (m_pBody)
		m_pBody->SetAwake(false);
}
void CEntityPhysics::EnablePhysics() {
	if (m_pBody)
		m_pBody->SetAwake(true);
}

void CEntityPhysics::DisableCollisions() {
	printf("Disabling ");
	if (m_bCollisionsEnabled) {
		printf("physics ");

		b2Fixture* pFixture = m_pBody->GetFixtureList();

		while (pFixture) {
			if (pFixture) {
				printf("collisions ");
				/* Set filter mask to NULL */
				b2Filter filter = pFixture->GetFilterData();
				m_uiLastMaskBits = filter.maskBits;
				filter.maskBits = 0;
				pFixture->SetFilterData(filter);

				m_bCollisionsEnabled = false;
			}
			pFixture = pFixture->GetNext();
		}
		
	}
}
void CEntityPhysics::EnableCollisions() {
	if (!m_bCollisionsEnabled) {
		b2Fixture* pFixture = m_pBody->GetFixtureList();

		if (pFixture) {
			/* Set filter mask to previous value */
			b2Filter filter = pFixture->GetFilterData();
			filter.maskBits = m_uiLastMaskBits;
			pFixture->SetFilterData(filter);

			m_bCollisionsEnabled = true;
		}
	}
}

bool CEntityPhysics::IsActive() {
	if (m_pBody)
		return m_pBody->IsActive();
	return false;
}