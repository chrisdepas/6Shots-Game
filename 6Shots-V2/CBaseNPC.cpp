#include "stdafx.h"
#include "CBaseNPC.h"

/*
**	Christopher De Pasquale
**	Created:	Unknown, sometime in 2017
**	Last Edit	26th April 2018
*/

CBaseNPC::CBaseNPC() {
	m_Hand.SetPosition(m_vLocation);
}

void CBaseNPC::DefaultDraw(CGame* pGame) {
	/* draw centre */
	sf::Vector2i centre = sf::Vector2i(m_vLocation) - sf::Vector2i(0, m_height / 2);

	/* Draw player sprite */
	m_PlayerSprite.Draw(pGame, centre, false);

	/* Draw Hand */
	float fWeaponRecoil = 0.0f;
	if (IsWeaponEquipped()) {
		fWeaponRecoil = m_pWeapon->GetRecoilRotation();
	}
	m_Hand.Draw(pGame, fWeaponRecoil * 57.2958f);

	/* Draw equipped weapon */
	if (IsWeaponEquipped()) {
		sf::Vector2i handPos = m_Hand.GetPosition2i();
		m_pWeapon->SetPosition(handPos);
		m_pWeapon->DefaultDraw(pGame);
	}
}


/* Update */
void CBaseNPC::Update(CGame* pGame, float fFrameTime) {
	if (IsDead())
		return;

	float fCurTime = pGame->m_Time.Time();

	// Update animation
	m_PlayerSprite.Update(fFrameTime, !GetBaseFlag(FL_FACING_LEFT));

	// Update position
	m_vLocation = m_PlayerPhysics.GetPosition() + sf::Vector2f(0.0f, m_height / 2.0f);

	// Update InAir
	m_bInAir = m_PlayerPhysics.InAir();

	if (!m_bInAir && m_bJump) {
		m_PlayerPhysics.SetVerticalVelocity(m_fJumpSpeed);
	}

	if (m_bMoveLeft) {
		SetBaseFlag(FL_FACING_LEFT, true);
		m_PlayerPhysics.SetHorizontalVelocity(-1.0f * m_fMoveSpeed);

	}
	else if (m_bMoveRight) {
		SetBaseFlag(FL_FACING_LEFT, false);
		m_PlayerPhysics.SetHorizontalVelocity(m_fMoveSpeed);

	}
	else {
		m_PlayerPhysics.SetHorizontalVelocity(0.0f);

	}

	if (m_pWeapon) {
		m_pWeapon->SetTrueRotation(m_Hand.GetTrueRotation());
		m_pWeapon->SetDrawRotation(m_Hand.GetDrawRotation());
		//m_pWeapon->Update(pPhysics, pGame, pProj, fElapsedTime);
	}

	/* Calculate hand velocity */
	if (fCurTime - m_fLastHandPollTime > HAND_POLL_TIME) {
		sf::Vector2f handDelta(m_Hand.GetPosition2i() - m_vLastHandPoll);
		m_vHandVelocity = handDelta / (fCurTime - m_fLastHandPollTime);

		m_fLastHandPollTime = fCurTime;
		m_vLastHandPoll = m_Hand.GetPosition2i();
	}


	sf::Vector2i centre = sf::Vector2i(m_vLocation) - sf::Vector2i(0, m_height / 2);
	bool bFacingLeft = (m_Hand.GetPosition2f().x < centre.x);

	/* Get hand direction & weapon recoil */
	m_Hand.SetBaseFlag(FL_FACING_LEFT, bFacingLeft);
	float recoil = 0.0f;
	if (IsWeaponEquipped()) {
		recoil = m_pWeapon->GetRecoilRotation();
		m_pWeapon->SetBaseFlag(FL_FACING_LEFT, bFacingLeft);
	}

	m_Hand.UpdatePosition(centre - sf::Vector2i(2, 2), centre, recoil);
}

/* Think */
void CBaseNPC::Think(CGame* pGame, float fFrameTime) {

}