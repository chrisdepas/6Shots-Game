#include "stdafx.h"
#include "CPhysicsDebugDraw.h"
#include "CGame.h"
#include "CWorldPhysics.h"

void CPhysicsDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
	if (IsActive()) {
		m_pGame->m_Drawing.DrawBox2DPolygon(m_pGame->GetWindowMgr(), vertices, vertexCount, color, METER_TO_PIXEL);
	}
}

void CPhysicsDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
	if (IsActive()) {
		m_pGame->m_Drawing.DrawBox2DPolygon(m_pGame->GetWindowMgr(), vertices, vertexCount, color, METER_TO_PIXEL);
	}
}

void CPhysicsDebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) { 
	if (IsActive()) { m_pGame->m_Drawing.DrawBox2DCircle(m_pGame->GetWindowMgr(), center, radius, color, METER_TO_PIXEL); }
}

void CPhysicsDebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) { 
	if (IsActive()) { 
		m_pGame->m_Drawing.DrawSolidCircleCentred(&m_pGame->m_WindowManager, util::toSFMLVec2f(center), (float)radius, util::toSFMLColor(color));
	}
}

void CPhysicsDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) { 
	if (IsActive()) {
		m_pGame->m_Drawing.DrawLine(m_pGame->GetWindowMgr(), util::toSFMLVec2f(p1), util::toSFMLVec2f(p2), util::toSFMLColor(color));
	}
}

void CPhysicsDebugDraw::DrawTransform(const b2Transform& transform) {}

// Removed in Box2D 2.3.1?
/*void CPhysicsDebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color) { 
	if (IsActive()) {
		m_pGame->m_Drawing.DrawRectangleCentred(m_pGame->GetWindowMgr(), util::toSFMLVec2f(p), sf::Vector2f(size, size), util::toSFMLColor(color));
	}
}*/