#ifndef __FPS_COUNTER_H__
#define __FPS_COUNTER_H__


class FPSCounter {
	sf::Clock m_Clock;
	float m_fLastTime = 0.0f;

	// Weight applied to new values, Current moving average with initial guess
	const float m_fWeight = 0.05f;
	float m_fAvg = 30.0f;

	// Wegh
	const float m_fCoWeight = 1.0f - m_fWeight;

public:
	FPSCounter();

	void Update();
	float GetFPS();
	int GetFPSInt();
};


#endif