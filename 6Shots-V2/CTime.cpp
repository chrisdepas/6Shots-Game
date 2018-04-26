#include "stdafx.h"
#include "CTime.h"

#include <time.h>
CTime::CTime()
{
	m_totalClock.restart();
	m_elapsedClock.restart();
}
float CTime::Time() {
	return m_totalClock.getElapsedTime().asSeconds();
}

void CTime::ResetFrameTime() {
	m_elapsedClock.restart();
}

float CTime::LastFrameTime() {
	return m_elapsedClock.restart().asSeconds();
}