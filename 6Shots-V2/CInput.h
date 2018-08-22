#ifndef __CINPUT_H__
#define __CINPUT_H__

#include "Actions.h"

class CGame;
class CWindowManager;

class CInput
{
public:
	
	// Eventually this should handle both controller and mouse input
	struct SCursorEvent : public sf::Event::MouseButtonEvent {
		bool m_bRelease;
		SCursorEvent(sf::Event::MouseButtonEvent event, bool bRelease) {
			button = event.button;
			x = event.x;
			y = event.y;
			m_bRelease = bRelease;
		}
		SCursorEvent() {}
		sf::Vector2i Position2i() { return sf::Vector2i(x, y); }
		sf::Vector2f Position2f() { return sf::Vector2f((float)x, (float)y); }
	};

private:
	bool m_bInitialised = false;
	std::map<sf::Keyboard::Key, EAction> m_KeyMap;
	std::map<sf::Keyboard::Key, EAction> m_DefaultMap;
	sf::Vector2i m_vMousePosition = sf::Vector2i(0, 0);
	std::vector<SCursorEvent> m_vCursorEvents;
	std::vector<sf::Event::KeyEvent> m_vKeyEvents;

	bool m_ButtonStates[sf::Mouse::ButtonCount];

	/* Set up the default key bindings (m_DefaultMap)*/
	void LoadDefaultKeys();

public: 
	CInput() { 	
		ClearMouseStates(); 
	}

	/* Init defaults */
	void Initialise();

	/* Clear event input buffer (Unparsed events for window), and optionally the current event queue (Already parsed events)*/
	void FlushInput(CWindowManager* pWindowMgr, bool bClearEventQueue=true);

	/* Read in / Save key bindings from file */ 
	void LoadBindings(char* szFileName);
	void SaveBindings(char* szFileName);

	/* Should be called by window loop */
	void InjectMousePress(sf::Event::MouseButtonEvent ev);
	void InjectMouseRelease(sf::Event::MouseButtonEvent ev);
	void InjectMouseMove(int x, int y);
	void InjectMouseWheel(int delta);
	void InjectKeyPress(sf::Event::KeyEvent ev);
	void LostFocus();

	/* Get current mouse position */
	sf::Vector2i GetMousePosition();

	/* Get next mouse event in queue */
	bool GetNextCursorEvent(CInput::SCursorEvent& cursorEvent);

	/* Get next key in queue */
	bool GetNextKeyEvent(sf::Event::KeyEvent& cursorEvent);

	/* Check for mouse events */
	bool HasCursorEvents() { return !m_vCursorEvents.empty(); }

	/* Check if the key for a specific action is pressed */
	bool ActionKeyPressed(EAction Action);

	/* Check if a mouse button is pressed */
	bool MouseButtonPressed(sf::Mouse::Button button);
	bool LeftMousePressed() { return MouseButtonPressed(sf::Mouse::Left); }
	bool MiddleMousePressed() { return MouseButtonPressed(sf::Mouse::Middle); }
	bool RightMousePressed() { return MouseButtonPressed(sf::Mouse::Right); }

	/* Set all mouse button states to not-pressed*/
	void CInput::ClearMouseStates() { memset(m_ButtonStates, 0, sizeof(m_ButtonStates)); }
};

#endif