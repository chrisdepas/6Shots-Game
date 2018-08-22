#ifndef __CENTITYPHYSICS_H__
#define __CENTITYPHYSICS_H__
/*
	Christopher De Pasquale
	Late 2016
	Noticed added feb 2017

*/


class CWorldPhysics;


class CEntityPhysics {
protected:
	b2Body* m_pBody;

	/* If collisions are enabled */
	bool m_bCollisionsEnabled = true;
	uint16 m_uiLastMaskBits = 0;
public:

	/* Physics types */
	enum EEntityCategory {
		CATEGORY_DEFAULT		= 1 << 0, /* Default category e.g. walls */
		CATEGORY_LADDER			= 1 << 1, /* Ladder */
		CATEGORY_PLAYER_PICKUP	= 1 << 2, /* Weapons and items */
		CATEGORY_BLOOD_AND_GORE = 1 << 3, /* Blood and gore effects */
		CATEGORY_BULLET			= 1 << 4, /* Bullets, projectiles */
		CATEGORY_PLAYER			= 1 << 5, /* Players and NPCs */
		CATEGORY_PHYSICAL_EFFECT= 1 << 6, /* Effects such as bullet casings, broken stuff etc*/
	};

	CEntityPhysics();

	/* Get position of physics object */
	sf::Vector2f GetPosition();

	/* Get rotation of physics object */
	float GetRotationDegree();
	float GetRotationRadian();

	/* Check categories of all fixtures of physics object */
	bool IsCategory(EEntityCategory category);

	/* Move physics object */
	void SetPosition(sf::Vector2i postion);
	void SetPosition(sf::Vector2f postion);
	
	/* Rotate */
	void SetRotation(float rotation);

	void SetRotationalVelocity(float rotationVel);

	/* Modify velocity */
	void SetHorizontalVelocity(float vel);
	void SetVerticalVelocity(float vel);
	void SetVelocity(sf::Vector2f vel);

	/* Init physics as box */
	void InitPhysics(CWorldPhysics* pWorldPhysics, int initPosX, int initPosY, int width, int height,
		CEntityPhysics::EEntityCategory ePhysicsCategory, int eIgnoreCollisionCategories);

	/* Init physics as box with ID */
	void InitPhysics(CWorldPhysics* pWorldPhysics, int initPosX, int initPosY, int width, int height,
		CEntityPhysics::EEntityCategory ePhysicsCategory, int eIgnoreCollisionCategories, int dataID);

	/* Init physics from provided collider 
	collider = (list of polygon vertices in CCW on each line) */
	void InitPhysics(CWorldPhysics* pWorldPhysics, int initPosX, int initPosY, char* szColliderFile,
		CEntityPhysics::EEntityCategory ePhysicsCategory, int eIgnoreCollisionCategories);

	/* Approximates collider from image */
	void InitPhysicsFromImage(CWorldPhysics* pWorldPhysics, int initPosX, int initPosY, sf::Image* pImg,
		CEntityPhysics::EEntityCategory ePhysicsCategory, int eIgnoreCollisionCategories);

	/**/
	void DestroyPhysics(CWorldPhysics* pWorldPhysics);
	
	/* Turn on/off physics */
	void DisablePhysics();
	void EnablePhysics();

	/* Turn on/off collisions */
	void DisableCollisions();
	void EnableCollisions();
	
	/* Check if physics is active */
	bool IsActive();
};

#endif