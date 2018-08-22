#ifndef __CSHADERMANAGER_H__
#define __CSHADERMANAGER_H__
#include "CShaderEffect.h"

class CShaderManager {
	std::vector<CShaderEffect*> m_ShaderEffects;
	int m_iLastID = 0;
	sf::Clock m_Clock;

public:
	bool ActiveShaders() {
		if (!sf::Shader::isAvailable())
			return false;

		for (unsigned int i = 0; i < m_ShaderEffects.size(); i++) {
			if (m_ShaderEffects[i]->IsEnabled())
				return true;
		}
		return false;
	}

	void AddShader(CShaderEffect* pEffect) {
		m_ShaderEffects.push_back(pEffect);
	}

	void ApplyShaders(sf::RenderWindow& window) {
		sf::Vector2u v = window.getSize();
		sf::IntRect scrRect(0, 0, v.x, v.y);

		// Copy window into buffer, to redraw with shader
		sf::Texture bufferTexture;
		bufferTexture.create(v.x, v.y);
		bufferTexture.update(window);

		for (unsigned int i = 0, length = m_ShaderEffects.size(); i < length; i++) {
			if (m_ShaderEffects[i]->IsEnabled()) {
				m_ShaderEffects[i]->UpdateTime(m_Clock.getElapsedTime().asSeconds());
				m_ShaderEffects[i]->ApplyEffect(bufferTexture, window, scrRect);
				if (i + 1 < length) {
					bufferTexture.update(window);
				}
			}
		}
	}
};

#endif