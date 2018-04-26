#ifndef __CDRAWINGMANAGER_H__
#define __CDRAWINGMANAGER_H__


#include "SFML/Graphics.hpp"
#include "CWindowManager.h"
#include "Box2D\Box2D.h"

class CDrawingManager
{
	sf::Font m_fFont;

public:
	CDrawingManager() {}
	~CDrawingManager() {}
	void Initialise();

	/* Box2D */
	void DrawBox2DPolygon(CWindowManager* pWindowManager, const b2Vec2* vertices, int32 vertexCount, const b2Color& color, float fScalingFactor);
	void DrawBox2DCircle(CWindowManager* pWindowManager, const b2Vec2& center, float32 radius, const b2Color& color, float fScalingFactor);

	/* Uncentred drawing -> Position supplied is top-left */
	void DrawSprite(CWindowManager* pWindowManager, int X, int Y, int iWidth, int iHeight, sf::Texture* pTexture);
	void DrawText(CWindowManager* pWindowManager, char* szText, int X, int Y, int iSize, unsigned __int8 R, unsigned __int8 G,
		unsigned __int8 B, unsigned __int8 A);
	int GetTextWidth(char* szText, int iSize);
	void DrawSpriteSheetSpriteCentred(CWindowManager* pWindowManager, int X, int Y, bool bMirror, Vector2i sheetPos, Vector2i sheetSize, Vector2i drawSize, sf::Texture* pTexture);
	void DrawRectangleOutline(CWindowManager* pWindowManager, Vector2i v1, Vector2i v2, unsigned __int8 R, unsigned __int8 G,
		unsigned __int8 B, unsigned __int8 A);
	 
	/* Centred drawing -> Position supplied is centre of where object is rendered */
	void DrawTextCentred(CWindowManager* pWindowManager, char* szText, int X, int Y, int iSize, unsigned __int8 R, unsigned __int8 G,
		unsigned __int8 B, unsigned __int8 A);
	void DrawTextCentredX(CWindowManager* pWindowManager, char* szText, int X, int Y, int iSize, unsigned __int8 R,
		unsigned __int8 G, unsigned __int8 B, unsigned __int8 A);
	void DrawSpriteCentred(CWindowManager* pWindowManager, int X, int Y, int iWidth, int iHeight, sf::Texture* pTexture);
	void DrawSpriteCentred(CWindowManager* pWindowManager, Vector2i vPosition, int iWidth, int iHeight, sf::Texture* pTexture);
	void DrawSpriteCentred(CWindowManager* pWindowManager, int X, int Y, int iWidth, int iHeight, float fAlpha, sf::Texture* pTexture);
	void DrawSpriteCentred(CWindowManager* pWindowManager, int X, int Y, int iWidth, int iHeight, float fAlpha, float fRotation, sf::Texture* pTexture);
	void DrawSquareCentred(CWindowManager* pWindowManager, Vector2i vPosition, int iSize, unsigned __int8 R, unsigned __int8 G,
		unsigned __int8 B, unsigned __int8 A);
	void DrawRectangleCentred(CWindowManager* pWindowManager, Vector2i vPosition, int iWidth, int iHeight, unsigned __int8 R, unsigned __int8 G,
		unsigned __int8 B, unsigned __int8 A);
	void DrawOutlinedRectangleCentred(CWindowManager* pWindowManager, Vector2i vPosition, int iWidth, int iHeight, unsigned __int8 R, unsigned __int8 G,
		unsigned __int8 B, unsigned __int8 A);
	void DrawLine(CWindowManager* pWindowManager, Vector2i vStart, Vector2i vEnd, unsigned __int8 R, unsigned __int8 G, unsigned __int8 B, unsigned __int8 A);
	void DrawCircleCentred(CWindowManager* pWindowManager, Vector2i vCentre, float radius, unsigned __int8 R, unsigned __int8 G, unsigned __int8 B, unsigned __int8 A);
	void DrawCircleCentred(CWindowManager* pWindowManager, Vector2f vCentre, float radius, unsigned __int8 R, unsigned __int8 G, unsigned __int8 B, unsigned __int8 A);

	// Draw to render target, instead of only window
	void DrawSpriteToTargetCentred(sf::RenderTarget* t, int X, int Y, int iWidth, int iHeight, sf::Texture* pTexture);
	void DrawSpriteToTargetCentred(sf::RenderTarget* t, int X, int Y, float fRotation, int iWidth, int iHeight, sf::Texture* pTexture);
	void DrawSquareToTargetCentred(sf::RenderTarget* t, Vector2i vPosition, int iSize, unsigned __int8 R, unsigned __int8 G,
		unsigned __int8 B, unsigned __int8 A);

	void DrawSquareToTarget(sf::RenderTarget* pTarget, Vector2i vPosition, int iSize, unsigned __int8 R, unsigned __int8 G,
		unsigned __int8 B, unsigned __int8 A);
	void DrawSpriteToTarget(sf::RenderTarget* pTarget, int X, int Y, int iWidth, int iHeight, sf::Texture* pTexture);
};

#endif