#include "stdafx.h"
#include "FPSCounter.h"

FPSCounter::FPSCounter() {
	m_Clock.restart();
	m_fLastTime = m_Clock.getElapsedTime().asSeconds();
}

void FPSCounter::Update() {
	float curTime = m_Clock.getElapsedTime().asSeconds();

	// Calculate weighted moving average of frame delta time
	// currentAverage = weight * frameDelta + (1-weight) * currentAverage
	m_fAvg = m_fWeight * (curTime - m_fLastTime) + (m_fCoWeight * m_fAvg);

	// m_fAvg contains seconds per frame, convert to frames per second
	m_fLastTime = curTime;
}

float FPSCounter::GetFPS() {
	return 1 / m_fAvg;
}

int FPSCounter::GetFPSInt() {
	return (int)(1.0f / m_fAvg);
}