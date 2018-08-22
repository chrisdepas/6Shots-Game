#include "stdafx.h"
#include "C6SMapMakerState.h"
#include "C6SMenuState.h"
#include "CGame.h"
#include "ASSETS.h"
#include "6SWeapons.h"
#include "CBaseNPC.h"
#include "GUIFactory.h"

#define FONT_SIZE 24
#define LOOK_SPEED 300.0f

#define TILESET_LINEBUFFER_SIZE 1024


void C6SMapMakerState::PublishMap(CGame* pGame) {
	tgui::Panel::Ptr panelSettings = (tgui::Panel::Ptr)m_pGUI->get<tgui::Panel>("SettingsPanel");
	if (panelSettings == NULL)
		return;
	tgui::EditBox::Ptr editMapPath = (tgui::EditBox::Ptr)panelSettings->get<tgui::EditBox>(sf::String("EditMapPath"));
	if (editMapPath == NULL)
		return;

	sf::String sfText = editMapPath->getText().toAnsiString();
	if (!sfText.isEmpty()) {
		C6SDevMapPublish* pMap = (C6SDevMapPublish*)&m_Map;
		pMap->Publish(sfText);
	}
}
void C6SMapMakerState::ExportMap(CGame* pGame) {
	tgui::Panel::Ptr panelSettings = (tgui::Panel::Ptr)m_pGUI->get<tgui::Panel>("SettingsPanel");
	if (panelSettings == NULL)
		return;
	tgui::EditBox::Ptr editMapPath = (tgui::EditBox::Ptr)panelSettings->get<tgui::EditBox>(sf::String("EditMapPath"));
	if (editMapPath == NULL)
		return;

	sf::String sfText = editMapPath->getText().toAnsiString();
	m_Map.SaveToFile(pGame, sfText);
}
void C6SMapMakerState::LoadMap(CGame* pGame) {
	tgui::Panel::Ptr panelSettings = (tgui::Panel::Ptr)m_pGUI->get<tgui::Panel>("SettingsPanel");
	if (panelSettings == NULL)
		return;
	tgui::EditBox::Ptr editMapPath = (tgui::EditBox::Ptr)panelSettings->get<tgui::EditBox>(sf::String("EditMapPath"));
	if (editMapPath == NULL)
		return;
	sf::String mapPath = editMapPath->getText().toAnsiString();

	try {
		m_Map.LoadFromFile(pGame, mapPath);
	} catch(...) {
		printf("Failed to initialise map from file!");
	}
}
void C6SMapMakerState::TilePressed(int index, CGame* pGame) {
	if (index < (int)m_Map.GetTileTextureCount()) {
		m_bPlacingTile = true;
		m_iPlacingTileIndex = index;
		HideMenu();

		/* Disable GUI to allow us to receive mouse data */
		pGame->m_WindowManager.SetGUIActive(false);
	}
}
void C6SMapMakerState::LoadTileset(char* szTilset, CGame* pGame) {
	std::ifstream file(szTilset);
	if (!file.good()) {
		printf("Unable to read from '%s' - bad file supplied\n", szTilset);
		return;
	}

	char lineBuf[TILESET_LINEBUFFER_SIZE];
	while (file.getline(lineBuf, TILESET_LINEBUFFER_SIZE)) {
		m_Map.InsertTileTexture(lineBuf);
	}
	file.close();

	auto textures = m_Map.GetTileTextures();
	tgui::Panel::Ptr tilesPanel = (tgui::Panel::Ptr)m_pGUI->get<tgui::Panel>("TilesPanel");
	int xOffset = 0;
	int yOffset = m_iTileTabOffset;
	for (unsigned int i = 0; i < textures.size(); i++) {
		auto tile = GUIFactory::Picture(textures[ i ], sf::Vector2i(32, 32));
		tile->setPosition(tgui::Layout((float)xOffset), tgui::Layout((float)yOffset));
		tile->connect("Clicked", &C6SMapMakerState::TilePressed, this, i, pGame);
		tilesPanel->add(tile);

		xOffset += 34;
		if (xOffset + 32 > tilesPanel->getSize().x) {
			xOffset = 0;
			yOffset += 34;
		}
	}
	
	printf("%i tiles loaded from tileset '%s'\n", textures.size(), szTilset);
}
void C6SMapMakerState::GridsizeChanged() {
	auto panel = m_pGUI->get<tgui::Panel>("TilesPanel");
	if (!panel)
		return;
	auto slider = panel->get<tgui::Slider>(SLD_GRIDSIZE_ID);
	auto label = panel->get<tgui::Label>(SLD_GRIDSIZE_LABEL_ID);
	if (!slider || !label)
		return;
	m_Map.SetGridSize((int)slider->getValue());
	label->setText(std::to_string(slider->getValue()));
}
void C6SMapMakerState::ResizePressed() {
	tgui::Panel::Ptr panelBackround = m_pGUI->get<tgui::Panel>("TilesPanel");
	if (panelBackround == NULL)
		return;
	tgui::EditBox::Ptr editY = (tgui::EditBox::Ptr)panelBackround->get<tgui::EditBox>(sf::String("EditMapY"));
	if (editY == NULL)
		return;
	tgui::EditBox::Ptr editX = (tgui::EditBox::Ptr)panelBackround->get<tgui::EditBox>(sf::String("EditMapX"));
	if (editX == NULL)
		return;

	sf::String sfTextX = editX->getText().toAnsiString();
	sf::String sfTextY = editY->getText().toAnsiString();

	sf::Vector2i vNewSize;
	if (!util::StrInt(sfTextX, vNewSize.x) || !util::StrInt(sfTextY, vNewSize.y)) {
		CDebugLogger::Error("Map Rezise - Invalid size " + std::to_string(vNewSize));
		return;
	}
	try {
		CDebugLogger::Msg("MapMaker::ResizePressed - Resizing to " + std::to_string(vNewSize));
		m_Map.Resize(vNewSize);
	}
	catch (const std::exception& e) {
		CDebugLogger::Error(std::string("MapMaker::Resize failed - ") + e.what());
	}
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
	m_ePlacingTileType = SMapTile::EMapTileType(eTileType);
	HideMenu();
	m_Map.EnableTileTypeRender(true);
	/* Disable GUI to allow us to receive mouse data */
	pGame->m_WindowManager.SetGUIActive(false);
}

void C6SMapMakerState::EndTileTypePlacement(CGame* pGame) {

	tgui::CheckBox::Ptr check = (tgui::CheckBox::Ptr)m_pGUI->get<tgui::CheckBox>(sf::String("CheckGUI"));
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
	m_Map.EnableBoundingBox(check->isChecked());
}

void C6SMapMakerState::GridChecked() {
	tgui::Panel::Ptr panelSettings = (tgui::Panel::Ptr)m_pGUI->get<tgui::Panel>("SettingsPanel");
	if (panelSettings == NULL)
		return;
	tgui::CheckBox::Ptr check = (tgui::CheckBox::Ptr)panelSettings->get<tgui::CheckBox>(sf::String("CheckGrid"));
	if (check == NULL)
		return; 
	m_Map.EnableGrid(check->isChecked());
}

void C6SMapMakerState::TileTypeChecked() {
	tgui::Panel::Ptr panelSettings = (tgui::Panel::Ptr)m_pGUI->get<tgui::Panel>("SettingsPanel");
	if (panelSettings == NULL)
		return;
	tgui::CheckBox::Ptr check = (tgui::CheckBox::Ptr)panelSettings->get<tgui::CheckBox>(sf::String("CheckTileType"));
	if (check == NULL)
		return;
	m_Map.EnableTileTypeRender(check->isChecked());
}

void C6SMapMakerState::LoadStaticBackground() {
	tgui::Panel::Ptr panelBackround = (tgui::Panel::Ptr)m_pGUI->get<tgui::Panel>("BackgroundPanel");
	if (panelBackround == NULL)
		return;
	tgui::EditBox::Ptr editStatic = (tgui::EditBox::Ptr)panelBackround->get<tgui::EditBox>(sf::String("EditStaticBackground"));
	if (editStatic == NULL)
		return;

	sf::String sBackground = editStatic->getText();
	m_Map.LoadStaticBackground(sBackground);
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
	
	sf::String sBackgroundFile = editDynamic->getText();
	try {
		CDebugLogger::Msg("Loading dynamic background " + sBackgroundFile + ", movescale " + to_string(moveScale));
		m_Map.LoadDynamicBackground(sBackgroundFile, sf::Vector2f(moveScale, moveScale));
	} catch (std::exception e) {
		CDebugLogger::Error("Failed to load dynamic background " + sBackgroundFile);
	}
}

void C6SMapMakerState::ExitToMenuPressed(CGame* pGame) {
	// Disable GUI handling
	pGame->m_WindowManager.SetGUI(NULL);
	pGame->m_WindowManager.SetGUIActive(false);

	/* BACK TO BLACK */
	pGame->m_StateHandler.ChangeState(C6SMenuState::Instance(), pGame);
}

void TabSelected(tgui::Gui* pGUI) {
	tgui::Tabs::Ptr tab = pGUI->get<tgui::Tabs>("TabsMenu");
	int selectedTab = tab->getSelectedIndex();

	switch (selectedTab) {
	case 0:
		pGUI->get("BackgroundPanel")->setVisible(true);
		pGUI->get("TilesPanel")->setVisible(false);
		pGUI->get("SettingsPanel")->setVisible(false);
		break;
	case 1:
		pGUI->get("BackgroundPanel")->setVisible(false);
		pGUI->get("TilesPanel")->setVisible(true);
		pGUI->get("SettingsPanel")->setVisible(false);
		break;
	case 2:
		pGUI->get("BackgroundPanel")->setVisible(false);
		pGUI->get("TilesPanel")->setVisible(false);
		pGUI->get("SettingsPanel")->setVisible(true);
		break;
	default:
		break;
	}
}

void C6SMapMakerState::HideMenu() {
	tgui::Tabs::Ptr tab = m_pGUI->get<tgui::Tabs>("TabsMenu");
	tab->setVisible(false);
	m_pGUI->get("BackgroundPanel")->setVisible(false);
	m_pGUI->get("TilesPanel")->setVisible(false);
	m_pGUI->get("SettingsPanel")->setVisible(false);
}

void C6SMapMakerState::ShowMenu() {
	tgui::Tabs::Ptr tab = m_pGUI->get<tgui::Tabs>("TabsMenu");
	tab->setVisible(true);

	int selectedTab = tab->getSelectedIndex();
	switch (selectedTab) {
	case 0:
		m_pGUI->get("BackgroundPanel")->setVisible(true);
		break;
	case 1:
		m_pGUI->get("TilesPanel")->setVisible(true);
		break;
	case 2:
		m_pGUI->get("SettingsPanel")->setVisible(true);
		break;
	default:
		break;
	}
}

void C6SMapMakerState::Init(CGame* pGame) {
	if (IsAlreadyInitialised()) {
		// Set view to correct size and position
		// Fixes case where screen has been resized after GUI is initialized
		m_pGUI->setView(pGame->m_WindowManager.GetGUIView());

	} else {
		// Create map
		try {
			m_Map.CreateBlank(ASSET_DEV_BACKGROUND);
		} catch (std::exception e) {
			CDebugLogger::Error(e.what());
			try {
				// Try again with no background
				m_Map.CreateBlank();
			} catch (std::exception e2) {
				CDebugLogger::Fatal(std::string("DevMap creation failed twice: ") + e2.what());
				exit(-1);
			}
		}

		/* Create GUI */
		try {
			m_pGUI = new tgui::Gui(*pGame->m_WindowManager.GetWindow());
			m_bGUIEnabled = true;

			// theme 
			//tgui::Theme::Ptr theme = std::make_shared<tgui::Theme>("widgets/Chris.txt");

			// Enable/disable GUI
			GUILayout::Checkbox(m_pGUI, tgui::Layout2d(0.0f, 0.0f), std::string("CheckGUI"), std::string("Menu"), true, &C6SMapMakerState::GUIChecked, this);
			//GUILayout::Checkbox(m_pGUI, tgui::Layout2d(0.0f, 0.0f), std::string("CheckGUI"), std::string("Menu"), true, &C6SMapMakerState::GUIChecked, this);
		
			// Tabbed menu
			auto tabs = GUIFactory::Tabs("Background", "Tiles", "Settings");
			tabs->setPosition(150, 0);
			tabs->connect("TabSelected", TabSelected, m_pGUI);
			m_pGUI->add(tabs, "TabsMenu");

			// Background panel
			auto backgroundPanel = GUIFactory::PanelSize(MENU_SIZE);
			backgroundPanel->setPosition(tabs->getPosition().x, tabs->getPosition().y + tabs->getSize().y);
			m_pGUI->add(backgroundPanel, "BackgroundPanel");

			// Tiles Panel
			tgui::Panel::Ptr tilesPanel = tgui::Panel::copy(backgroundPanel);
			m_pGUI->add(tilesPanel, "TilesPanel");

			// Settings Panel
			tgui::Panel::Ptr settingsPanel = tgui::Panel::copy(backgroundPanel);
			m_pGUI->add(settingsPanel, "SettingsPanel");


			/* ----		Tiles Panel Objects		----	*/
			tgui::Layout2d tilesVertOffset(0.0f, 10.0f);

			GUILayout::LabelContentVertical(tilesPanel, tilesVertOffset, "Set Tile Types:");
			GUILayout::MiniButtonHorizontal(tilesPanel, tilesVertOffset, "Air", &C6SMapMakerState::BeginTileTypePlacement, this, pGame, SMapTile::TYPE_AIR);
			GUILayout::MiniButtonHorizontal(tilesPanel, tilesVertOffset, "Solid", &C6SMapMakerState::BeginTileTypePlacement, this, pGame, SMapTile::TYPE_SOLID);
			GUILayout::MiniButtonHorizontal(tilesPanel, tilesVertOffset, "Ladder", &C6SMapMakerState::BeginTileTypePlacement, this, pGame, SMapTile::TYPE_LADDER);
			GUILayout::MiniButtonHorizontal(tilesPanel, tilesVertOffset, "Boundary", &C6SMapMakerState::BeginTileTypePlacement, this, pGame, SMapTile::TYPE_BOUNDARY);
			GUILayout::MiniButtonHorizontal(tilesPanel, tilesVertOffset, "Partial", &C6SMapMakerState::BeginTileTypePlacement, this, pGame, SMapTile::TYPE_PARTIALSOLID);
			GUILayout::MiniButtonHorizontal(tilesPanel, tilesVertOffset, "Water", &C6SMapMakerState::BeginTileTypePlacement, this, pGame, SMapTile::TYPE_WATER);
			GUILayout::MiniButtonHorizontal(tilesPanel, tilesVertOffset, "Death", &C6SMapMakerState::BeginTileTypePlacement, this, pGame, SMapTile::TYPE_INSTADEATH);
			GUILayout::MiniButtonHorizontal(tilesPanel, tilesVertOffset, "Hurt", &C6SMapMakerState::BeginTileTypePlacement, this, pGame, SMapTile::TYPE_SLOWDAMAGE);
			GUILayout::EndMiniButtonsHorizontal(tilesPanel, tilesVertOffset);

			/*InsertHorizontalButton(theme, tilesPanel, tilesVertOffset, TILETYPES_BTN_SET_SCALE, "Air", &C6SMapMakerState::BeginTileTypePlacement, this, pGame, SMapTile::TYPE_AIR);
			InsertHorizontalButton(theme, tilesPanel, tilesVertOffset, TILETYPES_BTN_SET_SCALE, "Solid", &C6SMapMakerState::BeginTileTypePlacement, this, pGame, SMapTile::TYPE_SOLID);
			InsertHorizontalButton(theme, tilesPanel, tilesVertOffset, TILETYPES_BTN_SET_SCALE, "Ladder", &C6SMapMakerState::BeginTileTypePlacement, this, pGame, SMapTile::TYPE_LADDER);
			InsertHorizontalButton(theme, tilesPanel, tilesVertOffset, TILETYPES_BTN_SET_SCALE, "Boundary", &C6SMapMakerState::BeginTileTypePlacement, this, pGame, SMapTile::TYPE_BOUNDARY);
			InsertHorizontalButton(theme, tilesPanel, tilesVertOffset, TILETYPES_BTN_SET_SCALE, "Partial", &C6SMapMakerState::BeginTileTypePlacement, this, pGame, SMapTile::TYPE_PARTIALSOLID);
			InsertHorizontalButton(theme, tilesPanel, tilesVertOffset, TILETYPES_BTN_SET_SCALE, "Water", &C6SMapMakerState::BeginTileTypePlacement, this, pGame, SMapTile::TYPE_WATER);
			InsertHorizontalButton(theme, tilesPanel, tilesVertOffset, TILETYPES_BTN_SET_SCALE, "Death", &C6SMapMakerState::BeginTileTypePlacement, this, pGame, SMapTile::TYPE_INSTADEATH);
			InsertHorizontalButton(theme, tilesPanel, tilesVertOffset, TILETYPES_BTN_SET_SCALE, "Hurt", &C6SMapMakerState::BeginTileTypePlacement, this, pGame, SMapTile::TYPE_SLOWDAMAGE);
			EndHorizontalButtons(theme, tilesPanel, tilesVertOffset, TILETYPES_BTN_SET_SCALE);
			*/
			GUILayout::VerticalSpacer(tilesVertOffset);

			GUILayout::LabelContentVertical(tilesPanel, tilesVertOffset, "Load Tiles:");

			auto editLoadTileset = GUIFactory::EditboxMedium();
			editLoadTileset->setPosition(tgui::Layout2d(0.0f, tilesVertOffset.y)); 
			tilesPanel->add(editLoadTileset, "EditLoadTileset");

			tgui::Button::Ptr btnLoadTileset = GUIFactory::ButtonMedium("Load");
			btnLoadTileset->setPosition(tgui::Layout2d(250.0f, tilesVertOffset.y));
			btnLoadTileset->connect("Pressed", &C6SMapMakerState::LoadTilesetPressed, this, pGame);
			tilesVertOffset.y = tilesVertOffset.y + GUIUtil::MaxHeight(btnLoadTileset, editLoadTileset) + VERTICAL_PADDING;
			tilesPanel->add(btnLoadTileset);

			GUILayout::VerticalSpacer(tilesVertOffset);

			GUILayout::LabelContentVertical(tilesPanel, tilesVertOffset, "Placement Mode:");

			/*
			tgui::RadioButton::Ptr rdioStickerMode = theme->load("RadioButton");
			rdioStickerMode->setSize(30, 30);
			rdioStickerMode->setPosition(tgui::Layout2d(10.0f, tilesVertOffset.y)); 
			rdioStickerMode->setText("Sticker");
//rdioStickerMode->connect("Checked Unchecked", &C6SMapMakerState::LoadTilesetPressed, this, pGame);
			tilesPanel->add(rdioStickerMode);

			tgui::RadioButton::Ptr rdioTilesMode = theme->load("RadioButton");
			rdioTilesMode->setSize(30, 30);
			rdioTilesMode->setPosition(tgui::Layout2d(150.0f, tilesVertOffset.y));
			rdioTilesMode->setText("Tiles");
		//	rdioTilesMode->connect("Checked Unchecked", &C6SMapMakerState::LoadTilesetPressed, this, pGame);
			tilesVertOffset.y += tgui::bindHeight(btnLoadTileset) + VERTICAL_PADDING;
			tilesPanel->add(rdioTilesMode);

			GUILayout::VerticalSpacer(tilesVertOffset);*/

			/* Map resize X*/
			tgui::EditBox::Ptr editMapX = GUIFactory::EditboxMedium();
			editMapX->setSize(150, 30);
			editMapX->setPosition(tilesVertOffset);
			editMapX->setText("32");
			editMapX->setTextSize(12);
			tilesPanel->add(editMapX, "EditMapX");

			/* Map resize Y*/
			tgui::EditBox::Ptr editMapY = GUIFactory::EditboxMedium();
			editMapY->setSize(150, 30);
			editMapY->setPosition(tgui::Layout2d(150.0f, tilesVertOffset.y));
			editMapY->setText("32");
			editMapY->setTextSize(12);
			tilesVertOffset.y = tilesVertOffset.y + GUIUtil::MaxHeight(editMapX, editMapY) + VERTICAL_PADDING;
			tilesPanel->add(editMapY, "EditMapY");

			/* Map resize btn */
			GUILayout::Button(tilesPanel, tilesVertOffset, "", "Resize Map", &C6SMapMakerState::ResizePressed, this);

			GUILayout::VerticalSpacer(tilesVertOffset);

			GUILayout::SliderInt(tilesPanel, tilesVertOffset, SLD_GRIDSIZE_ID, SLD_GRIDSIZE_LABEL_ID,
				"Tile Size", m_Map.GetGridSize(), m_Map.GetGridSizeLimits(), &C6SMapMakerState::GridsizeChanged, this);

			// Save offset so tiles can be added later 
			m_iTileTabOffset = (int)tilesVertOffset.y.getValue();


			/* ----		Settings Panel Objects	----	*/
			tgui::Layout2d settingsVertOffset(0.0f, 10.0f);

			m_Map.EnableBoundingBox(true);
			m_Map.EnableGrid(true);
			m_Map.EnableTileTypeRender(false);

			GUILayout::LabelContentVertical(settingsPanel, settingsVertOffset, "Debug Visualisations:");
			GUILayout::Checkbox(settingsPanel, settingsVertOffset, CHK_BOUNDINGBOX_ID, "Show Bounding Box", true, &C6SMapMakerState::BoundingBoxChecked, this);
			GUILayout::Checkbox(settingsPanel, settingsVertOffset, CHK_GRID_ID, "Show Grid", true, &C6SMapMakerState::GridChecked, this);
			GUILayout::Checkbox(settingsPanel, settingsVertOffset, CHK_TILE_TYPE_ID, "Show Tile Types", false, &C6SMapMakerState::TileTypeChecked, this);

			GUILayout::VerticalSpacer(tilesVertOffset);

			auto editMapPath = GUIFactory::EditboxMedium();
			editMapPath->setSize(300, 30);
			editMapPath->setPosition(0, settingsVertOffset.y); settingsVertOffset.y = settingsVertOffset.y + tgui::bindHeight(editMapPath) + VERTICAL_PADDING;
			editMapPath->setText("");
			editMapPath->setTextSize(12);
			settingsPanel->add(editMapPath, "EditMapPath");

			/*
			tgui::Button::Ptr btnLoadMap = theme->load("Button");
			btnLoadMap->setSize(settingsPanel->getSize().x/2, 30);
			btnLoadMap->setPosition(0, settingsVertOffset);
			btnLoadMap->setText("Load Map");
			btnLoadMap->connect("Pressed", &C6SMapMakerState::LoadMap, this, pGame);
			settingsPanel->add(btnLoadMap);
*/
			
			GUILayout::MiniButtonHorizontal(settingsPanel, settingsVertOffset, "Load Map", &C6SMapMakerState::LoadMap, this, pGame);
			GUILayout::MiniButtonHorizontal(settingsPanel, settingsVertOffset, "Save Map", &C6SMapMakerState::ExportMap, this, pGame);
			GUILayout::MiniButtonHorizontal(settingsPanel, settingsVertOffset, "Publish Map", &C6SMapMakerState::PublishMap, this, pGame);
			GUILayout::EndMiniButtonsHorizontal(settingsPanel, settingsVertOffset);

			GUILayout::VerticalSpacer(settingsVertOffset);
			settingsVertOffset.x = 0.0f;
			/*
			InsertButton(theme, settingsPanel, settingsVertOffset,  "Load Map", &C6SMapMakerState::LoadMap, this, pGame);
			InsertButton(theme, settingsPanel, settingsVertOffset, CHK_BTN_SAVEMAP_ID, "Export Map", &C6SMapMakerState::ExportMap, this, pGame);*/

			GUILayout::Button(settingsPanel, settingsVertOffset, CHK_BTN_EXIT_ID, "Exit to Menu", &C6SMapMakerState::ExitToMenuPressed, this, pGame);


			/* ----		Background Panel Objects	----	*/
			tgui::Layout2d bgVertOffset(0.0f, 10.0f);

			// STATIC BACKGROUND 
			GUILayout::LabelContentVertical(backgroundPanel, bgVertOffset, "Static Background:");
			GUILayout::EditBoxButton(backgroundPanel, bgVertOffset, "EditStaticBackground", "Load", "", &C6SMapMakerState::LoadStaticBackground, this);
			/*tgui::EditBox::Ptr editStaticBackground = theme->load("EditBox");
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
			backgroundPanel->add(btnLoadStaticBackground);*/

			GUILayout::VerticalSpacer(bgVertOffset);

			// DYNAMIC BACKGROUND 1
			GUILayout::LabelContentVertical(backgroundPanel, bgVertOffset, "Dynamic Background 1:");
			GUILayout::EditBoxButton(backgroundPanel, bgVertOffset, "EditDynamicBackground1", "Load", "", &C6SMapMakerState::LoadDynamicBackground1, this);

			/*tgui::EditBox::Ptr editDynBackground1 = theme->load("EditBox");
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
			backgroundPanel->add(btnLoadDynBackground1);*/

			GUILayout::VerticalSpacer(bgVertOffset);
			/*
			GUILayout::LabelContentVertical(backgroundPanel, bgVertOffset, "Dynamic 1 Movescale:");

			tgui::Slider::Ptr sliderDynBackground1Scale = theme->load("Slider");
			sliderDynBackground1Scale->setSize(200, 20);
			sliderDynBackground1Scale->setPosition(0, bgVertOffset); bgVertOffset += 30.0f;
			sliderDynBackground1Scale->setMinimum(0);
			sliderDynBackground1Scale->setMaximum(100);
			sliderDynBackground1Scale->setValue(20);
			backgroundPanel->add(sliderDynBackground1Scale, "SliderDynamicBackground1Scale");*/

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
	m_vLookPosition = sf::Vector2f(0.0, 0.0f);
	m_bMovingLeft = m_bMovingDown = m_bMovingRight = m_bMovingUp = false;
}

void C6SMapMakerState::Cleanup(CGame* pGame) {
	// Remove GUI from Window Manager 
	pGame->m_WindowManager.SetGUI(NULL);
	pGame->m_WindowManager.SetGUIActive(false);

	// Free GUI resources 
	delete m_pGUI;
}

void C6SMapMakerState::Draw(CGame* pGame){
	CWindowManager* pWnd = &pGame->m_WindowManager;
	m_Map.DrawBackground(pGame, m_vLookPosition);

	/* Centre Camera on Player */
	pWnd->MoveCamera(m_vLookPosition);

	/* Map & debug draw */
	m_Map.DrawLevel(pGame);

	pWnd->RestoreDefaultCamera();

	/* Show helper text if placing tile */
	if (m_bPlacingTileType || m_bPlacingTile) {
		pGame->m_Drawing.DrawTextCentredX(pGame->GetWindowMgr(), "Press <ESC> to end tile placement", pGame->m_WindowManager.GetScreenCentre().x, 50, 24, 255, 255, 255, 255);
	}

	/* GUI */
	pGame->m_Drawing.DrawRectangleCentred(pGame->GetWindowMgr(), sf::Vector2i(25, 10), 170, 30, 0, 0, 0, 255); // Background for menu toggle 
	m_pGUI->draw();
}

void C6SMapMakerState::Update(CGame* pGame, float fFrameTime){
	float fFrameMove = LOOK_SPEED * fFrameTime;
	sf::Vector2f vMove(0.0f, 0.0f);

	if (m_bMovingDown)
		vMove.y += 1.0f;
	if (m_bMovingUp)
		vMove.y -= 1.0f;
	if (m_bMovingLeft)
		vMove.x -= 1.0f;
	if (m_bMovingRight)
		vMove.x += 1.0f;

	if (vMove.x != 0.0f || vMove.y != 0.0f) {
		vMove = vec::UnitVector(vMove) * fFrameMove;
		m_vLookPosition += vMove;
	}
}

void C6SMapMakerState::PlaceTileTexture(sf::Vector2i vPosition) {
	// Find texture for selected tile, and place in map 
	m_Map.SetTileTexture(vPosition, m_iPlacingTileIndex);
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

	sf::Vector2i mousePosition = pInput->GetMousePosition();
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
			if (curEvent.button == sf::Mouse::Button::Left && !curEvent.m_bRelease) {
				// Perform screen->world transform, save mouse position 
				sf::Vector2i vMouse = pGame->m_WindowManager.ScreenToWorld(curEvent.Position2i());
				bMouseUpdateRequired = false;

				// Update map 
				if (m_bPlacingTileType) {
					m_Map.SetTileType(vMouse, m_ePlacingTileType);
				} else {
					PlaceTileTexture(vMouse);
				}
			}

			// Apply first tile deletion
			if (curEvent.button == sf::Mouse::Button::Right && !curEvent.m_bRelease) {
				sf::Vector2i vMouse = pGame->m_WindowManager.ScreenToWorld(curEvent.Position2i());
				bMouseUpdateRequired = false;

				// Update tiles 
				if (m_bPlacingTileType) {
					m_Map.DeleteTileType(vMouse);
				}
				else { m_Map.DeleteTileTexture(vMouse); }
			}
		}

		// If mouse has moved, and left mouse is pressed, update tiles 
		if (bMouseUpdateRequired && pInput->LeftMousePressed()) {
			sf::Vector2i vMouse = pGame->m_WindowManager.ScreenToWorld(mousePosition);
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
			sf::Vector2i vMouse = pGame->m_WindowManager.ScreenToWorld(mousePosition);

			// Update tiles 
			if (m_bPlacingTileType) {
				m_Map.DeleteTileType(vMouse);
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