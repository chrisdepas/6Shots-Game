#include "stdafx.h"
#include "C6SMapMakerState.h"
#include "C6SMenuState.h"
#include "CGame.h"
#include "ASSETS.h"
#include "6SWeapons.h"
#include "CBaseNPC.h"
#include <iostream>
#include <fstream>
#define FONT_SIZE 24
#define LOOK_SPEED 300.0f

#define TILESET_LINEBUFFER_SIZE 1024

const char* C6SMap::szMapTypeStrings[C6SMap::EMapType::TYPE_COUNT] = {
	"unknown",
	"develop",
	"default"
};

void C6SMapMakerState::ExportMap(CGame* pGame) {
	tgui::Panel::Ptr panelSettings = (tgui::Panel::Ptr)m_pGUI->get<tgui::Panel>("SettingsPanel");
	if (panelSettings == NULL)
		return;
	tgui::EditBox::Ptr editMapPath = (tgui::EditBox::Ptr)panelSettings->get<tgui::EditBox>(sf::String("EditMapPath"));
	if (editMapPath == NULL)
		return;

	sf::String sfText = editMapPath->getText();
	std::string stText = sfText.toAnsiString();
	const char* mapPath = stText.c_str();

	m_Map.ExportToFile(pGame, (char*)mapPath);
}
void C6SMapMakerState::LoadMap(CGame* pGame) {
	tgui::Panel::Ptr panelSettings = (tgui::Panel::Ptr)m_pGUI->get<tgui::Panel>("SettingsPanel");
	if (panelSettings == NULL)
		return;
	tgui::EditBox::Ptr editMapPath = (tgui::EditBox::Ptr)panelSettings->get<tgui::EditBox>(sf::String("EditMapPath"));
	if (editMapPath == NULL)
		return;
	sf::String sfText = editMapPath->getText();
	std::string stText = sfText.toAnsiString();
	const char* mapPath = stText.c_str();

	if (!m_Map.InitializeFromFile(pGame, (char*)mapPath)) {
		printf("Failed to initialise map from file");
	}
}
void C6SMapMakerState::TilePressed(int index, CGame* pGame) {
	if (index < (int)m_vTileset.size()) {
		m_bPlacingTile = true;
		m_iPlacingTileIndex = index;
		HideMenu();

		/* Disable GUI to allow us to receive mouse data */
		pGame->m_WindowManager.SetGUIActive(false);
	}
}
void C6SMapMakerState::LoadTileset(char* szTilset, CGame* pGame) {
	if (m_vTileset.size() != 0) {
		puts("Can't load tileset - tileset already loaded.");
		return;
	}

	std::ifstream file(szTilset);
	if (!file || !file.good()) {
		printf("Unable to read from '%s' - bad file supplied\n", szTilset);
		return;
	}

	int tileIndex = 0;
	char lineBuf[TILESET_LINEBUFFER_SIZE];
	while (file.getline(lineBuf, TILESET_LINEBUFFER_SIZE)) {
		STilesetTile newTile;
		if (newTile.texture.loadFromFile(lineBuf)) {
			newTile.tileindex = tileIndex;
			tileIndex++;
			m_vTileset.push_back(newTile);
			continue;
		}
	}
	file.close();

	tgui::Panel::Ptr tilesPanel = (tgui::Panel::Ptr)m_pGUI->get<tgui::Panel>("TilesPanel");
	int xOffset = 0;
	int yOffset = m_iTileTabOffset;
	for (unsigned int i = 0; i < m_vTileset.size(); i++) {
		tgui::Picture::Ptr picTile = std::make_shared<tgui::Picture>();
		picTile->setSize(32, 32);
		picTile->setPosition(tgui::Layout((float)xOffset), tgui::Layout((float)yOffset));
		picTile->setTexture(m_vTileset[i].texture);
		picTile->connect("Clicked", &C6SMapMakerState::TilePressed, this, m_vTileset[i].tileindex, pGame);
		tilesPanel->add(picTile);

		xOffset += 34;
		if (xOffset+32 > tilesPanel->getSize().x) {
			xOffset = 0;
			yOffset += 34;
		}
	}
	

	printf("%i tiles loaded from tileset '%s'\n", ++tileIndex, szTilset);
}
void C6SMapMakerState::ResizePressed() {
	tgui::Panel::Ptr panelBackround = (tgui::Panel::Ptr)m_pGUI->get<tgui::Panel>("TilesPanel");
	if (panelBackround == NULL)
		return;
	tgui::EditBox::Ptr editY = (tgui::EditBox::Ptr)panelBackround->get<tgui::EditBox>(sf::String("EditMapY"));
	if (editY == NULL)
		return;
	tgui::EditBox::Ptr editX = (tgui::EditBox::Ptr)panelBackround->get<tgui::EditBox>(sf::String("EditMapX"));
	if (editX == NULL)
		return;

	sf::String sfTextX = editX->getText();
	std::string stTextX = sfTextX.toAnsiString();
	sf::String sfTextY = editY->getText();
	std::string stTextY = sfTextY.toAnsiString();

	int newWidth = strtol(stTextX.c_str(), NULL, 10);
	int newHeight = strtol(stTextY.c_str(), NULL, 10);

	if (newWidth <= 0 || newHeight <= 0 || newWidth > C6SMapMakerState::MapMaxWidth || newHeight > C6SMapMakerState::MapMaxHeight) {
		printf("Can't resize - size (%i, %i) is out of range\n", newWidth, newHeight);
		return;
	}

	printf("Resizing map to (%i, %i)\n", newWidth, newHeight);
	m_Map.Resize(newWidth, newHeight, C6SMapMakerState::MapDefaultGridSize);
}

void C6SMapMakerState::LoadTilesetPressed(CGame* pGame) {
	tgui::Panel::Ptr panelBackround = (tgui::Panel::Ptr)m_pGUI->get<tgui::Panel>("TilesPanel");
	if (panelBackround == NULL)
		return;
	tgui::EditBox::Ptr editLoadTileset = (tgui::EditBox::Ptr)panelBackround->get<tgui::EditBox>(sf::String("EditLoadTileset"));
	if (editLoadTileset == NULL)
		return;

	sf::String sfText = editLoadTileset->getText();
	std::string stText = sfText.toAnsiString();
	const char* loadTileset = stText.c_str();

	printf("Loading tileset '%s'\n", stText.c_str());
	LoadTileset((char*)loadTileset, pGame);
}

void C6SMapMakerState::BeginTileTypePlacement(CGame* pGame, int eTileType) {
	m_bPlacingTileType = true;
	m_ePlacingTileType = C6SMap::SMapTile::ETileType(eTileType);
	HideMenu();
	/* Disable GUI to allow us to receive mouse data */
	pGame->m_WindowManager.SetGUIActive(false);
}

void C6SMapMakerState::GUIChecked() {
	tgui::CheckBox::Ptr check = (tgui::CheckBox::Ptr)m_pGUI->get<tgui::CheckBox>(sf::String("CheckGUI"));
	if (check == NULL)
		return;

	if (check->isChecked()) {
		m_bGUIEnabled = true;
		ShowMenu();
	}
	else {
		m_bGUIEnabled = false;
		HideMenu();
	}
}

void C6SMapMakerState::BoundingBoxChecked() {
	tgui::Panel::Ptr panelSettings = (tgui::Panel::Ptr)m_pGUI->get<tgui::Panel>("SettingsPanel");
	if (panelSettings == NULL)
		return;
	tgui::CheckBox::Ptr check = (tgui::CheckBox::Ptr)panelSettings->get<tgui::CheckBox>(sf::String("CheckBoundingBox"));
	if (check == NULL)
		return;
	m_bBoundingBox = check->isChecked();
}

void C6SMapMakerState::GridChecked() {
	tgui::Panel::Ptr panelSettings = (tgui::Panel::Ptr)m_pGUI->get<tgui::Panel>("SettingsPanel");
	if (panelSettings == NULL)
		return;
	tgui::CheckBox::Ptr check = (tgui::CheckBox::Ptr)panelSettings->get<tgui::CheckBox>(sf::String("CheckGrid"));
	if (check == NULL)
		return;
	m_bGrid = check->isChecked();
}

void C6SMapMakerState::TileTypeChecked() {
	tgui::Panel::Ptr panelSettings = (tgui::Panel::Ptr)m_pGUI->get<tgui::Panel>("SettingsPanel");
	if (panelSettings == NULL)
		return;
	tgui::CheckBox::Ptr check = (tgui::CheckBox::Ptr)panelSettings->get<tgui::CheckBox>(sf::String("CheckTileType"));
	if (check == NULL)
		return;
	m_bTileType = check->isChecked();
}

void C6SMapMakerState::LoadStaticBackground() {
	tgui::Panel::Ptr panelBackround = (tgui::Panel::Ptr)m_pGUI->get<tgui::Panel>("BackgroundPanel");
	if (panelBackround == NULL)
		return;
	tgui::EditBox::Ptr editStatic = (tgui::EditBox::Ptr)panelBackround->get<tgui::EditBox>(sf::String("EditStaticBackground"));
	if (editStatic == NULL)
		return;

	sf::String sfText = editStatic->getText();
	std::string stText = sfText.toAnsiString();
	const char* staticBackground = stText.c_str();

	printf("Loading static background '%s'\n", stText.c_str());
	if (m_Map.LoadStaticBackground((char*)staticBackground)) {
		/* Save static background path */
		if (m_szStaticBackground != 0) {
			free(m_szStaticBackground);
		}
		m_szStaticBackground = (char*)malloc(strlen(staticBackground) + 1);
		memcpy(m_szStaticBackground, staticBackground, strlen(staticBackground) + 1);
	}
}

void C6SMapMakerState::LoadDynamicBackground1() {
	tgui::Panel::Ptr panelBackround = (tgui::Panel::Ptr)m_pGUI->get<tgui::Panel>("BackgroundPanel");
	if (panelBackround == NULL)
		return;
	tgui::EditBox::Ptr editDynamic = (tgui::EditBox::Ptr)panelBackround->get<tgui::EditBox>(sf::String("EditDynamicBackground1"));
	if (editDynamic == NULL)
		return;
	tgui::Slider::Ptr sliderScale = (tgui::Slider::Ptr)panelBackround->get<tgui::Slider>(sf::String("SliderDynamicBackground1Scale"));
	if (sliderScale == NULL)
		return;
	float moveScale = sliderScale->getValue() / 10.0f;
	
	sf::String sfText = editDynamic->getText();
	std::string stText = sfText.toAnsiString();
	const char* background = stText.c_str();

	printf("Loading dynamic background '%s' with movescale %f\n", background, moveScale);
	if (m_Map.LoadDynamicBackground((char*)background, 1, moveScale)) {
		/* Save static background path */
		if (m_szDynamicBackground1 != 0) {
			free(m_szDynamicBackground1);
		}
		m_szDynamicBackground1 = (char*)malloc(strlen(background) + 1);
		memcpy(m_szDynamicBackground1, background, strlen(background) + 1);
		m_fDynamicBackground1MoveScale = moveScale;
	}
}

void C6SMapMakerState::ExitToMenuPressed(CGame* pGame) {
	// Disable GUI handling
	pGame->m_WindowManager.SetGUI(NULL);
	pGame->m_WindowManager.SetGUIActive(false);

	/* BACK TO BLACK */
	pGame->m_StateHandler.ChangeState(C6SMenuState::Instance(), pGame);
}

void TabSelected(tgui::Gui* pGUI)
{
	tgui::Tab::Ptr tab = (tgui::Tab::Ptr)pGUI->get<tgui::Tab>(sf::String("TabsMenu"));
	int selectedTab = tab->getSelectedIndex();

	switch (selectedTab) {
	case 0:
		pGUI->get("BackgroundPanel")->show();
		pGUI->get("TilesPanel")->hide();
		pGUI->get("SettingsPanel")->hide();
		break;
	case 1:
		pGUI->get("BackgroundPanel")->hide();
		pGUI->get("TilesPanel")->show();
		pGUI->get("SettingsPanel")->hide();
		break;
	case 2:
		pGUI->get("BackgroundPanel")->hide();
		pGUI->get("TilesPanel")->hide();
		pGUI->get("SettingsPanel")->show();
		break;
	default:
		break;
	}
}

void C6SMapMakerState::HideMenu() {
	tgui::Tab::Ptr tab = (tgui::Tab::Ptr)m_pGUI->get<tgui::Tab>(sf::String("TabsMenu"));
	tab->hide();
	m_pGUI->get("BackgroundPanel")->hide();
	m_pGUI->get("TilesPanel")->hide();
	m_pGUI->get("SettingsPanel")->hide();
}

void C6SMapMakerState::ShowMenu() {
	tgui::Tab::Ptr tab = (tgui::Tab::Ptr)m_pGUI->get<tgui::Tab>(sf::String("TabsMenu"));
	tab->show();

	int selectedTab = tab->getSelectedIndex();
	switch (selectedTab) {
	case 0:
		m_pGUI->get("BackgroundPanel")->show();
		break;
	case 1:
		m_pGUI->get("TilesPanel")->show();
		break;
	case 2:
		m_pGUI->get("SettingsPanel")->show();
		break;
	default:
		break;
	}
}

void C6SMapMakerState::Init(CGame* pGame) {
	if (!IsAlreadyInitialised()) {

		/* Default loaded data to NULL */
		m_szStaticBackground = NULL;
		m_szDynamicBackground1 = NULL;
		m_fDynamicBackground1MoveScale = 0.0f;

		m_iTileTabOffset = 0;

		/* Map */
		m_Map.InitializeDev(ASSET_DEV_BACKGROUND, C6SMapMakerState::MapDefaultWidth, C6SMapMakerState::MapDefaultHeight, C6SMapMakerState::MapDefaultGridSize);

		/* GUI */
		m_pGUI = new tgui::Gui(*pGame->m_WindowManager.GetWindow());
		m_bGUIEnabled = true;

		try {
			// theme 
			tgui::Theme::Ptr theme = std::make_shared<tgui::Theme>("widgets/Black.txt");

			// Enable/disable GUI
			tgui::CheckBox::Ptr checkGUI = theme->load("Checkbox");
			checkGUI->setSize(25, 25);
			checkGUI->setPosition(0, 0);
			checkGUI->setText("Menu");
			checkGUI->setTextSize(24);
			checkGUI->connect("checked unchecked", &C6SMapMakerState::GUIChecked, this);
			checkGUI->check();
			m_pGUI->add(checkGUI, "CheckGUI");

			// Tabbed menu
			tgui::Tab::Ptr tabs = theme->load("Tab");
			tabs->add("Background");
			tabs->add("Tiles");
			tabs->add("Settings");
			tabs->setPosition(150, 0);
			tabs->connect("TabSelected", TabSelected, m_pGUI);
			m_pGUI->add(tabs, "TabsMenu");

			// Background panel
			tgui::Panel::Ptr backgroundPanel = theme->load("Panel");
			backgroundPanel->setSize(400, 300);
			backgroundPanel->setPosition(tabs->getPosition().x, tabs->getPosition().y + tabs->getTabHeight());
			m_pGUI->add(backgroundPanel, "BackgroundPanel");

			// Tiles Panel
			tgui::Panel::Ptr tilesPanel = tgui::Panel::copy(backgroundPanel);
			m_pGUI->add(tilesPanel, "TilesPanel");

			// Settings Panel
			tgui::Panel::Ptr settingsPanel = tgui::Panel::copy(backgroundPanel);
			m_pGUI->add(settingsPanel, "SettingsPanel");


			/* ----		Tiles Panel Objects		----	*/
			tgui::Layout tilesVertOffset(10.0f);

			tgui::Label::Ptr labelTileTypes = std::make_shared<tgui::Label>();
			labelTileTypes->setText("Set Tile Types:");
			labelTileTypes->setPosition(0.0f, tilesVertOffset); tilesVertOffset += 20.0f;
			labelTileTypes->setTextColor(sf::Color::White);
			tilesPanel->add(labelTileTypes);
			/* Air */
			tgui::Button::Ptr btnAir = theme->load("Button");
			btnAir->setText("Air");
			btnAir->setPosition(0.0f, tilesVertOffset);
			btnAir->setSize(100, 20.0f);
			btnAir->connect("pressed", &C6SMapMakerState::BeginTileTypePlacement, this, pGame, C6SMap::SMapTile::TYPE_AIR);
			tilesPanel->add(btnAir);
			/* Solid */
			tgui::Button::Ptr btnSolid = theme->load("Button");
			btnSolid->setText("Solid");
			btnSolid->setPosition(100.0f, tilesVertOffset);
			btnSolid->setSize(100, 20.0f);
			btnSolid->connect("pressed", &C6SMapMakerState::BeginTileTypePlacement, this, pGame, C6SMap::SMapTile::TYPE_SOLID);
			tilesPanel->add(btnSolid);
			/* Ladder */
			tgui::Button::Ptr btnLadder = theme->load("Button");
			btnLadder->setText("Ladder");
			btnLadder->setPosition(200.0f, tilesVertOffset); 
			btnLadder->setSize(100, 20.0f);
			btnLadder->connect("pressed", &C6SMapMakerState::BeginTileTypePlacement, this, pGame, C6SMap::SMapTile::TYPE_LADDER);
			tilesPanel->add(btnLadder);
			/* Boundary */
			tgui::Button::Ptr btnBoundary = theme->load("Button");
			btnBoundary->setText("Boundary");
			btnBoundary->setPosition(300.0f, tilesVertOffset); 
			btnBoundary->setSize(100, 20.0f);
			btnBoundary->connect("pressed", &C6SMapMakerState::BeginTileTypePlacement, this, pGame, C6SMap::SMapTile::TYPE_BOUNDARY);
			tilesPanel->add(btnBoundary);
			// NEXT LAYER 
			tilesVertOffset += 20.0f;
			/* Partial Solid */
			tgui::Button::Ptr btnPartial = theme->load("Button");
			btnPartial->setText("Partial");
			btnPartial->setPosition(0.0f, tilesVertOffset);
			btnPartial->setSize(100, 20.0f);
			btnPartial->connect("pressed", &C6SMapMakerState::BeginTileTypePlacement, this, pGame, C6SMap::SMapTile::TYPE_PARTIALSOLID);
			tilesPanel->add(btnPartial);
			/* Partial Solid */
			tgui::Button::Ptr btnWater = theme->load("Button");
			btnWater->setText("Water");
			btnWater->setPosition(100.0f, tilesVertOffset);
			btnWater->setSize(100, 20.0f);
			btnWater->connect("pressed", &C6SMapMakerState::BeginTileTypePlacement, this, pGame, C6SMap::SMapTile::TYPE_WATER);
			tilesPanel->add(btnWater);
			/* InstantKill */
			tgui::Button::Ptr btnDie = theme->load("Button");
			btnDie->setText("Death");
			btnDie->setPosition(200.0f, tilesVertOffset);
			btnDie->setSize(100, 20.0f);
			btnDie->connect("pressed", &C6SMapMakerState::BeginTileTypePlacement, this, pGame, C6SMap::SMapTile::TYPE_INSTADEATH);
			tilesPanel->add(btnDie);
			/* TYPE_SLOWDAMAGE */
			tgui::Button::Ptr btnHurt = theme->load("Button");
			btnHurt->setText("Hurt");
			btnHurt->setPosition(300.0f, tilesVertOffset);
			btnHurt->setSize(100, 20.0f);
			btnHurt->connect("pressed", &C6SMapMakerState::BeginTileTypePlacement, this, pGame, C6SMap::SMapTile::TYPE_SLOWDAMAGE);
			tilesPanel->add(btnHurt);
			// NEXT LAYER 
			tilesVertOffset += 20.0f;

			tgui::Label::Ptr labelTiles = std::make_shared<tgui::Label>();
			labelTiles->setText("Load Tiles:");
			labelTiles->setPosition(0.0f, tilesVertOffset); tilesVertOffset += 20.0f;
			labelTiles->setTextColor(sf::Color::White);
			tilesPanel->add(labelTiles);

			tgui::EditBox::Ptr editLoadTileset = theme->load("EditBox");
			editLoadTileset->setSize(300, 30);
			editLoadTileset->setPosition(0, tilesVertOffset); 
			editLoadTileset->setText("");
			editLoadTileset->setTextSize(12);
			tilesPanel->add(editLoadTileset, "EditLoadTileset");

			tgui::Button::Ptr btnLoadTileset = theme->load("Button");
			btnLoadTileset->setSize(tilesPanel->getSize().x - 300, 30);
			btnLoadTileset->setPosition(250, tilesVertOffset); tilesVertOffset += 30.0f;
			btnLoadTileset->setText("Load");
			btnLoadTileset->connect("Pressed", &C6SMapMakerState::LoadTilesetPressed, this, pGame);
			tilesPanel->add(btnLoadTileset);

			tgui::Label::Ptr labelTilePlaceMode = std::make_shared<tgui::Label>();
			labelTilePlaceMode->setText("Placement Mode:");
			labelTilePlaceMode->setPosition(0.0f, tilesVertOffset); tilesVertOffset += 20.0f;
			labelTilePlaceMode->setTextColor(sf::Color::White);
			tilesPanel->add(labelTilePlaceMode);

			tgui::RadioButton::Ptr rdioStickerMode = theme->load("RadioButton");
			rdioStickerMode->setSize(30, 30);
			rdioStickerMode->setPosition(10, tilesVertOffset); 
			rdioStickerMode->setText("Sticker");
//rdioStickerMode->connect("Checked Unchecked", &C6SMapMakerState::LoadTilesetPressed, this, pGame);
			tilesPanel->add(rdioStickerMode);

			tgui::RadioButton::Ptr rdioTilesMode = theme->load("RadioButton");
			rdioTilesMode->setSize(30, 30);
			rdioTilesMode->setPosition(150, tilesVertOffset); tilesVertOffset += 30.0f;
			rdioTilesMode->setText("Tiles");
		//	rdioTilesMode->connect("Checked Unchecked", &C6SMapMakerState::LoadTilesetPressed, this, pGame);
			tilesPanel->add(rdioTilesMode);

			/* Map resize X*/
			tgui::EditBox::Ptr editMapX = theme->load("EditBox");
			editMapX->setSize(150, 30);
			editMapX->setPosition(0, tilesVertOffset);
			editMapX->setText("32");
			editMapX->setTextSize(12);
			tilesPanel->add(editMapX, "EditMapX");

			/* Map resize Y*/
			tgui::EditBox::Ptr editMapY = theme->load("EditBox");
			editMapY->setSize(150, 30);
			editMapY->setPosition(150, tilesVertOffset); tilesVertOffset += 30.0f;
			editMapY->setText("32");
			editMapY->setTextSize(12);
			tilesPanel->add(editMapY, "EditMapY");

			/* Map resize btn */
			tgui::Button::Ptr btnResize = theme->load("Button");
			btnResize->setSize(tilesPanel->getSize().x - 300, 30);
			btnResize->setPosition(250, tilesVertOffset); tilesVertOffset += 30.0f;
			btnResize->setText("Resize Map");
			btnResize->connect("Pressed", &C6SMapMakerState::ResizePressed, this);
			tilesPanel->add(btnResize);

			// Save offset so tiles can be added later 
			m_iTileTabOffset = (int)tilesVertOffset.getValue();

			/* ----		Settings Panel Objects	----	*/
			tgui::Layout settingsVertOffset(10.0f);
			tgui::CheckBox::Ptr checkBoundingBox = theme->load("CheckBox");
			checkBoundingBox->setSize(25, 25);
			checkBoundingBox->setPosition(0, settingsVertOffset); settingsVertOffset += 25.0f;
			checkBoundingBox->setText("Show Bounding Box");
			checkBoundingBox->check();
			checkBoundingBox->connect("checked unchecked", &C6SMapMakerState::BoundingBoxChecked, this);
			settingsPanel->add(checkBoundingBox, "CheckBoundingBox");

			tgui::CheckBox::Ptr checkGrid = theme->load("CheckBox");
			checkGrid->setSize(25, 25);
			checkGrid->setPosition(0, settingsVertOffset); settingsVertOffset += 25.0f;
			checkGrid->setText("Show Grid");
			checkGrid->check();
			checkGrid->connect("checked unchecked", &C6SMapMakerState::GridChecked, this);
			settingsPanel->add(checkGrid, "CheckGrid");

			tgui::CheckBox::Ptr checkTileType = theme->load("CheckBox");
			checkTileType->setSize(25, 25);
			checkTileType->setPosition(0, settingsVertOffset); settingsVertOffset += 35.0f;
			checkTileType->setText("Show Tile Types");
			checkTileType->uncheck();
			checkTileType->connect("checked unchecked", &C6SMapMakerState::TileTypeChecked, this);
			settingsPanel->add(checkTileType, "CheckTileType");

			tgui::EditBox::Ptr editMapPath = theme->load("EditBox");
			editMapPath->setSize(300, 30);
			editMapPath->setPosition(0, settingsVertOffset); settingsVertOffset += 30.0f;
			editMapPath->setText("");
			editMapPath->setTextSize(12);
			settingsPanel->add(editMapPath, "EditMapPath");

			tgui::Button::Ptr btnLoadMap = theme->load("Button");
			btnLoadMap->setSize(settingsPanel->getSize().x/2, 30);
			btnLoadMap->setPosition(0, settingsVertOffset);
			btnLoadMap->setText("Load Map");
			btnLoadMap->connect("Pressed", &C6SMapMakerState::LoadMap, this, pGame);
			settingsPanel->add(btnLoadMap);

			tgui::Button::Ptr btnSaveMap = theme->load("Button");
			btnSaveMap->setSize(settingsPanel->getSize().x / 2, 30);
			btnSaveMap->setPosition(settingsPanel->getSize().x / 2, settingsVertOffset); settingsVertOffset += 30.0f;
			btnSaveMap->setText("Export Map");
			btnSaveMap->connect("Pressed", &C6SMapMakerState::ExportMap, this, pGame);
			settingsPanel->add(btnSaveMap);

			tgui::Button::Ptr btnExitToMenu = theme->load("Button");
			btnExitToMenu->setSize(settingsPanel->getSize().x / 2, 30);
			btnExitToMenu->setPosition(0, settingsVertOffset); settingsVertOffset += 30.0f;
			btnExitToMenu->setText("Exit To Menu");
			btnExitToMenu->connect("Pressed", &C6SMapMakerState::ExitToMenuPressed, this, pGame);
			settingsPanel->add(btnExitToMenu);


			/* ----		Background Panel Objects	----	*/
			tgui::Layout bgVertOffset(10.0f);
			// STATIC BACKGROUND 
			tgui::Label::Ptr labelStaticBackground = std::make_shared<tgui::Label>();
			labelStaticBackground->setText("Static Background:");
			labelStaticBackground->setPosition(0.0f, bgVertOffset); bgVertOffset += 20.0f;
			labelStaticBackground->setTextColor(sf::Color::White);
			backgroundPanel->add(labelStaticBackground);

			tgui::EditBox::Ptr editStaticBackground = theme->load("EditBox");
			editStaticBackground->setSize(300, 30);
			editStaticBackground->setPosition(0, bgVertOffset);
			editStaticBackground->setText("");
			editStaticBackground->setTextSize(12);
			backgroundPanel->add(editStaticBackground, "EditStaticBackground");

			tgui::Button::Ptr btnLoadStaticBackground = theme->load("Button");
			btnLoadStaticBackground->setSize(backgroundPanel->getSize().x - 300, 30);
			btnLoadStaticBackground->setPosition(250, bgVertOffset); bgVertOffset += 30.0f;
			btnLoadStaticBackground->setText("Load");
			btnLoadStaticBackground->connect("Pressed", &C6SMapMakerState::LoadStaticBackground, this);
			backgroundPanel->add(btnLoadStaticBackground);

			// DYNAMIC BACKGROUND 1
			tgui::Label::Ptr lblDynBackground1 = std::make_shared<tgui::Label>();
			lblDynBackground1->setText("Dynamic Background 1:");
			lblDynBackground1->setPosition(0, bgVertOffset); bgVertOffset += 20.0f;
			lblDynBackground1->setTextColor(sf::Color::White);
			backgroundPanel->add(lblDynBackground1);

			tgui::EditBox::Ptr editDynBackground1 = theme->load("EditBox");
			editDynBackground1->setSize(300, 30);
			editDynBackground1->setPosition(0, bgVertOffset);
			editDynBackground1->setText("");
			editDynBackground1->setTextSize(12);
			backgroundPanel->add(editDynBackground1, "EditDynamicBackground1");

			tgui::Button::Ptr btnLoadDynBackground1 = theme->load("Button");
			btnLoadDynBackground1->setSize(backgroundPanel->getSize().x - 300, 30);
			btnLoadDynBackground1->setPosition(250, bgVertOffset); bgVertOffset += 30.0f;
			btnLoadDynBackground1->setText("Load");
			btnLoadDynBackground1->connect("Pressed", &C6SMapMakerState::LoadDynamicBackground1, this);
			backgroundPanel->add(btnLoadDynBackground1);

			tgui::Label::Ptr lblDynBackground1Scale = std::make_shared<tgui::Label>();
			lblDynBackground1Scale->setText("Dynamic 1 Movescale:");
			lblDynBackground1Scale->setPosition(0, bgVertOffset); bgVertOffset += 20.0f;
			lblDynBackground1Scale->setTextColor(sf::Color::White);
			backgroundPanel->add(lblDynBackground1Scale);

			tgui::Slider::Ptr sliderDynBackground1Scale = theme->load("Slider");
			sliderDynBackground1Scale->setSize(200, 20);
			sliderDynBackground1Scale->setPosition(0, bgVertOffset); bgVertOffset += 30.0f;
			sliderDynBackground1Scale->setMinimum(0);
			sliderDynBackground1Scale->setMaximum(100);
			sliderDynBackground1Scale->setValue(20);
			backgroundPanel->add(sliderDynBackground1Scale, "SliderDynamicBackground1Scale");

		}
		catch (...) {
			pGame->ErrorQuit("Unable to create main menu");
		}
	}

	pGame->m_WindowManager.SetGUI(m_pGUI);
	pGame->m_WindowManager.SetGUIActive(true);

	pGame->m_Time.ResetFrameTime();

	m_bPlacingTile = false;
	m_bPlacingTileType = false;

	/* Movement */
	m_vLookPosition.X = m_vLookPosition.Y = 0;
	m_bMovingLeft = m_bMovingDown = m_bMovingRight = m_bMovingUp = false;
	
	/* Dev Drawing */
	m_bBoundingBox = true;
	m_bGrid = true;
	m_bTileType = false;
}

void C6SMapMakerState::Cleanup(CGame* pGame) {
	// Remove GUI from Window Manager 
	pGame->m_WindowManager.SetGUI(NULL);
	pGame->m_WindowManager.SetGUIActive(false);

	// Free GUI resources 
	delete m_pGUI;

	// Free map resources 
	m_Map.Destroy();
}

void C6SMapMakerState::Draw(CGame* pGame){
	CWindowManager* pWnd = &pGame->m_WindowManager;
	m_Map.DrawBackground(pGame, m_vLookPosition);

	/* Centre Camera on Player */
	pWnd->MoveCamera(m_vLookPosition);

	/* Map & debug draw */
	m_Map.DrawLevel(pGame);
	if (m_bBoundingBox)
		m_Map.RenderBoundingBox(pGame);
	if (m_bGrid)
		m_Map.RenderGrid(pGame);
	if (m_bTileType || m_bPlacingTileType)
		m_Map.RenderTileType(pGame);

	pWnd->RestoreDefaultCamera();

	/* Show helper text if placing tile */
	if (m_bPlacingTileType || m_bPlacingTile) {
		pGame->m_Drawing.DrawTextCentredX(pGame->GetWindowMgr(), "Press <ESC> to end tile placement", pGame->m_WindowManager.GetScreenCentre().X, 50, 24, 255, 255, 255, 255);
	}

	/* GUI */
	pGame->m_Drawing.DrawRectangleCentred(pGame->GetWindowMgr(), Vector2i(25, 10), 170, 30, 0, 0, 0, 255); // Background for menu toggle 
	m_pGUI->draw();
}

void C6SMapMakerState::Update(CGame* pGame){
	float fLastFrameTime = pGame->m_Time.LastFrameTime();

	float fFrameMove = LOOK_SPEED * fLastFrameTime;
	Vector2f vMove(0.0f, 0.0f);

	if (m_bMovingDown)
		vMove.Y += 1.0f;
	if (m_bMovingUp)
		vMove.Y -= 1.0f;
	if (m_bMovingLeft)
		vMove.X -= 1.0f;
	if (m_bMovingRight)
		vMove.X += 1.0f;

	if (vMove.X != 0.0f || vMove.Y != 0.0f) {
		vMove.MakeUnitVector();
		vMove *= fFrameMove;
		m_vLookPosition.X += vMove.X;
		m_vLookPosition.Y += vMove.Y;
	}
}



void C6SMapMakerState::PlaceTileTexture(Vector2i vPosition) {
	// Find texture for selected tile, and place in map 
	for (unsigned int i = 0; i < m_vTileset.size(); i++) {
		if (m_vTileset[i].tileindex == m_iPlacingTileIndex) {
			m_Map.SetTileTexture(vPosition, &m_vTileset[i].texture, m_iPlacingTileIndex);
			break;
		}
	}
}

void C6SMapMakerState::HandleInput(CGame* pGame){
	CInput* pInput = &pGame->m_Input;

	if (m_bGUIEnabled && !m_bPlacingTileType && !m_bPlacingTile) {
		// Disable movement if GUI is enabled, and we aren't making edits
		m_bMovingLeft = m_bMovingRight = m_bMovingUp = m_bMovingDown = false;
	}
	else {
		// Update movement keys 
		m_bMovingLeft = pInput->ActionKeyPressed(EAction::Action_Move_Left);
		m_bMovingRight = pInput->ActionKeyPressed(EAction::Action_Move_Right);
		m_bMovingUp = pInput->ActionKeyPressed(EAction::Action_Jump);
		m_bMovingDown = pInput->ActionKeyPressed(EAction::Action_Duck);
	}

	// Handle menu toggle 
	if (pInput->ActionKeyPressed(EAction::Action_Menu_Toggle)) {
		if (m_bPlacingTileType || m_bPlacingTile) {
			/* Disable tile placement */
			m_bPlacingTileType = m_bPlacingTile = false;
			if (m_bGUIEnabled)
				ShowMenu();

			/* Re-enable GUI */
			pGame->m_WindowManager.SetGUIActive(true);
		}
	}

	Vector2i mousePosition = pInput->GetMousePosition();
	bool bMouseUpdateRequired = mousePosition != m_vLastProcessedMousePosition;
	if (bMouseUpdateRequired) m_vLastProcessedMousePosition = mousePosition;

	/* if placing tile types OR textures, and there is input to be processed */
	if ((m_bPlacingTileType || m_bPlacingTile) && (pInput->HasCursorEvents() || bMouseUpdateRequired)) {

		// Apply camera transform to fix screen to world 
		pGame->m_WindowManager.MoveCamera(m_vLookPosition); 

		// Process cursor events 
		CInput::SCursorEvent curEvent;
		while (pGame->m_Input.GetNextCursorEvent(curEvent)) {

			// Apply first tile / texture placement 
			if (curEvent.m_button == sf::Mouse::Button::Left && !curEvent.m_bRelease) {
				// Perform screen->world transform, save mouse position 
				Vector2i vMouse = pGame->m_WindowManager.ScreenToWorld(curEvent.m_vPosition);
				bMouseUpdateRequired = false;

				// Update map 
				if (m_bPlacingTileType) {
					m_Map.SetTileType(vMouse, m_ePlacingTileType);
				} else {
					PlaceTileTexture(vMouse);
				}
			}

			// Apply first tile deletion
			if (curEvent.m_button == sf::Mouse::Button::Right && !curEvent.m_bRelease) {
				Vector2i vMouse = pGame->m_WindowManager.ScreenToWorld(curEvent.m_vPosition);
				bMouseUpdateRequired = false;

				// Update tiles 
				if (m_bPlacingTileType) {
					m_Map.SetTileType(vMouse, C6SMap::SMapTile::TYPE_AIR);
				} else { m_Map.DeleteTileTexture(vMouse); }
			}
		}

		// If mouse has moved, and left mouse is pressed, update tiles 
		if (bMouseUpdateRequired && pInput->LeftMousePressed()) {
			Vector2i vMouse = pGame->m_WindowManager.ScreenToWorld(mousePosition);
			bMouseUpdateRequired = false;

			// Update tiles 
			if (m_bPlacingTileType) {
				m_Map.SetTileType(vMouse, m_ePlacingTileType);
			} else {
				PlaceTileTexture(vMouse);
			}
		}

		// If mouse has moved, and right mouse is pressed, delete tiles 
		if (bMouseUpdateRequired && pInput->RightMousePressed()) {
			Vector2i vMouse = pGame->m_WindowManager.ScreenToWorld(mousePosition);

			// Update tiles 
			if (m_bPlacingTileType) {
				m_Map.SetTileType(vMouse, C6SMap::SMapTile::TYPE_AIR);
			}
			else {
				m_Map.DeleteTileTexture(vMouse);
			}
		}

		// Restore original camera transform 
		pGame->m_WindowManager.RestoreDefaultCamera();
	}

}

void C6SMapMakerState::PauseState() {

}
void C6SMapMakerState::ResumeState() {

}