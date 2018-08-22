#include "stdafx.h"
#include "CAnimatedSprite.h"
#include "CGame.h"
#include "DebugUtil.h"	
#include "Util.h"


void CAnimatedSprite::CAnimationFrame::Draw(CGame* pGame, sf::Shader& shader, sf::Vector2i pos, bool bMirror, sf::Texture* spriteSheet, std::vector<CSpriteImg>& sprites) {
	DbgAssert(m_Images.size() > 0 && m_Images.size() < 100);

	for (unsigned int i = 0; i < m_Images.size(); i++) {
		sf::Vector2f p(m_Images[i].m_vPosition);

		// Find image if not already found
		if (m_Images[i].m_pSpriteImg == 0) {
			const char* sImgName = m_Images[i].m_sSpriteName.c_str();
			for (unsigned int j = 0; j < sprites.size(); j++) {
				if (sprites[j].m_sSpriteName.compare(sImgName) == 0) {
					m_Images[i].m_pSpriteImg = &sprites[j];
				}
			}
		}
		CSpriteImg* img = m_Images[i].m_pSpriteImg;
		pGame->m_Drawing.DrawSpriteSheetSpriteCentred(pGame->GetWindowMgr(), sf::Vector2f(pos) + p, bMirror, img->m_vPosition, img->m_vSize, img->m_vSize, spriteSheet);
	}
}
void CAnimatedSprite::CAnimationFrame::Draw(CGame* pGame, sf::Vector2i pos, bool bMirror, sf::Texture* spriteSheet, std::vector<CSpriteImg>& sprites) {
	DbgAssert(m_Images.size() > 0 && m_Images.size() < 100);

	for (unsigned int i = 0; i < m_Images.size(); i++) {
		sf::Vector2f p(m_Images[i].m_vPosition);

		// Find image if not already found
		if (m_Images[i].m_pSpriteImg == 0) {
			const char* sImgName = m_Images[i].m_sSpriteName.c_str();
			for (unsigned int j = 0; j < sprites.size(); j++) { 
				if (sprites[j].m_sSpriteName.compare(sImgName) == 0) {
					m_Images[i].m_pSpriteImg = &sprites[j];
				} 
			}
		}
		CSpriteImg* img = m_Images[i].m_pSpriteImg;
		pGame->m_Drawing.DrawSpriteSheetSpriteCentred(pGame->GetWindowMgr(), sf::Vector2f(pos) + p, bMirror, img->m_vPosition, img->m_vSize, img->m_vSize, spriteSheet);
	}
}  

void CAnimatedSprite::CAnimation::Draw(CGame* pGame, sf::Shader& shader, sf::Vector2i pos, sf::Texture* spriteSheet, bool bMirror, std::vector<CSpriteImg>& sprites) {
	m_Frames[m_iCurFrame].Draw(pGame, pos, bMirror, spriteSheet, sprites);
}
void CAnimatedSprite::CAnimation::Draw(CGame* pGame, sf::Vector2i pos, sf::Texture* spriteSheet, bool bMirror, std::vector<CSpriteImg>& sprites) {
	m_Frames[m_iCurFrame].Draw(pGame, pos, bMirror, spriteSheet, sprites);
}

void CAnimatedSprite::CAnimation::Update(float fDelta, bool bMirror) {
	if (m_bStaticAnimation) { return; } 
	
	if (!bMirror) { 
		// Run in reverse if mirrored
		m_fTime -= fDelta;

		while (m_fTime < 0.0f) {
			m_fTime = m_fMaxTime + m_fTime;
		}
	} else {
		m_fTime += fDelta;

		while (m_fTime > m_fMaxTime) {
			m_fTime = m_fTime - m_fMaxTime;
		}
	}

	if (m_fTime > m_Frames[m_iCurFrame].m_fEndTime) {
		// Find next frame which hasn't ended
		for (unsigned int i = m_iCurFrame; i < m_Frames.size(); i++) {
			if (m_fTime <= m_Frames[i].m_fEndTime) {
				m_iCurFrame = i;
				break;
			}
		}
	}

	if (m_fTime < m_Frames[m_iCurFrame].m_fStartTime) {
		// Find previous frame which has started / should start
		for (int i = m_iCurFrame-1; i > 0; i--) {
			if (m_Frames[i].m_fStartTime <= m_fTime) {
				m_iCurFrame = i;
				break;
			}
		}
	}
}

void CAnimatedSprite::CAnimation::Finalise(float fFrameTime) {
	m_bStaticAnimation = (m_Frames.size() == 1);

	/* Calculate total time of animation & start/end for each frame */
	m_fMaxTime = 0.0f;
	for (unsigned int i = 0; i < m_Frames.size(); i++) {
		m_Frames[i].m_fStartTime = m_fMaxTime;

		m_fMaxTime += m_Frames[i].m_iDelayMult * fFrameTime;

		m_Frames[i].m_fEndTime = m_fMaxTime;
	}
	
	/* Default to first frame of animation at time 0*/
	m_iCurFrame = 0;
	m_fTime = 0.0f;
}

bool CAnimatedSprite::ParseAnimationListXML(rapidxml::xml_document<>& xml) {
	rapidxml::xml_node<>* node = xml.first_node("animations");
	if (!node) { throw std::runtime_error("Sprite is missing animations section in animation definition.\n"); }

	node = node->first_node("anim");
	if (!node) { throw std::runtime_error("Sprite has no animations, unable to parse as animated sprite.\n"); }

	rapidxml::xml_attribute<>* attr = NULL;
	rapidxml::xml_node<>* cell = NULL;
	rapidxml::xml_node<>* sprite = NULL;

	while (node) {
		CAnimation newAnimation;

		// Find animation name 
		attr = node->first_attribute("name");
		if (!attr) { throw std::runtime_error("Unable to parse sprite, an animation has no name\n"); }
		
		// Copy animation name
		newAnimation.m_sAnimationName = std::string(attr->value(), attr->value_size());

		// load each of animation frames 
		cell = node->first_node("cell");
		while (cell) {
			CAnimationFrame newFrame;

			// Load frame delay 
			attr = cell->first_attribute("delay");
			if (attr == NULL) {
				CDebugLogger::LogWarning("Sprite is missing a frame delay for animation %s, defaulting to 1\n", newAnimation.m_sAnimationName.c_str());
				newFrame.m_iDelayMult = 1;
			} else {
				int _delay = util::strtoint(attr->value(), attr->value_size());
				if (_delay < 0) {
					CDebugLogger::LogWarning("Sprite has an invalid a frame delay for animation %s, defaulting to 1\n", newAnimation.m_sAnimationName.c_str());
					newFrame.m_iDelayMult = 1;
				} else {
					newFrame.m_iDelayMult = _delay;
				}
			}

			// Load each subsprite position for this frame 
			sprite = cell->first_node("spr");
			while (sprite) {
				CAnimSprite newImg;
				for (attr = sprite->first_attribute(); attr; attr = attr->next_attribute()) {
					
					if (strcmp(attr->name(), "name") == 0) {
						// Read name, remove '/' prefix
						int value_size = attr->value_size();
						char* value = attr->value();
						if (value[0] == '/') {
							value_size--;
							value++;
						}
						newImg.m_sSpriteName = std::string(value, value_size);

					}
					else if (strcmp(attr->name(), "x") == 0) {
						// Read position X
						int _x = strtol(attr->value(), NULL, 10);
						newImg.m_vPosition.x = _x;

					}
					else if (strcmp(attr->name(), "y") == 0) {
						// Read position Y
						int _y = strtol(attr->value(), NULL, 10);
						newImg.m_vPosition.y = _y;

					}
					else if (strcmp(attr->name(), "z") == 0) {
						// Read Z index
						int _z = strtol(attr->value(), NULL, 10);
						newImg.m_iZ = _z;

					}
				}

				// Insert subsprite into this frame
				newFrame.m_Images.push_back(newImg);
				sprite = sprite->next_sibling();
			}

			// Add frame to animation
			newAnimation.m_Frames.push_back(newFrame);
			cell = cell->next_sibling();
		}

		// Convert to playable animation, insert, and load next
		newAnimation.Finalise(CAnimatedSprite::ANIMATION_FRAME_TIME);
		m_Animations.push_back(newAnimation);
		node = node->next_sibling("anim");
	}
	
	return true;
}

bool CAnimatedSprite::ParseSubspriteXML(rapidxml::xml_document<>& xml) {
	// Follow nodes down to sprite list
	rapidxml::xml_node<>* curNode = xml.first_node("img");
	if (!curNode) { throw std::runtime_error("[SPRITE] Sprite is missing img section in subsprite definition, unable to parse\n"); }

	curNode = curNode->first_node("definitions");
	if (!curNode) { throw std::runtime_error("[SPRITE] Sprite is missing definitions section in subsprite definition, unable to parse\n"); }

	curNode = curNode->first_node("dir");
	if (!curNode) { throw std::runtime_error("[SPRITE] Sprite missing dir section in subsprite definition, unable to parse\n"); }

	// Finally, at first node in list of sprites 
	curNode = curNode->first_node("spr");
	if (!curNode) { throw std::runtime_error("[SPRITE] Sprite is missing subsprite definition nodes, unable to parse\n"); }

	// Load all sub-sprites in main image 
	while (curNode) {
		CSpriteImg newSprite;
		newSprite.m_sSpriteName = "";
		for (rapidxml::xml_attribute<> *attr = curNode->first_attribute(); attr; attr = attr->next_attribute()) {
			
			if (strcmp(attr->name(), "name") == 0) {
				// Read name
				if (newSprite.m_sSpriteName.length() != 0) {
					CDebugLogger::LogWarning("[SPRITE] Sprite has multiple name sections, ignoring\n");
				} else {
					newSprite.m_sSpriteName = std::string(attr->value(), attr->value_size());  
				}

			}
			else if (strcmp(attr->name(), "x") == 0) {
				// Load sprite position X 
				int _x = strtol(attr->value(), NULL, 10);
				newSprite.m_vPosition.x = _x;

			}
			else if (strcmp(attr->name(), "y") == 0) {
				// Load sprite position Y
				int _y = strtol(attr->value(), NULL, 10);
				newSprite.m_vPosition.y = _y;

			}
			else if (strcmp(attr->name(), "w") == 0) {
				// Load sprite width 
				int _w = strtol(attr->value(), NULL, 10);
				newSprite.m_vSize.x = _w;

			}
			else if (strcmp(attr->name(), "h") == 0) {
				// Load sprite height 
				int _h = strtol(attr->value(), NULL, 10);
				newSprite.m_vSize.y = _h;

			}
		}

		// Add sprite & find next definition 
		m_Sprites.push_back(newSprite);
		curNode = curNode->next_sibling("spr");
	}

	return true;
}

bool CAnimatedSprite::Load(char* szSprite) {
	DbgAssert(szSprite != NULL);
	m_bValid = false;

	// Load the sprite sheet image -> the actual sheet of images
	std::string spriteSheetImg = szSprite;
	spriteSheetImg += ".png";
	if (!m_SpriteSheet.loadFromFile(spriteSheetImg.c_str())) {
		CDebugLogger::LogError("[SPRITE] Unable to load animated sprite %s", spriteSheetImg.c_str());
		return false;
	}

	try {
		// Read subsprite definition file
		std::string spriteSheetFile = szSprite; 
		spriteSheetFile += ".sprites";
		rapidxml::file<> xmlFileSprite(spriteSheetFile.c_str());

		// Parse XML
		rapidxml::xml_document<> docDef;
		docDef.parse<rapidxml::parse_full>(xmlFileSprite.data());

		// Parse XML as subsprite definition
		if (!ParseSubspriteXML(docDef)) throw std::runtime_error("Failed to read animated sprite subsprite XML");
		docDef.clear();

		// Read animation definition file 
		spriteSheetFile = szSprite;
		spriteSheetFile += ".anim";
		rapidxml::file<> xmlFileAnim(spriteSheetFile.c_str());
		
		// Parse XML as animation definition
		docDef.parse<rapidxml::parse_full>(xmlFileAnim.data());
		if (!ParseAnimationListXML(docDef)) throw std::runtime_error("Failed to read animated sprite animation list XML");

	} catch (const std::runtime_error& e) {
		CDebugLogger::LogError("Failed to parse animated sprite XML File. Reason: %s\n", e.what());
	}

	m_bValid = true;
	return true;
}

void CAnimatedSprite::Draw(CGame* pGame, sf::Vector2i pos, bool bMirror) {
	if (!m_bValid)
		return;
	if (m_pCurAnimation != 0)
		m_pCurAnimation->Draw(pGame, pos, &m_SpriteSheet, bMirror, m_Sprites);
}

void CAnimatedSprite::SetAnimation(char* szAnimation) {
	if (!m_bValid)
		return;
	for (unsigned int i = 0; i < m_Animations.size(); i++) {
		if (_strcmpi(m_Animations[i].m_sAnimationName.c_str(), szAnimation) == 0) {
			m_pCurAnimation = &m_Animations[i];
		}
	}
}
void CAnimatedSprite::Update(float fTime, bool bMirror) {
	if (!m_bValid)
		return;
	if (m_pCurAnimation != 0)
		m_pCurAnimation->Update(fTime, bMirror);
}