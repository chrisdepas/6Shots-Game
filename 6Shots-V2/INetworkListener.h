#ifndef __INETWORKLISTENER_H__
#define __INETWORKLISTENER_H__

#include "Vector.h"

/*		Network listener interface
 *		Use to connect Server/client and a game state 
 *
 *		Recieves both Server & Client events, must parse
 *		differently based on mode
 *
 */

class CGame;

class INetworkListener {
public: 

	/* #################### BOTH SERVER AND CLIENT EVENTS #################### */

	/* Player position has been updated */
	virtual void OnTick(CGame* pGame, int iClientID, Vector2f vPosition, Vector2f vHandPosition, float fHandRotation, bool bLeftFacing, bool bWalking) = 0;


	/* ####################       SERVER->CLIENT ONLY EVENTS      #################### */

	/* Server has created an entity */
	virtual void OnRemoteEntityCreate(CGame* pGame, int iEntityID, int iType, Vector2f vCreatePos, float fCreateAngle, int iInstanceID, int iClientGiveID) = 0;

	/* Player has picked up a weapon */
	virtual void OnPlayerPickupWeapon(CGame* pGame, int clientID, int entityID) = 0;

	/* Server started game round */
	virtual void OnRoundStart(CGame* pGame) = 0; 

	/* Player threw weapon */
	virtual void OnPlayerThrowWeapon(CGame* pGame, int clientID, Vector2f vThrowPos, Vector2f vThrowVelocity, float fThrowRotation) = 0;
	 
	/* Player shot weapon */
	virtual void OnPlayerShootWeapon(CGame* pGame, int clientID, Vector2f vPosition, Vector2f vVelocity, int projectileID) = 0;

	/* Player attempted to shoot with no ammo */
	virtual void OnPlayerDryFire(CGame* pGame, int clientID) = 0;

	/* Player took damage */
	virtual void OnPlayerDamage(CGame* pGame, int iClientID, Vector2f vDamagePosition, Vector2f vDamageVelocity, int iDamageAmount) = 0;

	/* Player died */
	virtual void OnPlayerDeath(CGame* pGame, int iClientID, Vector2f vDeathDamagePosition, Vector2f vDeathDamageVelocity) = 0;

	/* Player sent console command */
	virtual void OnPlayerCommand(CGame* pGame, int iClientID, int eConCmd) = 0;

	/* Server broadcasts a message */
	virtual void OnBroadcastMessage(CGame* pGame, float fTime, std::string& sMessage) = 0;

	/* Server teleports client */
	//virtual void OnPlayerTeleport(CGame* pGame, Vector2f vPosition) = 0;

	/* Server resets round */
	virtual void OnRoundReset(CGame* pGame) = 0;
	 
	/* ####################      CLIENT->SERVER ONLY EVENTS       #################### */
		
	/* Player wants to pickup weapon or shoot */
	virtual void OnPlayerMousePress(CGame* pGame, int clientID) = 0;

	/* Player wants to throw weapon / other action */
	virtual void OnPlayerRightMousePress(CGame* pGame, int clientID, Vector2f vThrowVelocity, float fThrowRotation) = 0;
};

#endif