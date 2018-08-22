#ifndef __CWINDOWMANAGER_H__
#define __CWINDOWMANAGER_H__

#include "CSettings.h"
#include "CInput.h"
#include "CShaderManager.h"
#include "FPSCounter.h"

class CWindowManager
{
	sf::Vector2i m_vSize = sf::Vector2i(0, 0);
	bool m_bWindowCreated = false, m_bWindowClosed = false;
	bool m_bFocus = true, m_bGUIActive = false;

	FPSCounter m_FPS;
	sf::Font m_FPSFont;
	sf::Text m_FPSText;
	int m_LastFPS = -1;

	tgui::Gui* m_pGUI = NULL;
	sf::RenderWindow m_Window;
	CShaderManager m_Shaders;

	void DrawFPS();

public:
	CWindowManager();

	//	Create window with supplied (and assumed sanitized!) settings
	void Create(CSettings* Settings, char* szTitle);
	//	Create window with supplied (and assumed sanitized!) settings again 
	void ReCreate(CSettings* pSettings, char* szTitle);

	// Update window
	void HandleEvents(CInput* pInput);

	sf::RenderWindow* GetWindow();

	void Resize(sf::Vector2i vSize);

	/* Start/End drawing */
	void Begin();
	void Present();

	void CloseWindow();

	bool WindowClosed();
	bool HasFocus();

	// Half of screen size
	sf::Vector2i GetScreenCentre();
	sf::Vector2f GetScreenCentre2f();

	// Screen size 
	sf::Vector2i GetScreenDimensions();
	sf::Vector2f GetScreenDimensions2f();

	// Camera centre position 
	sf::Vector2f GetViewCentre();

	sf::Vector2f GetViewSize() { return m_Window.getView().getSize(); }

	// Get view for GUI centred in middle of screen, at dimensions of screen
	sf::View GetGUIView() { return sf::View(GetScreenCentre2f(), GetScreenDimensions2f()); }

	void Draw(sf::Drawable* Item);
	void DrawVertices(sf::Vertex* vertices, int count, sf::PrimitiveType type);
	void RestoreDefaultCamera();
	void MoveCamera(sf::Vector2f CentrePos);

	// GUI functions 
	void SetGUI(tgui::Gui* pGUI);
	void SetGUIActive(bool bActive);
	void ToggleGUI();
	bool IsGUIActive();
	void RemoveGUI(); 

	sf::Vector2i ScreenToWorld(sf::Vector2i screenPos);
};

#endif