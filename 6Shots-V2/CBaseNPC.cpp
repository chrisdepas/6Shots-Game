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
	Vector2i c((int)m_vLocation.X, (int)(m_vLocation.Y - (float)m_height / 2.0f));

	/* Draw player sprite */
	m_PlayerSprite.Draw(pGame, c, false);

	/* Draw Hand */
	float fWeaponRecoil = 0.0f;
	if (IsWeaponEquipped()) {
		fWeaponRecoil = m_pWeapon->GetRecoilRotation();
	}
	m_Hand.Draw(pGame, fWeaponRecoil * 57.2958f);

	/* Draw equipped weapon */
	if (IsWeaponEquipped()) {
		Vector2i handPos = m_Hand.GetPosition2i();
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
	Vector2f physPos = m_PlayerPhysics.GetPosition();
	m_vLocation.X = physPos.X;
	m_vLocation.Y = physPos.Y + m_height / 2.0f;

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
		Vector2i handDelta = m_Hand.GetPosition2i() - m_vLastHandPoll;
		m_vHandVelocity.X = handDelta.X / (fCurTime - m_fLastHandPollTime);
		m_vHandVelocity.Y = handDelta.Y / (fCurTime - m_fLastHandPollTime);

		m_fLastHandPollTime = fCurTime;
		m_vLastHandPoll = m_Hand.GetPosition2i();
	}


	Vector2i centre((int)m_vLocation.X, (int)(m_vLocation.Y - (float)m_height / 2.0f));
	bool bFacingLeft = (m_Hand.GetPosition2f().X < centre.X);

	/* Get hand direction & weapon recoil */
	m_Hand.SetBaseFlag(FL_FACING_LEFT, bFacingLeft);
	float recoil = 0.0f;
	if (IsWeaponEquipped()) {
		recoil = m_pWeapon->GetRecoilRotation();
		m_pWeapon->SetBaseFlag(FL_FACING_LEFT, bFacingLeft);
	}

	m_Hand.UpdatePosition(Vector2i(centre.X - 2, centre.Y - 2), centre, recoil);
}

/* Think */
void CBaseNPC::Think(CGame* pGame, float fFrameTime) {

}