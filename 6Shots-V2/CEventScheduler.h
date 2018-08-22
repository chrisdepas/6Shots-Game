#ifndef __CEVENTSCHEDULER_H__
#define __CEVENTSCHEDULER_H__

/*
	Simple event scheduler
	30 Jan 2017
	Chris De Pasquale 
*/

class CEventScheduler {

public:
	struct SScheduledEvent {
		std::function<void()> pEventFn;
		float fEventTimeRemaining;

		SScheduledEvent() { fEventTimeRemaining = 0;  }
		SScheduledEvent(std::function<void()> eventFn, float fTime) {
			pEventFn = eventFn;
			fEventTimeRemaining = fTime;
		}
	};

protected:
	std::vector<SScheduledEvent> m_vScheduledEvent;

public:
	CEventScheduler();

	// Schedule an event function with any args.
	// Args will be bound, changing args can be passed by reference using 
	// std::ref. Example:
	//	ScheduleEvent(0.5s, CThisClass::Function, this, std::ref(numCows));
	template <typename Func, typename... Args>
	void ScheduleEvent(float fTime, Func func, Args... args) {
		std::function<void()> fn = std::bind(func, args...);
		m_vScheduledEvent.push_back(SScheduledEvent(fn, fTime));
	}

	// Update scheduled events, requires time since last frame as an arg
	void Update(float fFrameTime);

	// Clear remaining scheduled events
	void Clear();
};

#endif