#include "stdafx.h"
#include "CEventManager.h"

/*Michael De Pasquale
17-3-16*/

bool CEventManager::RegisterListener(IEventListener* listener, std::string eventName){
	/*Check if listener already exists*/
	for (std::vector<IEventListener*>::iterator it = m_Listeners[eventName].begin();
		it != m_Listeners[eventName].end(); it++){
		
		if (listener == *it)
			return true;
	}

	m_Listeners[eventName].push_back(listener);

	return true;
}

void CEventManager::DeregisterListener(IEventListener* listener, std::string eventName){
	/*Check if event exists*/
	auto it = m_Listeners.find(eventName);

	if (it == m_Listeners.end())
		return;

	for (std::vector<IEventListener*>::iterator it = m_Listeners[eventName].begin();
		it != m_Listeners[eventName].end(); it++){

		if (listener == *it){
			m_Listeners[eventName].erase(it);
			break;
		}
	}

	/*Erase map if listener vector is empty*/
	if (m_Listeners[eventName].size() == 0){
		it = m_Listeners.find(eventName);
		m_Listeners.erase(it);
	}
}

void CEventManager::RaiseEvent(CGame* pGame, std::string eventName, void* data, size_t len){  
	/*Check if event exists*/
	auto it = m_Listeners.find(eventName);

	if (it == m_Listeners.end())
		return;

	for (std::vector<IEventListener*>::iterator it = m_Listeners[eventName].begin();
		it != m_Listeners[eventName].end(); it++){

		IEventListener* pEvt = *it;
		pEvt->OnEvent(pGame, eventName, data, len);
	}
}