#include "stdafx.h"
#include "CPhysicsDebugDraw.h"
#include "CGame.h"
#include "CWorldPhysics.h"
CPhysicsDebugDraw::CPhysicsDebugDraw() {
	m_bInit = false;
	m_pGame = NULL;
}
CPhysicsDebugDraw::~CPhysicsDebugDraw() {

}
void CPhysicsDebugDraw::Init(CGame* pGame) {
	m_bInit = pGame != NULL;
	m_pGame = pGame;
}
void CPhysicsDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) { 
	if (!m_bInit)
		return;

	m_pGame->m_Drawing.DrawBox2DPolygon(m_pGame->GetWindowMgr(), vertices, vertexCount, color, METER_TO_PIXEL);
}

void CPhysicsDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) { 
	if (!m_bInit)
		return;
	m_pGame->m_Drawing.DrawBox2DPolygon(m_pGame->GetWindowMgr(), vertices, vertexCount, color, METER_TO_PIXEL);
}

void CPhysicsDebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) { 
	if (!m_bInit)
		return;
	m_pGame->m_Drawing.DrawBox2DCircle(m_pGame->GetWindowMgr(), center, radius, color, METER_TO_PIXEL);
}

void CPhysicsDebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) { 
	if (!m_bInit)
		return;
}

void CPhysicsDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) { 
	if (!m_bInit)
		return;
	m_pGame->m_Drawing.DrawLine(m_pGame->GetWindowMgr(), Vector2i(p1.x, p1.y), Vector2i(p2.x, p2.y),
		(char)(color.r * 255), (char)(color.g * 255), (char)(color.b * 255), (char)(color.a * 255));
}

void CPhysicsDebugDraw::DrawTransform(const b2Transform& xf) { 
	if (!m_bInit)
		return;
}

void CPhysicsDebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color) { 
	if (!m_bInit)
		return;
	m_pGame->m_Drawing.DrawSquareCentred(m_pGame->GetWindowMgr(), Vector2i(p.x, p.y), (int)size, 
		(char)(color.r * 255), (char)(color.g * 255), (char)(color.b * 255), (char)(color.a * 255));
}