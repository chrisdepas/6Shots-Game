#include "stdafx.h"
#include "CWindowManager.h"
#include "CDebugLogger.h"
#include "CTestShader.h"

CWindowManager::CWindowManager() {
	m_Shaders.AddShader(new CRaymarch());

	if (!m_FPSFont.loadFromFile("./ASSETS/DejaVuSans.ttf")) {
		throw std::runtime_error("Failed to load font DejaVuSans.ttf");
	}
	m_FPSText.setFont(m_FPSFont);
	m_FPSText.setCharacterSize(24);
	m_FPSText.setOutlineColor(sf::Color::Black);
	m_FPSText.setFillColor(sf::Color::Yellow);
	m_FPSText.setStyle(sf::Text::Bold);
	m_FPSText.setPosition(10.0f, 10.0f);
}

void CWindowManager::Create(CSettings* pCfg, char* szTitle) {
	if (m_bWindowCreated)
		return; 

	m_vSize = pCfg->m_vScreenSize;

	// Create window with desired videomode 
	sf::VideoMode videomode(pCfg->m_vScreenSize.x, pCfg->m_vScreenSize.y);
	m_Window.create(videomode, szTitle, pCfg->m_bFullscreen ? (sf::Style::Fullscreen) : (sf::Style::Titlebar | sf::Style::Close));

	if (pCfg->m_bVSync) {
		CDebugLogger::LogInfo("VSync Enabled");
		m_Window.setVerticalSyncEnabled(true);
	}
	else if (pCfg->m_bLimitFPS) {
		CDebugLogger::LogInfo("FPS Limited to %i\n", pCfg->m_iFPSCountLimit);
		m_Window.setFramerateLimit(pCfg->m_iFPSCountLimit);
	}

	// Initialize view size
	Resize(m_vSize);

	m_bWindowCreated = true;
}

void CWindowManager::ReCreate(CSettings* pSettings, char* szTitle) {
	if (!m_bWindowCreated)
		return;
	m_bWindowCreated = false;

	m_Window.close();
	Create(pSettings, szTitle);
}

void CWindowManager::HandleEvents(CInput* pInput) {
	sf::Event e;
	while (m_Window.pollEvent(e)) {

		// TGUI Returns TRUE if event was handled
		if (HasFocus() && IsGUIActive() && m_pGUI && m_pGUI->handleEvent(e)) {
			continue;
		}

		switch (e.type) {

		case sf::Event::Closed:
			m_bWindowClosed = true; break;

		case sf::Event::GainedFocus:
			m_bFocus = true; break;

		case sf::Event::LostFocus:
			m_bFocus = false; break;

		case sf::Event::Resized: 
			Resize(sf::Vector2i(e.size.width, e.size.height)); 
			break;

		case sf::Event::MouseMoved:
			if (HasFocus()) {
				pInput->InjectMouseMove(e.mouseMove.x, e.mouseMove.y);
			}
			break;

		case sf::Event::MouseButtonPressed:
			if (HasFocus()) {
				pInput->InjectMousePress(e.mouseButton);
			}
			break;

		case sf::Event::MouseButtonReleased:
			if (HasFocus()) {
				pInput->InjectMouseRelease(e.mouseButton);
			}
			break;

		case sf::Event::KeyPressed:
			if (HasFocus()) {
				pInput->InjectKeyPress(e.key);
			}
			break;
		}
	}
}

void CWindowManager::Begin() {
	m_Window.clear();
}
void CWindowManager::DrawFPS() {
	// Render FPS
	m_FPS.Update();

	int fps = (int)m_FPS.GetFPS();
	if (m_LastFPS != fps) {
		m_FPSText.setString(std::string("FPS: ") + std::to_string(fps));
		m_LastFPS = fps;
	}
	Draw(&m_FPSText);
}
void CWindowManager::Present() {
	// Draw shaders 
	m_Shaders.ApplyShaders(m_Window);

	// Render GUI if active 
	if (IsGUIActive()) {
		m_pGUI->draw();
	}
	DrawFPS();

	m_Window.display();
}

void CWindowManager::CloseWindow() {
	m_Window.close();
	m_bWindowClosed = true;
}
bool CWindowManager::WindowClosed() {
	return m_bWindowClosed;
}
 
bool CWindowManager::HasFocus() {
	return m_bFocus;
}
sf::Vector2i CWindowManager::GetScreenCentre() {
	return m_vSize / 2;
}
sf::Vector2f CWindowManager::GetScreenCentre2f() {
	return sf::Vector2f(m_vSize) / 2.0f;
}
sf::Vector2i CWindowManager::GetScreenDimensions() {
	return m_vSize;
}
sf::Vector2f CWindowManager::GetScreenDimensions2f() {
	return sf::Vector2f(m_vSize);
}
sf::Vector2f CWindowManager::GetViewCentre() {
	return m_Window.getView().getCenter(); 
}
void CWindowManager::Draw(sf::Drawable* Item) {
	m_Window.draw(*Item);
}
void CWindowManager::DrawVertices(sf::Vertex* vertices, int count, sf::PrimitiveType type) {
	m_Window.draw(vertices, count, type);
}
void CWindowManager::RestoreDefaultCamera() {
	m_Window.setView(m_Window.getDefaultView());
}
void CWindowManager::MoveCamera(sf::Vector2f vCentre) {
	m_Window.setView(sf::View(vCentre, GetScreenDimensions2f()));
}	
sf::RenderWindow* CWindowManager::GetWindow() {
	return &m_Window;
}

sf::Vector2i CWindowManager::ScreenToWorld(sf::Vector2i screenPos) {
	return sf::Vector2i(m_Window.mapPixelToCoords(screenPos));
}

void CWindowManager::SetGUI(tgui::Gui* pGUI) {
	m_pGUI = pGUI;
}

void CWindowManager::RemoveGUI() {
	SetGUIActive(false);
	m_pGUI = NULL;
}

void CWindowManager::Resize(sf::Vector2i vSize) {
	m_Window.setView(sf::View(
		m_Window.getView().getCenter(), // Maintain camera position
		sf::Vector2f(vSize) // Change window size
	));
}

void CWindowManager::SetGUIActive(bool bActive) {
	m_bGUIActive = bActive;
}

void CWindowManager::ToggleGUI() {
	m_bGUIActive = !m_bGUIActive;
}

bool CWindowManager::IsGUIActive() {
	return m_bGUIActive;
}