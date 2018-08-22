#ifndef __C6SWEAPONMAKERSTATE_H__
#define __C6SWEAPONMAKERSTATE_H__

/*
	State for creating, testing, and exporting new game weapons
	Chris De Pasquale
	Created		16/05/18
	Last Edit	16/05/18
*/

#include "CGameState.h"
#include "C6SMap.h"
#include "C6SPlayer.h"
#include "CGlobal.h"
#include "CProjectileManager.h"
#include "CEntityManager.h"
#include "CWorldPhysics.h"
#include "CGoreEngine.h"
//#include "CLuaLink.h"
#include "CBaseNPC.h"
#include "CBaseMeleeWeapon.h"
#include "CBaseProjectileWeapon.h"
#include "6SWeapons.h"

class C6SWeaponMakerState : public CGameState {
	tgui::Gui m_Gui;	

	CProjectileManager m_ProjectileManager;
	C6SReleaseMap m_Map;
	C6SPlayer m_Player;
	CGlobal m_gameSettings;
	CEntityManager m_EntMgr;
	CWorldPhysics m_Physics;

	CBaseMeleeWeapon* pMeleeWeapon = NULL;
	C6SBaseDevPistol* pProjectileWeapon = NULL;

	const char* BTN_RELOAD_ID = "BtnReload";

	const char* CHK_INFINITE_AMMO = "ChkInfAmmo";
	const char* CHK_AFFECTED_BY_GRAVITY = "CheckGravity";
	const char* CHK_AUTOMATIC_ID = "CheckAutoWpn";
	
	const char* PRIMARY_CLIP_SLIDER_ID = "SldPrimClip";
	const char* PRIMARY_CLIP_LABEL_ID = "LblPrimClip";
	const char* FIRESPEED_SLIDER_ID = "SldFireSpeed";
	const char* FIRESPEED_LABEL_ID = "LblFireSpeed";
	const char* FIREDELAY_SLIDER_ID = "SldFireDelay";
	const char* FIREDELAY_LABEL_ID = "LblFireDelay";
	const char* RECOIL_MIN_SLIDER_ID = "SldRecoilMin";
	const char* RECOIL_MIN_LABEL_ID = "LbLRecoilMin";
	const char* RECOIL_MAX_SLIDER_ID = "SldRecoilMax";
	const char* RECOIL_MAX_LABEL_ID = "LblRecoilMax";
	const char* SPREAD_MAX_SLIDER_ID = "SldSpreadMax";
	const char* SPREAD_MAX_LBL_ID = "LblSpreadMax";
	const char* BULLETS_PER_SHOT_SLIDER_ID = "SldBulletsPerShot";
	const char* BULLETS_PER_SHOT_LBL_ID = "LblBulletsPerShot";
	const char* BULLET_DAMAGE_SLIDER_ID = "SldBulletDamage";
	const char* BULLET_DAMAGE_LABEL_ID = "LblBulletDamage";
	const int SLD_FIRESPEED_MIN = 100;
	const int SLD_FIRESPEED_MAX = 10000;
	const sf::Vector2f SLD_FIREDELAY_RANGE = sf::Vector2f(0.0f, 10.0f);
	const int SLD_FIREDELAY_FACTOR = 10;
	const sf::Vector2f SLD_RECOIL_MIN_RANGE = sf::Vector2f(0.0f, 10.0f);
	const int SLD_RECOIL_MIN_FACTOR = 10;
	const sf::Vector2f SLD_RECOIL_MAX_RANGE = sf::Vector2f(0.0f, 10.0f);
	const int SLD_RECOIL_MAX_FACTOR = 10;
	const sf::Vector2f SLD_SPREAD_MAX_RANGE = sf::Vector2f(0.0f, 10.0f);
	const int SLD_SPREAD_MAX_FACTOR = 10;
	const int SLD_BULLETS_PER_SHOT_MIN = 0;
	const int SLD_BULLETS_PER_SHOT_MAX = 30;
	const int SLD_BULLET_DAMAGE_MIN = 0;
	const int SLD_BULLET_DAMAGE_MAX = 200;
	const int SLD_PRIMARY_CLIP_MIN = 0;
	const int SLD_PRIMARY_CLIP_MAX = 200;


	const int TEXT_SIZE = 24;
	const int TEXT_SPACING = 12;

	const int MENU_HEIGHT = 400; 
	const tgui::Layout MENU_PADDING = tgui::Layout(4.0f);

	const sf::Color SLIDER_TEXT_COLOR = sf::Color::White;
	const tgui::Layout2d SLIDER_SIZE = tgui::Layout2d(150.0f, (const float)TEXT_SIZE);

	const tgui::Layout2d CHECKBOX_SIZE = tgui::Layout2d(20.0f, 20.0f);

	const float BUTTON_WIDTH = 150.0f;
	const float BUTTON_HEIGHT = 26.0f;

	bool m_bGUIEnabled = false;

	void FireSpeedChanged();
	void FireDelayChanged();
	void RecoilMinChanged();
	void RecoilMaxChanged();
	void SpreadMaxChanged();
	void BulletsPerShotChanged();
	void BulletDamageChanged();

	void PrimaryClipMaxChanged();
	void WeaponAutomaticChecked();
	void InfiniteBulletsChecked();
	void AffectedByGravityChecked();
	void ReloadPressed();

	
	template<typename T> void PanelLabelUpdate(tgui::Panel::Ptr panel, T value, const char* szLabelID);
	tgui::Panel::Ptr GetWeaponPanel();
	tgui::Panel::Ptr GetStatsPanel();
	tgui::Panel::Ptr GetMiscPanel();

public:
	void Init(CGame* pGame);
	void Cleanup(CGame* pGame);

	void Draw(CGame* pGame);
	void Update(CGame* pGame, float fFrameTime);
	void HandleInput(CGame* pGame);

	void PauseState();
	void ResumeState();

	void LoadFromFilePressed(CGame* pGame);

	void ReturnToMenu(CGame* pGame);

	void Cancel(CGame* pGame);

	static C6SWeaponMakerState* Instance() {
		static C6SWeaponMakerState state;
		return &state;
	}
};

#endif

