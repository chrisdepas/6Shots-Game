#ifndef __CSOUNDMANAGER_H__
#define __CSOUNDMANAGER_H__

/*
	Christopher De Pasquale & Michael De Pasquale	
	2015
	Noticed added feb 2017

*/

#include "CSoundItem.h"
#include <vector>

class CSettings;
class CSoundManager 
{
	std::vector< CSoundItem* > m_vSounds;
	sf::Music m_Music;
	char* m_szCurMusic; /* Used to check for identical music by ptr only, !not safe! */
public:
	CSoundManager() {
		m_szCurMusic = 0;
	}

	/* Play sound by ID */
	bool PlaySound(int ID);

	/* Play sound by sound path */
	bool PlaySound(char* szSound);

	/* Stream music from file. Only one at a time */
	bool PlayMusic(char* szMusic, CSettings* pSettings);

	/* Get an ID from sound, loads if not already loaded */
	int GetID(char* szSoundName);
};

#endif