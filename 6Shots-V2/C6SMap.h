#ifndef __C6SMAP_H__
#define __C6SMAP_H__

/*
		Christopher De Pasquale
Unknown date, notice added 12 December 2016
*/

class CGame;
#include "Vector.h"
#include "SFML\Graphics.hpp"
#include <vector>
#include <rapidxml.hpp>

#define MAP_6S_VERSION 0.6f
#define MAP_PREFIX "6map"

class C6SMap {
public:
	struct SMapTile;

private:
	
	enum EMapType {
		TYPE_UNKNOWN = 0,
		TYPE_DEVELOP,/* Contains tileset data */
		TYPE_PLAY,
		TYPE_COUNT,
	};

	static const char* szMapTypeStrings[];

	struct SDynamicBackground {
		int id;
		sf::Texture texture;
		float moveScale;
	};

	/*************************************************************************/

	/* Map has been loaded */
	bool m_bInit = false;
	
	/* Type - May be a develop map (with tile data), or playable only */
	EMapType m_MapType = EMapType::TYPE_UNKNOWN;

	/* Tile data for map */
	bool m_bUsingMapTiles = false;
	Vector2i m_vTileSize = Vector2i(0, 0);
	SMapTile* m_Tiles = NULL;
	int m_iGridSize = 0; 

	/* Map background data */
	std::vector<SDynamicBackground> m_DynamicBackgrounds;
	bool m_bBackgroundLoaded = false;
	sf::Texture m_StaticBackground;

	/* Map size in pixels */
	Vector2i m_vSize;

	/* Map tiles as texture */
	sf::Texture m_MapTexture;
	bool m_bUsingMapTexture = false;

	/* Map file extension */
	static const char* MAP_FILETYPE;

	/* Parse Map XML */
	bool C6SMap::ParseXML(rapidxml::xml_document<>& xml);
	
public:
	struct SMapTile {
		enum  ETileType 
		{
			/* 'standard' tiles */
			TYPE_AIR,			/* Empty tile */
			TYPE_SOLID,			/* Solid game wall inside bounds of map */
			TYPE_LADDER,		/* Climbable ladder tile */
			TYPE_BOUNDARY,		/* Boundary tile, outside bounds of normal play of map */
			TYPE_PARTIALSOLID,	/* Allows bullets to go through, but not players */

			/* Special Tiles */
			TYPE_WATER,			/* Water .. */
			TYPE_INSTADEATH,	/* Instant kill */
			TYPE_SLOWDAMAGE,	/* Damage over time, non-solid */

		};
		sf::Texture* pForeground;
		sf::Texture* pBackground;
		char tileType;
		int iTileIndex;

		/* Debug draw type */
		void RenderType(CGame* pGame, Vector2i pos, int iSize);

		/* Debug draw type */
		bool IsSolid();
	};

	struct SMapShape {
		Vector2i vCentrePos;
		Vector2i vSize;
		SMapTile::ETileType eShapeType;
	};

	C6SMap();

	/* Get Extension */
	static const char* GetMapFileExtension() { return C6SMap::MAP_FILETYPE; }

	/* Draw */
	void DrawBackground(CGame* pGame, Vector2f playerPosition);
	void DrawLevel(CGame* pGame);

	/* Collision testing */
	bool FloorCollision(Vector2i pos);
	bool WallCollisionLeft(Vector2i pos);
	bool WallCollisionRight(Vector2i pos);
	bool Collision(Vector2i pos);
	
	/* Spawns */
	Vector2i GetSpawn(CGame* pGame);
	void ResetSpawns();  

	/* */
	void Reset();

	/* Decompose the map into a list of collision shapes */
	bool ShapeDecompose(std::vector<SMapShape>& shapes);
	/* Decompose the map into a list of better collision shapes */
	bool ShapeDecomposeNew(std::vector<SMapShape>& shapes);

	/* Get map size */
	Vector2i GetSize();

	/* Dev Drawing */
	void RenderBoundingBox(CGame* pGame);
	void RenderGrid(CGame* pGame);
	void RenderTileType(CGame* pGame);

	bool LoadStaticBackground(char* szBackground);
	bool LoadDynamicBackground(char* szBackground, int index, float movescale);
	void SetTileType(Vector2i worldPos, SMapTile::ETileType type);
	void SetTileTexture(Vector2i worldPos, sf::Texture* pTexture, int index);
	void DeleteTileTexture(Vector2i worldPos);

	/* ## Load and Save */
	bool ExportToFile(CGame* pGame, char* szOutput, bool bAsDevMap=false); 	/* ExportToFile: Expects szOutput to be a extension-less path to the map name,
															e.g. '.\Map\MapName', the function will append names */

	bool InitializeFromFile(CGame* pGame, char* szMap); /* Load playable map from file, without tiles */
	void InitializeDev(char* background, int width, int height, int gridsize); /* Load blank map of some size, for dev */

	void Destroy();

	/* Resize */
	void Resize(int width, int height, int gridsize);
};

#endif