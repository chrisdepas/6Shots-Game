#include "stdafx.h"
#include "CPlayerHand.h"

void CPlayerHand::Init(char* handTexture, int handSize, int reachDist, sf::Vector2i playerCentre) {
	m_texture.loadFromFile(handTexture);
	m_iReachDistance = reachDist;
	m_vLocation = sf::Vector2f(playerCentre);
	m_iHandSize = handSize;
	m_fDrawRotation = 0.0f;
	m_fTrueRotation = 0.0f;
}
void CPlayerHand::UpdatePosition(sf::Vector2i mousePosition, sf::Vector2i playerCentre, float fRecoilRotation) {
	sf::Vector2i handRay = mousePosition - playerCentre;
	vec::LimitLength(handRay, (float)m_iReachDistance);
	vec::Rotate(handRay, fRecoilRotation);
	m_vLocation = sf::Vector2f(playerCentre + handRay);

	// HACK
	m_fDrawRotation = (270.0f - atan2f((float)handRay.x, (float)handRay.y) * 57.2958f);
	m_fTrueRotation = atan2f((float)handRay.x, (float)handRay.y);
}
void CPlayerHand::UpdatePosition(sf::Vector2f vPosition, float fRotation) {
	m_vLocation = vPosition;
	m_fTrueRotation = fRotation;
	m_fDrawRotation = (270.0f - m_fTrueRotation * 57.2958f);
}
void CPlayerHand::Draw(CGame* pGame, float fRecoilRotation) {
	pGame->m_Drawing.DrawSpriteCentred(pGame->GetWindowMgr(), (int)m_vLocation.x, (int)m_vLocation.y, m_iHandSize, m_iHandSize, 1.0f, m_fDrawRotation + fRecoilRotation, &m_texture);
}
float CPlayerHand::GetDrawRotation() {
	return m_fDrawRotation;
}
float CPlayerHand::GetTrueRotation() {
	return m_fTrueRotation;
}