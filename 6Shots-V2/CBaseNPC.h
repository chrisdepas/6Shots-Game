#ifndef __CBASENPC_H__
#define __CBASENPC_H__

#include "C6SPlayer.h"

/*
**	Christopher De Pasquale
**	Created:	Unknown, sometime in 2017
**	Last Edit	26th April 2018
*/

class CBaseNPC : public C6SPlayer {
public:
	CBaseNPC();

	/* Default drawing*/
	void DefaultDraw(CGame* pGame);

	/* Draw	- Defaults to 'DefaultDraw' */
	virtual void Draw(CGame* pGame) { DefaultDraw(pGame); }

	/* Update */
	virtual void Update(CGame* pGame, float fFrameTime);

	/* Think */
	virtual void Think(CGame* pGame, float fFrameTime);
}; 

#endif