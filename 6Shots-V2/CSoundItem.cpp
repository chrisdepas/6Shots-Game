#include "stdafx.h"
#include "CSoundItem.h"
#include "CDebugLogger.h"

CSoundItem::CSoundItem(char* szName)
{
	m_szSoundName = szName;
	m_bLoaded = false;
	m_bCanLoad = true;
}
bool CSoundItem::MatchesName(char* szName)
{
	if (!szName || !m_szSoundName)
		return false;
	return !_stricmp(szName, m_szSoundName);
} 
bool CSoundItem::PlaySound()
{
	/* This sound cannot be loaded, don't play it */
	if (!m_bCanLoad)
		return false;

	/* If sound is loaded already, play it */
	if (m_bLoaded)
	{
		m_Sound.setBuffer(m_SoundBuffer);
		m_Sound.setVolume(100);
		m_Sound.play();

		return true;
	}

	/* check if sound has a name */
	if (!m_szSoundName)
		return false;

	/* Load sound file */
	m_bCanLoad = m_bLoaded = m_SoundBuffer.loadFromFile(m_szSoundName);
	if (!m_bLoaded) {
		CDebugLogger::LogError("[SOUND] Unable to play sound file %s - Can't load asset\n", m_szSoundName);
		return false;
	}

	/* Play sound */
	m_Sound.setBuffer(m_SoundBuffer);
	m_Sound.setVolume(100);
	m_Sound.play();

	return true;
}