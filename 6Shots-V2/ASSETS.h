#ifndef __ASSETS_H__
#define __ASSETS_H__

/* Assets for the game */

/* GUI WIDGETS */
#define ASSET_GUI_WIDGETS "widgets/Black.txt"

/* MAPS */
#define ASSET_MAP_SPACEGRAVE "./Map/SpaceGrave.6map"

/* BLOOD & GORE */
#define ASSET_FLOOR_BLOOD_TEXTURE "./ASSETS/FloorBlood.png"


/* MUSIC */
#define TITLE_MUSIC "./ASSETS/pizza_man.wav"

/* FONTS */
#define ASSET_DEFAULT_FONT "./ASSETS/DejaVuSans.ttf"



/* PLAYERS */
#define ASSET_PLAYER_TEXTURE "./ASSETS/playerNew"
#define ASSET_NPC_TEXTURE "./ASSETS/NPCRed"
#define ASSET_PLAYER_HAND "./ASSETS/Hand.png"



/* WEAPONS */
	// Knife
#define ASSET_KNIFE_TEXTURE "./ASSETS/Knife.png"

	// Revolver 
#define ASSET_REVOLVER_TEXTURE "./ASSETS/Revolver.png"
#define ASSET_REVOLVER_SOUND "./ASSETS/Revolver_Fire.wav"
	
	// Sawn-off shotgun
#define ASSET_SAWNOFF_TEXTURE "./ASSETS/SawnOff.png"
#define ASSET_SAWNOFF_SOUND ASSET_REVOLVER_SOUND

	// AK47	
#define ASSET_AK47_COLLIDER "./ASSETS/AK47Collider.6cs"
#define ASSET_AK47_TEXTURE "./ASSETS/AK47.png"
#define ASSET_AK47_SOUND ASSET_REVOLVER_SOUND


	// tmp
#define ASSET_TMP_TEXTURE "./ASSETS/TMP.png"
#define ASSET_TMP_SOUND ASSET_REVOLVER_SOUND

	// All weapon assets
#define ASSET_DRYFIRE "./ASSETS/DryFire.wav"
	// Casings
#define ASSET_SHOTGUN_CASING "./ASSETS/ShellCasing.png"
#define ASSET_PISTOL_CASING "./ASSETS/PistolCasing.png"
#define ASSET_RIFLE_CASING "./ASSETS/RifleCasing.png"

/* BACKGROUND */
#define ASSET_DEV_BACKGROUND "./ASSETS/shitbg.png"


#endif