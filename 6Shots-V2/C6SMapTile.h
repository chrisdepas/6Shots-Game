#ifndef __C6SMAPTILE_H__
#define __C6SMAPTILE_H__

/*
		Christopher De Pasquale
Unknown date, notice added 12 December 2016
*/

class CGame;
#include "Vector.h"
#include "SFML\Graphics.hpp"
class C6SMapTile {
	Vector2i m_vSize;
	bool m_bInit;
	int m_iTexture;
public: 
	
	void Draw(CGame* pGame);
	bool Collision(int x, int y);
	bool Collision(Vector2i pos);

	void Initialize(char* image, int width, int height, int x, int y);
};

#endif