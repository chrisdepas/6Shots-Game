#include "stdafx.h"
#include "C6SMap.h"
#include "CGame.h"
#include <fstream>
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>

/*
	Christopher De Pasquale
	Created:		Sometime in 2016
	Last Edited:	19th of August 2018
*/

const char* C6SMap::MAP_FILETYPE = ".6map";

void C6SMap::SMapTile::RenderType(CGame* pGame, Vector2i pos, int iSize) {
	unsigned char r, g, b;
	r = g = b = 0;

	/* Special case - PartialSolid -> Chequered */
	if (this->tileType == TYPE_PARTIALSOLID) {
		pos.X += iSize / 4;
		pos.Y += iSize / 4;

		g = 255;
		pGame->m_Drawing.DrawRectangleCentred(&pGame->m_WindowManager, 
			Vector2i(pos.X, pos.Y), iSize/2, iSize/2, r, g, b, 80);
		pGame->m_Drawing.DrawRectangleCentred(&pGame->m_WindowManager,
			Vector2i(pos.X + iSize/2, pos.Y + iSize/2), iSize/2, iSize/2, r, g, b, 80);

		g = 0; r = 255;
		pGame->m_Drawing.DrawRectangleCentred(&pGame->m_WindowManager,
			Vector2i(pos.X + iSize / 2, pos.Y), iSize / 2, iSize / 2, r, g, b, 80);
		pGame->m_Drawing.DrawRectangleCentred(&pGame->m_WindowManager,
			Vector2i(pos.X, pos.Y + iSize / 2), iSize / 2, iSize / 2, r, g, b, 80);

		return;
	}
	/* Special case - instakill -> Text */
	else if (this->tileType == TYPE_INSTADEATH) {
		pos.X += iSize / 2;
		pos.Y += iSize / 2;
		pGame->m_Drawing.DrawRectangleCentred(&pGame->m_WindowManager, pos, iSize, iSize, 255, 0, 0, 80);
		pGame->m_Drawing.DrawTextCentredX(&pGame->m_WindowManager, "DIE", 
			pos.X, pos.Y, 12, 255, 0, 0, 255);
		return;
	}
	/* Special case - slow damage -> text */ 
	else  if (this->tileType == TYPE_SLOWDAMAGE) {
		pos.X += iSize / 2;
		pos.Y += iSize / 2;
		pGame->m_Drawing.DrawRectangleCentred(&pGame->m_WindowManager, pos, iSize, iSize, 0, 255, 0, 80);
		pGame->m_Drawing.DrawTextCentredX(&pGame->m_WindowManager, "HRT",
			pos.X, pos.Y, 12, 255, 0, 0, 255);
		return;
	}

	/* Get color from type */
	switch (this->tileType) {
	case TYPE_AIR:
		g = 255;
		break;
	case TYPE_LADDER:
		r = 255;
		g = 230;
		break;
	case TYPE_SOLID:
		r = 255;
		break;
	case TYPE_BOUNDARY:
		g = r = b = 0;
		break;
	case TYPE_WATER:
		b = 255;
		break;
	}

	// Draw colored debug rectangle 
	pos.X += iSize / 2;
	pos.Y += iSize / 2;
	pGame->m_Drawing.DrawRectangleCentred(&pGame->m_WindowManager, pos, iSize, iSize, r, g, b, 80);
}

bool C6SMap::SMapTile::IsSolid() {
	return (tileType == TYPE_SOLID);
}

Vector2i C6SMap::GetSize() {
	return m_vSize;
}

bool C6SMap::ShapeDecomposeNew(std::vector<SMapShape>& shapes) {
	Vector2i startTile;
	Vector2i endTile;
	bool bInShape = false;
	bool bSearching = false;

	char* tileInShape = new char[m_vTileSize.X * m_vTileSize.Y];
	memset(tileInShape, 0, sizeof(char) * m_vTileSize.X * m_vTileSize.Y);

	/* Decompose solid tiles into shapes */
	for (int j = 0; j < m_vTileSize.Y; j++) {
		for (int i = 0; i < m_vTileSize.X; i++) {

			if (m_Tiles[j*m_vTileSize.X + i].tileType == SMapTile::TYPE_SOLID && tileInShape[j*m_vTileSize.X + i] == 0) {
				// record start of new shape
				startTile = Vector2i(i, j);
				// And record end, for case where size is 1 x 1
				endTile = startTile;
				
				// Get width of this rectangle by searching for the first shape or non-solid tile
				for (; i < m_vTileSize.X; i++) {
					if (m_Tiles[j*m_vTileSize.X + i].tileType != SMapTile::TYPE_SOLID || tileInShape[j*m_vTileSize.X + i] != 0) {
						break;
					}
					endTile.X = i;
				}

				// Move one tile under the start
				j += 1;

				// Get height of this rectangle by finding the first row with a shape or a non-solid tile
				bSearching = true;
				for (; j < m_vTileSize.Y; j++) {
					for (i = startTile.X; i < m_vTileSize.X && i <= endTile.X; i++) {
						if (m_Tiles[j*m_vTileSize.X + i].tileType != SMapTile::TYPE_SOLID || tileInShape[j*m_vTileSize.X + i] != 0) {
							bSearching = false;
							break;
						}
					}
					if (!bSearching) 
						break;
					endTile.Y = j;
				}
						
				// Calculate size in tiles, add one to encompass tiles
				SMapShape shape;
				shape.eShapeType = SMapTile::TYPE_SOLID;
				shape.vSize.X = endTile.X - startTile.X + 1;
				shape.vSize.Y = endTile.Y - startTile.Y + 1;

				// set inShape array to True for each tile
				for (j = startTile.Y; j <= endTile.Y; j++) {
					memset(tileInShape + j*m_vTileSize.X + startTile.X, 1, shape.vSize.X);
				}

				// Get shape centre & size in game units, add to list
				shape.vSize *= m_iGridSize;
				shape.vCentrePos.X = (startTile.X * m_iGridSize) + (shape.vSize.X / 2);
				shape.vCentrePos.Y = (startTile.Y * m_iGridSize) + (shape.vSize.Y / 2);
				shapes.push_back(shape);

				// Restore i, j
				i = endTile.X;
				j = startTile.Y;
			}
		}
	}

	/* Decompose Ladder tiles into shapes */
	for (int j = 0; j < m_vTileSize.Y; j++) {
		for (int i = 0; i < m_vTileSize.X; i++) {
			if (m_Tiles[j*m_vTileSize.X + i].tileType == SMapTile::TYPE_LADDER) {
				if (!bInShape) {
					// Entered solid shape 
					startTile.X = i;
					startTile.Y = j;
					endTile = startTile;

					bInShape = true;
				}
				else {
					// Still in solid shape
					if (startTile.Y != j) {
						// Have moved to a different Y level, save last shape
						SMapShape shape;
						shape.eShapeType = SMapTile::TYPE_LADDER;
						shape.vSize.X = (endTile.X - startTile.X) * m_iGridSize;
						shape.vSize.Y = m_iGridSize;
						shape.vCentrePos.X = (startTile.X * m_iGridSize) + (shape.vSize.X / 2);
						shape.vCentrePos.Y = (startTile.Y * m_iGridSize) + (shape.vSize.Y / 2);
						shapes.push_back(shape);

						// Begin new shape 
						startTile.X = i;
						startTile.Y = j;
					}
					endTile.X = i;
					endTile.Y = j;
				}
			}
			else {
				// Not in solid shape 
				if (bInShape) {
					// Was previously in shape, save last shape 
					SMapShape shape;
					shape.eShapeType = SMapTile::TYPE_LADDER;
					shape.vSize.X = ((endTile.X - startTile.X) + 1) * m_iGridSize;
					shape.vSize.Y = m_iGridSize;
					shape.vCentrePos.X = (startTile.X * m_iGridSize) + (shape.vSize.X / 2);
					shape.vCentrePos.Y = (startTile.Y * m_iGridSize) + (shape.vSize.Y / 2);
					shapes.push_back(shape);

					bInShape = false;
				}
			}
		}
	}

	// Free memory 
	delete tileInShape;

	return true;
}

// Todo : More efficient, less dogshit way of doing this 
bool C6SMap::ShapeDecompose(std::vector<SMapShape>& shapes) {
	return ShapeDecomposeNew(shapes);

	Vector2i startTile;
	Vector2i endTile;
	bool bInShape = false;


	/* Solid tiles */
	for (int j = 0; j < m_vTileSize.Y; j++) {
		for (int i = 0; i < m_vTileSize.X; i++) {
			if (m_Tiles[j*m_vTileSize.X + i].tileType == SMapTile::TYPE_SOLID) {
				if (!bInShape) {
					// Entered solid shape 
					startTile.X = i;
					startTile.Y = j;
					endTile = startTile;

					bInShape = true;
				} else {
					// Still in solid shape
					if (startTile.Y != j) {
						// Have moved to a different Y level, save last shape
						SMapShape shape;
						shape.eShapeType = SMapTile::TYPE_SOLID;
						shape.vSize.X = (endTile.X - startTile.X) * m_iGridSize;
						shape.vSize.Y = m_iGridSize;
						shape.vCentrePos.X = (startTile.X * m_iGridSize) + (shape.vSize.X / 2);
						shape.vCentrePos.Y = (startTile.Y * m_iGridSize) + (shape.vSize.Y / 2);
						shapes.push_back(shape);

						// Begin new shape 
						startTile.X = i;
						startTile.Y = j;
					}
					endTile.X = i;
					endTile.Y = j;
				}
			} else {
				// Not in solid shape 
				if (bInShape) {
					// Was previously in shape, save last shape 
					SMapShape shape;
					shape.eShapeType = SMapTile::TYPE_SOLID;
					shape.vSize.X = ((endTile.X - startTile.X)+1) * m_iGridSize;
					shape.vSize.Y = m_iGridSize;
					shape.vCentrePos.X = (startTile.X * m_iGridSize) + (shape.vSize.X / 2);
					shape.vCentrePos.Y = (startTile.Y * m_iGridSize) + (shape.vSize.Y / 2);
					shapes.push_back(shape);

					bInShape = false;
				}
			}
		}
	}

	/* Ladder tiles */
	for (int j = 0; j < m_vTileSize.Y; j++) {
		for (int i = 0; i < m_vTileSize.X; i++) {
			if (m_Tiles[j*m_vTileSize.X + i].tileType == SMapTile::TYPE_LADDER) {
				if (!bInShape) {
					// Entered solid shape 
					startTile.X = i;
					startTile.Y = j;
					endTile = startTile;

					bInShape = true;
				}
				else {
					// Still in solid shape
					if (startTile.Y != j) {
						// Have moved to a different Y level, save last shape
						SMapShape shape;
						shape.eShapeType = SMapTile::TYPE_LADDER;
						shape.vSize.X = (endTile.X - startTile.X) * m_iGridSize;
						shape.vSize.Y = m_iGridSize;
						shape.vCentrePos.X = (startTile.X * m_iGridSize) + (shape.vSize.X / 2);
						shape.vCentrePos.Y = (startTile.Y * m_iGridSize) + (shape.vSize.Y / 2);
						shapes.push_back(shape);

						// Begin new shape 
						startTile.X = i;
						startTile.Y = j;
					}
					endTile.X = i;
					endTile.Y = j;
				}
			}
			else {
				// Not in solid shape 
				if (bInShape) {
					// Was previously in shape, save last shape 
					SMapShape shape;
					shape.eShapeType = SMapTile::TYPE_LADDER;
					shape.vSize.X = ((endTile.X - startTile.X) + 1) * m_iGridSize;
					shape.vSize.Y = m_iGridSize;
					shape.vCentrePos.X = (startTile.X * m_iGridSize) + (shape.vSize.X / 2);
					shape.vCentrePos.Y = (startTile.Y * m_iGridSize) + (shape.vSize.Y / 2);
					shapes.push_back(shape);

					bInShape = false;
				}
			}
		}
	}

	return true;
}

void C6SMap::DrawBackground(CGame* pGame, Vector2f playerPosition) {
	if (!m_bInit) 
		return;
	/* Static Background */
	// BUG Why is this upside-down? 
	pGame->m_Drawing.DrawSpriteCentred(&pGame->m_WindowManager, pGame->m_WindowManager.GetScreenCentre(), m_vSize.X, -1*m_vSize.Y, &m_StaticBackground);

	/* Dynamic Backgrounds */
	for (unsigned int i = 0; i < m_DynamicBackgrounds.size(); i++) {
		Vector2f drawDelta = Vector2f(m_DynamicBackgrounds[i].moveScale * ((m_vSize.X / 2.0f) - playerPosition.X), 
			m_DynamicBackgrounds[i].moveScale * ((m_vSize.Y / 2.0f) - playerPosition.Y));

		pGame->m_Drawing.DrawSprite(&pGame->m_WindowManager, (int)drawDelta.X,(int)drawDelta.Y, 
			m_vSize.X, m_vSize.Y, &m_DynamicBackgrounds[i].texture);
	}
}

void C6SMap::DrawLevel(CGame* pGame) {
	if (!m_bInit)
		return;
	if (m_bUsingMapTexture) {
		pGame->m_Drawing.DrawSprite(pGame->GetWindowMgr(), 0, 0,
			m_vSize.X, m_vSize.Y, &m_MapTexture);
	} 

	if (m_bUsingMapTiles) {
		for (int j = 0; j < m_vTileSize.Y; j++) {
			for (int i = 0; i < m_vTileSize.X; i++) {
				if (m_Tiles[j*m_vTileSize.X + i].pBackground) {
					pGame->m_Drawing.DrawSprite(pGame->GetWindowMgr(), i*m_iGridSize, j*m_iGridSize,
						m_iGridSize, m_iGridSize, m_Tiles[j*m_vTileSize.X + i].pBackground);
				}
			}
		}
	}
}

void C6SMap::RenderBoundingBox(CGame* pGame) {
	pGame->m_Drawing.DrawRectangleOutline(&pGame->m_WindowManager, Vector2i(0, 0), m_vSize, 255, 0, 0, 255);
}
void C6SMap::RenderGrid(CGame* pGame) {
	for (int i = 0; i < m_vSize.X; i += m_iGridSize) {
		pGame->m_Drawing.DrawLine(&pGame->m_WindowManager, Vector2i(i, 0), Vector2i(i, m_vSize.Y), 255, 0, 0, 255);
	}
	for (int i = 0; i < m_vSize.Y; i += m_iGridSize) {
		pGame->m_Drawing.DrawLine(&pGame->m_WindowManager, Vector2i(0, i), Vector2i(m_vSize.X, i), 255, 0, 0, 255);
	}
}
void C6SMap::RenderTileType(CGame* pGame) {
	for (int j = 0; j < m_vTileSize.Y; j++) {
		for (int i = 0; i < m_vTileSize.X; i++) {
			m_Tiles[j*m_vTileSize.X + i].RenderType(pGame, Vector2i(i*m_iGridSize, j*m_iGridSize), m_iGridSize);
		}
	}
}

bool C6SMap::LoadStaticBackground(char* szBackground) {
	m_bBackgroundLoaded = m_StaticBackground.loadFromFile(szBackground);
	return m_bBackgroundLoaded;
}

bool C6SMap::LoadDynamicBackground(char* szBackground, int index, float movescale) {
	for (unsigned int i = 0; i < m_DynamicBackgrounds.size(); i++) {
		if (m_DynamicBackgrounds[i].id == index) {
			/* Background already exists, update */
			m_DynamicBackgrounds[i].moveScale = movescale;
			return m_DynamicBackgrounds[i].texture.loadFromFile(szBackground);
		}
	}
	SDynamicBackground newDynamic;
	newDynamic.id = index;
	newDynamic.moveScale = movescale;
	if (!newDynamic.texture.loadFromFile(szBackground))
		return false;
	m_DynamicBackgrounds.push_back(newDynamic);
	return true;
}

bool C6SMap::FloorCollision(Vector2i pos) {
	if (!m_bInit)
		return true;
	if (pos.Y <= 0 || pos.Y >= m_vSize.Y)
		return true;

	Vector2i tile(pos.X / m_iGridSize, pos.Y / m_iGridSize);
	if (m_Tiles[tile.Y*m_vTileSize.X + tile.X].IsSolid())
		return true;
	// Todo: map items etc
	return false;
}

bool C6SMap::WallCollisionLeft(Vector2i pos) {
	if (!m_bInit)
		return true;
	if (pos.X <= 0)
		return true;
	// Todo: map items etc 
	return false;
}

bool C6SMap::WallCollisionRight(Vector2i pos) {
	if (!m_bInit)
		return true;
	if (pos.Y >= m_vSize.X)
		return true;
	// Todo: map items etc
	return false;
}

bool C6SMap::Collision(Vector2i pos) {
	if (!m_bInit)
		return true;
	if (pos.X <= 0 || pos.Y <= 0 || pos.X >= m_vSize.X || pos.Y >= m_vSize.Y)
		return true;
	// Todo: map items etc
	return false;
}

C6SMap::C6SMap() {
	m_bUsingMapTiles = false;
}

bool C6SMap::ParseXML(rapidxml::xml_document<>& xml) {
	// Get 6SMap Node 
	rapidxml::xml_node<>* curNode = xml.first_node("6SMap");
	if (!curNode) {
		CDebugLogger::LogError("Unable to load map - Missing 6SMap node.");
		return false;
	}

	// Read version from 6SMap Node
	rapidxml::xml_attribute<>* curAttribute = curNode->first_attribute("version");
	if (!curAttribute || !curAttribute->value()) {
		CDebugLogger::LogWarning("Missing version attribute or value in map, continuing anyway.");
	}
	else {
		float version = strtof(curAttribute->value(), NULL);
		if (version != MAP_6S_VERSION) {
			printf("Warning - Map was made with a different version of the game, and may not load correctly (Map version: %f, Game version: %f)", version, MAP_6S_VERSION);
		}
	}

	// Read type from 6SMap Node
	curAttribute = curNode->first_attribute("type");
	if (!curAttribute || curAttribute->value() == NULL) {
		puts("Warning - Missing type attribute or value in map, continuing anyway.");
		m_MapType = EMapType::TYPE_PLAY;
	}
	else {
		// Find ID for map type string 
		for (int i = 0; i <= EMapType::TYPE_COUNT; i++) {

			if (i >= EMapType::TYPE_COUNT) {
				// Case: No match found 
				printf("Warning - missing or invalid map type, assuming default and continuing anyway.");
				m_MapType = EMapType::TYPE_PLAY;
			}
			else if (_strcmpi(szMapTypeStrings[i], curAttribute->value())) {
				// Case: Match found, copy ID 
				m_MapType = (EMapType)i;
				break;
			}
		}
	}

	if (m_MapType == EMapType::TYPE_DEVELOP) {
		// Find tileset
		// TODO
	}

	// Map image section
	curNode = xml.first_node("MapImage");
	if (!curNode) {
		CDebugLogger::LogError("Unable to load map - Missing MapImage section");
		return false;
	}

	curAttribute = curNode->first_attribute();
	m_vSize.X = m_vSize.Y = -1;
	while (curAttribute) {
		if (strcmp(curAttribute->name(), "src") == 0) {
			// Map tiles as image 
			if (!m_MapTexture.loadFromFile(curAttribute->value())) {
				CDebugLogger::LogError("Unable to load map - Can't load tiles image '%s'", curAttribute->value());
				return false;
			}
			m_bUsingMapTexture = true;
		}
		else if (strcmp(curAttribute->name(), "width") == 0) {
			// Map width in pixels
			int _x = strtol(curAttribute->value(), NULL, 10);
			m_vSize.X = _x;
		}
		else if (strcmp(curAttribute->name(), "height") == 0) {
			// Map height in pixels
			int _y = strtol(curAttribute->value(), NULL, 10);
			m_vSize.Y = _y;
		}

		curAttribute = curAttribute->next_attribute();
	}
	if (m_vSize.X == -1 || m_vSize.Y == -1) {
		printf("Unable to load map - MapImage section is missing width/height");
		return false;
	}

	// Map static background 
	curNode = xml.first_node("MapStaticBackground");
	if (!curNode) {
		printf("Unable to load map - Missing MapStaticBackground section");
		return false;
	}

	bool bLoadedBackground = false;
	curAttribute = curNode->first_attribute();
	while (curAttribute) {
		if (strcmp(curAttribute->name(), "src") == 0) {
			if (!m_StaticBackground.loadFromFile(curAttribute->value())) {
				printf("Unable to load map - Can't load static background '%s'", curAttribute->value());
				return false;
			}
			bLoadedBackground = true;
			break;
		}

		curAttribute = curAttribute->next_attribute();
	}
	if (!bLoadedBackground) {
		printf("Unable to load map - Missing map background ");
		return false;
	}

	// Map tile types (solid/air)
	curNode = xml.first_node("TileTypes");
	if (!curNode) {
		printf("Unable to load map - Missing TileTypes section");
		return false;
	}
	m_vTileSize.X = m_vTileSize.Y = m_iGridSize = -1;
	curAttribute = curNode->first_attribute();
	while (curAttribute) {
		if (strcmp(curAttribute->name(), "ArrayWidth") == 0) {
			int _x = strtol(curAttribute->value(), NULL, 10);
			m_vTileSize.X = _x;
		}
		else if (strcmp(curAttribute->name(), "ArrayHeight") == 0) {
			int _y = strtol(curAttribute->value(), NULL, 10);
			m_vTileSize.Y = _y;
		}
		else if (strcmp(curAttribute->name(), "GridSize") == 0) {
			int _size = strtol(curAttribute->value(), NULL, 10);
			m_iGridSize = _size;
		}

		curAttribute = curAttribute->next_attribute();
	}
	if (m_vTileSize.X <= 0 || m_vTileSize.Y <= 0 || m_iGridSize <= 0) {
		printf("Unable to load map - Missing gridsize/tilesize");
		return false;
	}

	// Init tiles to air
	m_Tiles = (SMapTile*)malloc(sizeof(SMapTile) * m_vTileSize.X * m_vTileSize.Y);
	for (int j = 0; j < m_vTileSize.Y; j++) {
		for (int i = 0; i < m_vTileSize.X; i++) {
			m_Tiles[j*m_vTileSize.X + i].pBackground = 0;
			m_Tiles[j*m_vTileSize.X + i].pForeground = 0;
			m_Tiles[j*m_vTileSize.X + i].iTileIndex = -1;
			m_Tiles[j*m_vTileSize.X + i].tileType = SMapTile::TYPE_AIR;
		}
	}

	// Load Tiles from XML
	rapidxml::xml_node<>* nodeTile = curNode->first_node("Tile");
	while (nodeTile) {
		int tileX = -1;
		int tileY = -1;
		int tileState = -1;
		for (rapidxml::xml_attribute<>* attr = nodeTile->first_attribute(); attr; attr = attr->next_attribute())
		{
			if (!attr)
				continue;
			if (strcmp(attr->name(), "XIndex") == 0) {
				tileX = strtol(attr->value(), NULL, 10);
			}
			else if (strcmp(attr->name(), "YIndex") == 0) {
				tileY = strtol(attr->value(), NULL, 10);
			}
			else if (strcmp(attr->name(), "State") == 0) {
				tileState = strtol(attr->value(), NULL, 10);
			}
		}
		if (tileX == -1 || tileY == -1 || tileState == -1) {
			printf("Unable to load map '%s' - Tile state missing data.");
			return false;
		}
		// Save type type
		m_Tiles[tileY*m_vTileSize.X + tileX].tileType = tileState;

		nodeTile = nodeTile->next_sibling();
	}


	m_bInit = true;
	return true;
}

bool C6SMap::InitializeFromFile(CGame* pGame, char* szMap) {
	// Free resources if already initialised
	if (m_bInit) Destroy();

	try {
		// Read file and parse as XML
		rapidxml::file<> xmlFile(szMap);
		rapidxml::xml_document<> docDef;
		docDef.parse<rapidxml::parse_full>(xmlFile.data());

		// Parse as map
		if (!ParseXML(docDef)) throw std::runtime_error("Failed to read map XML");
		return true;

	} catch (const std::runtime_error& e) {
		CDebugLogger::LogError("Failed to parse XML File. Reason: %s\n", e.what());
	}

	return false;
}
bool C6SMap::ExportToFile(CGame* pGame, char* szOutput, bool bAsDevMap) {
	float timeStart = pGame->m_Time.Time();
	printf("#\n#		Starting map generation\n#\n\n\tMap Details: ");

	if (bAsDevMap) {
		printf("  Map Type\tDevelop Map");
	} else {
		printf("  Map Type\tStandard");
	}
	printf("  Map Size\t%f, %f", m_vSize);

	if (!szOutput) {
		puts("Unable to export map - no output file given");
		return false;
	}

	/* Construct filename */
	std::string exportedMapFile = szOutput;
	exportedMapFile += GetMapFileExtension();

	/* Open file for writing */
	std::ofstream file(exportedMapFile);
	if (file.bad()) {
		printf("Unable to export map - unable to write to file '%s'", exportedMapFile.c_str());
		return false;
	}

	/* create map texture */
	sf::RenderTexture m_MapTexture;
	if (!m_MapTexture.create(m_vSize.X, m_vSize.Y)) {
		printf("Unable to export map - unable to generate texture of size (%i, %i)\n", m_vSize.X, m_vSize.Y);
		file.close();
		return false;
	}

	/* Set transparent  */
	m_MapTexture.clear(sf::Color(255, 255, 255, 0));

	/* Render map as texture */
	for (int j = 0; j < m_vTileSize.Y; j++) {
		for (int i = 0; i < m_vTileSize.X; i++) {
			if (m_Tiles[j*m_vTileSize.X + i].pBackground) {
				/* Texture */
				pGame->m_Drawing.DrawSpriteToTarget(&m_MapTexture, i*m_iGridSize, j*m_iGridSize,
					m_iGridSize, m_iGridSize, m_Tiles[j*m_vTileSize.X + i].pBackground);
			}
		}
	}

	/* Save texture to file */
	std::string mapTextureFile = szOutput; mapTextureFile += ".png";
	sf::Image mapImage = m_MapTexture.getTexture().copyToImage();
	mapImage.flipVertically();
	if (!mapImage.saveToFile(mapTextureFile)) {
		file.close();
		printf("Unable to export map - unable to save map texture file '%s'", mapTextureFile.c_str());
		return false;
	}

	/* Copy Background To map path */
	std::string mapBackgroundFile = szOutput; mapBackgroundFile += "background.png";
	if (m_bBackgroundLoaded) {
		sf::Image backgroundImage = m_StaticBackground.copyToImage();
		backgroundImage.flipVertically();
		if (!backgroundImage.saveToFile(mapBackgroundFile)) {
			file.close();
			printf("Unable to export map - unable to save map background file '%s'", mapBackgroundFile.c_str());
			return false;
		}
	}

	/* Export map as XML. */
	file.write("<?xml version=\"1.0\"?>\n", strlen("<?xml version=\"1.0\"?>\n"));
	file.write("<!-- 6S Map, Dev Build -->\n", strlen("<!-- 6S Map, Dev Build -->\n"));

	// Filetype and version 
	char mapImageBuf[2048];
	int len = sprintf_s(mapImageBuf, 2048, "<6SMap version=\"%2.3f\" />\n", MAP_6S_VERSION);
	if (len == -1) {
		file.close();
		printf("Unable to export map - error writing map version XML\n");
		return false;
	}
	file.write(mapImageBuf, len);

	
	// Image 
	len = sprintf_s(mapImageBuf, 2048, "<MapImage src=\"%s\" width=\"%i\" height=\"%i\" />\n", mapTextureFile.c_str(), m_vSize.X, m_vSize.Y);
	if (len == -1) {
		file.close();
		printf("Unable to export map - error writing map texture XML\n");
		return false;
	}
	file.write(mapImageBuf, len); 

	// Background & TODO: Dynamic background 
	if (m_bBackgroundLoaded) {
		len = sprintf_s(mapImageBuf, 2048, "<MapStaticBackground src=\"%s\" />\n", mapBackgroundFile.c_str());
		if (len == -1) {
			file.close();
			printf("Unable to export map - error writing map background XML\n");
			return false;
		}
		file.write(mapImageBuf, len);
	}


	// Tile States Start 
	len = sprintf_s(mapImageBuf, 2048, "<TileTypes ArrayWidth=\"%i\" ArrayHeight=\"%i\" GridSize=\"%i\" >\n", m_vTileSize.X, m_vTileSize.Y, m_iGridSize);
	if (len == -1) {
		file.close();
		printf("Unable to export map - error writing tile types XML\n");
		return false;
	}
	file.write(mapImageBuf, len);
	// Each tile state 
	for (int j = 0; j < m_vTileSize.Y; j++) {
		for (int i = 0; i < m_vTileSize.X; i++) {
			if (m_Tiles[j*m_vTileSize.X + i].tileType != SMapTile::TYPE_AIR) {
				len = sprintf_s(mapImageBuf, 2048, "  <Tile XIndex=\"%i\" YIndex=\"%i\" State=\"%i\" />\n", i, j, m_Tiles[j*m_vTileSize.X + i].tileType);
				if (len == -1) {
					file.close();
					printf("Unable to export map - error tile types XML\n");
					return false;
				}
				file.write(mapImageBuf, len);
			}
		}
	}
	// Tile States End 
	file.write("</TileTypes>\n", strlen("</TileTypes>\n"));

	file.close();
	printf("#		Map generation complete, took %f seconds.\n", pGame->m_Time.Time() - timeStart);
	return true;
}

void C6SMap::InitializeDev(char* background, int width, int height, int gridsize) {
	if (m_bInit) {
		// If initialised, free old resources
		Destroy();
	}

	m_vSize = Vector2i(width, height);
	m_StaticBackground.loadFromFile(background);
	m_bInit = true;
	m_iGridSize = gridsize;
	
	assert(gridsize != 0 && "Gridsize supplied cannot be 0");

	int xTiles = width / gridsize;
	int yTiles = height / gridsize;
	m_vTileSize = Vector2i(xTiles, yTiles);

	m_Tiles = (SMapTile*)malloc(sizeof(SMapTile) * xTiles * yTiles);

	for (int j = 0; j < yTiles; j++) {
		for (int i = 0; i < xTiles; i++) {
			m_Tiles[j*xTiles + i].pBackground = 0;
			m_Tiles[j*xTiles + i].pForeground = 0;
			m_Tiles[j*xTiles + i].iTileIndex = -1;
			m_Tiles[j*xTiles + i].tileType = SMapTile::TYPE_AIR;
		}
	}

	m_bUsingMapTiles = true;
}

void C6SMap::Destroy() {
	m_bInit = false;
	m_bUsingMapTiles = false;

	free(m_Tiles);
}

void C6SMap::Resize(int width, int height, int gridsize) {
	m_vSize = Vector2i(width, height);
	m_bInit = true;
	m_iGridSize = gridsize;

	assert(gridsize != 0 && "Gridsize supplied cannot be 0");

	int xTiles = width / gridsize;
	int yTiles = height / gridsize;
	m_vTileSize = Vector2i(xTiles, yTiles);

	/* Change size of allocated memory */
	m_Tiles = (SMapTile*)realloc(m_Tiles, sizeof(SMapTile) * xTiles * yTiles);

	/* Init memory */
	for (int j = 0; j < yTiles; j++) {
		for (int i = 0; i < xTiles; i++) {
			m_Tiles[j*xTiles + i].pBackground = 0;
			m_Tiles[j*xTiles + i].pForeground = 0;
			m_Tiles[j*xTiles + i].iTileIndex = -1;
			m_Tiles[j*xTiles + i].tileType = SMapTile::TYPE_AIR;
		}
	}

	m_bUsingMapTiles = true;
}

void C6SMap::SetTileType(Vector2i worldPos, SMapTile::ETileType type) {
	int xTile = worldPos.X / m_iGridSize;
	int yTile = worldPos.Y / m_iGridSize;
	if (xTile < 0 || yTile < 0 || xTile >= m_vTileSize.X || yTile >= m_vTileSize.Y)
		return;
	m_Tiles[yTile * m_vTileSize.X + xTile].tileType = type;
}
void C6SMap::SetTileTexture(Vector2i worldPos, sf::Texture* pTexture, int index) {
	int xTile = worldPos.X / m_iGridSize;
	int yTile = worldPos.Y / m_iGridSize;
	if (xTile < 0 || yTile < 0 || xTile >= m_vTileSize.X || yTile >= m_vTileSize.Y)
		return;
	m_Tiles[yTile * m_vTileSize.X + xTile].pBackground = pTexture;
	m_Tiles[yTile * m_vTileSize.X + xTile].iTileIndex = index;
}
void C6SMap::DeleteTileTexture(Vector2i worldPos) {
	int xTile = worldPos.X / m_iGridSize;
	int yTile = worldPos.Y / m_iGridSize;
	if (xTile < 0 || yTile < 0 || xTile >= m_vTileSize.X || yTile >= m_vTileSize.Y)
		return;
	m_Tiles[yTile * m_vTileSize.X + xTile].pBackground = 0;
	m_Tiles[yTile * m_vTileSize.X + xTile].iTileIndex = -1;
}

Vector2i C6SMap::GetSpawn(CGame* pGame) {
	// HACK todo
	return Vector2i(pGame->m_Random.RandomInt(300, 700), 200);
}

void C6SMap::ResetSpawns() {
	// Todo
}

void C6SMap::Reset() {
	// Todo
}