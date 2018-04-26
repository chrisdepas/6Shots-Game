#pragma once
#include "vector.h"
#include "CIniReader.h"
#include "CIniWriter.h"

#define SETTINGS_FILE_PATH "./GameSettings.ini"

#define SHADERS_ENABLED_STRING "ShadersOn"
#define DEFAULT_SHADERS_ENABLED true

#define SCREEN_WIDTH_STRING "ScreenWidth"
#define DEFAULT_SCREEN_WIDTH 800
#define SCREEN_WIDTH_MIN 100
#define SCREEN_WIDTH_MAX 100000

#define SCREEN_HEIGHT_STRING "ScreenHeight"
#define DEFAULT_SCREEN_HEIGHT 600
#define SCREEN_HEIGHT_MIN 100
#define SCREEN_HEIGHT_MAX 100000

#define FULLSCREEN_STRING "Fullscreen"
#define DEFAULT_FULLSCREEN false

#define LIMIT_FPS_STRING "LimitFPS"
#define DEFAULT_LIMIT_FPS true

#define FPS_COUNT_LIMIT_STRING "MaxFPS"
#define DEFAULT_FPS_COUNT_LIMIT 60
#define FPS_COUNT_LIMIT_MIN 30 
#define FPS_COUNT_LIMIT_MAX 100000

#define VSYNC_STRING "VSyncEnabled"
#define DEFAULT_VSYNC false

#define MUSIC_VOLUME_STRING "MusicVol"
#define DEFAULT_MUSIC_VOLUME 60.0f
#define MUSIC_VOLUME_MIN 0.0f
#define MUSIC_VOLUME_MAX 100.0f

#define SOUND_VOLUME_STRING "SoundVol"
#define DEFAULT_SOUND_VOLUME 70.0f
#define SOUND_VOLUME_MIN 0.0f
#define SOUND_VOLUME_MAX 100.0f

class CSettings
{
public:

	// Graphics
	Vector2i m_vScreenSize;
	bool m_bFullscreen;
	bool m_bLimitFPS;
	int m_iFPSCountLimit;
	bool m_bVSync;
	bool m_bShadersEnabled;

	// Debug
	bool m_bFullbright;
	
	float m_fMusicVolume; /* 0.0 to 100.0 */
	float m_fSoundVolume; /* 0.0 to 100.0 */

	CSettings();
	~CSettings();
	void LoadFromFile(char* szFileName, CIniReader* pIniReader);
	void SaveToFile(char* szFileName, CIniWriter* pIniWriter);

	/* Exposed functions to change settings */
	void SetFullscreen(bool bFullscreen);
	void SetResolution(int iWidth, int iHeight);
	void SetSoundVolume(float fSoundvol);
	void SetMusicVolume(float fMusicvol);
	void SetShadersEnabled(bool bEnable);


	bool operator!=(const CSettings& other) {
		// [VIDEO]
		if (this->m_vScreenSize.X != other.m_vScreenSize.X)
			return true;
		if (this->m_vScreenSize.Y != other.m_vScreenSize.Y)
			return true;
		if (this->m_bFullscreen != other.m_bFullscreen)
			return true;
		if (this->m_bLimitFPS != other.m_bLimitFPS)
			return true;
		if (this->m_bVSync != other.m_bVSync)
			return true;
		if (this->m_bShadersEnabled != other.m_bShadersEnabled)
			return true;

		// [DEBUG]
		if (this->m_iFPSCountLimit != other.m_iFPSCountLimit)
			return true;
		if (this->m_bFullbright != other.m_bFullbright)
			return true;

		// [AUDIO]
		if (this->m_fMusicVolume != other.m_fMusicVolume)
			return true;
		if (this->m_fSoundVolume != other.m_fSoundVolume)
			return true;

		return false;
	}

	bool operator==(const CSettings& other) {
		return !(*this != other);
	}
};