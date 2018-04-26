#ifndef __CTIME_H__
#define __CTIME_H__

#include "SFML\System\Clock.hpp"
class CTime
{
	sf::Clock m_totalClock;
	sf::Clock m_elapsedClock;
public:
	CTime();
	float Time();
	float LastFrameTime();
	void ResetFrameTime();
};

#endif
