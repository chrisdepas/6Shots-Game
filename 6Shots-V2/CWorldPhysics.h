#ifndef __CWORLDPHYSICS_H__
#define __CWORLDPHYSICS_H__

/*
	Christopher De Pasquale
	Late 2016
	Noticed added feb 2017

*/

#include "C6SMap.h"
#include <Box2D\Box2D.h>
class CPhysicsDebugDraw;

#define METER_TO_PIXEL 60.0f
#define PIXEL_TO_METER 1.0f/METER_TO_PIXEL


class CWorldPhysics : public b2ContactListener {
	bool m_bInit;
	b2Vec2 m_vGravity;
	b2World* m_pWorldPhysics;
	b2Body* m_pGround;
	CPhysicsDebugDraw* m_pDebugDraw;

	/* Multiple physics listeners */
	std::vector< b2ContactListener* > m_pListeners;

public:

	static float GetMeterToPixelFactor();
	static float GetPixelToMeterFactor();

	CWorldPhysics();

	/* Init World Physics */
	void InitPhysics(CGame* pGame, float fGravity, bool bDebugDraw);

	/* Debug draw (Won't draw unless enabled)*/
	void Draw(); 

	/* Clear map tiles etc */
	void ClearWorldMap();

	/* Load collision spaces from map */
	void ParseWorldMap(C6SMap* map);

	/* Create body */
	b2Body* CreateBody(b2BodyDef* pDef);

	/* Delete physics body */ 
	void DestroyBody(b2Body* pBody);

	/* Add a contact listener */
	void SetContactListener(b2ContactListener* listener);

	/* Stop a contact listener */
	void RemoveContactListener(b2ContactListener* listener);

	/* Step */
	void Step(float fTime);

	/* b2ContactListener Callbacks */
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
};

#endif