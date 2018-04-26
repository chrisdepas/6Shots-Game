#ifndef __CTICKRATE_H__
#define __CTICKRATE_H__

#include "SFML\System\Clock.hpp"
class CTickRate {
	bool m_bInit;
	float m_fTickRate; 
	sf::Clock m_TickClock;
public:
	void Reset();
	CTickRate(int ticksPerSecond);
	CTickRate();
	void Init(int ticksPerSecond);
	bool ShouldSendTick();
};
#endif