#ifndef __ACTIONS_H__
#define __ACTIONS_H__

enum EAction {
	Action_None,
	Action_Move_Left,
	Action_Move_Right,
	Action_Jump,
	Action_Duck,
	Action_Menu_Toggle,
	Action_Console_Toggle,
	Action_Sprint,
	Action_Show_Chat,
	Action_Last, // LEAVE THIS AS THE LAST ACTION OR GAME WON'T FUNCTION
};

#endif