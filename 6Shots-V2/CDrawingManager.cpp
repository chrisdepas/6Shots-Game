#include "stdafx.h"
#include "CDrawingManager.h"
#include "ASSETS.h"


void CDrawingManager::DrawBox2DCircle(CWindowManager* pWindowManager, const b2Vec2& center, float32 radius, const b2Color& color, float fScalingFactor) {
	sf::CircleShape circle;

	circle.setPosition(sf::Vector2f(center.x, center.y));
	circle.setRadius(radius);

	/* Convert Box2D color to SFML color */
	sf::Color sfmlColor((char)(255 * color.r), (char)(255 * color.g), 
		(char)(255 * color.b), (char)(255 * color.a));

	pWindowManager->Draw(&circle);
}

void CDrawingManager::DrawBox2DPolygon(CWindowManager* pWindowManager, const b2Vec2* vertices, int32 vertexCount, const b2Color& color, float fScalingFactor) {
	sf::ConvexShape polygon;
	polygon.setPointCount(vertexCount);
	 
	for (int i = 0; i < vertexCount; i++) {
		polygon.setPoint(i, sf::Vector2f(vertices[i].x * fScalingFactor, vertices[i].y * fScalingFactor));
	}

	/* Convert Box2D color to SFML color */
	sf::Color sfmlColor((char)(255 * color.r), (char)(255 * color.g),
		(char)(255 * color.b), (char)(255 * color.a));

	polygon.setFillColor(sf::Color(0));
	polygon.setOutlineColor(sfmlColor);
	polygon.setOutlineThickness(1);
	polygon.setPosition(0, 0);

	pWindowManager->Draw(&polygon);
}

void CDrawingManager::Initialise()
{
	m_fFont.loadFromFile(ASSET_DEFAULT_FONT);
}

void CDrawingManager::DrawSprite(CWindowManager* pWindowManager, int X, int Y, int iWidth, int iHeight, sf::Texture* pTexture)
{
	if (!pTexture)
		return;

	/* Create a renderable sprite */
	sf::Sprite sp;
	sp.setTexture(*pTexture);
	sp.setPosition(sf::Vector2f((float)X, (float)Y));

	/* Scale to required dimensions */
	sf::Vector2u f = pTexture->getSize();
	sf::Vector2f scale((float)iWidth / f.x, (float)iHeight / f.y);
	sp.setScale(scale);

	/* Draw to window*/
	pWindowManager->Draw(&sp);
}
void CDrawingManager::DrawSpriteSheetSpriteCentred(CWindowManager* pWindowManager, int X, int Y, bool bMirror, Vector2i sheetPos, Vector2i sheetSize, Vector2i drawSize, sf::Texture* pTexture) {
	if (!pTexture)
		return;

	/* Create a renderable sprite */
	sf::Sprite sp;
	sp.setTexture(*pTexture);
	sp.setTextureRect(sf::IntRect(sheetPos.X, sheetPos.Y, sheetSize.X, sheetSize.Y));
	sp.setPosition(sf::Vector2f((float)X, (float)Y));

	sp.setOrigin(drawSize.X / 2.0f, drawSize.Y / 2.0f);

	if (bMirror)
		sp.setScale(-1.0f, 1.0f);
	/* Scale to required dimensions */
	//sf::Vector2u f = pTexture->getSize();
	//sf::Vector2f scale((float)drawSize.X / f.x, (float)drawSize.Y / f.y);
	//sp.setScale(scale);

	/* Draw to window*/
	pWindowManager->Draw(&sp);
}
void CDrawingManager::DrawRectangleOutline(CWindowManager* pWindowManager, Vector2i v1, Vector2i v2, unsigned __int8 R, unsigned __int8 G,
	unsigned __int8 B, unsigned __int8 A) {

	sf::Vertex rectangle[] =
	{
		sf::Vertex(sf::Vector2f((float)v1.X, (float)v1.Y), sf::Color(R, G, B, A)),		// (X1, Y1) to (X1, Y2)
		sf::Vertex(sf::Vector2f((float)v1.X, (float)v2.Y), sf::Color(R, G, B, A)),

		sf::Vertex(sf::Vector2f((float)v1.X, (float)v1.Y), sf::Color(R, G, B, A)),		// (X1, Y1) to (X2, Y1)
		sf::Vertex(sf::Vector2f((float)v2.X, (float)v1.Y), sf::Color(R, G, B, A)),

		sf::Vertex(sf::Vector2f((float)v2.X, (float)v1.Y), sf::Color(R, G, B, A)),		// (X2, Y1) to (X2, Y2)
		sf::Vertex(sf::Vector2f((float)v2.X, (float)v2.Y), sf::Color(R, G, B, A)),

		sf::Vertex(sf::Vector2f((float)v1.X, (float)v2.Y), sf::Color(R, G, B, A)),		// (X1, Y2) to (X2, Y2)
		sf::Vertex(sf::Vector2f((float)v2.X, (float)v2.Y), sf::Color(R, G, B, A))
	};
	pWindowManager->DrawVertices(rectangle, 8, sf::Lines);

}
int CDrawingManager::GetTextWidth(char* szText, int iSize)
{
	sf::Text text;
	text.setFont(m_fFont);
	text.setString(szText);
	text.setCharacterSize(iSize);
	return (int)text.getGlobalBounds().width;
}
void CDrawingManager::DrawText(CWindowManager* pWindowManager, char* szText, int X, int Y, int iSize, unsigned __int8 R, unsigned __int8 G,
	unsigned __int8 B, unsigned __int8 A)
{
	sf::Text text;
	text.setFont(m_fFont);
	text.setString(szText);
	text.setCharacterSize(iSize);
	text.setColor(sf::Color(R, G, B, A));
	text.setPosition((float)X, (float)Y);
	pWindowManager->Draw(&text);
}
void CDrawingManager::DrawTextCentred(CWindowManager* pWindowManager, char* szText, int X, int Y, int iSize, unsigned __int8 R,
	unsigned __int8 G, unsigned __int8 B, unsigned __int8 A)
{
	sf::Text text;
	text.setFont(m_fFont);
	text.setString(szText);
	text.setCharacterSize(iSize);

	/* Centre the text */
	sf::FloatRect bound = text.getGlobalBounds();
	text.setOrigin(bound.left + bound.width / 2.0f, bound.top + bound.height / 2.0f);

	text.setColor(sf::Color(R, G, B, A));
	text.setPosition((float)X, (float)Y);
	pWindowManager->Draw(&text);
}
void CDrawingManager::DrawTextCentredX(CWindowManager* pWindowManager, char* szText, int X, int Y, int iSize, unsigned __int8 R,
	unsigned __int8 G, unsigned __int8 B, unsigned __int8 A)
{
	sf::Text text;
	text.setFont(m_fFont);
	text.setString(szText);
	text.setCharacterSize(iSize);

	/* Centre the text */
	sf::FloatRect bound = text.getGlobalBounds();
	text.setOrigin(bound.left + bound.width / 2.0f, 0);

	text.setColor(sf::Color(R, G, B, A));
	text.setPosition((float)X, (float)Y);
	pWindowManager->Draw(&text);
}
void CDrawingManager::DrawSpriteCentred(CWindowManager* pWindowManager, int X, int Y, int iWidth, int iHeight, float fAlpha, sf::Texture* pTexture)
{
	if (!pTexture)
		return;

	/* Create a renderable sprite */
	sf::Sprite sp;
	sp.setTexture(*pTexture);
	sp.setPosition(sf::Vector2f((float)X, (float)Y));

	sp.setOrigin(pTexture->getSize().x / 2.0f, pTexture->getSize().y / 2.0f);

	/* Calculate integer alpha and set */
	int Alpha = (int)(255 * fAlpha);
	sp.setColor(sf::Color(255, 255, 255, Alpha));

	/* Scale to required dimensions */
	sf::Vector2u f = pTexture->getSize();
	sf::Vector2f scale((float)iWidth / f.x, (float)iHeight / f.y);
	sp.setScale(scale);

	/* Draw to window*/
	pWindowManager->Draw(&sp);
}
void CDrawingManager::DrawSpriteCentred(CWindowManager* pWindowManager, int X, int Y, int iWidth, int iHeight, float fAlpha, float fRotation, sf::Texture* pTexture)
{
	if (!pTexture)
		return;

	/* Create a renderable sprite */
	sf::Sprite sp;
	sp.setTexture(*pTexture);
	sp.setPosition(sf::Vector2f((float)X, (float)Y));

	sp.setOrigin(pTexture->getSize().x / 2.0f, pTexture->getSize().y / 2.0f);
	sp.rotate(fRotation);

	/* Calculate integer alpha and set */
	int Alpha = (int)(255 * fAlpha);
	sp.setColor(sf::Color(255, 255, 255, Alpha));

	/* Scale to required dimensions */
	sf::Vector2u f = pTexture->getSize();
	sf::Vector2f scale((float)iWidth / f.x, (float)iHeight / f.y);
	sp.setScale(scale);

	/* Draw to window*/
	pWindowManager->Draw(&sp);
}
void CDrawingManager::DrawSpriteCentred(CWindowManager* pWindowManager, int X, int Y, int iWidth, int iHeight, sf::Texture* pTexture)
{
	if (!pTexture)
		return;

	/* Create a renderable sprite */
	sf::Sprite sp;
	sp.setTexture(*pTexture);
	sp.setPosition(sf::Vector2f((float)X, (float)Y));

	sp.setOrigin(pTexture->getSize().x / 2.0f, pTexture->getSize().y / 2.0f); sp.setOrigin(pTexture->getSize().x / 2.0f, pTexture->getSize().y / 2.0f);

	/* Scale to required dimensions */
	sf::Vector2u f = pTexture->getSize();
	sf::Vector2f scale((float)iWidth / f.x, (float)iHeight / f.y);
	sp.setScale(scale);

	/* Draw to window*/
	pWindowManager->Draw(&sp);
}
void CDrawingManager::DrawSpriteCentred(CWindowManager* pWindowManager, Vector2i vPosition, int iWidth, int iHeight, sf::Texture* pTexture)
{
	if (!pTexture)
		return;

	/* Create a renderable sprite */
	sf::Sprite sp;
	sp.setTexture(*pTexture);
	sp.setPosition((float)vPosition.X, (float)vPosition.Y);

	sp.setOrigin(pTexture->getSize().x / 2.0f, pTexture->getSize().y / 2.0f);

	/* Scale to required dimensions */
	sf::Vector2u f = pTexture->getSize();
	sf::Vector2f scale((float)iWidth / f.x, (float)iHeight / f.y);
	sp.setScale(scale);

	/* Draw to window*/
	pWindowManager->Draw(&sp);
}

void CDrawingManager::DrawSquareCentred(CWindowManager* pWindowManager, Vector2i vPosition, int iSize, unsigned __int8 R, unsigned __int8 G,
	unsigned __int8 B, unsigned __int8 A)
{
	sf::RectangleShape sq(sf::Vector2f((float)iSize, (float)iSize));

	sq.setOrigin(iSize / 2.0f, iSize / 2.0f);
	sq.setPosition((float)vPosition.X, (float)vPosition.Y);
	sq.setFillColor(sf::Color(R, G, B, A));
	sq.setOutlineThickness(0.0f);

	pWindowManager->Draw(&sq);
}
void CDrawingManager::DrawRectangleCentred(CWindowManager* pWindowManager, Vector2i vPosition, int iWidth, int iHeight, unsigned __int8 R, unsigned __int8 G,
	unsigned __int8 B, unsigned __int8 A)
{
	sf::RectangleShape sq(sf::Vector2f((float)iWidth, (float)iHeight));

	sq.setOrigin(iWidth / 2.0f, iHeight / 2.0f);
	sq.setPosition((float)vPosition.X, (float)vPosition.Y);
	sq.setFillColor(sf::Color(R, G, B, A));
	sq.setOutlineThickness(0.0f);

	pWindowManager->Draw(&sq);
}

void CDrawingManager::DrawOutlinedRectangleCentred(CWindowManager* pWindowManager, Vector2i vPosition, int iWidth, int iHeight, unsigned __int8 R, unsigned __int8 G,
	unsigned __int8 B, unsigned __int8 A)
{
	sf::RectangleShape sq(sf::Vector2f((float)iWidth, (float)iHeight));

	sq.setOrigin(iWidth / 2.0f, iHeight / 2.0f);
	sq.setPosition((float)vPosition.X, (float)vPosition.Y);
	sq.setFillColor(sf::Color(R, G, B, A));
	sq.setOutlineThickness(1.0f);

	pWindowManager->Draw(&sq);
}

void CDrawingManager::DrawLine(CWindowManager* pWindowManager, Vector2i vStart, Vector2i vEnd, unsigned __int8 R, unsigned __int8 G, unsigned __int8 B, unsigned __int8 A) {
	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f((float)vStart.X, (float)vStart.Y), sf::Color(R,G,B,A)),
		sf::Vertex(sf::Vector2f((float)vEnd.X, (float)vEnd.Y), sf::Color(R, G, B, A))
	};
	pWindowManager->DrawVertices(line, 2, sf::Lines);
}
void CDrawingManager::DrawCircleCentred(CWindowManager* pWindowManager, Vector2i vCentre, float radius, unsigned __int8 R, unsigned __int8 G, unsigned __int8 B, unsigned __int8 A) {
	sf::CircleShape c;
	c.setRadius((float)radius); 
	c.setFillColor(sf::Color(R, G, B, A));
	c.setPosition((float)vCentre.X, (float)vCentre.Y);
	c.setOrigin(radius / 2.0f, radius / 2.0f);
	pWindowManager->Draw(&c);
}
void CDrawingManager::DrawCircleCentred(CWindowManager* pWindowManager, Vector2f vCentre, float radius, unsigned __int8 R, unsigned __int8 G, unsigned __int8 B, unsigned __int8 A) {
	sf::CircleShape c;
	c.setRadius((float)radius);
	c.setFillColor(sf::Color(R, G, B, A));
	c.setPosition(vCentre.X, vCentre.Y);
	c.setOrigin(radius / 2.0f, radius / 2.0f);
	pWindowManager->Draw(&c);
}
void CDrawingManager::DrawSquareToTargetCentred(sf::RenderTarget* pTarget, Vector2i vPosition, int iSize, unsigned __int8 R, unsigned __int8 G,
	unsigned __int8 B, unsigned __int8 A)
{
	sf::RectangleShape sq(sf::Vector2f((float)iSize, (float)iSize));

	sq.setOrigin(iSize / 2.0f, iSize / 2.0f);
	sq.setPosition((float)vPosition.X, (float)vPosition.Y);
	sq.setFillColor(sf::Color(R, G, B, A));
	sq.setOutlineThickness(0.0f);

	pTarget->draw(sq);
}

void CDrawingManager::DrawSpriteToTargetCentred(sf::RenderTarget* pTarget, int X, int Y, int iWidth, int iHeight, sf::Texture* pTexture)
{
	if (!pTexture)
		return;

	/* Create a renderable sprite */
	sf::Sprite sp;
	sp.setTexture(*pTexture);
	sp.setPosition(sf::Vector2f((float)X, (float)Y));

	sp.setOrigin(pTexture->getSize().x / 2.0f, pTexture->getSize().y / 2.0f);

	/* Scale to required dimensions */
	sf::Vector2u f = pTexture->getSize();
	sf::Vector2f scale((float)iWidth / f.x, (float)iHeight / f.y);
	sp.setScale(scale);

	/* Draw to target*/
	pTarget->draw(sp);
}

void CDrawingManager::DrawSquareToTarget(sf::RenderTarget* pTarget, Vector2i vPosition, int iSize, unsigned __int8 R, unsigned __int8 G,
	unsigned __int8 B, unsigned __int8 A)
{
	sf::RectangleShape sq(sf::Vector2f((float)iSize, (float)iSize));

	sq.setPosition((float)vPosition.X, (float)vPosition.Y);
	sq.setFillColor(sf::Color(R, G, B, A));
	sq.setOutlineThickness(0.0f);

	pTarget->draw(sq);
}

void CDrawingManager::DrawSpriteToTarget(sf::RenderTarget* pTarget, int X, int Y, int iWidth, int iHeight, sf::Texture* pTexture)
{
	if (!pTexture)
		return;

	/* Create a renderable sprite */
	sf::Sprite sp;
	sp.setTexture(*pTexture);
	sp.setPosition(sf::Vector2f((float)X, (float)Y));

	/* Scale to required dimensions */
	sf::Vector2u f = pTexture->getSize();
	sf::Vector2f scale((float)iWidth / f.x, (float)iHeight / f.y);
	sp.setScale(scale);

	/* Draw to target*/
	pTarget->draw(sp);
}