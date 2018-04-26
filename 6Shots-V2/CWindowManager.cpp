#include "stdafx.h"
#include "CWindowManager.h"
#include "CDebugLogger.h"
CWindowManager::CWindowManager()
{
	m_vSize = Vector2i(0, 0);
	m_pWindow = NULL;
	m_bWindowCreated = false;
	m_bWindowClosed = false;
	m_bFocus = true;
	m_pGUI = NULL;
	m_bGUIActive = false;
}
CWindowManager::~CWindowManager()
{
	if (m_bWindowCreated && m_pWindow)
		delete m_pWindow;
}

/*	Create a window with supplied settings.
	WARNING: Assumes settings are sanitary! */
void CWindowManager::Create(CSettings* pSettings, char* szTitle) {
	if (m_bWindowCreated)
		return; 

	m_bWindowCreated = true;

	sf::VideoMode videomode(pSettings->m_vScreenSize.X, pSettings->m_vScreenSize.Y);

	m_vSize = pSettings->m_vScreenSize;

	if (pSettings->m_bFullscreen)
		m_pWindow = new sf::RenderWindow(videomode, szTitle, sf::Style::Fullscreen);
	else
		m_pWindow = new sf::RenderWindow(videomode, szTitle, sf::Style::Titlebar | sf::Style::Close);

	if (pSettings->m_bVSync) {
		CDebugLogger::Instance()->LogLevel(CDebugLogger::LOGLEVEL_INFO)->Log("VSync Enabled");
		m_pWindow->setVerticalSyncEnabled(true);
	} else if (pSettings->m_bLimitFPS) { 
		CDebugLogger::Instance()->LogLevel(CDebugLogger::LOGLEVEL_INFO)->Log("FPS Limited to %i\n", pSettings->m_iFPSCountLimit);
		m_pWindow->setFramerateLimit(pSettings->m_iFPSCountLimit);
	}
}

void CWindowManager::ReCreate(CSettings* pSettings, char* szTitle) {
	if (!m_bWindowCreated)
		return;

	/* Close old window*/
	m_pWindow->close();

	sf::VideoMode videomode(pSettings->m_vScreenSize.X, pSettings->m_vScreenSize.Y);
	m_vSize = pSettings->m_vScreenSize;

	/* Recreate window with new settings */
	if (pSettings->m_bFullscreen)
		m_pWindow->create(videomode, szTitle, sf::Style::Fullscreen);
	else
		m_pWindow->create(videomode, szTitle, sf::Style::Titlebar | sf::Style::Close);
	
	if (pSettings->m_bVSync) {
		CDebugLogger::Instance()->LogLevel(CDebugLogger::LOGLEVEL_INFO)->Log("VSync Enabled");
		m_pWindow->setVerticalSyncEnabled(true);
	}
	else if (pSettings->m_bLimitFPS) {
		CDebugLogger::Instance()->LogLevel(CDebugLogger::LOGLEVEL_INFO)->Log("FPS Limited to %i\n", pSettings->m_iFPSCountLimit);
		m_pWindow->setFramerateLimit(pSettings->m_iFPSCountLimit);
	}

	//m_pWindow->setKeyRepeatEnabled( false );
}

void CWindowManager::HandleEvents(CInput* pInput) {
	sf::Event e;
	while (m_pWindow->pollEvent(e))
	{
		switch (e.type)
		{
		case sf::Event::Closed:
			m_bWindowClosed = true;
			break;
		case sf::Event::GainedFocus:
			m_bFocus = true; 
			break;
		case sf::Event::LostFocus:
			m_bFocus = false;
			break;
		case sf::Event::MouseMoved:
			if (HasFocus() && !m_bGUIActive)
				pInput->InjectMouseMove(e.mouseMove.x, e.mouseMove.y);
			break;
		case sf::Event::MouseButtonPressed:
			if (HasFocus() && !m_bGUIActive)
				pInput->InjectMousePress(e.mouseButton.x, e.mouseButton.y, e.mouseButton.button);
			break;
		case sf::Event::MouseButtonReleased:
			if (HasFocus() && !m_bGUIActive)
				pInput->InjectMouseRelease(e.mouseButton.x, e.mouseButton.y, e.mouseButton.button);
			break;

		}

		if (m_bGUIActive && m_pGUI)
			m_pGUI->handleEvent(e);
	}
}

void CWindowManager::Begin() {
	if (m_bWindowCreated && m_pWindow)
		m_pWindow->clear();
}

void CWindowManager::Present() {
	if (m_bWindowCreated && m_pWindow)
		m_pWindow->display();
}

bool CWindowManager::WindowClosed() {
	return m_bWindowClosed;
}
 
bool CWindowManager::HasFocus() {
	return m_bFocus;
}
Vector2i CWindowManager::GetScreenCentre() {
	return m_vSize / 2;
}
Vector2i CWindowManager::GetScreenDimensions() {
	return m_vSize;
}
void CWindowManager::Draw(sf::Drawable* Item) {
	m_pWindow->draw(*Item);
}
void CWindowManager::DrawVertices(sf::Vertex* vertices, int count, sf::PrimitiveType type) {
	m_pWindow->draw(vertices, count, type);
}
void CWindowManager::RestoreDefaultCamera() {
	m_pWindow->setView(m_pWindow->getDefaultView());
}
void CWindowManager::MoveCamera(Vector2f CentrePos) {
	Vector2i ScreenCentre = GetScreenCentre();
	sf::View view(sf::Vector2f(CentrePos.X, CentrePos.Y), sf::Vector2f((float)m_vSize.X, (float)m_vSize.Y));
	m_pWindow->setView(view);
}	
sf::RenderWindow* CWindowManager::GetWindow() {
	return m_pWindow;
}

Vector2i CWindowManager::ScreenToWorld(Vector2i screenPos) {
	sf::Vector2f v = m_pWindow->mapPixelToCoords(sf::Vector2i(screenPos.X, screenPos.Y));
	return (Vector2i((int)v.x, (int)v.y));
}

void CWindowManager::SetGUI(tgui::Gui* pGUI) {
	m_pGUI = pGUI;
}
void CWindowManager::SetGUIActive(bool bActive) {
	m_bGUIActive = bActive;
}
bool CWindowManager::IsGUIActive() {
	return m_bGUIActive;
}