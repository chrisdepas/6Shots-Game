#ifndef __CSHADEREFFECT_H__
#define __CSHADEREFFECT_H__

#include <SFML\Graphics\Shader.hpp>
class CGame;

class CShaderEffect {
	bool m_bEnabled = true;
	sf::Shader shader;

public:
	virtual void Init(CGame* pGame);
	virtual void ApplyEffect();
	
	void EnableEffect(bool bEnable = true) { m_bEnabled = bEnable; }
	void DisableEffect() { m_bEnabled = false; }
};

#endif