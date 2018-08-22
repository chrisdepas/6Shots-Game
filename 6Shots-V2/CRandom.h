#ifndef __CRANDOM_H__
#define __CRANDOM_H__

class CRandom {
	std::mt19937 m_RandEngine{ std::random_device{}() };

public:
	CRandom() = default;
	CRandom(std::mt19937::result_type seed) { m_RandEngine.seed(seed); }

	// Random integer from 0 to INT_MAX
	int RandomInt();

	// Random integer from iMin to iMax inclusive 
	int RandomInt(int iMin, int iMax);
	
	// Random integer from 0 to iMax inclusive 
	int RandomInt(int iMax);

	// Random float from fMin to fMax inclusive 
	float RandomFloat(float fMin, float fMax);
};

#endif