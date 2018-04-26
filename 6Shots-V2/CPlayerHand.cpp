#include "stdafx.h"
#include "CPlayerHand.h"
#include <math.h>
void CPlayerHand::Init(char* handTexture, int handSize, int reachDist, Vector2i playerCentre) {
	m_texture.loadFromFile(handTexture);
	m_iReachDistance = reachDist;
	m_vLocation.X = (float)playerCentre.X;
	m_vLocation.Y = (float)playerCentre.Y;
	m_iHandSize = handSize;
	m_fDrawRotation = 0.0f;
	m_fTrueRotation = 0.0f;
}
void CPlayerHand::UpdatePosition(Vector2i mousePosition, Vector2i playerCentre, float fRecoilRotation) {
	Vector2i ray = mousePosition - playerCentre;
	ray.LimitLength(m_iReachDistance); 
	ray.Rotate(fRecoilRotation);
	Vector2i delta = playerCentre + ray;
	m_vLocation.X = (float)delta.X;
	m_vLocation.Y = (float)delta.Y;
	m_fDrawRotation = (270.0f - atan2f((float)ray.X, (float)ray.Y) * 57.2958f);
	m_fTrueRotation = atan2f((float)ray.X, (float)ray.Y);
}
void CPlayerHand::UpdatePosition(Vector2f vPosition, float fRotation) {
	m_vLocation = vPosition;
	m_fTrueRotation = fRotation;
	m_fDrawRotation = (270.0f - m_fTrueRotation * 57.2958f);
}
void CPlayerHand::Draw(CGame* pGame, float fRecoilRotation) {
	pGame->m_Drawing.DrawSpriteCentred(pGame->GetWindowMgr(), (int)m_vLocation.X, (int)m_vLocation.Y, m_iHandSize, m_iHandSize, 1.0f, m_fDrawRotation + fRecoilRotation, &m_texture);
}
float CPlayerHand::GetDrawRotation() {
	return m_fDrawRotation;
}
float CPlayerHand::GetTrueRotation() {
	return m_fTrueRotation;
}