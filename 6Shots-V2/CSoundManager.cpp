#include "stdafx.h"
#include "CSoundManager.h"
#include "CSettings.h"
#include "CDebugLogger.h"

int CSoundManager::GetID(char* szSoundName)
{
	/* If sound has already been saved, return the index */
	for (unsigned int i = 0; i < m_vSounds.size(); i++)
	{
		if (m_vSounds[i]->MatchesName(szSoundName))
			return i; 
	}

	/* sound hasn't been saved, add and return index */
	m_vSounds.push_back(new CSoundItem(szSoundName));
	return (m_vSounds.size() - 1);
}

bool CSoundManager::PlaySound(int id)
{
	return m_vSounds[id]->PlaySound();
}

bool CSoundManager::PlaySound(char* szSoundName) {
	for (unsigned int i = 0; i < m_vSounds.size(); i++)
	{
		if (m_vSounds[i]->MatchesName(szSoundName)) {
			m_vSounds[i]->PlaySound();
			return true;
		}

	}
	return false;
}


bool CSoundManager::PlayMusic(char* szMusic, CSettings* pSettings) {	
	/* Don't play identical music, fine to not 
	save str as it will be identical in ASSETS.h */
	if (m_szCurMusic == szMusic)
		return true;
	m_szCurMusic = szMusic;

	/* Open music file */
	if (!m_Music.openFromFile(szMusic)) {
		CDebugLogger::LogError("[SOUND] Unable to play music file %s - Can't load asset\n", szMusic);
		return false;
	}

	/* Set volume and play */
	m_Music.setVolume(pSettings->m_fMusicVolume);
	m_Music.play();

	return true;
}