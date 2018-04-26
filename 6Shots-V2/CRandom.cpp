#include "stdafx.h"
#include "CRandom.h"

CRandom::CRandom() {
	m_twister.seed((unsigned long)std::chrono::system_clock::now().time_since_epoch().count());
}

int CRandom::RandomInt() {
	return m_twister();
}

float CRandom::RandomFloat(float fMin, float fMax) {
	double step = (((double)RandomInt() / (double)m_twister.max()) + 1.0f) / 2.0f;
	float stepmax = fMax - fMin;
	return (float)(step*stepmax + fMin);
}
int CRandom::RandomInt(int iMin, int iMax) {
	double step = (((double)RandomInt() / (double)m_twister.max()) + 1.0f) / 2.0f;
	int stepmax = iMax - iMin;
	return (int)(step*stepmax + iMin);
}