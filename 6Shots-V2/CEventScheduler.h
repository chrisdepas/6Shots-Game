#ifndef __CEVENTSCHEDULER_H__
#define __CEVENTSCHEDULER_H__

/*
	Simple event scheduler
	30 Jan 2017
	Chris De Pasquale 
*/

#include <vector>
#include <functional>

class CEventScheduler {
public:
	struct SScheduledEvent {
		std::function<void()> pEventFn;
		float fEventTimeRemaining;

		SScheduledEvent() { fEventTimeRemaining = 0; }
		SScheduledEvent(std::function<void()> eventFn, float fTime) {
			pEventFn = eventFn;
			fEventTimeRemaining = fTime;
		}
	};
protected:
	std::vector<SScheduledEvent> m_vScheduledEvent;
public:
	CEventScheduler();

	/* Schedule an event function */
	template <typename Func, typename... Args>
	void ScheduleEvent(float fTime, Func func, Args... args) {
		std::function<void()> fn = std::bind(func, args...);
		m_vScheduledEvent.push_back(SScheduledEvent(fn, fTime));
	}

	/* Update scheduler */
	void Update(float fFrameTime);
};

#endif