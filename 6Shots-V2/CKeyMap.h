#ifndef __CKEYMAP_H__
#define __CKEYMAP_H__


enum Action {
	INVALID = -1, // Bad/Unknown Action
	FIRST = 0, // Keep this after INVALID

	// Weapon interactions 
	ATTACK_PRIM,
	ATTACK_ALT,
	ATTACK_RELOAD,
	ATTACK_THROW,

	// Weapon selection
	WEAPON_SWAP,
	WEAPON_1,
	WEAPON_2,
	WEAPON_3,
	WEAPON_4,
	WEAPON_NEXT,
	WEAPON_PREV,

	// Player Movement
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_DOWN,
	MOVE_JUMP,
	MOVE_DUCK,
	MOVE_SPRINT,

	// Menu actions
	MENU_TOGGLE,
	CONSOLE_TOGGLE,
	CHAT_TOGGLE,
	QUIT_TO_DESKTOP,

	LAST // Keep this last
};

class CKeyMap {
	std::map<sf::Keyboard::Key, Action> m_KeyMap;
	std::map<sf::Keyboard::Key, Action> m_DefaultMap;

public:


};

#endif