#include "stdafx.h"
#include "CSettings.h"
#include "CDebugLogger.h"

CSettings::CSettings() {
	/* Debug settings */
	m_bFullbright = false;
}

CSettings::~CSettings() {}

void CSettings::LoadFromFile(char* szFileName, CIniReader* pIniReader) {
	if (!pIniReader)
		return;
	if (!pIniReader->ParseFile(szFileName))
		return;

	if (pIniReader->ItemExists(SCREEN_WIDTH_STRING)) {
		pIniReader->GetIntValue(SCREEN_WIDTH_STRING, m_vScreenSize.x);
		if (m_vScreenSize.x < SCREEN_WIDTH_MIN || m_vScreenSize.x > SCREEN_WIDTH_MAX)
			m_vScreenSize.x = DEFAULT_SCREEN_WIDTH;
	}

	if (pIniReader->ItemExists(SCREEN_HEIGHT_STRING)) {
		pIniReader->GetIntValue(SCREEN_HEIGHT_STRING, m_vScreenSize.y);
		if (m_vScreenSize.y < SCREEN_HEIGHT_MIN || m_vScreenSize.y > SCREEN_HEIGHT_MAX)
			m_vScreenSize.y = DEFAULT_SCREEN_HEIGHT;
	}

	if (pIniReader->ItemExists(FULLSCREEN_STRING)) {
		pIniReader->GetBoolValue(FULLSCREEN_STRING, m_bFullscreen);
	}

	if (pIniReader->ItemExists(SHADERS_ENABLED_STRING)) {
		pIniReader->GetBoolValue(SHADERS_ENABLED_STRING, m_bShadersEnabled);
	}

	if (pIniReader->ItemExists(FPS_COUNT_LIMIT_STRING)) {
		pIniReader->GetIntValue(FPS_COUNT_LIMIT_STRING, m_iFPSCountLimit);
		if (m_iFPSCountLimit < FPS_COUNT_LIMIT_MIN || m_iFPSCountLimit > FPS_COUNT_LIMIT_MAX)
			m_iFPSCountLimit = DEFAULT_FPS_COUNT_LIMIT;
	}

	if (pIniReader->ItemExists(VSYNC_STRING)) {
		pIniReader->GetBoolValue(VSYNC_STRING, m_bVSync);
	}

	if (pIniReader->ItemExists(LIMIT_FPS_STRING)) {
		pIniReader->GetBoolValue(LIMIT_FPS_STRING, m_bLimitFPS);
	}

	if (pIniReader->ItemExists(MUSIC_VOLUME_STRING)) {
		pIniReader->GetFloatValue(MUSIC_VOLUME_STRING, m_fMusicVolume);
		if (m_fMusicVolume < MUSIC_VOLUME_MIN || m_fMusicVolume > MUSIC_VOLUME_MAX) {
			/* Bad music volume level */
			CDebugLogger::LogWarning("[INI] MusicVol out of range (%f, %f), defaulting to %f\n", MUSIC_VOLUME_MIN, MUSIC_VOLUME_MAX, DEFAULT_MUSIC_VOLUME);
			m_fMusicVolume = DEFAULT_MUSIC_VOLUME;
		} else {
			CDebugLogger::LogInfo("[INI] MusicVol set to %f\n", m_fMusicVolume);
		}
	}

	if (pIniReader->ItemExists(SOUND_VOLUME_STRING)) {
		pIniReader->GetFloatValue(SOUND_VOLUME_STRING, m_fSoundVolume);
		if (m_fSoundVolume < SOUND_VOLUME_MIN || m_fSoundVolume > SOUND_VOLUME_MAX) {
			/* Bad sound volume level */
			CDebugLogger::LogWarning("[INI] SoundVol out of range (%f, %f), defaulting to %f\n", SOUND_VOLUME_MIN, SOUND_VOLUME_MAX, DEFAULT_SOUND_VOLUME);
			m_fSoundVolume = DEFAULT_SOUND_VOLUME;
		} else {
			CDebugLogger::LogWarning("[INI] SoundVol set to %f\n", m_fSoundVolume);
		}
	}
}

void CSettings::SaveToFile(char* szFileName, CIniWriter* pIniWriter) {
	/* Clear old content */
	pIniWriter->ClearItems();

	pIniWriter->StoreComment("6Shots Settings");

	/* Video */
	pIniWriter->StoreSectionHeader("VIDEO");
	pIniWriter->StoreIntItem(SCREEN_WIDTH_STRING, m_vScreenSize.x);
	pIniWriter->StoreIntItem(SCREEN_HEIGHT_STRING, m_vScreenSize.y);
	pIniWriter->StoreBoolItem(FULLSCREEN_STRING, m_bFullscreen);
	pIniWriter->StoreBoolItem(LIMIT_FPS_STRING, m_bLimitFPS);
	pIniWriter->StoreIntItem(FPS_COUNT_LIMIT_STRING, m_iFPSCountLimit);
	pIniWriter->StoreBoolItem(VSYNC_STRING, m_bVSync);
	pIniWriter->StoreBoolItem(SHADERS_ENABLED_STRING, m_bShadersEnabled);

	/* Sound & Music volume */
	pIniWriter->StoreSectionHeader("AUDIO");
	pIniWriter->StoreFloatItem(MUSIC_VOLUME_STRING, m_fMusicVolume);
	pIniWriter->StoreFloatItem(SOUND_VOLUME_STRING, m_fSoundVolume);

	/* Write new content */
	pIniWriter->WriteToFile(szFileName);
}

void CSettings::SetFullscreen(bool bFullscreen) {
	m_bFullscreen = bFullscreen;
}
void CSettings::SetResolution(int iWidth, int iHeight) {
	/* Todo: Validate */
	m_vScreenSize.x = iWidth;
	m_vScreenSize.y = iHeight;
}
void CSettings::SetSoundVolume(float fSoundvol) {
	m_fSoundVolume = fSoundvol;
	if (m_fSoundVolume > SOUND_VOLUME_MAX)
		m_fSoundVolume = SOUND_VOLUME_MAX;
	if (m_fSoundVolume < SOUND_VOLUME_MIN)
		m_fSoundVolume = SOUND_VOLUME_MIN;
}
void CSettings::SetMusicVolume(float fMusicvol) {
	m_fMusicVolume = fMusicvol;
}