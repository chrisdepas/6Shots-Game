#include "stdafx.h"
#include "CEventScheduler.h"

#include <algorithm>
CEventScheduler::CEventScheduler() {

}
/*
void CEventScheduler::ScheduleEvent(float fTime, ScheduledEventFn_t pEventFn) {
	if (!pEventFn)
		return;
	m_vScheduledEvent.push_back(SScheduledEvent(pEventFn, fTime));
}*/
bool DeadEvent(CEventScheduler::SScheduledEvent event) {
	return event.fEventTimeRemaining <= 0;
}
void CEventScheduler::Update(float fFrameTime) {
	/* Update timing */
	bool bRemoveEvent = false;
	for (unsigned int i = 0; i < m_vScheduledEvent.size(); i++) {
		m_vScheduledEvent[i].fEventTimeRemaining -= fFrameTime;
		if (m_vScheduledEvent[i].fEventTimeRemaining <= 0) {
			m_vScheduledEvent[i].pEventFn();
			bRemoveEvent = true;
		}
	}

	/* Remove dead events */
	if (bRemoveEvent) {
		m_vScheduledEvent.erase(std::remove_if(m_vScheduledEvent.begin(), m_vScheduledEvent.end(), DeadEvent), m_vScheduledEvent.end());
	}
}