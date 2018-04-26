#ifndef __CINPUT_H__
#define __CINPUT_H__

#include <vector>
#include <map>
#include "Vector.h"
#include <SFML/Window.hpp>
#include "Actions.h"

class CGame;
class CWindowManager;
class CInput
{
public:
	struct SCursorEvent { // Eventually this should handle both controller and mouse input
		bool m_bRelease;
		sf::Mouse::Button m_button;
		Vector2i m_vPosition;
		SCursorEvent(sf::Mouse::Button button, Vector2i position, bool bRelease) {
			m_button = button;
			m_vPosition = position;
			m_bRelease = bRelease;
		}
		SCursorEvent() {}
	};

private:
	bool m_bInitialised;
	std::map< sf::Keyboard::Key, EAction > m_KeyMap;
	std::map< sf::Keyboard::Key, EAction > m_DefaultMap;
	Vector2i m_vMousePosition;
	std::vector< SCursorEvent > m_vCursorEvents;

	bool m_ButtonStates[sf::Mouse::ButtonCount];

	/* Set up the default key bindings (m_DefaultMap)*/
	void LoadDefaultKeys();

public: 
	CInput() { m_bInitialised = false; m_vMousePosition.X = m_vMousePosition.Y = 0; ClearMouseStates(); }
	~CInput();

	/* Init defaults */
	void Initialise();

	/* Clear event input buffer (Unparsed events for window), and optionally the current event queue (Already parsed events)*/
	void FlushInput(CWindowManager* pWindowMgr, bool bClearEventQueue=true);

	/* Read in / Save key bindings from file */ 
	void LoadBindings(char* szFileName);
	void SaveBindings(char* szFileName);

	/* Should be called by window loop */
	void InjectMousePress(int x, int y, sf::Mouse::Button Button);
	void InjectMouseRelease(int x, int y, sf::Mouse::Button Button);
	void InjectMouseMove(int x, int y);
	void InjectMouseWheel(int delta);
	void LostFocus();

	/* Get current mouse position */
	Vector2i GetMousePosition();

	/* Get next mouse event in queue */
	bool GetNextCursorEvent(CInput::SCursorEvent& cursorEvent);
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

	/* Blocks until a character is pressed, returns it in __char_out. returns false on ESC press */
	bool GetChar(char& __char_out, CGame* pGame);
};

#endif