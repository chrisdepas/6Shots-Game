#ifndef __CRANDOM_H__
#define __CRANDOM_H__

#include <chrono>
#include <random>
class CRandom {
	std::mt19937 m_twister;
public:
	CRandom();
	int RandomInt();
	int RandomInt(int iMin, int iMax);
	float RandomFloat(float fMin, float fMax);
};

#endif