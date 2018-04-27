#include "stdafx.h"
#include "6SWeapons.h"
#include "ASSETS.h"
#include "CGame.h"

void C6SKnife::Init(CWorldPhysics* pWorldPhysics, Vector2i pos) {
	m_vHoldOffset.X = 9;
	m_vHoldOffset.Y = 0;
	m_bAffectedByGravity = true; 
	m_fDrawRotation = 0.0f;
	m_fTrueRotation = 0.0f;
	m_texture.loadFromFile(ASSET_KNIFE_TEXTURE);
	m_vSize.X = m_texture.getSize().x;
	m_vSize.Y = m_texture.getSize().y;
	m_vLocation.X = (float)pos.X;
	m_vLocation.Y = (float)pos.Y;

	this->m_bIsProjectileWeapon = false;
	this->m_szName = "Dev Knife";

	/* Initialise physics */
	m_Physics.InitPhysics(pWorldPhysics, pos.X, pos.Y, m_vSize.X, m_vSize.Y, CEntityPhysics::CATEGORY_PLAYER_PICKUP,
		CEntityPhysics::CATEGORY_PLAYER_PICKUP | CEntityPhysics::CATEGORY_PLAYER);
}

void C6SRevolver::Init(CWorldPhysics* pWorldPhysics, Vector2i pos, CGame* pGame) {
	m_vHoldOffset.X = 11;
	m_vHoldOffset.Y = 0;// 2;

	m_vFireOffset.X = 20;
	m_vFireOffset.Y = -4;
	m_vCasingOffset.X = 10;
	m_vCasingOffset.Y = -4;

	m_iFireSpeed = 50000;

	m_bAffectedByGravity = true;
	m_fDrawRotation = 0.0f;
	m_fTrueRotation = 0.0f;
	m_texture.loadFromFile(ASSET_REVOLVER_TEXTURE);
	m_vSize.X = m_texture.getSize().x;
	m_vSize.Y = m_texture.getSize().y;
	m_vLocation.X = (float)pos.X;
	m_vLocation.Y = (float)pos.Y;

	m_iPrimaryClip = m_iPrimaryClipMax = 6;

	m_iWeaponSoundID = pGame->m_SoundManager.GetID(ASSET_REVOLVER_SOUND);
	m_iDryFireSoundID = pGame->m_SoundManager.GetID(ASSET_DRYFIRE);
	 
	m_bIsAutomatic = false;
	m_bIsProjectileWeapon = true;
	m_fFireDelay = 0.2f;
	m_fNextAttackTime = 0.0f;
	m_szName = "Dev Revolver";

	m_iBulletDamage = 56;
	
	/* Recoil data */
	m_fRecoilMax = 0.7f;
	m_fRecoilMin = 0.5f;

	/* Spread Data */
	m_fSpreadMax = 0.2f;

	/* Initialise physics */
	m_Physics.InitPhysics(pWorldPhysics, pos.X, pos.Y, m_vSize.X, m_vSize.Y, CEntityPhysics::CATEGORY_PLAYER_PICKUP, 
		CEntityPhysics::CATEGORY_PLAYER_PICKUP | CEntityPhysics::CATEGORY_PLAYER);
}

void C6SAk47::Init(CWorldPhysics* pPhysics, Vector2f pos, CGame* pGame) {
	m_vHoldOffset.X = 7;
	m_vHoldOffset.Y = 0;// 2;

	m_vFireOffset.X = 20; 
	m_vFireOffset.Y = -4;
	m_vCasingOffset.X = 10;
	m_vCasingOffset.Y = -4;

	m_iFireSpeed = 50000;

	m_iBulletDamage = 26;

	m_bAffectedByGravity = true;
	m_fDrawRotation = 0.0f;
	m_fTrueRotation = 0.0f;
	m_texture.loadFromFile(ASSET_AK47_TEXTURE);
	m_vSize.X = (int)(m_texture.getSize().x);
	m_vSize.Y = (int)(m_texture.getSize().y);
	m_vLocation.X = pos.X;
	m_vLocation.Y = pos.Y;
	  
	m_iPrimaryClip = m_iPrimaryClipMax = 3000;
	 
	m_iWeaponSoundID = pGame->m_SoundManager.GetID(ASSET_AK47_SOUND);
	m_iDryFireSoundID = pGame->m_SoundManager.GetID(ASSET_DRYFIRE);

	m_bIsAutomatic = true;
	m_bIsProjectileWeapon = true;
	m_fFireDelay = 0.08f;
	m_fNextAttackTime = 0.0f;
	m_szName = "Dev AK47";

	/* Initialise physics */
	m_Physics.InitPhysics(pPhysics, (int)pos.X, (int)pos.Y, ASSET_AK47_COLLIDER, CEntityPhysics::CATEGORY_PLAYER_PICKUP,
		CEntityPhysics::CATEGORY_PLAYER_PICKUP | CEntityPhysics::CATEGORY_PLAYER);
}

CSpentCasingEffect* C6SAk47::CreateCasingEffect(CWorldPhysics* pPhysics, Vector2i vPos, Vector2f vEjectDirection, float fEjectRotation) {
	CSpentCasingEffect* casing = new CSpentCasingEffect;
	casing->Init(pPhysics, ASSET_RIFLE_CASING, Vector2i(20, 10), vPos, vEjectDirection * 230.0f, fEjectRotation, 750.0f);
	return casing;
}

void C6SSawnOff::Init(CWorldPhysics* pPhysics, Vector2i pos, CGame* pGame) {
	m_vHoldOffset.X = 24;
	m_vHoldOffset.Y = 0;// 7;

	m_vFireOffset.X = 40;
	m_vFireOffset.Y = -7;
	m_vCasingOffset.X = 20;
	m_vCasingOffset.Y = -4;

	m_iFireSpeed = 50000;

	m_iBulletDamage = 16;

	m_bAffectedByGravity = true;
	m_fDrawRotation = 0.0f;
	m_fTrueRotation = 0.0f;
	m_texture.loadFromFile(ASSET_SAWNOFF_TEXTURE);
	m_vSize.X = m_texture.getSize().x;
	m_vSize.Y = m_texture.getSize().y;
	m_vLocation.X = (float)pos.X;
	m_vLocation.Y = (float)pos.Y;

	m_iPrimaryClip = m_iPrimaryClipMax = 200;

	m_iWeaponSoundID = pGame->m_SoundManager.GetID(ASSET_SAWNOFF_SOUND);
	m_iDryFireSoundID = pGame->m_SoundManager.GetID(ASSET_DRYFIRE);

	m_bIsAutomatic = false;
	m_bIsProjectileWeapon = true;

	m_fNextAttackTime = 0.0f;
	m_szName = "Dev SawedOff";

	m_fFireDelay = 0.4f;
	m_fRecoilMax = 1.8f;
	m_fRecoilMin = 1.3f;

	m_fSpreadMax = 0.35f;

	m_iBulletsPerShot = 13;

	/* Initialise physics */
	m_Physics.InitPhysics(pPhysics, pos.X, pos.Y, m_vSize.X, m_vSize.Y, CEntityPhysics::CATEGORY_PLAYER_PICKUP,
		CEntityPhysics::CATEGORY_PLAYER_PICKUP | CEntityPhysics::CATEGORY_PLAYER);
}

void C6STMP::Init(CWorldPhysics* pPhysics, Vector2i pos, CGame* pGame) {
	m_vHoldOffset.X = 7;
	m_vHoldOffset.Y = 0;// 2;

	m_vFireOffset.X = 20;
	m_vFireOffset.Y = -4;
	m_vCasingOffset.X = 10;
	m_vCasingOffset.Y = -4;

	m_iFireSpeed = 5000;

	m_iBulletDamage = 20;

	m_bAffectedByGravity = true;
	m_fDrawRotation = 0.0f;
	m_fTrueRotation = 0.0f;
	m_texture.loadFromFile(ASSET_TMP_TEXTURE);
	m_vSize.X = (int)(m_texture.getSize().x);
	m_vSize.Y = (int)(m_texture.getSize().y);
	m_vLocation.X = (float)pos.X;
	m_vLocation.Y = (float)pos.Y;

	m_iPrimaryClip = m_iPrimaryClipMax = 20;

	m_iWeaponSoundID = pGame->m_SoundManager.GetID(ASSET_TMP_SOUND);
	m_iDryFireSoundID = pGame->m_SoundManager.GetID(ASSET_DRYFIRE);

	m_bIsAutomatic = true;
	m_bIsProjectileWeapon = true;
	m_fFireDelay = 0.05f;
	m_fNextAttackTime = 0.0f;
	m_szName = "Dev TMP";


	/* Initialise physics */
	m_Physics.InitPhysics(pPhysics, pos.X, pos.Y, ASSET_AK47_COLLIDER, CEntityPhysics::CATEGORY_PLAYER_PICKUP,
		CEntityPhysics::CATEGORY_PLAYER_PICKUP | CEntityPhysics::CATEGORY_PLAYER);
}

CSpentCasingEffect* C6STMP::CreateCasingEffect(CWorldPhysics* pPhysics, Vector2i vPos, Vector2f vEjectDirection, float fEjectRotation) {
	CSpentCasingEffect* casing = new CSpentCasingEffect();
	casing->Init(pPhysics, ASSET_PISTOL_CASING, Vector2i(20, 10), vPos, vEjectDirection * 210.0f, fEjectRotation, 4.0f);
	return casing;
}