#ifndef __CANIMATEDSPRITE_H__
#define __CANIMATEDSPRITE_H__

/* Handler for Darkfunction exported sprites 
http://darkfunction.com/editor/documentation
*/
#include <SFML\Graphics.hpp>
#include "Vector.h"
#include "CGame.h"
#include <rapidxml.hpp>

class CAnimatedSprite {
	/* Time for each frame in an animation */
	const float ANIMATION_FRAME_TIME = 0.02f;

	class CSpriteImg {

	public:
		std::string m_sSpriteName;
		Vector2i m_vPosition;
		Vector2i m_vSize;
	};

	class CAnimSprite {
	
	public:
		CSpriteImg* m_pSpriteImg;
		std::string m_sSpriteName;
		Vector2i m_vPosition;
		int m_iZ;
		CAnimSprite() {
			m_pSpriteImg = 0;
		}
	};

	/* Single frame in animation */
	class CAnimationFrame {
	
	public:
		std::vector<CAnimSprite> m_Images;
		int m_iDelayMult;
		float m_fStartTime = 0.0f; // Start time in animation timescale
		float m_fEndTime = 0.0f; // End time in animation timescale
		void Draw(CGame* pGame, Vector2i pos, bool bMirror, sf::Texture* spriteSheet, std::vector<CSpriteImg>& sprites);
	};

	class CAnimation {
	
	public:
		std::vector<CAnimationFrame> m_Frames;
		std::string m_sAnimationName;

		bool m_bStaticAnimation; // If true, 'animation' is just a single image
		float m_fTime; // Current animation time, range [0, maxTime]
		float m_fMaxTime; // Total time of each frame added up, end time for animation
		int m_iCurFrame = 0; // Current frame index

		void Draw(CGame* pGame, Vector2i pos, sf::Texture* spriteSheet, bool bMirror, std::vector<CSpriteImg>& sprites);
		void Update(float fDelta, bool bMirror);

		// Converts loaded frame data into playable animation
		void Finalise(float fFrameTime);
	};

	std::vector<CAnimation> m_Animations;
	std::vector<CSpriteImg> m_Sprites;
	sf::Texture m_SpriteSheet;
	CAnimation* m_pCurAnimation = NULL;
	bool m_bValid = false;

	/* Init sprite from XML */
	bool ParseAnimationListXML(rapidxml::xml_document<>& xml);
	bool ParseSubspriteXML(rapidxml::xml_document<>& xml);

public:

	/* Load an exported sprite file, e.g. Supply 'player' to load player.png, player.anim,  */
	bool Load(char* szSpriteName);
	
	/* Set current sprite animation */
	void SetAnimation(char* szAnimation);

	/* Update animation frame */
	void Update(float fTime, bool bMirror);

	/* Draw animated sprite */
	void Draw(CGame* pGame, Vector2i pos, bool bMirror);
};
#endif