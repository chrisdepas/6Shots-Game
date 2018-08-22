#include "stdafx.h"
#include "CBaseEntity.h"
#include "CGame.h"

CBaseEntity::CBaseEntity() {
	m_Flags = FL_FACING_LEFT;
}
sf::Vector2f CBaseEntity::GetPosition2f() {
	return m_vLocation;
}

sf::Vector2i CBaseEntity::GetPosition2i() {
	return sf::Vector2i(m_vLocation);
}

void CBaseEntity::SetPosition(sf::Vector2f p) {
	m_vLocation = p;
}
void CBaseEntity::SetPosition(sf::Vector2i p) {
	m_vLocation = sf::Vector2f(p);
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
	pGame->m_Drawing.DrawSpriteCentred(pGame->GetWindowMgr(), GetPosition2i(), m_vSize, &m_texture);
}

sf::Vector2f CBaseEntity::GetVelocity() {
	return m_vVelocity;
}
void CBaseEntity::SetVelocity(sf::Vector2f v) {
	m_vVelocity = v;
}