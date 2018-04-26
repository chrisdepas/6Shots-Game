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

	/* Arbitrary with and height limits */
	static const int MapMaxWidth = 65536;
	static const int MapMaxHeight = 65536;

	/* Default map grid size */
	static const int MapDefaultGridSize = 32; 
	static const int MapDefaultWidth = 1024; /* Pixels */
	static const int MapDefaultHeight = 1024; /* Pixels */

	C6SMap m_Map;
	
	/* Input state */
	Vector2f m_vLookPosition;
	bool m_bMovingLeft;
	bool m_bMovingRight;
	bool m_bMovingUp;
	bool m_bMovingDown;
	Vector2i m_vLastProcessedMousePosition = Vector2i(-1, -1);

	/* GUI data */
	bool m_bGUIEnabled;
	tgui::Gui* m_pGUI;

	/* dev drawing options */
	bool m_bBoundingBox;
	bool m_bGrid;
	bool m_bTileType;

	/* Tileset */
	int m_iTileTabOffset;
	std::vector<STilesetTile> m_vTileset;

	/* Loaded background data */
	char* m_szStaticBackground;
	char* m_szDynamicBackground1;
	float m_fDynamicBackground1MoveScale;

	/* If a tile type is being placed */
	bool m_bPlacingTileType;
	C6SMap::SMapTile::ETileType m_ePlacingTileType;

	/* If a tile is being placed */
	bool m_bPlacingTile;
	int m_iPlacingTileIndex;

	/* Placing as 'sticker' (Any size, free placement) */
	bool m_bStickerPlacingMode;
	Vector2i m_vStickerPlacingSize;
	bool m_bStickerOnTop; 

	/* Load a tileset file (.6ts), should be a list of paths to tiles, 1 per line */
	void C6SMapMakerState::LoadTileset(char* szTilset, CGame* pGame);

	/* Helper - Place current tileset texture at position */
	void C6SMapMakerState::PlaceTileTexture(Vector2i vPosition);

	/* GUI Callback functions */
	void C6SMapMakerState::TilePressed(int index, CGame* pGame);
	void C6SMapMakerState::GUIChecked();
	void C6SMapMakerState::BoundingBoxChecked();
	void C6SMapMakerState::GridChecked();
	void C6SMapMakerState::TileTypeChecked();
	void C6SMapMakerState::LoadStaticBackground();
	void C6SMapMakerState::LoadDynamicBackground1();
	void C6SMapMakerState::ResizePressed();
	void C6SMapMakerState::LoadTilesetPressed(CGame* pGame);
	void C6SMapMakerState::ExportMap(CGame* pGame);
	void C6SMapMakerState::LoadMap(CGame* pGame);
	void C6SMapMakerState::HideMenu();
	void C6SMapMakerState::ShowMenu();
	void C6SMapMakerState::BeginTileTypePlacement(CGame* pGame, int eTileType);
	void C6SMapMakerState::ExitToMenuPressed(CGame* pGame);

public:

	void C6SMapMakerState::Init(CGame* pGame);
	void C6SMapMakerState::Cleanup(CGame* pGame);

	void C6SMapMakerState::Draw(CGame* pGame);
	void C6SMapMakerState::Update(CGame* pGame);
	void C6SMapMakerState::HandleInput(CGame* pGame);

	void C6SMapMakerState::PauseState();
	void C6SMapMakerState::ResumeState();

	static C6SMapMakerState* Instance()
	{
		static C6SMapMakerState makerState;
		return &makerState;
	}
};

#endif

