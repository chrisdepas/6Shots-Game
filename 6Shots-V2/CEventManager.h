#ifndef __CEVENTMANAGER_H__
#define __CEVENTMANAGER_H__

/*Michael De Pasquale
17-3-16
Modifications by Christopher De Pasquale
*/

#include "IEventListener.h"
#include <string>
#include <stdlib.h>
#include <map>
#include <vector>

class CGame;
class CEventManager {
	
	std::map<std::string, std::vector<IEventListener*>> m_Listeners;
	
public:

	CEventManager() {}

	bool RegisterListener(IEventListener* listener, std::string eventName);

	void DeregisterListener(IEventListener* listener, std::string eventName);

	void DeregisterAll(IEventListener* listener);

	void RaiseEvent(CGame* pGame, std::string eventName, void* data, size_t len);

};

#endif