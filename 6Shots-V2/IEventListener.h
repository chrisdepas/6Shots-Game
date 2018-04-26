#ifndef __IEVENTLISTENER_H__
#define __IEVENTLISTENER_H__

/*Michael De Pasquale 
17-3-16*/

#include <string>

class CGame;
class IEventListener {
public:
	/* Chat message */
	struct SChatEvent {
		std::string* user;
		std::string* message;
	};

	virtual void OnEvent(CGame* pGame, std::string evtName, void* data, size_t len) = 0;
};

#endif