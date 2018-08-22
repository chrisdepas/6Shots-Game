#ifndef __C6SMAPMAKERSTATE_H__
#define __C6SMAPMAKERSTATE_H__

/*
Christopher De Pasquale
Sometime in 2016

*/
#include "CGameState.h"
#include "C6SMap.h"
#include "C6SPlayer.h"
#include "CGlobal.h"
#include "CProjectileManager.h"
#include "CEntityManager.h"

class C6SMapMakerState : public CGameState {
	struct STilesetTile {
		int tileindex;
		sf::Texture texture;
	};

	C6SDevMap m_Map;

	/* Input state */
	sf::Vector2f m_vLookPosition;
	bool m_bMovingLeft;
	bool m_bMovingRight;
	bool m_bMovingUp;
	bool m_bMovingDown;
	sf::Vector2i m_vLastProcessedMousePosition = sf::Vector2i(-1, -1);

	/* GUI data */
	bool m_bGUIEnabled;
	tgui::Gui* m_pGUI;

	/* Tileset */
	int m_iTileTabOffset = 0;

	/* Loaded background data */
	std::string m_sStaticBackground = "";
	std::string m_sDynamicBackground1 = "";
	float m_fDynamicBackground1MoveScale = 0.0f;

	/* If a tile type is being placed */
	bool m_bPlacingTileType;
	SMapTile::EMapTileType m_ePlacingTileType;

	/* If a tile is being placed */
	bool m_bPlacingTile;
	int m_iPlacingTileIndex;

	/* Placing as 'sticker' (Any size, free placement) */
	bool m_bStickerPlacingMode;
	sf::Vector2i m_vStickerPlacingSize;
	bool m_bStickerOnTop; 

	const sf::Vector2i MENU_SIZE = sf::Vector2i(400, 500);
	const tgui::Layout2d CHECKBOX_SIZE = tgui::Layout2d(20.0f, 20.0f);
	const int TEXT_SIZE = 24;
	const tgui::Layout VERTICAL_PADDING = tgui::Layout(2.0f);
	const sf::Color SLIDER_TEXT_COLOR = sf::Color::White;
	const sf::Color LABEL_COLOR = sf::Color::White;
	const tgui::Layout2d SLIDER_SIZE = tgui::Layout2d(150.0f, (const float)TEXT_SIZE);
	const tgui::Layout2d BUTTON_SIZE = tgui::Layout2d(150.0f, 26.0f);
	const tgui::Layout2d SEPERATOR_SPACING = tgui::Layout2d(0.0f, 20.0f);
	const sf::Vector2f TILETYPES_BTN_SET_SCALE = sf::Vector2f(0.25f, 0.9f);
	const sf::Vector2f LOADSIZE_MAP_BTN_SET_SCALE = sf::Vector2f(0.33f, 0.95f);

	const std::string CHK_BOUNDINGBOX_ID = "CheckBoundingBox";
	const std::string CHK_GRID_ID = "CheckGrid";
	const std::string CHK_TILE_TYPE_ID = "CheckTileType";
	const std::string CHK_TILES_ID = "CheckTiles";
	const std::string CHK_BTN_EXIT_ID = "BtnExitMenu";
	const std::string SLD_GRIDSIZE_ID = "SldGridsize";
	const std::string SLD_GRIDSIZE_LABEL_ID = "SldLblGridsize";

	/* Load a tileset file (.6ts), should be a list of paths to tiles, 1 per line */
	void LoadTileset(char* szTilset, CGame* pGame);

	/* Helper - Place current tileset texture at position */
	void PlaceTileTexture(sf::Vector2i vPosition);
		
	/* Helper - Get checkbox value */
	bool GetCheckboxValue(const char* szID);


	/* GUI Callback functions */
	void TilePressed(int index, CGame* pGame);
	void GUIChecked();
	void BoundingBoxChecked();
	void GridChecked();
	void TileTypeChecked();
	void LoadStaticBackground();
	void LoadDynamicBackground1();
	void GridsizeChanged();
	void ResizePressed();
	void LoadTilesetPressed(CGame* pGame);
	void ExportMap(CGame* pGame);
	void PublishMap(CGame* pGame);
	void LoadMap(CGame* pGame);
	void HideMenu();
	void ShowMenu();
	void BeginTileTypePlacement(CGame* pGame, int eTileType);
	void EndTileTypePlacement(CGame* pGame);
	void ExitToMenuPressed(CGame* pGame);

public:

	void Init(CGame* pGame);
	void Cleanup(CGame* pGame);

	void Draw(CGame* pGame);
	void Update(CGame* pGame, float fFrameTime);
	void HandleInput(CGame* pGame);

	void PauseState();
	void ResumeState();

	static C6SMapMakerState* Instance()
	{
		static C6SMapMakerState makerState;
		return &makerState;
	}
};

#endif

