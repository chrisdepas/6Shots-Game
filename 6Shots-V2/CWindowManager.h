#ifndef __CWINDOWMANAGER_H__
#define __CWINDOWMANAGER_H__
#include "Vector.h"
#include <SFML/Graphics.hpp>
#include "CSettings.h"
#include "CInput.h"
#include <TGUI\TGUI.hpp>

class CWindowManager
{
	Vector2i m_vSize;
	bool m_bWindowCreated;
	bool m_bFocus;
	sf::RenderWindow* m_pWindow;
	bool m_bWindowClosed;
	tgui::Gui* m_pGUI;
	bool m_bGUIActive;

public:
	CWindowManager();
	~CWindowManager();
	void Create(CSettings* Settings, char* szTitle);
	void ReCreate(CSettings* pSettings, char* szTitle);
	void HandleEvents(CInput* pInput);
	sf::RenderWindow* GetWindow();

	/* Begin drawing */
	void Begin();
	/* Present scene */
	void Present();

	bool WindowClosed();
	bool HasFocus();
	Vector2i GetScreenCentre();
	Vector2i GetScreenDimensions();

	void Draw(sf::Drawable* Item);
	void DrawVertices(sf::Vertex* vertices, int count, sf::PrimitiveType type);
	void RestoreDefaultCamera();
	void MoveCamera(Vector2f CentrePos);

	/* GUI functions */
	void SetGUI(tgui::Gui* pGUI);
	void SetGUIActive(bool bActive);
	bool IsGUIActive();

	Vector2i ScreenToWorld(Vector2i screenPos);
};

#endif