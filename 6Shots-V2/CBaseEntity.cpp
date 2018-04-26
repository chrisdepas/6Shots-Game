#include "stdafx.h"
#include "CBaseEntity.h"
#include "CGame.h"

CBaseEntity::CBaseEntity() {
	m_Flags = FL_FACING_LEFT;
}
Vector2f CBaseEntity::GetPosition2f() {
	return m_vLocation;
}

Vector2i CBaseEntity::GetPosition2i() {
	return Vector2i((int)m_vLocation.X, (int)m_vLocation.Y);
}

void CBaseEntity::SetPosition(Vector2f p) {
	m_vLocation = p;
}
void CBaseEntity::SetPosition(Vector2i p) {
	m_vLocation.X = (float)p.X;
	m_vLocation.Y = (float)p.Y;
}
void CBaseEntity::SetBaseFlag(EBaseFlags f, bool b) { 
	if (b)
		m_Flags |= f;
	else
		m_Flags &= ~f;
}
bool CBaseEntity::GetBaseFlag(EBaseFlags f) {
	return (m_Flags & f) != 0; 
}
void CBaseEntity::DefaultDraw(CGame* pGame) {
	pGame->m_Drawing.DrawSpriteCentred(pGame->GetWindowMgr(), GetPosition2i(), m_vSize.X, m_vSize.Y, &m_texture);
}

Vector2f CBaseEntity::GetVelocity() {
	return m_vVelocity;
}
void CBaseEntity::SetVelocity(Vector2f v) {
	m_vVelocity = v;
}