#include "stdafx.h"
#include "CInput.h"
#include "CGame.h"

void CInput::LoadDefaultKeys()
{   
	/* Menu buttons */
	m_DefaultMap.insert(std::make_pair(sf::Keyboard::BackSlash, Action_Show_Chat));
	m_DefaultMap.insert(std::make_pair(sf::Keyboard::F1, Action_Console_Toggle)); 
	m_DefaultMap.insert(std::make_pair(sf::Keyboard::Escape, Action_Menu_Toggle)); // Escape = Action_Menu_Toggle 
//	m_DefaultMap.insert(std::make_pair(sf::Keyboard::Return, Action_Menu_Select)); // Return = Action_Menu_Select
//	m_DefaultMap.insert(std::make_pair(sf::Keyboard::Up, Action_Menu_Up));		   // Up 
//	m_DefaultMap.insert(std::make_pair(sf::Keyboard::Down, Action_Menu_Down));     // down 

	/* Movement buttons */
	m_DefaultMap.insert(std::make_pair(sf::Keyboard::A, Action_Move_Left));		// LeftArrow = move left 
	m_DefaultMap.insert(std::make_pair(sf::Keyboard::D, Action_Move_Right));	// RightArrow = move right
	m_DefaultMap.insert(std::make_pair(sf::Keyboard::W, Action_Jump));			// , = move Up 
	m_DefaultMap.insert(std::make_pair(sf::Keyboard::S, Action_Duck));	// . = move down
	m_DefaultMap.insert(std::make_pair(sf::Keyboard::LShift, Action_Sprint));
	 
	/* zoom */
//	m_DefaultMap.insert(std::make_pair(sf::Keyboard::Add, Action_Zoom_In));
//	m_DefaultMap.insert(std::make_pair(sf::Keyboard::Subtract, Action_Zoom_Out));

	/* Action buttons */
//	m_DefaultMap.insert(std::make_pair(sf::Keyboard::G, Action_Grab));				// G = Grab
//	m_DefaultMap.insert(std::make_pair(sf::Keyboard::E, Action_Eat));				// E = eat
//	m_DefaultMap.insert(std::make_pair(sf::Keyboard::Q, Action_Equip));				// Q = eQuip
//	m_DefaultMap.insert(std::make_pair(sf::Keyboard::Period, Action_Wait));			// . = wait

//	m_DefaultMap.insert(std::make_pair(sf::Keyboard::F, Action_Fire));			// F = Fire
}

void CInput::Initialise()
{
	if (!m_bInitialised) {
		LoadDefaultKeys();
		m_bInitialised = true;
	}
}
void CInput::FlushInput(CWindowManager* pWindowMgr, bool bClearEventQueue) {
	sf::RenderWindow* window = pWindowMgr->GetWindow();
	sf::Event event;
	while (window->pollEvent(event));
	if (bClearEventQueue) m_vCursorEvents.clear();
}

void CInput::LoadBindings(char* szFileName) {
}

void CInput::SaveBindings(char* szFileName) {
}

void CInput::InjectMouseMove(int x, int y) { 
	m_vMousePosition = sf::Vector2i(x, y);
}

void CInput::InjectKeyPress(sf::Event::KeyEvent ev) {
	m_vKeyEvents.push_back(ev);
}

bool CInput::GetNextCursorEvent(CInput::SCursorEvent& cursorEvent) {
	if (m_vCursorEvents.empty()) {
		return false;
	}
	cursorEvent = m_vCursorEvents.back();
	m_vCursorEvents.pop_back();
	return true;
}
bool CInput::GetNextKeyEvent(sf::Event::KeyEvent& cursorEvent) {
	if (m_vKeyEvents.empty()) {
		return false;
	}
	cursorEvent = m_vKeyEvents.back();
	m_vKeyEvents.pop_back();
	return true;
}
sf::Vector2i CInput::GetMousePosition() {
	return m_vMousePosition;
}

bool CInput::ActionKeyPressed(EAction Action) {
	std::map< sf::Keyboard::Key, EAction >::iterator it;

	for (it = m_KeyMap.begin(); it != m_KeyMap.end(); it++) {
		if (it->second == Action) {
			return sf::Keyboard::isKeyPressed(it->first);
		}
	}

	for (it = m_DefaultMap.begin(); it != m_DefaultMap.end(); it++) {
		if (it->second == Action) {
			return sf::Keyboard::isKeyPressed(it->first);
		}
	}

	return false;
}

bool CInput::MouseButtonPressed(sf::Mouse::Button button) {
	if (button < sf::Mouse::Left || button > sf::Mouse::ButtonCount) {
		CDebugLogger::LogError("Recieved unknown mouse button ID %i, out of expected range [%i, %i)",
			button, sf::Mouse::Left, sf::Mouse::ButtonCount);
		return false;
	}

	return m_ButtonStates[button];
}


void CInput::InjectMousePress(sf::Event::MouseButtonEvent ev) {
	if (ev.button < sf::Mouse::Left || ev.button > sf::Mouse::ButtonCount) {
		CDebugLogger::LogError("Recieved unknown mouse button ID %i, out of expected range [%i, %i)", 
			ev.button, sf::Mouse::Left, sf::Mouse::ButtonCount);
		return;
	}

	// Change state, and insert Mouse event
	m_ButtonStates[ev.button] = true;
	m_vCursorEvents.push_back(SCursorEvent(ev, false));
}

void CInput::InjectMouseRelease(sf::Event::MouseButtonEvent ev) {
	if (ev.button < sf::Mouse::Left || ev.button > sf::Mouse::ButtonCount) {
		CDebugLogger::LogError("Recieved unknown mouse button ID %i, out of expected range [%i, %i)",
			ev.button, sf::Mouse::Left, sf::Mouse::ButtonCount);
		return;
	}
	
	// Change state, and insert mouse event 
	m_ButtonStates[ev.button] = false;
	m_vCursorEvents.push_back(SCursorEvent(ev, true));
}