#ifndef __CSOUNDITEM_H__
#define __CSOUNDITEM_H__

/*
	Christopher De Pasquale & Michael De Pasquale
	2015
	Noticed added feb 2017
*/

#include <SFML\Audio.hpp>
class CSoundItem {

private:
	sf::Sound m_Sound;
	sf::SoundBuffer m_SoundBuffer;
	char* m_szSoundName;
	bool m_bCanLoad;
	bool m_bLoaded;

public:
	CSoundItem( char* m_szName );
	~CSoundItem();
	bool MatchesName( char* szName );
	bool PlaySound();
	char* GetName();
};

#endif