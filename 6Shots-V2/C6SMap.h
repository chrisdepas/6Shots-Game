#ifndef __C6SMAP_H__
#define __C6SMAP_H__

/*
		Christopher De Pasquale
Unknown date, notice added 12 December 2016
*/

class CGame;

// Serializing
#include <cereal/cereal.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/common.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/base_class.hpp>
#include "CerealTypes.h"

#include "DebugUtil.h"
#include "CDrawingManager.h"
#define MAP_FILE_RELEASE_VERSION 1
#define MAP_FILE_RELEASE_EXTENSION "6map"
#define MAP_FILE_DEV_VERSION 1
#define MAP_FILE_DEV_EXTENSION "6mapd"

struct SMapTile {
	enum EMapTileType {
		TYPE_AIR,			/* Empty tile */
		TYPE_SOLID,			/* Solid game wall inside bounds of map */
		TYPE_LADDER,		/* Climbable ladder tile */
		TYPE_BOUNDARY,		/* Boundary tile, outside bounds of normal play of map */
		TYPE_PARTIALSOLID,	/* Allows bullets to go through, but not players */

		/* Special Tiles */
		TYPE_WATER,			/* Water .. */
		TYPE_INSTADEATH,	/* Instant kill */
		TYPE_SLOWDAMAGE,	/* Damage over time, non-solid */

		TYPE_MAX,
	};

	SMapTile() = default;
	SMapTile(EMapTileType type) { tileType = type; }
	
	EMapTileType tileType;

	/* Debug draw type */
	void RenderType(CGame* pGame, sf::Vector2i pos, int iSize);

	bool IsPlayerSolid() { return (tileType == TYPE_SOLID || tileType == TYPE_PARTIALSOLID); }
	bool IsBulletSolid() { return (tileType == TYPE_SOLID); }
	bool IsLadder() { return (tileType == TYPE_LADDER); }

	SMapTile& operator = (const SMapTile& tile) {
		tileType = tile.tileType;
	}

	template<class Archive> void serialize(Archive& ar) {
		ar(cereal::make_nvp("tiletype", tileType));
	}
};

struct SDevTile : public SMapTile {
	static const int INVALID_TEXTURE = -1;
	unsigned int iTextureIndex;

	bool HasTexture() { return iTextureIndex != INVALID_TEXTURE; }
	SDevTile() = default;
	SDevTile(EMapTileType type, int textureIndex = INVALID_TEXTURE) { 
		tileType = type; 
		iTextureIndex = textureIndex; 
	}
	SDevTile& operator = (const SDevTile& tile) {
		tileType = tile.tileType;
		iTextureIndex = tile.iTextureIndex;
		return *this;
	}
	EMapTileType GetTileType() { return tileType; }
	int GetTextureIndex() { return iTextureIndex; }

	template<class Archive> void serialize(Archive& ar) {
		ar(
			cereal::make_nvp("tiletype", tileType),
			cereal::make_nvp("textureindex", iTextureIndex)
		);
	}
};

class C6SBaseMap {

protected:
	enum EMapType {
		MAP_TYPE_INVALID = 0,
		MAP_DEVELOP, MAP_RELEASE,
		MAP_TYPE_MAX,
	};
	
	/* Arbitrary width and height limits, in tiles */
	static const sf::Vector2i MAX_SIZE;
	static const sf::Vector2i MIN_SIZE;
	static const sf::Vector2i GRID_SIZE_LIMITS;

	// Default size (in tiles), size per tile
	static const sf::Vector2i DEFAULT_SIZE;
	static const int DEFAULT_GRID_SIZE = 32;

	// Tilemap vars
	sf::Vector2i m_vTileSize = sf::Vector2i(0, 0); // Size in tiles
	sf::Vector2i m_vSize; // Size in pixels
	int m_iGridSize = 0; // Size of each tile, in pixels

	// Background image
	sf::Texture m_StaticBG;

	// Backgrounds which move based on player position
	std::vector<std::pair<sf::Texture, sf::Vector2f>> m_ParallaxBGs;

	EMapType m_MapType = EMapType::MAP_TYPE_INVALID;

	C6SBaseMap() {}
public:
	
	struct SMapShape {
		sf::Vector2i vCentrePos, vSize;
		SMapTile::EMapTileType eShapeType;

		SMapShape() = default;
		SMapShape(SMapTile::EMapTileType type, sf::Vector2i centre, sf::Vector2i size) {
			vCentrePos = centre; vSize = size; eShapeType = type;
		}
	};

	int GetGridSize() { return m_iGridSize; }

	static sf::Vector2i GetGridSizeLimits() { return GRID_SIZE_LIMITS; }
	static sf::Vector2i GetTileSizeMin() { return MIN_SIZE; }
	static sf::Vector2i GetTileSizeMax() { return MAX_SIZE; }

	/* Compose map tiles into vector of collision rectangles */
	virtual std::vector<SMapShape> ShapeCompose() = 0;

	virtual void LoadFromFile(CGame* pGame, const std::string& sFilename) = 0;

	virtual void DrawLevel(CGame* pGame) = 0;
	
	/* Spawns */
	virtual sf::Vector2i GetSpawn(CGame* pGame) = 0;
	virtual void ResetSpawns() { }

	/* State - TODO */
	virtual void ResetState() { }

	/* Collision testing */
	virtual bool TileCollisionPlayer(sf::Vector2i worldPos) = 0;
	virtual bool TileCollisionBullet(sf::Vector2i worldPos) = 0;
	virtual bool FloorCollision(sf::Vector2i worldPos) = 0;

	/* Boundary testing */
	bool OutOfBoundsLeft(sf::Vector2i worldPos);
	bool OutOfBoundsRight(sf::Vector2i worldPos);
	bool OutOfBounds(sf::Vector2i worldPos);
	bool InBoundsTile(sf::Vector2i vTilePos);
	bool InBoundsWorld(sf::Vector2i vTilePos);

	void DrawBackground(CGame* pGame, sf::Vector2f playerPosition);

	sf::Vector2i GetSize() { return m_vSize; }

	sf::Vector2i WorldToTileUnits(sf::Vector2i vWorld);
	sf::Vector2i TileToWorldUnits(sf::Vector2i vWorld);

	void LoadStaticBackground(const std::string& sBackground);
	void LoadDynamicBackground(const std::string& sBackground, sf::Vector2f vMovescale);
	void ReplaceDynamicBackground(unsigned int index, const std::string& sBackground, sf::Vector2f vMovescale);
};

class C6SDevMapPublish;
class C6SDevMap : public C6SBaseMap {
protected:
	// Map tiles
	std::unique_ptr<SDevTile[]> m_pTiles;

	// Tilemap textures
	std::vector<sf::Texture> m_TileTextures;

	static const unsigned int MAX_TILE_TEXTURES = 256;

	// Debug rendering
	bool m_bGridEnabled = true;
	bool m_bBoundingBox = true;
	bool m_bTileTypesRender = true;

	void InitializeTiles(sf::Vector2i vTileSize, int iGridSize);

	void ShapeComposeType(std::vector<SMapShape>& shapes, 
		SMapTile::EMapTileType tileType, std::unique_ptr<char[]>& pTileInShape);

	sf::Texture* GetTileTexture(const SDevTile& tile);

	// Colors for bounding box and grid
	sf::Color m_DebugColor = sf::Color::Red;
	 
	void RenderBoundingBox(CGame* pGame);
	void RenderGrid(CGame* pGame);
	void RenderTileTypes(CGame* pGame, sf::Vector2i vStart, sf::Vector2i vEnd);
	void RenderTileTypes(CGame* pGame);
public:
	C6SDevMap() { m_MapType = MAP_DEVELOP; }

	unsigned int GetTileTextureCount() { return m_TileTextures.size(); }
	const std::vector<sf::Texture>& GetTileTextures() { return m_TileTextures; }
	bool RemoveTileTextureByIndex(int tileTextureIndex);
	bool InsertTileTexture(const std::string& sTileTexture);

	void SetDebugRenderColor(sf::Color color) { m_DebugColor = color; }

	static const char* GetMapFileExtension() { return MAP_FILE_DEV_EXTENSION; }
	static const short GetMapVersion() { return MAP_FILE_DEV_VERSION; }	
	const C6SBaseMap::EMapType GetMapType() { return m_MapType; }

	std::vector<SMapShape> ShapeCompose() override;

	void SetGridSize(int iGridsize);

	void EnableGrid(bool bEnabled) { m_bGridEnabled = bEnabled; }
	void EnableBoundingBox(bool bEnabled) { m_bBoundingBox = bEnabled; }
	void EnableTileTypeRender(bool bEnabled) { m_bTileTypesRender = bEnabled; }

	// Cereal serialization - write to archive
	template<class Archive> void save(Archive & archive) const {
		archive(cereal::make_nvp("Version", GetMapVersion()));
		archive(cereal::make_nvp("Type", m_MapType));
		archive(cereal::make_nvp("Size", m_vTileSize));
		archive(cereal::make_nvp("GridSize", m_iGridSize));

		// Tile data
		for (int i = 0; i < m_vTileSize.x*m_vTileSize.y; i++) {
			archive(cereal::make_nvp(std::to_string(i), m_pTiles[i]));
		}

		// Tileset
		archive(cereal::make_nvp(std::string("tile_count"), (unsigned int)m_TileTextures.size()));
		for (unsigned int i = 0; i < m_TileTextures.size(); i++) {
			archive(cereal::make_nvp(std::string("tile_") + std::to_string(i), m_TileTextures[i]));
		}

		// Background
		archive(cereal::make_nvp("Static_Background", m_StaticBG));

		// Parallax backgrounds
		/*
		for (sf::Uint8 i; i < m_ParallaxBGs.size(); i++) {
			archive(
				cereal::make_nvp(std::string("ParallaxImg") + std::to_string(i), m_ParallaxBGs[i].first),
				cereal::make_nvp(std::string("ParallaxScale") + std::to_string(i), m_ParallaxBGs[i].second)
			);
		}*/
	}

	// Cereal serialization - read from archive
	template<class Archive> void load(Archive & archive) {

		// Read and validate version
		short version; 
		archive(cereal::make_nvp("Version", version));
		if (version != GetMapVersion()) {
			throw std::runtime_error(std::string("DevMap::Load - Incorrect map version ") + \
				std::to_string(version) + ", expected " + std::to_string(GetMapVersion()));
		}

		archive(
			cereal::make_nvp("Type", m_MapType),
			cereal::make_nvp("Size", m_vTileSize),// Size in tiles
			cereal::make_nvp("GridSize", m_iGridSize)// Size (width,height) of tile in pixels
		);

		// Validate map type
		if (m_MapType != GetMapType()) {
			throw std::runtime_error("DevMap::Load - Tried to load non-develop map");
		}

		// Init tile buffer & load each tile into it
		// InitializeTiles() will validate tilesize & gridsize
		InitializeTiles(m_vTileSize, m_iGridSize);
		ClearTileData();
		for (int i = 0; i < m_vTileSize.x*m_vTileSize.y; i++) {
			archive(cereal::make_nvp(std::string("tile_") + std::to_string(i), m_pTiles[i]));
		}

		// Tileset
		unsigned int tilecount = -1;
		archive(cereal::make_nvp("tile_count", tilecount));
		if (tilecount < 0 || tilecount > MAX_TILE_TEXTURES) {
			for (unsigned int i = 0; i < m_TileTextures.size(); i++) {
				archive(cereal::make_nvp(std::string("tile_") + std::to_string(i), m_TileTextures[i]));
			}
		}

		// Load map background
		archive(cereal::make_nvp("Static_Background", m_StaticBG));

		// Load each parallax background
		/*
		m_ParallaxBGs.clear();
		sf::Uint8 parallaxBackgrounds;
		archive(cereal::make_nvp("ParallaxCount", parallaxBackgrounds));

		for (sf::Uint8 i; i < parallaxBackgrounds; i++) {
		m_ParallaxBGs.push_back(std::make_pair(sf::Texture(), sf::Vector2f(0.0f, 0.0f)));
		archive(
		cereal::make_nvp(std::string("ParallaxImg") + std::to_string(i), m_ParallaxBGs[i].first),
		cereal::make_nvp(std::string("ParallaxScale") + std::to_string(i), m_ParallaxBGs[i].second)
		);
		}*/
	}
	sf::Vector2i GetSpawn(CGame* pGame) override;

	virtual bool TileCollisionPlayer(sf::Vector2i worldPos) { return false; };
	virtual bool TileCollisionBullet(sf::Vector2i worldPos) { return false; };
	virtual bool FloorCollision(sf::Vector2i worldPos) { return false; };

	void LoadFromFile(CGame* pGame, const std::string& sFilename) override;

	void SaveToFile(CGame* pGame, const std::string& sFilename);

	// Create a new map in development mode
	// Development maps are stored as tiles
	void CreateBlank(sf::Vector2i vSize, int iGridSize, const std::string& sBackground=std::string());
	void CreateBlank(const std::string& sBackground = std::string());

	/* Change tile dimensions. Doesnt Preserve existing data TODO: Preserve
	Throws bad_alloc if out of memory, runtime_error if params are out of bounds */
	void Resize(sf::Vector2i vTileSize, int iGridSize);
	void Resize(sf::Vector2i vTileSize) { Resize(vTileSize, m_iGridSize); }

	/* Parse Map XML */
	void ClearTileData(SMapTile::EMapTileType defaultType = SMapTile::TYPE_AIR);

	void SetTileType(sf::Vector2i worldPos, SMapTile::EMapTileType type);
	void SetTileTypeTileUnits(sf::Vector2i tilePos, SMapTile::EMapTileType type);
	void DeleteTileType(sf::Vector2i worldPos);

	void SetTileTextureTile(sf::Vector2i tilePos, int textureIndex);
	void SetTileTexture(sf::Vector2i worldPos, int textureIndex);
	void DeleteTileTexture(sf::Vector2i worldPos);

	/* Draw game map*/
	void DrawLevel(CGame* pGame) override;
};

class C6SDevMapPublish : public C6SDevMap {
public:
	// Cereal serialization - write to archive
	template<class Archive> void save(Archive & archive) const {
		archive(cereal::make_nvp("Version", GetMapVersion()));
		archive(cereal::make_nvp("Type", m_MapType));
		archive(cereal::make_nvp("Size", m_vTileSize));
		archive(cereal::make_nvp("GridSize", m_iGridSize));

		// Map texture 
		sf::RenderTexture m_MapTexture;
		if (!m_MapTexture.create(m_vSize.x, m_vSize.y)) {
			throw std::runtime_error("Unable to export map - unable to generate texture");
		}
		m_MapTexture.clear(sf::Color(255, 255, 255, 0));

		// Render map as texture 
		for (int j = 0; j < m_vTileSize.y; j++) {
			for (int i = 0; i < m_vTileSize.y; i++) {
				if (m_pTiles[j*m_vTileSize.x + i].HasTexture()) {
					// Texture 
					CDrawingManager::DrawTileToTarget(&m_MapTexture,
						sf::Vector2f((float)(i*m_iGridSize), (float)(j*m_iGridSize)),
						m_iGridSize, &m_TileTextures[m_pTiles[j*m_vTileSize.x + i].iTextureIndex]);
				}
			}
		}
		archive(m_MapTexture.getTexture());

		// Tile data
		SMapTile output;
		for (int i = 0; i < m_vTileSize.x*m_vTileSize.y; i++) {
			output.tileType = m_pTiles[i].tileType;
			archive(cereal::make_nvp(std::string("tile_") + std::to_string(i), output));
		}

		
		sf::Texture texture = m_MapTexture.getTexture();
		sf::Image flipImg = texture.copyToImage();
		texture.update(flipImg);

		// Background
		archive(cereal::make_nvp("TileImg", texture));

		// Parallax backgrounds
		/*
		for (sf::Uint8 i; i < m_ParallaxBGs.size(); i++) {
		archive(
		cereal::make_nvp(std::string("ParallaxImg") + std::to_string(i), m_ParallaxBGs[i].first),
		cereal::make_nvp(std::string("ParallaxScale") + std::to_string(i), m_ParallaxBGs[i].second)
		);
		}*/
	}

	// Cereal serialization - read from archive
	template<class Archive> void load(Archive & archive) {
		throw std::exception("Tried to load Published dev map!");
	}

	void Publish(const std::string& sFile);
};

class C6SReleaseMap : public C6SBaseMap {
	std::unique_ptr<SMapTile[]> m_pTiles;

	// Rendered tile texture
	sf::Texture m_MapTexture;

public:
	virtual sf::Vector2i GetSpawn(CGame* pGame);
	virtual void ResetSpawns() {}

	static const char* GetMapFileExtension() { return MAP_FILE_RELEASE_EXTENSION; }
	static const short GetMapVersion() { return MAP_FILE_RELEASE_VERSION; }

	/* Compose map tiles into vector of collision rectangles */
	std::vector<SMapShape> ShapeCompose() override;
	void ShapeComposeType(std::vector<SMapShape>& shapes,
		SMapTile::EMapTileType tileType, std::unique_ptr<char[]>& pTileInShape);

	void DrawLevel(CGame* pGame) override;

	bool TileCollisionPlayer(sf::Vector2i worldPos) override;
	bool TileCollisionBullet(sf::Vector2i worldPos) override;
	bool FloorCollision(sf::Vector2i worldPos) override;

	void InitializeTiles(sf::Vector2i vTileSize, int iGridSize);
	void ClearTileData(SMapTile::EMapTileType defaultType = SMapTile::TYPE_AIR);

	template<class Archive> void save(Archive & archive) const { assert(false); }
	template<class Archive> void load(Archive & archive) {
		// Read and validate version
		short version; archive(cereal::make_nvp("Version", version));
		if (version != GetMapVersion()) {
		//	throw std::runtime_error(std::string("Map::Load - Incorrect map version ") + \
			//	std::to_string(version) + ", expected " + std::to_string(GetMapVersion()));
		}

		archive(
			cereal::make_nvp("Type", m_MapType),
			cereal::make_nvp("Size", m_vTileSize),// Size in tiles
			cereal::make_nvp("GridSize", m_iGridSize)// Size (width,height) of tile in pixels
			);

		// Read Map texture 
		archive(cereal::make_nvp("TileImg", m_MapTexture));

		// Init tile buffer & load each tile into it
		InitializeTiles(m_vTileSize, m_iGridSize);
		ClearTileData();
		for (int i = 0; i < m_vTileSize.x*m_vTileSize.y; i++) {
			archive(cereal::make_nvp(std::string("tile_") + std::to_string(i), m_pTiles[i]));
		}


		sf::Image flipImg = m_MapTexture.copyToImage();
		flipImg.flipVertically();
		m_MapTexture.update(flipImg);
	}

	void LoadFromFile(CGame* pGame, const std::string& sFilename);
};

#endif