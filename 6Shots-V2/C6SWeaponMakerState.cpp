#include "stdafx.h"
#include "C6SWeaponMakerState.h"
#include "CGame.h"
#include "ASSETS.h"
#include "6SWeapons.h"
#include "CBaseNPC.h"
#include "C6SMenuState.h"
#include "GUIFactory.h"

#define GRAVITY 12.0f
#define TERMINAL 200.0f
#define FONT_SIZE 24
#define PLAYER_JUMPSPEED -290.0f
#define PLAYER_MOVESPEED 200.0f

void WeaponMakerTabClicked(tgui::Gui* pGUI) {
	tgui::Tabs::Ptr tab = pGUI->get<tgui::Tabs>("WeaponMkrTabMenu");
	int selectedTab = tab->getSelectedIndex();

	switch (selectedTab) {
	case 0:
		pGUI->get("WeaponPanel")->setVisible(true);
		pGUI->get("StatsPanel")->setVisible(false);
		pGUI->get("MiscPanel")->setVisible(false);
		break;
	case 1:
		pGUI->get("WeaponPanel")->setVisible(false);
		pGUI->get("StatsPanel")->setVisible(true);
		pGUI->get("MiscPanel")->setVisible(false);
		break;
	case 2:
		pGUI->get("WeaponPanel")->setVisible(false);
		pGUI->get("StatsPanel")->setVisible(false);
		pGUI->get("MiscPanel")->setVisible(true);
		break;
	default:
		break;
	}
}
template<typename T>
void C6SWeaponMakerState::PanelLabelUpdate(tgui::Panel::Ptr panel, T value, const char* szLabelID) {
	if (!pProjectileWeapon || !panel)
		return;
	tgui::Label::Ptr lbl = panel->get<tgui::Label>(szLabelID);
	if (!lbl)
		return;
	lbl->setText(std::to_string(value));
}
tgui::Panel::Ptr C6SWeaponMakerState::GetWeaponPanel() {
	return m_Gui.get<tgui::Panel>("WeaponPanel");
}
tgui::Panel::Ptr C6SWeaponMakerState::GetStatsPanel() {
	return m_Gui.get<tgui::Panel>("StatsPanel");
}
tgui::Panel::Ptr C6SWeaponMakerState::GetMiscPanel() {
	return m_Gui.get<tgui::Panel>("MiscPanel");
}

void C6SWeaponMakerState::ReloadPressed() {
	if (!pProjectileWeapon) 
		return;
	pProjectileWeapon->RefillPrimaryClip();
}

void C6SWeaponMakerState::InfiniteBulletsChecked() {
	if (!pProjectileWeapon) 
		return;
	auto panel = GetMiscPanel();
	if (!panel)
		return;
	tgui::CheckBox::Ptr check = panel->get<tgui::CheckBox>(CHK_INFINITE_AMMO);
	if (check == NULL)
		return;

	bool bInfiniteAmmo = check->isChecked(); 
	pProjectileWeapon->SetInfiniteAmmo(bInfiniteAmmo);
	printf("Infinite ammo %s\n", bInfiniteAmmo ? "Enabled" : "Disabled");
}
void C6SWeaponMakerState::AffectedByGravityChecked() {
	auto panel = GetMiscPanel();
	if (!pProjectileWeapon || !panel)
		return;
	tgui::CheckBox::Ptr check = panel->get<tgui::CheckBox>(CHK_AFFECTED_BY_GRAVITY);
	if (check == NULL)
		return;

	bool bItem = check->isChecked();
	pProjectileWeapon->SetAffectedByGravity(bItem);
	printf("Affected by gravity %s\n", bItem ? "Enabled" : "Disabled");
}
void C6SWeaponMakerState::WeaponAutomaticChecked() {
	auto panel = GetStatsPanel();
	if (!pProjectileWeapon || !panel) 
		return;
	tgui::CheckBox::Ptr check = panel->get<tgui::CheckBox>(CHK_AUTOMATIC_ID);
	if (check == NULL)
		return;
	pProjectileWeapon->SetAutomatic(check->isChecked());
}

void C6SWeaponMakerState::FireSpeedChanged() {
	auto panel = GetStatsPanel();
	if (!pProjectileWeapon || !panel)
		return;
	tgui::Slider::Ptr sld = panel->get<tgui::Slider>(FIRESPEED_SLIDER_ID);
	if (sld == NULL)
		return;

	float sldVal = sld->getValue();
	PanelLabelUpdate(panel, sldVal, FIRESPEED_LABEL_ID);
	pProjectileWeapon->SetFirespeed((int)sldVal);
}

void C6SWeaponMakerState::FireDelayChanged() {
	auto panel = GetStatsPanel();
	if (!pProjectileWeapon || !panel)
		return;
	tgui::Slider::Ptr sld = panel->get<tgui::Slider>(FIREDELAY_SLIDER_ID);
	if (sld == NULL)
		return;

	float sldVal = sld->getValue();
	float trueValue = sldVal / (float)SLD_FIREDELAY_FACTOR;
	PanelLabelUpdate(panel, trueValue, FIREDELAY_LABEL_ID);
	pProjectileWeapon->SetFireDelay(trueValue);
}
void C6SWeaponMakerState::RecoilMinChanged() {
	auto panel = GetStatsPanel();
	if (!pProjectileWeapon || !panel)
		return;
	tgui::Slider::Ptr sldMin = panel->get<tgui::Slider>(RECOIL_MIN_SLIDER_ID);
	tgui::Slider::Ptr sldMax = panel->get<tgui::Slider>(RECOIL_MAX_SLIDER_ID);
	if (sldMin == NULL || sldMax == NULL)
		return;

	// Get value as float
	float sldVal = sldMin->getValue();
	float trueValue = sldVal / (float)SLD_RECOIL_MIN_FACTOR;

	// If min > max, update max to equal min
	if (sldVal > sldMax->getValue()) {
		sldMax->setValue(sldVal);
		PanelLabelUpdate(panel, trueValue, RECOIL_MAX_LABEL_ID);
		pProjectileWeapon->SetRecoilMax(trueValue);
	}

	PanelLabelUpdate(panel, trueValue, RECOIL_MIN_LABEL_ID);
	pProjectileWeapon->SetRecoilMin(trueValue);
}


void C6SWeaponMakerState::RecoilMaxChanged() {
	auto panel = GetStatsPanel();
	if (!pProjectileWeapon || !panel)
		return;
	tgui::Slider::Ptr sldMin = panel->get<tgui::Slider>(RECOIL_MIN_SLIDER_ID);
	tgui::Slider::Ptr sldMax = panel->get<tgui::Slider>(RECOIL_MAX_SLIDER_ID);
	if (sldMin == NULL || sldMax == NULL)
		return;

	// Get value as float
	float sldVal = sldMax->getValue();
	float trueValue = sldVal / (float)SLD_RECOIL_MAX_FACTOR;

	// If max < min, update min to equal max
	if (sldVal < sldMin->getValue()) {
		sldMin->setValue(sldVal);
		PanelLabelUpdate(panel, trueValue, RECOIL_MIN_LABEL_ID);
		pProjectileWeapon->SetRecoilMin(trueValue);
	}

	PanelLabelUpdate(panel, trueValue, RECOIL_MAX_LABEL_ID);
	pProjectileWeapon->SetRecoilMax(trueValue);
}

void C6SWeaponMakerState::SpreadMaxChanged() {
	auto panel = GetStatsPanel();
	if (!pProjectileWeapon || !panel)
		return;
	tgui::Slider::Ptr sld = panel->get<tgui::Slider>(SPREAD_MAX_SLIDER_ID);
	if (sld == NULL)
		return;

	float sldVal = sld->getValue();
	float trueValue = sldVal / (float)SLD_SPREAD_MAX_FACTOR;
	pProjectileWeapon->SetSpreadMax(trueValue);
	PanelLabelUpdate(panel, trueValue, SPREAD_MAX_LBL_ID);
}

void C6SWeaponMakerState::BulletsPerShotChanged() {
	auto panel = GetStatsPanel();
	if (!pProjectileWeapon || !panel)
		return;
	tgui::Slider::Ptr sld = panel->get<tgui::Slider>(BULLETS_PER_SHOT_SLIDER_ID);
	if (sld == NULL)
		return;

	pProjectileWeapon->SetBulletsPerShot((int)sld->getValue());
	PanelLabelUpdate(panel, sld->getValue(), BULLETS_PER_SHOT_LBL_ID);
}

void C6SWeaponMakerState::BulletDamageChanged() {
	auto panel = GetStatsPanel();
	if (!pProjectileWeapon || !panel)
		return;
	tgui::Slider::Ptr sld = panel->get<tgui::Slider>(BULLET_DAMAGE_SLIDER_ID);
	if (sld == NULL)
		return;

	pProjectileWeapon->SetBulletDamage((int)sld->getValue());
	PanelLabelUpdate(panel, sld->getValue(), BULLET_DAMAGE_LABEL_ID);
}

void C6SWeaponMakerState::PrimaryClipMaxChanged() {
	auto panel = GetStatsPanel();
	if (!pProjectileWeapon || !panel)
		return;
	tgui::Slider ::Ptr sldClipSize = panel->get<tgui::Slider>(PRIMARY_CLIP_SLIDER_ID);
	if (sldClipSize == NULL)
		return;
	pProjectileWeapon->SetPrimaryClipMax((int)sldClipSize->getValue());
	PanelLabelUpdate(panel, sldClipSize->getValue(), PRIMARY_CLIP_LABEL_ID);
}

void C6SWeaponMakerState::ReturnToMenu(CGame* pGame) {
	pGame->m_WindowManager.RemoveGUI();
	pGame->m_StateHandler.ChangeState(C6SMenuState::Instance(), pGame);
}

void C6SWeaponMakerState::Init(CGame* pGame) {
	/* Initialize physics engine */
	m_Physics.InitPhysics(pGame, GRAVITY, false);

	if (IsAlreadyInitialised()) {
		m_Gui.setView(pGame->m_WindowManager.GetGUIView());

	} else {
		// Test Weapon - pistol
		pProjectileWeapon = new C6SBaseDevPistol();
		pProjectileWeapon->Init(&m_Physics, sf::Vector2i(1000, 1000), pGame);
		m_EntMgr.AddWeapon(pProjectileWeapon);

		// Load theme & init GUI
		m_Gui.setTarget(*pGame->m_WindowManager.GetWindow());

		// Create Tabs
		tgui::Tabs::Ptr tabs = GUIFactory::Tabs("Weapon", "Stats", "Misc");
		tabs->connect("TabSelected", WeaponMakerTabClicked, &m_Gui);
		m_Gui.add(tabs, "WeaponMkrTabMenu");

		// Create panels for each tab, with a Left/Right box layout 
		auto weaponPanel = GUIFactory::PanelCentredSize(m_Gui, sf::Vector2i((int)tgui::bindWidth(m_Gui).getValue(), MENU_HEIGHT)); 
		weaponPanel->setPosition(tabs->getPosition().x, tabs->getPosition().y + tgui::bindHeight(tabs));
		m_Gui.add(weaponPanel, "WeaponPanel");

		tgui::Panel::Ptr statsPanel = tgui::Panel::copy(weaponPanel);
		m_Gui.add(statsPanel, "StatsPanel");

		tgui::Panel::Ptr miscPanel = tgui::Panel::copy(statsPanel);
		m_Gui.add(miscPanel, "MiscPanel");

		const tgui::Layout2d PanelTopLeft = weaponPanel->getPosition();
		const tgui::Layout2d PanelSize = weaponPanel->getSize();

		// Create titles for each tab
		tgui::Layout2d weaponCursor = tgui::Layout2d(MENU_PADDING, MENU_PADDING);
		tgui::Layout2d statCursor = weaponCursor;
		tgui::Layout2d miscCursor = weaponCursor;

		////
		////	WEAPON PANEL ITEMS
		////

		////
		////	STAT PANEL ITEMS
		////

		// isAutomatic
		GUILayout::Checkbox(statsPanel, statCursor, CHK_AUTOMATIC_ID, "Automatic", pProjectileWeapon->GetAutomatic(), &C6SWeaponMakerState::WeaponAutomaticChecked, this);

		// Prim clip
		GUILayout::SliderInt(statsPanel, statCursor, PRIMARY_CLIP_SLIDER_ID, PRIMARY_CLIP_LABEL_ID, "Primary Clip Max", pProjectileWeapon->GetPrimaryClipMax(), 
			sf::Vector2i(SLD_PRIMARY_CLIP_MIN, SLD_PRIMARY_CLIP_MAX), &C6SWeaponMakerState::PrimaryClipMaxChanged, this);

		// Recoil Min
		GUILayout::SliderFloat(statsPanel, statCursor, RECOIL_MIN_SLIDER_ID, RECOIL_MIN_LABEL_ID, "Recoil Min", pProjectileWeapon->GetRecoilMin(), 
			SLD_RECOIL_MIN_RANGE, &C6SWeaponMakerState::RecoilMinChanged, this);

		// Recoil Max
		GUILayout::SliderFloat(statsPanel, statCursor, RECOIL_MAX_SLIDER_ID, RECOIL_MAX_LABEL_ID, "Recoil Max", 
			pProjectileWeapon->GetRecoilMax(), SLD_RECOIL_MAX_RANGE, &C6SWeaponMakerState::RecoilMaxChanged, this);

		// Spread max
		GUILayout::SliderFloat(statsPanel, statCursor, SPREAD_MAX_SLIDER_ID, SPREAD_MAX_LBL_ID, "Spread", pProjectileWeapon->GetSpreadMax(), 
			SLD_SPREAD_MAX_RANGE, &C6SWeaponMakerState::SpreadMaxChanged, this);

		// Firespeed 
		GUILayout::SliderInt(statsPanel, statCursor, FIRESPEED_SLIDER_ID, FIRESPEED_LABEL_ID, "Speed", pProjectileWeapon->GetFirespeed(), 
			sf::Vector2i(SLD_FIRESPEED_MIN, SLD_FIRESPEED_MAX), &C6SWeaponMakerState::FireSpeedChanged, this);

		// Fire Delay
		GUILayout::SliderFloat(statsPanel, statCursor, FIREDELAY_SLIDER_ID, FIREDELAY_LABEL_ID, "Delay", pProjectileWeapon->GetFireDelay(), 
			SLD_FIREDELAY_RANGE, &C6SWeaponMakerState::FireDelayChanged, this);

		// Bullets per shot
		GUILayout::SliderInt(statsPanel, statCursor, BULLETS_PER_SHOT_SLIDER_ID, BULLETS_PER_SHOT_LBL_ID, "Bullets Per Shot", pProjectileWeapon->GetBulletsPerShot(), 
			sf::Vector2i(SLD_BULLETS_PER_SHOT_MIN, SLD_BULLETS_PER_SHOT_MAX), &C6SWeaponMakerState::BulletsPerShotChanged, this);

		// Bullet damage
		GUILayout::SliderInt(statsPanel, statCursor, BULLET_DAMAGE_SLIDER_ID, BULLET_DAMAGE_LABEL_ID, "Bullet Damage", pProjectileWeapon->GetBulletDamage(), 
			sf::Vector2i(SLD_BULLET_DAMAGE_MIN, SLD_BULLET_DAMAGE_MAX), &C6SWeaponMakerState::BulletDamageChanged, this);

		////
		////	MISC PANEL ITEMS
		////

		// Infinite bullets
		GUILayout::Checkbox(miscPanel, miscCursor, CHK_INFINITE_AMMO, "Infinite Bullets", pProjectileWeapon->GetInfiniteAmmo(),
			&C6SWeaponMakerState::InfiniteBulletsChecked, this);

		// IsAffectedByGravity
		GUILayout::Checkbox(miscPanel, miscCursor, CHK_AFFECTED_BY_GRAVITY, "Affected By Gravity", pProjectileWeapon->GetInfiniteAmmo(),
			&C6SWeaponMakerState::AffectedByGravityChecked, this);

		// Reload btn
		GUILayout::Button(miscPanel, miscCursor, "Reload", BTN_RELOAD_ID, &C6SWeaponMakerState::ReloadPressed, this);

		// Reload btn
		GUILayout::Button(miscPanel, miscCursor, "Exit to Menu", 0,
			&C6SWeaponMakerState::ReturnToMenu, this, pGame);

		////
		//// END GUI
		////

		/* Load map */
		m_Map.LoadFromFile(pGame, "publi");

		/* Init map collision spaces */
		m_Physics.ParseWorldMap(&m_Map);

		// Init player & Give dev weapon
		m_Player.Initialize(&m_Physics, ASSET_PLAYER_TEXTURE, ASSET_PLAYER_HAND, 10, 32, 64, 32, PLAYER_JUMPSPEED, PLAYER_MOVESPEED, m_Map.GetSpawn(pGame));
		m_Player.EquipWeapon(pProjectileWeapon, &m_EntMgr);

		CBaseNPC* npc = new CBaseNPC();
		npc->Initialize(&m_Physics, ASSET_NPC_TEXTURE, ASSET_PLAYER_HAND, 10, 25, 64, 32, PLAYER_JUMPSPEED, PLAYER_MOVESPEED, m_Map.GetSpawn(pGame));
		m_EntMgr.AddNPC(npc);		
	}

	/* Reset elapsed time */
	pGame->m_Time.ResetFrameTime();

	/* Reset map data */
	m_Map.ResetState();
	m_Map.ResetSpawns();

	/* Player pos */
	m_Player.SetPosition(m_Map.GetSpawn(pGame));

	// Set GUI as inactive
	m_bGUIEnabled = false;
	pGame->m_WindowManager.RemoveGUI();

	m_gameSettings.Init(GRAVITY, TERMINAL);
}

void C6SWeaponMakerState::Cleanup(CGame* pGame) {}

void C6SWeaponMakerState::Draw(CGame* pGame) {
	CWindowManager* pWnd = &pGame->m_WindowManager;
	m_Map.DrawBackground(pGame, m_Player.GetPosition2f());

	/* Centre Camera on Player */
	pWnd->MoveCamera(m_Player.GetPosition2f());

	m_Map.DrawLevel(pGame);
	m_Player.Draw(pGame);
	m_EntMgr.Draw(pGame);
	m_ProjectileManager.DrawProjectiles(pGame);

	/* Debug draw if enabled */
	m_Physics.Draw();

	/* Restore camera */
	pWnd->RestoreDefaultCamera();

	if (m_bGUIEnabled)
		m_Gui.draw();
}

void C6SWeaponMakerState::Update(CGame* pGame, float fFrameTime) {
	m_Player.Update(fFrameTime, pGame, &m_ProjectileManager, &m_Physics);

	m_ProjectileManager.UpdateProjectiles(fFrameTime);
	if (m_ProjectileManager.HasCollisionEvents()) {
		CProjectileManager::SProjectileEvent e;
		while (m_ProjectileManager.NextProjectileEvent(e)) {
			printf("Projectile hit client %i for %i damage\n", e.iPlayerID, e.iProjectileDamage);
		}
	}

	if (m_bGUIEnabled && pGame->m_Input.ActionKeyPressed(EAction::Action_Menu_Toggle)) {
		printf("Enabled GUI\n");
		pGame->m_WindowManager.RemoveGUI();
		m_bGUIEnabled = false;
	}
	if (!m_bGUIEnabled && pGame->m_Input.ActionKeyPressed(EAction::Action_Show_Chat)) {
		printf("Disabled GUI\n");
		pGame->m_WindowManager.SetGUI(&m_Gui);
		pGame->m_WindowManager.SetGUIActive(true);
		m_bGUIEnabled = true;
	}

	m_EntMgr.Update(fFrameTime, pGame, &m_Physics);
	m_Physics.Step(fFrameTime);
}
void C6SWeaponMakerState::HandleInput(CGame* pGame){
	// This is a hack to fix world to screen stuff
	CWindowManager* pWnd = &pGame->m_WindowManager;
	pWnd->MoveCamera(m_Player.GetPosition2f());
	m_Player.HandleInput(pGame, &m_EntMgr, true);
	pWnd->RestoreDefaultCamera();
}

void C6SWeaponMakerState::PauseState() {}
void C6SWeaponMakerState::ResumeState() {}