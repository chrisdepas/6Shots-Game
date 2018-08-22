#include "stdafx.h"
#include "CRandom.h"

int CRandom::RandomInt() {
	return m_RandEngine();
}

float CRandom::RandomFloat(float fMin, float fMax) {
	// Overhead of std::uniform_real_distribution is minimal
	return std::uniform_real_distribution<float> {fMin, fMax}(m_RandEngine);
}
int CRandom::RandomInt(int iMin, int iMax) {
	// Overhead of std::uniform_int_distribution is minimal
	return std::uniform_int_distribution<int> {iMin, iMax}(m_RandEngine);
}

int CRandom::RandomInt(int iMax) {
	return RandomInt(0, iMax);
}