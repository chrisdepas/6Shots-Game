#include "stdafx.h"
#include "6SWeapons.h"
#include "ASSETS.h"
#include "CGame.h"

void C6SKnife::Init(CWorldPhysics* pWorldPhysics, sf::Vector2i pos) {
	m_vHoldOffset = sf::Vector2i(9, 0);
	m_bAffectedByGravity = true; 
	m_fDrawRotation = m_fTrueRotation = 0.0f;
	m_texture.loadFromFile(ASSET_KNIFE_TEXTURE);
	m_vSize = sf::Vector2i(m_texture.getSize());
	m_vLocation = sf::Vector2f(pos);

	m_bIsProjectileWeapon = false;
	m_szName = "Dev Knife";

	/* Initialise physics */
	m_Physics.InitPhysics(pWorldPhysics, pos.x, pos.y, m_vSize.x, m_vSize.y, CEntityPhysics::CATEGORY_PLAYER_PICKUP,
		CEntityPhysics::CATEGORY_PLAYER_PICKUP | CEntityPhysics::CATEGORY_PLAYER);
}

void C6SRevolver::Init(CWorldPhysics* pWorldPhysics, sf::Vector2i pos, CGame* pGame) {
	m_vHoldOffset = sf::Vector2i(11, 0);
	m_vFireOffset = sf::Vector2i(20, -4);
	m_vCasingOffset = sf::Vector2i(10, -4);

	m_iFireSpeed = 50000;

	m_bAffectedByGravity = true;
	m_fDrawRotation = 0.0f;
	m_fTrueRotation = 0.0f;
	m_texture.loadFromFile(ASSET_REVOLVER_TEXTURE);
	m_vSize = sf::Vector2i(m_texture.getSize());
	m_vLocation = sf::Vector2f(pos);

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
	m_Physics.InitPhysics(pWorldPhysics, pos.x, pos.y, m_vSize.x, m_vSize.y, CEntityPhysics::CATEGORY_PLAYER_PICKUP, 
		CEntityPhysics::CATEGORY_PLAYER_PICKUP | CEntityPhysics::CATEGORY_PLAYER);
}

void C6SBaseDevPistol::Init(CWorldPhysics* pPhysics, sf::Vector2i vSpawnPos, CGame* pGame) {
	m_vHoldOffset = sf::Vector2i(11, 0);
	m_vFireOffset = sf::Vector2i(20, -4);
	m_vCasingOffset = sf::Vector2i(10, -4);

	m_bIsAutomatic = false;
	m_bIsProjectileWeapon = true;
	m_bAffectedByGravity = true;

	m_iFireSpeed = 50000;
	m_iBulletDamage = 56;
	m_iBulletsPerShot = 1;
	m_iPrimaryClip = m_iPrimaryClipMax = 6;

	m_fFireDelay = 0.2f;
	m_fNextAttackTime = 0.0f;

	m_fDrawRotation = m_fTrueRotation = 0.0f;

	m_iWeaponSoundID = pGame->m_SoundManager.GetID(ASSET_REVOLVER_SOUND);
	m_iDryFireSoundID = pGame->m_SoundManager.GetID(ASSET_DRYFIRE);
	m_texture.loadFromFile(ASSET_REVOLVER_TEXTURE);
	m_vSize = sf::Vector2i(m_texture.getSize());
	m_vLocation = sf::Vector2f(vSpawnPos);

	m_szName = "Dev Base Pistol";

	m_fRecoilMax = 0.7f;
	m_fRecoilMin = 0.5f;
	m_fSpreadMax = 0.2f;

	/* Initialise physics */
	m_Physics.InitPhysics(pPhysics, vSpawnPos.x, vSpawnPos.y, m_vSize.x, m_vSize.y, CEntityPhysics::CATEGORY_PLAYER_PICKUP,
		CEntityPhysics::CATEGORY_PLAYER_PICKUP | CEntityPhysics::CATEGORY_PLAYER);
}
void C6SBaseDevPistol::SetName(std::string sName) {
	if (m_bWrittenName && m_szName) {
		delete m_szName;
	}
	else {
		m_bWrittenName = true;
	}
	m_szName = new char[sName.length() + 1];
	m_szName[sName.length()] = NULL;
	memcpy(m_szName, sName.c_str(), sName.length());
}
bool C6SBaseDevPistol::SetGunFireSoundFromFile(std::string sFireSoundFile, CGame* pGame) {
	if (util::FileExists(sFireSoundFile)) {
		m_iWeaponSoundID = pGame->m_SoundManager.GetID((char*)sFireSoundFile.c_str());
		return true;
	}
	else {
		CDebugLogger::LogError("ERROR - Failed to load sound '%s' from file\n", sFireSoundFile.c_str());
		return false;
	}
}

bool C6SBaseDevPistol::SetGunDryFireSoundFromFile(std::string sDryFireSoundFile, CGame* pGame) {
	if (util::FileExists(sDryFireSoundFile)) {
		m_iDryFireSoundID = pGame->m_SoundManager.GetID((char*)sDryFireSoundFile.c_str());
		return true;
	}
	else {
		CDebugLogger::LogError("ERROR - Failed to load texture '%s' from file\n", sDryFireSoundFile.c_str());
		return false;
	}
}

bool C6SBaseDevPistol::SetGunTextureFromFile(std::string sTextureFilePath) {
	if (util::FileExists(sTextureFilePath) && m_texture.loadFromFile(sTextureFilePath)) {
		m_vSize = sf::Vector2i(m_texture.getSize());
		return true;
	} else {
		CDebugLogger::LogError("ERROR - Failed to load texture '%s' from file\n", sTextureFilePath.c_str());
		return false;
	}
}
void C6SBaseDevPistol::SetCasingTypeRifle() { m_sCasingAsset = ASSET_RIFLE_CASING; }
void C6SBaseDevPistol::SetCasingTypeShotgun() { m_sCasingAsset = ASSET_SHOTGUN_CASING; }
void C6SBaseDevPistol::SetCasingTypePistol() { m_sCasingAsset = ASSET_PISTOL_CASING; }

CSpentCasingEffect* C6SBaseDevPistol::CreateCasingEffect(CWorldPhysics* pPhysics, sf::Vector2i vPos, sf::Vector2f vEjectDirection, float fEjectRotation) {
	if (m_bCanCreateCasing && m_sCasingAsset.size() > 0) {
		CSpentCasingEffect* casing = new CSpentCasingEffect;
		casing->Init(pPhysics, (char*)m_sCasingAsset.c_str(), m_vCasingSize, vPos, vEjectDirection * m_fCasingEjectSpeed, fEjectRotation, m_fCasingEjectAngularSpeed);
		return casing;
	}
	return NULL;
}

void C6SAk47::Init(CWorldPhysics* pPhysics, sf::Vector2f pos, CGame* pGame) {
	m_vHoldOffset = sf::Vector2i(7, 0);// or 7, 2;
	m_vFireOffset = sf::Vector2i(20, -4);
	m_vCasingOffset = sf::Vector2i(10, -4);

	m_iFireSpeed = 50000;
	m_iBulletDamage = 26;

	m_fDrawRotation = m_fTrueRotation = 0.0f;

	m_bAffectedByGravity = true;

	m_texture.loadFromFile(ASSET_AK47_TEXTURE);
	m_vSize = sf::Vector2i(m_texture.getSize());
	m_vLocation = pos;
	  
	m_iPrimaryClip = m_iPrimaryClipMax = 3000;
	 
	m_iWeaponSoundID = pGame->m_SoundManager.GetID(ASSET_AK47_SOUND);
	m_iDryFireSoundID = pGame->m_SoundManager.GetID(ASSET_DRYFIRE);

	m_bIsAutomatic = true;
	m_bIsProjectileWeapon = true;
	m_fFireDelay = 0.08f;
	m_fNextAttackTime = 0.0f;
	m_szName = "Dev AK47";

	/* Initialise physics */
	m_Physics.InitPhysics(pPhysics, (int)pos.x, (int)pos.y, ASSET_AK47_COLLIDER, CEntityPhysics::CATEGORY_PLAYER_PICKUP,
		CEntityPhysics::CATEGORY_PLAYER_PICKUP | CEntityPhysics::CATEGORY_PLAYER);
}

CSpentCasingEffect* C6SAk47::CreateCasingEffect(CWorldPhysics* pPhysics, sf::Vector2i vPos, sf::Vector2f vEjectDirection, float fEjectRotation) {
	CSpentCasingEffect* casing = new CSpentCasingEffect;
	casing->Init(pPhysics, ASSET_RIFLE_CASING, sf::Vector2i(20, 10), vPos, vEjectDirection * 230.0f, fEjectRotation, 750.0f);
	return casing;
}

void C6SSawnOff::Init(CWorldPhysics* pPhysics, sf::Vector2i pos, CGame* pGame) {
	m_vHoldOffset = sf::Vector2i(24, 0);
	m_vFireOffset = sf::Vector2i(40, -7);
	m_vCasingOffset = sf::Vector2i(20, -4);

	m_iFireSpeed = 50000;

	m_iBulletDamage = 16;

	m_bAffectedByGravity = true;
	m_fDrawRotation = m_fTrueRotation =  0.0f;
	m_texture.loadFromFile(ASSET_SAWNOFF_TEXTURE);
	m_vSize = sf::Vector2i(m_texture.getSize());
	m_vLocation = sf::Vector2f(pos);

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
	m_Physics.InitPhysics(pPhysics, pos.x, pos.y, m_vSize.x, m_vSize.y, CEntityPhysics::CATEGORY_PLAYER_PICKUP,
		CEntityPhysics::CATEGORY_PLAYER_PICKUP | CEntityPhysics::CATEGORY_PLAYER);
}

void C6STMP::Init(CWorldPhysics* pPhysics, sf::Vector2i pos, CGame* pGame) {
	m_vHoldOffset = sf::Vector2i(7, 0);
	m_vFireOffset = sf::Vector2i(20, -4);
	m_vCasingOffset = sf::Vector2i(10, -4);

	m_iFireSpeed = 5000;

	m_iBulletDamage = 20;

	m_bAffectedByGravity = true;
	m_fDrawRotation = m_fTrueRotation = 0.0f;
	m_texture.loadFromFile(ASSET_TMP_TEXTURE);
	m_vSize = sf::Vector2i(m_texture.getSize());
	m_vLocation = sf::Vector2f(pos);

	m_iPrimaryClip = m_iPrimaryClipMax = 20;

	m_iWeaponSoundID = pGame->m_SoundManager.GetID(ASSET_TMP_SOUND);
	m_iDryFireSoundID = pGame->m_SoundManager.GetID(ASSET_DRYFIRE);

	m_bIsAutomatic = true;
	m_bIsProjectileWeapon = true;
	m_fFireDelay = 0.05f;
	m_fNextAttackTime = 0.0f;
	m_szName = "Dev TMP";


	/* Initialise physics */
	m_Physics.InitPhysics(pPhysics, pos.x, pos.y, ASSET_AK47_COLLIDER, CEntityPhysics::CATEGORY_PLAYER_PICKUP,
		CEntityPhysics::CATEGORY_PLAYER_PICKUP | CEntityPhysics::CATEGORY_PLAYER);
}

CSpentCasingEffect* C6STMP::CreateCasingEffect(CWorldPhysics* pPhysics, sf::Vector2i vPos, sf::Vector2f vEjectDirection, float fEjectRotation) {
	CSpentCasingEffect* casing = new CSpentCasingEffect();
	casing->Init(pPhysics, ASSET_PISTOL_CASING, sf::Vector2i(20, 10), vPos, vEjectDirection * 210.0f, fEjectRotation, 4.0f);
	return casing;
}