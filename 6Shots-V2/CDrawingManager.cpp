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

void CDrawingManager::DrawSprite(CWindowManager* pWindowManager, int X, int Y, int size, sf::Texture* pTexture) {
	sf::Sprite sp;
	sp.setTexture(*pTexture);
	sp.setPosition(sf::Vector2f((float)X, (float)Y));

	sf::Vector2u f = pTexture->getSize();
	sf::Vector2f scale((float)size / f.x, (float)size / f.y);
	sp.setScale(scale);

	pWindowManager->Draw(&sp);
}
void CDrawingManager::DrawSprite(CWindowManager* pWindowManager, int X, int Y, int iWidth, int iHeight, sf::Texture* pTexture) {
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
void CDrawingManager::DrawSpriteSheetSpriteCentred(CWindowManager* pWindowManager, sf::Vector2f position, bool bMirror, sf::Vector2i sheetPos, sf::Vector2i sheetSize, sf::Vector2i drawSize, sf::Texture* pTexture) {
	if (!pTexture)
		return;

	/* Create a renderable sprite */
	sf::Sprite sp;
	sp.setTexture(*pTexture);
	sp.setTextureRect(sf::IntRect(sheetPos, sheetSize));
	sp.setPosition(position);
	sp.setOrigin(sf::Vector2f(drawSize) / 2.0f);

	if (bMirror)
		sp.setScale(-1.0f, 1.0f);
	/* Scale to required dimensions */
	//sf::Vector2u f = pTexture->getSize();
	//sf::Vector2f scale((float)drawSize.X / f.x, (float)drawSize.Y / f.y);
	//sp.setScale(scale);

	/* Draw to window*/
	pWindowManager->Draw(&sp);
}
void CDrawingManager::DrawRectangleOutline(CWindowManager* pWindowManager, sf::Vector2i v1, sf::Vector2i v2, sf::Color color) {
	
	sf::Vertex rectangle[] =
	{
		sf::Vertex(sf::Vector2f((float)v1.x, (float)v1.y), color),		// (X1, Y1) to (X1, Y2)
		sf::Vertex(sf::Vector2f((float)v1.x, (float)v2.y), color),

		sf::Vertex(sf::Vector2f((float)v1.x, (float)v1.y), color),		// (X1, Y1) to (X2, Y1)
		sf::Vertex(sf::Vector2f((float)v2.x, (float)v1.y), color),

		sf::Vertex(sf::Vector2f((float)v2.x, (float)v1.y), color),		// (X2, Y1) to (X2, Y2)
		sf::Vertex(sf::Vector2f((float)v2.x, (float)v2.y), color),

		sf::Vertex(sf::Vector2f((float)v1.x, (float)v2.y), color),		// (X1, Y2) to (X2, Y2)
		sf::Vertex(sf::Vector2f((float)v2.x, (float)v2.y), color)
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
	text.setFillColor(sf::Color(R, G, B, A));
	text.setPosition((float)X, (float)Y);
	pWindowManager->Draw(&text);
}
void CDrawingManager::DrawTextCentred(CWindowManager* pWindowManager, const std::string& szText, sf::Vector2f drawPos, int iSize, sf::Color color)
{
	sf::Text text;
	text.setFont(m_fFont);
	text.setString(szText);
	text.setCharacterSize(iSize);

	/* Centre the text */
	sf::FloatRect bound = text.getGlobalBounds();
	text.setOrigin(bound.left + bound.width / 2.0f, bound.top + bound.height / 2.0f);

	text.setFillColor(color);
	text.setPosition(drawPos);
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

	text.setFillColor(sf::Color(R, G, B, A));
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

	text.setFillColor(sf::Color(R, G, B, A));
	text.setPosition((float)X, (float)Y);
	pWindowManager->Draw(&text);
}

void CDrawingManager::DrawTextCentredX(CWindowManager* pWindowManager, char* szText, sf::Vector2f position, int textSize, sf::Color color) {
	sf::Text text;
	text.setFont(m_fFont);
	text.setString(szText);
	text.setCharacterSize(textSize);

	/* Centre the text */
	sf::FloatRect bound = text.getGlobalBounds();
	text.setOrigin(bound.left + bound.width / 2.0f, 0);

	text.setFillColor(color);
	text.setPosition(position);
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
void CDrawingManager::DrawSpriteCentred(CWindowManager* pWindowManager, sf::Vector2i vPosition, sf::Vector2i size, sf::Texture* pTexture)
{
	if (!pTexture)
		return;

	/* Create a renderable sprite */
	sf::Sprite sp;
	sp.setTexture(*pTexture);
	sp.setPosition(sf::Vector2f(vPosition));
	sp.setOrigin(sf::Vector2f(pTexture->getSize()) / 2.0f);

	/* Scale to required dimensions */
	sf::Vector2f tSize = sf::Vector2f(pTexture->getSize());
	sf::Vector2f scale(size.x / tSize.x, tSize.y / tSize.y);
	sp.setScale(scale);

	/* Draw to window*/
	pWindowManager->Draw(&sp);
}
void CDrawingManager::DrawSpriteCentred(CWindowManager* pWindowManager, sf::Vector2i vPosition, int iWidth, int iHeight, sf::Texture* pTexture)
{
	if (!pTexture)
		return;

	/* Create a renderable sprite */
	sf::Sprite sp;
	sp.setTexture(*pTexture);
	sp.setPosition((float)vPosition.x, (float)vPosition.y);

	sp.setOrigin(pTexture->getSize().x / 2.0f, pTexture->getSize().y / 2.0f);

	/* Scale to required dimensions */
	sf::Vector2u f = pTexture->getSize();
	sf::Vector2f scale((float)iWidth / f.x, (float)iHeight / f.y);
	sp.setScale(scale);

	/* Draw to window*/
	pWindowManager->Draw(&sp);
}

void CDrawingManager::DrawSquareCentred(CWindowManager* pWindowManager, sf::Vector2i vPosition, int iSize, unsigned __int8 R, unsigned __int8 G,
	unsigned __int8 B, unsigned __int8 A)
{
	sf::RectangleShape sq(sf::Vector2f((float)iSize, (float)iSize));

	sq.setOrigin(iSize / 2.0f, iSize / 2.0f);
	sq.setPosition((float)vPosition.x, (float)vPosition.y);
	sq.setFillColor(sf::Color(R, G, B, A));

	pWindowManager->Draw(&sq);
}
void CDrawingManager::DrawRectangleCentred(CWindowManager* pWindowManager, sf::Vector2i vPosition, int iWidth, int iHeight, unsigned __int8 R, unsigned __int8 G,
	unsigned __int8 B, unsigned __int8 A)
{
	sf::RectangleShape sq(sf::Vector2f((float)iWidth, (float)iHeight));

	sq.setOrigin(iWidth / 2.0f, iHeight / 2.0f);
	sq.setPosition((float)vPosition.x, (float)vPosition.y);
	sq.setFillColor(sf::Color(R, G, B, A));

	pWindowManager->Draw(&sq);
}

void CDrawingManager::DrawRectangleCentred(CWindowManager* pWindowManager, sf::Vector2f vPosition, sf::Vector2f size, sf::Color color) {
	sf::RectangleShape rect(size);

	rect.setOrigin(size / 2.0f);
	rect.setPosition(vPosition);
	rect.setFillColor(color);

	pWindowManager->Draw(&rect);
}

void CDrawingManager::DrawRectangle(CWindowManager* pWindowManager, sf::Vector2f vPosition, sf::Vector2f size, sf::Color color) {
	sf::RectangleShape rect(size);
	rect.setPosition(vPosition);
	rect.setFillColor(color);
	pWindowManager->Draw(&rect);
}

void CDrawingManager::DrawOutlinedRectangleCentred(CWindowManager* pWindowManager, sf::Vector2i vPosition, int iWidth, int iHeight, unsigned __int8 R, unsigned __int8 G,
	unsigned __int8 B, unsigned __int8 A)
{
	sf::RectangleShape sq(sf::Vector2f((float)iWidth, (float)iHeight));

	sq.setOrigin(iWidth / 2.0f, iHeight / 2.0f);
	sq.setPosition((float)vPosition.x, (float)vPosition.y);
	sq.setFillColor(sf::Color(R, G, B, A));
	sq.setOutlineThickness(1.0f);

	pWindowManager->Draw(&sq);
}

void CDrawingManager::DrawLine(CWindowManager* pWindowManager, sf::Vector2i vStart, sf::Vector2i vEnd, unsigned __int8 R, unsigned __int8 G, unsigned __int8 B, unsigned __int8 A) {
	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f((float)vStart.x, (float)vStart.y), sf::Color(R,G,B,A)),
		sf::Vertex(sf::Vector2f((float)vEnd.x, (float)vEnd.y), sf::Color(R, G, B, A))
	};
	pWindowManager->DrawVertices(line, 2, sf::Lines);
}

void CDrawingManager::DrawLine(CWindowManager* pWindowManager, sf::Vector2f vStart, sf::Vector2f vEnd, sf::Color col) {
	sf::Vertex line[] =
	{
		sf::Vertex(vStart, col),
		sf::Vertex(vEnd, col)
	};
	pWindowManager->DrawVertices(line, 2, sf::Lines);
}

void CDrawingManager::DrawCircleCentred(CWindowManager* pWindowManager, sf::Vector2i vCentre, float radius, unsigned __int8 R, unsigned __int8 G, unsigned __int8 B, unsigned __int8 A) {
	sf::CircleShape c;
	c.setRadius(radius); 
	c.setFillColor(sf::Color(R, G, B, A));
	c.setPosition((float)vCentre.x, (float)vCentre.y);
	c.setOrigin(radius / 2.0f, radius / 2.0f);
	pWindowManager->Draw(&c);
}
void CDrawingManager::DrawSolidCircleCentred(CWindowManager* pWindowManager, sf::Vector2f vCentre, float radius, sf::Color color) {
	sf::CircleShape c;
	c.setRadius(radius);
	c.setPosition(vCentre);
	c.setFillColor(color);
	c.setOrigin(radius / 2.0f, radius / 2.0f);
	pWindowManager->Draw(&c);
}
void CDrawingManager::DrawCircleCentred(CWindowManager* pWindowManager, sf::Vector2f vCentre, float radius, unsigned __int8 R, unsigned __int8 G, unsigned __int8 B, unsigned __int8 A) {
	sf::CircleShape c;
	c.setRadius(radius);
	c.setFillColor(sf::Color(R, G, B, A));
	c.setPosition(vCentre.x, vCentre.y);
	c.setOrigin(radius / 2.0f, radius / 2.0f);
	pWindowManager->Draw(&c);
}
void CDrawingManager::DrawCircleCentred(CWindowManager* pWindowManager, sf::Vector2f vCentre, float radius, sf::Color color) {
	sf::CircleShape c;
	c.setRadius(radius);
	c.setFillColor(color);
	c.setPosition(vCentre);
	c.setOrigin(radius / 2.0f, radius / 2.0f);
	pWindowManager->Draw(&c);
}
void CDrawingManager::DrawSquareToTargetCentred(sf::RenderTarget* pTarget, sf::Vector2i vPosition, int iSize, unsigned __int8 R, unsigned __int8 G,
	unsigned __int8 B, unsigned __int8 A)
{
	sf::RectangleShape sq(sf::Vector2f((float)iSize, (float)iSize));

	sq.setOrigin(iSize / 2.0f, iSize / 2.0f);
	sq.setPosition((float)vPosition.x, (float)vPosition.y);
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

void CDrawingManager::DrawSquareToTarget(sf::RenderTarget* pTarget, sf::Vector2i vPosition, int iSize, unsigned __int8 R, unsigned __int8 G,
	unsigned __int8 B, unsigned __int8 A)
{
	sf::RectangleShape sq(sf::Vector2f((float)iSize, (float)iSize));

	sq.setPosition((float)vPosition.x, (float)vPosition.y);
	sq.setFillColor(sf::Color(R, G, B, A));
	sq.setOutlineThickness(0.0f);

	pTarget->draw(sq);
}
void CDrawingManager::DrawTileToTarget(sf::RenderTarget* pTarget, sf::Vector2f pos, int size, const sf::Texture* pTexture) {
	sf::Sprite sp;
	sp.setTexture(*pTexture);
	sp.setPosition(pos);	
	sf::Vector2u f = pTexture->getSize();
	sf::Vector2f scale((float)size / f.x, (float)size / f.y);
	sp.setScale(scale);
	pTarget->draw(sp);
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