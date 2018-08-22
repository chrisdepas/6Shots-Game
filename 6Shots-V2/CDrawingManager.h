#ifndef __CDRAWINGMANAGER_H__
#define __CDRAWINGMANAGER_H__

#include "CWindowManager.h"

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
	void DrawSprite(CWindowManager* pWindowManager, int X, int Y, int size, sf::Texture* pTexture);
	void DrawSprite(CWindowManager* pWindowManager, int X, int Y, int iWidth, int iHeight, sf::Texture* pTexture);
	void DrawText(CWindowManager* pWindowManager, char* szText, int X, int Y, int iSize, unsigned __int8 R, unsigned __int8 G,
		unsigned __int8 B, unsigned __int8 A);
	int GetTextWidth(char* szText, int iSize);
	void DrawSpriteSheetSpriteCentred(CWindowManager* pWindowManager, sf::Vector2f position, bool bMirror, sf::Vector2i sheetPos, sf::Vector2i sheetSize, sf::Vector2i drawSize, sf::Texture* pTexture);
	void DrawRectangleOutline(CWindowManager* pWindowManager, sf::Vector2i v1, sf::Vector2i v2, sf::Color color);
	 
	/* Centred drawing -> Position supplied is centre of where object is rendered */
	void DrawTextCentred(CWindowManager* pWindowManager, char* szText, int X, int Y, int iSize, unsigned __int8 R,
		unsigned __int8 G, unsigned __int8 B, unsigned __int8 A);
	void DrawTextCentred(CWindowManager* pWindowManager, const std::string& szText, sf::Vector2f drawPos, int iSize, sf::Color color);
	void DrawTextCentredX(CWindowManager* pWindowManager, char* szText, int X, int Y, int iSize, unsigned __int8 R,
		unsigned __int8 G, unsigned __int8 B, unsigned __int8 A);
	void DrawTextCentredX(CWindowManager* pWindowManager, char* szText, sf::Vector2f position, int textSize, sf::Color color);
	void DrawSpriteCentred(CWindowManager* pWindowManager, sf::Vector2i vPosition, sf::Vector2i size, sf::Texture* pTexture);
	void DrawSpriteCentred(CWindowManager* pWindowManager, int X, int Y, int iWidth, int iHeight, sf::Texture* pTexture);
	void DrawSpriteCentred(CWindowManager* pWindowManager, sf::Vector2i vPosition, int iWidth, int iHeight, sf::Texture* pTexture);
	void DrawSpriteCentred(CWindowManager* pWindowManager, int X, int Y, int iWidth, int iHeight, float fAlpha, sf::Texture* pTexture);
	void DrawSpriteCentred(CWindowManager* pWindowManager, int X, int Y, int iWidth, int iHeight, float fAlpha, float fRotation, sf::Texture* pTexture);
	void DrawSquareCentred(CWindowManager* pWindowManager, sf::Vector2i vPosition, int iSize, unsigned __int8 R, unsigned __int8 G,
		unsigned __int8 B, unsigned __int8 A);
	void DrawRectangleCentred(CWindowManager* pWindowManager, sf::Vector2i vPosition, int iWidth, int iHeight, unsigned __int8 R, unsigned __int8 G,
		unsigned __int8 B, unsigned __int8 A);
	void DrawRectangleCentred(CWindowManager* pWindowManager, sf::Vector2f vPosition, sf::Vector2f size, sf::Color color);
	void DrawRectangle(CWindowManager* pWindowManager, sf::Vector2f vPosition, sf::Vector2f size, sf::Color color);
	void DrawOutlinedRectangleCentred(CWindowManager* pWindowManager, sf::Vector2i vPosition, int iWidth, int iHeight, unsigned __int8 R, unsigned __int8 G,
		unsigned __int8 B, unsigned __int8 A);
	void DrawLine(CWindowManager* pWindowManager, sf::Vector2i vStart, sf::Vector2i vEnd, unsigned __int8 R, unsigned __int8 G, unsigned __int8 B, unsigned __int8 A);
	void DrawLine(CWindowManager* pWindowManager, sf::Vector2f vStart, sf::Vector2f vEnd, sf::Color col);
	void DrawCircleCentred(CWindowManager* pWindowManager, sf::Vector2i vCentre, float radius, unsigned __int8 R, unsigned __int8 G, unsigned __int8 B, unsigned __int8 A);
	void DrawCircleCentred(CWindowManager* pWindowManager, sf::Vector2f vCentre, float radius, unsigned __int8 R, unsigned __int8 G, unsigned __int8 B, unsigned __int8 A);
	void DrawCircleCentred(CWindowManager* pWindowManager, sf::Vector2f vCentre, float radius, sf::Color color);
	void DrawSolidCircleCentred(CWindowManager* pWindowManager, sf::Vector2f vCentre, float radius, sf::Color color);

	// Draw to render target, instead of only window
	void DrawSpriteToTargetCentred(sf::RenderTarget* t, int X, int Y, int iWidth, int iHeight, sf::Texture* pTexture);
	void DrawSpriteToTargetCentred(sf::RenderTarget* t, int X, int Y, float fRotation, int iWidth, int iHeight, sf::Texture* pTexture);
	void DrawSquareToTargetCentred(sf::RenderTarget* t, sf::Vector2i vPosition, int iSize, unsigned __int8 R, unsigned __int8 G,
		unsigned __int8 B, unsigned __int8 A);

	void DrawSquareToTarget(sf::RenderTarget* pTarget, sf::Vector2i vPosition, int iSize, unsigned __int8 R, unsigned __int8 G,
		unsigned __int8 B, unsigned __int8 A);
	void DrawSpriteToTarget(sf::RenderTarget* pTarget, int X, int Y, int iWidth, int iHeight, sf::Texture* pTexture);
	static void DrawTileToTarget(sf::RenderTarget* pTarget, sf::Vector2f pos, int size, const sf::Texture* pTexture);
};

#endif