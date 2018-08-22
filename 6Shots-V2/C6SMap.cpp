#include "stdafx.h"
#include "C6SMap.h"
#include "CGame.h"

/*
	Christopher De Pasquale
	Created:		Sometime in 2016
	Last Edited:	31th of May 2018
*/

/*
bool C6SDevMap::SaveToFile(std::string filename) {
	std::ofstream fStream(filename, std::ios::out | std::ios::binary | std::ios::trunc);
	if (!fStream.good())
		return false;
	cereal::BinaryOutputArchive outArchive(fStream);
	outArchive(*this);
	return true;
}*/

const sf::Vector2i C6SBaseMap::MAX_SIZE = sf::Vector2i(256, 256);
const sf::Vector2i C6SBaseMap::MIN_SIZE = sf::Vector2i(1, 1);
const sf::Vector2i C6SBaseMap::GRID_SIZE_LIMITS = sf::Vector2i(1, 128);
const sf::Vector2i C6SBaseMap::DEFAULT_SIZE = sf::Vector2i(64, 48);

void DrawMultiTile(sf::Vector2f pos, sf::Vector2f size, sf::Color colorA, sf::Color colorB, CGame* pGame) {
	size /= 2.0f;
	pGame->m_Drawing.DrawRectangle(&pGame->m_WindowManager, pos - size, size, colorA);
	pGame->m_Drawing.DrawRectangle(&pGame->m_WindowManager, pos, size, colorA);
	pGame->m_Drawing.DrawRectangle(&pGame->m_WindowManager, pos - sf::Vector2f(0.0f, size.y), size, colorB);
	pGame->m_Drawing.DrawRectangle(&pGame->m_WindowManager, pos - sf::Vector2f(size.x, 0.0f), size, colorB);
	
	pGame->m_Drawing.DrawLine(&pGame->m_WindowManager, pos - sf::Vector2f(0.0f, size.y), pos + sf::Vector2f(0.0f, size.y), sf::Color::Red);
	pGame->m_Drawing.DrawLine(&pGame->m_WindowManager, pos - sf::Vector2f(size.x, 0.0f), pos + sf::Vector2f(size.x, 0.0f), sf::Color::Red);
}
void DrawNameTile(const std::string& name, sf::Vector2f pos, sf::Vector2f size, sf::Color boxColor, sf::Color nameColor, CGame* pGame) {
	pGame->m_Drawing.DrawRectangleCentred(&pGame->m_WindowManager, pos, size, boxColor);
	pGame->m_Drawing.DrawTextCentred(&pGame->m_WindowManager, name, pos, (int)(size.x / 3.0f), nameColor);
}
void DrawColorTile(CGame* pGame, sf::Vector2f drawPos, sf::Vector2f drawSize, sf::Color drawColor) {
	pGame->m_Drawing.DrawRectangleCentred(&pGame->m_WindowManager, drawPos, drawSize, drawColor);
}
void SMapTile::RenderType(CGame* pGame, sf::Vector2i pos, int iSize) {
	sf::Color color(0, 0, 0, 80);
	sf::Vector2f drawSize((float)iSize, (float)iSize);
	sf::Vector2f drawPos = sf::Vector2f(pos) + drawSize / 2.0f;

	switch (this->tileType) {
		case TYPE_PARTIALSOLID:
			DrawMultiTile(drawPos, drawSize, sf::Color(0x00ff0050), sf::Color(0xff000050), pGame);
			break;
		case TYPE_INSTADEATH:
			DrawNameTile("DIE", drawPos, drawSize, sf::Color(0x00000050), sf::Color(0x000000ff), pGame);
			break;
		case TYPE_SLOWDAMAGE:
			DrawNameTile("HURT", drawPos, drawSize, sf::Color(0xff000050), sf::Color(0xaa0000ff), pGame);
			break;
		case TYPE_LADDER:
			DrawNameTile("L", drawPos, drawSize, sf::Color(0xffE00050), sf::Color(0x9f8500ff), pGame);
			break;
		case TYPE_SOLID:
			DrawColorTile(pGame, drawPos, drawSize, sf::Color(0xff000050));
			break;
		case TYPE_BOUNDARY:
			DrawColorTile(pGame, drawPos, drawSize, sf::Color(0x00000050));
			break;
		case TYPE_WATER:
			DrawColorTile(pGame, drawPos, drawSize, sf::Color(0x0000ff50));
			break;
		case TYPE_AIR:
			break;
		default:
			DrawNameTile("?", drawPos, drawSize, sf::Color(0xff000050), sf::Color(0xff0000ff), pGame);
	}
}

void C6SDevMap::ShapeComposeType(std::vector<SMapShape>& shapes, SMapTile::EMapTileType tileType, std::unique_ptr<char[]>& pTileInShape) {
	sf::Vector2i startTile, endTile;
	bool bSearching = false;

	for (int j = 0; j < m_vTileSize.y; j++) {
		for (int i = 0; i < m_vTileSize.x; i++) {

			if (m_pTiles[j*m_vTileSize.x + i].tileType == tileType && pTileInShape[j*m_vTileSize.x + i] == 0) {
				// record start of new shape
				startTile = sf::Vector2i(i, j);

				// And record end, for case where size is 1 x 1
				endTile = startTile;

				// Get width of this rectangle by searching for the first shape or non-solid tile
				for (;i < m_vTileSize.x; i++) {
					if (m_pTiles[j*m_vTileSize.x + i].tileType != tileType || pTileInShape[j*m_vTileSize.x + i] != 0) {
						break;
					}
					endTile.x = i;
				}

				// Move one tile under the start
				j += 1;

				// Get height of this rectangle by finding the first row with a shape or a non-solid tile
				bSearching = true;
				for (; j < m_vTileSize.y; j++) {
					for (i = startTile.x; i < m_vTileSize.x && i <= endTile.x; i++) {
						if (m_pTiles[j*m_vTileSize.x + i].tileType != tileType || pTileInShape[j*m_vTileSize.x + i] != 0) {
							bSearching = false;
							break;
						}
					}
					if (!bSearching)
						break;
					endTile.y = j;
				}

				// Calculate size in tiles, set InShape for each tile
				sf::Vector2i size = endTile - startTile + sf::Vector2i(1, 1);
				for (j = startTile.y; j <= endTile.y; j++) {
					memset(pTileInShape.get() + j*m_vTileSize.x + startTile.x, 1, size.x);
				}

				// Convert tile units to game units, insert as shape
				size *= m_iGridSize; 
				shapes.push_back(SMapShape(tileType, 
					startTile*m_iGridSize + (size / 2), size));

				// Restore i, j
				i = endTile.x;
				j = startTile.y;
			}
		}
	}

}

std::vector<C6SBaseMap::SMapShape> C6SDevMap::ShapeCompose() {
	std::vector<C6SBaseMap::SMapShape> shapes;
	auto pTileInShape = std::make_unique<char[]>(m_vTileSize.x * m_vTileSize.y);

	ShapeComposeType(shapes, SMapTile::TYPE_SOLID, pTileInShape); 
	ShapeComposeType(shapes, SMapTile::TYPE_LADDER, pTileInShape);

	return shapes;
}

void C6SReleaseMap::ShapeComposeType(std::vector<SMapShape>& shapes, SMapTile::EMapTileType tileType, std::unique_ptr<char[]>& pTileInShape) {
	sf::Vector2i startTile, endTile;
	bool bSearching = false;

	for (int j = 0; j < m_vTileSize.y; j++) {
		for (int i = 0; i < m_vTileSize.x; i++) {

			if (m_pTiles[j*m_vTileSize.x + i].tileType == tileType && pTileInShape[j*m_vTileSize.x + i] == 0) {
				// record start of new shape
				startTile = sf::Vector2i(i, j);

				// And record end, for case where size is 1 x 1
				endTile = startTile;

				// Get width of this rectangle by searching for the first shape or non-solid tile
				for (; i < m_vTileSize.x; i++) {
					if (m_pTiles[j*m_vTileSize.x + i].tileType != tileType || pTileInShape[j*m_vTileSize.x + i] != 0) {
						break;
					}
					endTile.x = i;
				}

				// Move one tile under the start
				j += 1;

				// Get height of this rectangle by finding the first row with a shape or a non-solid tile
				bSearching = true;
				for (; j < m_vTileSize.y; j++) {
					for (i = startTile.x; i < m_vTileSize.x && i <= endTile.x; i++) {
						if (m_pTiles[j*m_vTileSize.x + i].tileType != tileType || pTileInShape[j*m_vTileSize.x + i] != 0) {
							bSearching = false;
							break;
						}
					}
					if (!bSearching)
						break;
					endTile.y = j;
				}

				// Calculate size in tiles, set InShape for each tile
				sf::Vector2i size = endTile - startTile + sf::Vector2i(1, 1);
				for (j = startTile.y; j <= endTile.y; j++) {
					memset(pTileInShape.get() + j*m_vTileSize.x + startTile.x, 1, size.x);
				}

				// Convert tile units to game units, insert as shape
				size *= m_iGridSize;
				shapes.push_back(SMapShape(tileType,
					startTile*m_iGridSize + (size / 2), size));

				// Restore i, j
				i = endTile.x;
				j = startTile.y;
			}
		}
	}

}

std::vector<C6SBaseMap::SMapShape> C6SReleaseMap::ShapeCompose() {
	std::vector<C6SBaseMap::SMapShape> shapes;
	auto pTileInShape = std::make_unique<char[]>(m_vTileSize.x * m_vTileSize.y);

	ShapeComposeType(shapes, SMapTile::TYPE_SOLID, pTileInShape);
	ShapeComposeType(shapes, SMapTile::TYPE_LADDER, pTileInShape);

	return shapes;
}
void C6SBaseMap::DrawBackground(CGame* pGame, sf::Vector2f playerPosition) {
	/* Static Background */
	pGame->m_Drawing.DrawSpriteCentred(&pGame->m_WindowManager, 
		pGame->m_WindowManager.GetScreenCentre(), m_vSize, &m_StaticBG);

	/* Dynamic Backgrounds */
	for (unsigned int i = 0; i < m_ParallaxBGs.size(); i++) {
		sf::Vector2f moveFactor = (sf::Vector2f(m_vSize / 2) - playerPosition);
		sf::Vector2f moveDelta(m_ParallaxBGs[i].second.x * moveFactor.x, 
							   m_ParallaxBGs[i].second.y * moveFactor.y);

		pGame->m_Drawing.DrawSprite(&pGame->m_WindowManager, (int)moveDelta.x, (int)moveDelta.y,
			m_vSize.x, m_vSize.y, &m_ParallaxBGs[i].first);
	}
}

void C6SReleaseMap::DrawLevel(CGame* pGame) {
	pGame->m_Drawing.DrawSprite(pGame->GetWindowMgr(), 0, 0,
		m_vSize.x, m_vSize.y, &m_MapTexture);
}

sf::Texture* C6SDevMap::GetTileTexture(const SDevTile& tile) {
	if (tile.iTextureIndex >= m_TileTextures.size())
		throw new std::runtime_error("Attempted to get tile texture with invalid index " + to_string(tile.iTextureIndex));
	return &m_TileTextures[tile.iTextureIndex];
}

void C6SDevMap::DrawLevel(CGame* pGame) {
	// Map screen view bounds to start and end map tile bounds
	sf::Vector2f vHalfScrSize = pGame->m_WindowManager.GetScreenCentre2f();
	sf::Vector2f vViewPos = pGame->m_WindowManager.GetViewCentre();
	sf::Vector2i start((vViewPos - vHalfScrSize) / (float)m_iGridSize);
	sf::Vector2i end((vViewPos + vHalfScrSize) / (float)m_iGridSize);

	// We are truncating end tile pos into view, add a tile leeway to avoid 
	// tiles dissapearing while still on-screen
	end.x++; end.y++;

	// cap start to range [0, tilesize.x), end to range [0, tilesize.y)
	start.x = std::min(std::max(start.x, 0), m_vTileSize.x);
	start.y = std::min(std::max(start.y, 0), m_vTileSize.y);
	end.x = std::min(std::max(end.x, 0), m_vTileSize.x);
	end.y = std::min(std::max(end.y, 0), m_vTileSize.y);

	for (int j = start.y; j < end.y; j++) {
		for (int i = start.x; i < end.x; i++) {
			if (m_pTiles[j*m_vTileSize.x + i].HasTexture()) {
				pGame->m_Drawing.DrawSprite(pGame->GetWindowMgr(), 
					i*m_iGridSize, j*m_iGridSize, // X, Y
					m_iGridSize,  // Size
					GetTileTexture(m_pTiles[j*m_vTileSize.x + i]));
			}
		}
	}

	if (m_bTileTypesRender) {
		RenderTileTypes(pGame, start, end);
	}

	if (m_bGridEnabled)
		RenderGrid(pGame);
	else if (m_bBoundingBox)
		RenderBoundingBox(pGame);
}


void C6SDevMap::RenderBoundingBox(CGame* pGame) {
	pGame->m_Drawing.DrawRectangleOutline(&pGame->m_WindowManager, sf::Vector2i(0, 0), m_vSize, m_DebugColor);
}

void C6SDevMap::RenderGrid(CGame* pGame) {
	for (int i = 0; i <= m_vSize.x; i += m_iGridSize) {
		pGame->m_Drawing.DrawLine(&pGame->m_WindowManager,
			sf::Vector2f((float)i, 0.0f), sf::Vector2f((float)i, (float)m_vSize.y), m_DebugColor);
	}
	for (int i = 0; i <= m_vSize.y; i += m_iGridSize) {
		pGame->m_Drawing.DrawLine(&pGame->m_WindowManager, 
			sf::Vector2f(0.0f, (float)i), sf::Vector2f((float)m_vSize.x, (float)i), m_DebugColor);
	}
}

void C6SDevMap::RenderTileTypes(CGame* pGame, sf::Vector2i vStart, sf::Vector2i vEnd) {
	for (int j = vStart.y; j < vEnd.y; j++) {
		for (int i = vStart.x; i < vEnd.x; i++) {
			m_pTiles[j*m_vTileSize.x + i].RenderType(pGame, sf::Vector2i(i*m_iGridSize, j*m_iGridSize), m_iGridSize);
		}
	}
}

void C6SDevMap::RenderTileTypes(CGame* pGame) {
	RenderTileTypes(pGame, sf::Vector2i(0, 0), m_vTileSize);
}

bool C6SDevMap::InsertTileTexture(const std::string& sTileTexture) {
	m_TileTextures.push_back(sf::Texture());
	if (!m_TileTextures.back().loadFromFile(sTileTexture)) {
		m_TileTextures.pop_back();
		return false;
	}
	return true;
}

void C6SDevMapPublish::Publish(const std::string& sFile) {
	std::ofstream fStream;
	fStream.open(sFile, std::ios::binary);
	if (!fStream.good())
		throw std::runtime_error("DevMapPublish::Publish() - Unable to open file " + sFile);

	cereal::PortableBinaryOutputArchive outArchive(fStream);
	outArchive(*this);
}

void C6SBaseMap::LoadStaticBackground(const std::string& sBackground) {
	if (!m_StaticBG.loadFromFile(sBackground)) {
		throw std::runtime_error("Map::LoadStaticBackground - Failed to read background file " + sBackground);
	}
}

void C6SBaseMap::ReplaceDynamicBackground(unsigned int index, const std::string& sBackground, sf::Vector2f vMovescale) {
	if (index >= m_ParallaxBGs.size()) {
		throw std::runtime_error("ReplaceDynamicBackground - Invalid index " + to_string(index));
	}

	m_ParallaxBGs[index] = std::make_pair(sf::Texture(), vMovescale);
	if (!m_ParallaxBGs[index].first.loadFromFile(sBackground)) {
		throw std::runtime_error("LoadDynamicBackground - Failed to load texture " + sBackground);
	}
}
void C6SBaseMap::LoadDynamicBackground(const std::string& sBackground, sf::Vector2f vMovescale) {
	auto pair = std::make_pair(sf::Texture(), vMovescale);
	if (!pair.first.loadFromFile(sBackground)) {
		throw std::runtime_error("LoadDynamicBackground - Failed to load texture " + sBackground);
	}

	m_ParallaxBGs.push_back(std::move(pair));
}

bool C6SReleaseMap::TileCollisionPlayer(sf::Vector2i worldPos) {
	worldPos = WorldToTileUnits(worldPos);
	return m_pTiles[worldPos.y*m_vTileSize.x + worldPos.x].IsPlayerSolid();
}
bool C6SReleaseMap::TileCollisionBullet(sf::Vector2i worldPos) {
	worldPos = WorldToTileUnits(worldPos);
	return m_pTiles[worldPos.y*m_vTileSize.x + worldPos.x].IsBulletSolid();
}
bool C6SReleaseMap::FloorCollision(sf::Vector2i worldPos) {
	return (worldPos.y < 0 || !InBoundsWorld(worldPos) || TileCollisionPlayer(worldPos));
}
void C6SReleaseMap::InitializeTiles(sf::Vector2i vTileSize, int iGridSize) {
	if (iGridSize <= 0 || vTileSize.x <= 0 || vTileSize.y <= 0)
		throw std::runtime_error("Error Initializing Dev Map Tiles - Size/GridSize out of bounds");
	m_iGridSize = iGridSize;
	m_vSize = vTileSize * iGridSize;
	m_vTileSize = vTileSize;
	m_pTiles = std::unique_ptr<SMapTile[]>(new SMapTile[m_vTileSize.x * m_vTileSize.y]);
}
void C6SReleaseMap::ClearTileData(SMapTile::EMapTileType defaultType) {
	SMapTile defaultTile(defaultType);
	int size = m_vTileSize.x * m_vTileSize.y;
	for (SMapTile* ptr = m_pTiles.get(), *end = ptr + size; ptr != end; ptr++) {
		memcpy(ptr, &defaultTile, sizeof(SMapTile));
	}
}
void C6SReleaseMap::LoadFromFile(CGame* pGame, const std::string& sFilename) {
	std::ifstream fStream;
	fStream.open(sFilename, std::ios::binary);
	if (!fStream.good())
		throw std::runtime_error("Map::LoadFromFile() - Unable to open file " + sFilename);

	{// Data read may not occur until cereal archive destructor is called
		cereal::PortableBinaryInputArchive inArchive(fStream);
		inArchive(*this);
	}

	// Validate input
	//if (m_MapType != C6SBaseMap::MAP_RELEASE) {
	//	throw std::runtime_error("DevMap::Load - Tried to load non-develop map");
	//}
}
bool C6SBaseMap::OutOfBoundsLeft(sf::Vector2i worldPos) {
	return (worldPos.x <= 0);
}

bool C6SBaseMap::OutOfBoundsRight(sf::Vector2i worldPos) {
	return (worldPos.y >= m_vSize.x);
}

bool C6SBaseMap::OutOfBounds(sf::Vector2i pos) {
	return (pos.x <= 0 || pos.y <= 0 || pos.x >= m_vSize.x || pos.y >= m_vSize.y);
}
void C6SDevMap::SetGridSize(int iGridsize) {
	if (GRID_SIZE_LIMITS.x <= 0 || iGridsize > GRID_SIZE_LIMITS.y)
		throw std::runtime_error("Grid size out of range");
	m_iGridSize = iGridsize;
	m_vSize = m_vTileSize * m_iGridSize;
}
sf::Vector2i C6SDevMap::GetSpawn(CGame* pGame) {
	return sf::Vector2i(pGame->m_Random.RandomInt(m_vSize.x), 200);
}
void C6SDevMap::InitializeTiles(sf::Vector2i vTileSize, int iGridSize) {
	if (iGridSize <= 0 || vTileSize.x <= 0 || vTileSize.y <= 0)
		throw std::runtime_error("Error Initializing Dev Map Tiles - Size/GridSize out of bounds");
	m_iGridSize = iGridSize;
	m_vSize = vTileSize * iGridSize;
	m_vTileSize = vTileSize;
	m_pTiles = std::unique_ptr<SDevTile[]>(new SDevTile[m_vTileSize.x * m_vTileSize.y]);
}
void C6SDevMap::LoadFromFile(CGame* pGame, const std::string& sFilename) {
	std::ifstream fStream;
	fStream.open(sFilename, std::ios::binary);
	if (!fStream.good()) {
		throw std::runtime_error("DevMap::LoadFromFile() - Unable to open file " + sFilename);
	}

	{// Data read may not occur until cereal archive destructor is called
		cereal::XMLInputArchive inArchive(fStream);
		inArchive(*this);
	}
}
void C6SDevMap::SaveToFile(CGame* pGame, const std::string& sFilename) {
	std::ofstream fStream;
	fStream.open(sFilename, std::ios::binary);
	if (!fStream.good())
		throw std::runtime_error("DevMap::SaveToFile() - Unable to open file " + sFilename);

	cereal::XMLOutputArchive outArchive(fStream);
	outArchive(*this);
}
void C6SDevMap::CreateBlank(sf::Vector2i vSize, int iGridSize, const std::string& sBackground) {
	if (iGridSize <= 0 || vSize.x <= 0 || vSize.y <= 0)
		throw std::runtime_error("Error creating Dev Map - Invalid Initialize args");

	InitializeTiles(vSize, iGridSize);
	ClearTileData();

	if (sBackground.empty()) {
		// No background provided -> Initialize black background
		sf::Image img; img.create(vSize.x, vSize.y);
		m_StaticBG.create(vSize.x, vSize.y);
		m_StaticBG.update(img);
		return;
	}

	if (!m_StaticBG.loadFromFile(sBackground)) {
		throw std::runtime_error("Error creating Dev Map - Invalid background file " + sBackground);
	}
}
void C6SDevMap::CreateBlank(const std::string& sBackground) {
	CreateBlank(C6SDevMap::DEFAULT_SIZE, C6SDevMap::DEFAULT_GRID_SIZE, sBackground);
}
void C6SDevMap::Resize(sf::Vector2i vSize, int iGridSize) {
	if (m_vTileSize == vSize)
		return;
	if (vSize.x > MAX_SIZE.x || vSize.y > MAX_SIZE.y) {
		throw std::runtime_error("DevMap::Resize() - New size " + std::to_string(vSize) + \
			" greater than max: " + std::to_string(MAX_SIZE));
	}
	if (vSize.x < MIN_SIZE.x || vSize.y < MIN_SIZE.y) {
		throw std::runtime_error(std::string("DevMap::Resize() - New size ") + std::to_string(vSize) + \
			std::string(" smaller than max: ") + std::to_string(MAX_SIZE));
	}
	if (iGridSize < GRID_SIZE_LIMITS.x || iGridSize > GRID_SIZE_LIMITS.y) {
		throw std::runtime_error("DevMap::Resize() - New tile size " + std::to_string(iGridSize) + \
			" not in range: " + std::to_string(GRID_SIZE_LIMITS));
	}

	// Copy previous tiles
	sf::Vector2i prevSize = m_vTileSize;
	auto tileBackup = std::make_unique<SDevTile[]>(m_vTileSize.x * m_vTileSize.y);
	memcpy(tileBackup.get(), m_pTiles.get(), m_vTileSize.x*m_vTileSize.y*sizeof(SDevTile));
	
	// Create new tile grid
	InitializeTiles(vSize, iGridSize);
	ClearTileData(); 
	printf("Backup = %s\n", std::to_string(prevSize).c_str());
	printf("New    = %s\n", std::to_string(m_vTileSize).c_str());
	printf("\n");
	for (int j = 0; j < prevSize.y; j++) {
		for (int i = 0; i < prevSize.x; i++) {
			printf("%i", tileBackup[j*prevSize.y + i].tileType);
		}
		printf("\n");
	}
	printf("\n");
	// Copy old tiles into new grid
	for (int j = 0; j < m_vTileSize.y && j < prevSize.y; j++) {
		for (int i = 0; i < m_vTileSize.x && i < prevSize.x; i++) {
				m_pTiles[j*m_vTileSize.y + i].tileType = tileBackup[j*prevSize.y + i].tileType;
				printf("%i, %i - Backup [T-%i] %i, New [T-%i] %i\n", i, j, tileBackup[j*prevSize.y + i].tileType, j*prevSize.y + i, m_pTiles[j*m_vTileSize.y + i].tileType, j*m_vTileSize.y + i);
		}
		printf("\n");
	}
}

void C6SDevMap::ClearTileData(SMapTile::EMapTileType defaultType) {
	SDevTile defaultTile(defaultType);
	int size = m_vTileSize.x * m_vTileSize.y;
	for (SDevTile* ptr = m_pTiles.get(), *end = ptr+size; ptr != end; ptr++) {
		memcpy(ptr, &defaultTile, sizeof(SDevTile));
	}
}

bool C6SBaseMap::InBoundsTile(sf::Vector2i vTilePos) {
	return !(vTilePos.x < 0 || vTilePos.y < 0 || 
			 vTilePos.x >= m_vTileSize.x ||  vTilePos.y >= m_vTileSize.y);
}

bool C6SBaseMap::InBoundsWorld(sf::Vector2i vWorld) {
	return !(vWorld.x < 0 || vWorld.x >= m_vSize.x || vWorld.y < 0 || vWorld.y >= m_vSize.y);
}

sf::Vector2i C6SBaseMap::WorldToTileUnits(sf::Vector2i vWorld) {
	return sf::Vector2i(sf::Vector2f(vWorld) / (float)m_iGridSize);
}

void C6SDevMap::SetTileType(sf::Vector2i worldPos, SMapTile::EMapTileType type) {
	if (!InBoundsWorld(worldPos)) 
		return;
	sf::Vector2i tile = WorldToTileUnits(worldPos);	
	m_pTiles[tile.y * m_vTileSize.x + tile.x].tileType = type;
}

void C6SDevMap::SetTileTypeTileUnits(sf::Vector2i tilePos, SMapTile::EMapTileType type) {
	if (!InBoundsTile(tilePos))
		return;
	m_pTiles[tilePos.y * m_vTileSize.x + tilePos.x].tileType = type;
}

void C6SDevMap::DeleteTileType(sf::Vector2i worldPos) {
	SetTileType(worldPos, SMapTile::TYPE_AIR);
}
void C6SDevMap::SetTileTextureTile(sf::Vector2i tilePos, int textureIndex) {
	if (!InBoundsTile(tilePos))
		return;
	m_pTiles[tilePos.y * m_vTileSize.x + tilePos.x].iTextureIndex = textureIndex;
}
void C6SDevMap::SetTileTexture(sf::Vector2i worldPos, int textureIndex) {
	if (!InBoundsWorld(worldPos))
		return;
	sf::Vector2i tile = WorldToTileUnits(worldPos);
	m_pTiles[tile.y * m_vTileSize.x + tile.x].iTextureIndex = textureIndex;
}
void C6SDevMap::DeleteTileTexture(sf::Vector2i worldPos) {
	SetTileTexture(worldPos, SDevTile::INVALID_TEXTURE);
}

sf::Vector2i C6SReleaseMap::GetSpawn(CGame* pGame) {
	return sf::Vector2i(pGame->m_Random.RandomInt(m_vSize.x), 200);
}