#include "stdafx.h"
#include "CEventScheduler.h"

CEventScheduler::CEventScheduler() {

}

bool DeadEvent(CEventScheduler::SScheduledEvent event) {
	return event.fEventTimeRemaining <= 0;
}

void CEventScheduler::Update(float fFrameTime) {
	/* Update timing */
	bool bRemoveEvent = false;
	for (unsigned int i = 0; i < m_vScheduledEvent.size(); i++) {
		m_vScheduledEvent[i].fEventTimeRemaining -= fFrameTime;
		if (m_vScheduledEvent[i].fEventTimeRemaining <= 0) {
			if (m_vScheduledEvent[i].pEventFn != NULL)
				m_vScheduledEvent[i].pEventFn();
			bRemoveEvent = true;
		}
	}

	/* Remove dead events */
	if (bRemoveEvent) {
		m_vScheduledEvent.erase(std::remove_if(m_vScheduledEvent.begin(), m_vScheduledEvent.end(), DeadEvent), m_vScheduledEvent.end());
	}
}

void CEventScheduler::Clear() {
	m_vScheduledEvent.clear();
}