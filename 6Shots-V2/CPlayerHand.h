#ifndef __CPLAYERHAND_H__
#define __CPLAYERHAND_H__

#include "CBaseEntity.h"
#include "SFML\Graphics.hpp"
#include "CGame.h"
class CPlayerHand : public CBaseEntity {
	sf::Texture handTexture;
	
	/* Max length to extend hand */
	int m_iReachDistance;

	/* Size of hand */
	int m_iHandSize;

	/* Rotation to draw weapon at, in degrees */
	float m_fDrawRotation;

	/* Aim rotation without recoil */
	float m_fTrueRotation;
public:

	/* Init hand */
	void Init(char* handTexture, int handSize, int reachDist, Vector2i playerCentre);

	/* Update position of hand from mouse
		- playerCentre is center of player texture 
		- fRecoilRotation is extra rotation of gun due to recoil, in radians  */
	void UpdatePosition(Vector2i mousePosition, Vector2i playerCentre, float fRecoilRotation);

	/* Update position & rotation of hand from network */
	void UpdatePosition(Vector2f handPosition, float fHandRotation);

	/* Draw hand
		- fRecoilRotation is extra rotation of gun due to recoil, in radians */
	void Draw(CGame* pGame, float fRecoilRotation);

	float GetDrawRotation();
	float GetTrueRotation();

	/**/
	void SetTrueRotation(float fTrueRot);
};

#endif