#include "stdafx.h"
#include "CTickRate.h"

CTickRate::CTickRate() {
	m_bInit = false;
}
CTickRate::CTickRate(int ticksPerSecond) {
	m_bInit = true;
	m_fTickRate = 1.0f / (float)ticksPerSecond; // Seconds per tick
	Reset();
}

void CTickRate::Reset() {
	m_TickClock.restart();
}

bool CTickRate::ShouldSendTick() {
	if (!m_bInit)
		return false;
	float time = m_TickClock.getElapsedTime().asSeconds();
	if (time > m_fTickRate) {
		m_TickClock.restart();
		return true;
	}
	return false;
}
void CTickRate::Init(int tps) {
	m_bInit = true; 
	m_fTickRate = 1.0f / (float)tps;
	Reset();
}