#ifndef __CSHADEREFFECT_H__
#define __CSHADEREFFECT_H__

class CGame;

class CShaderEffect {

	bool m_bEnabled = true;

protected:

public:

	sf::Shader m_Shader;
	sf::RenderStates m_RenderStates = sf::RenderStates::Default;
	bool IsEnabled() { return m_bEnabled; }

	void InitRenderState() {
		m_RenderStates.shader = &m_Shader;
	}

	virtual void ApplyEffect(sf::Texture& windowTexture, sf::RenderWindow& window, sf::IntRect& scrRect) {
		window.draw(sf::Sprite(windowTexture, scrRect), m_RenderStates);
	}

	void UpdateTime(float fElapsed) {
		m_Shader.setUniform("g_fGameTime", fElapsed);
	}
	
	void EnableEffect(bool bEnable = true) { m_bEnabled = bEnable; }

	void DisableEffect() { m_bEnabled = false; }
};

class CTempShaderEffect : public CShaderEffect {
	float m_fLifetime;
	float m_fCreateTime = -1;
	float m_fDeathTime;
public:

	CTempShaderEffect(float fLifetime) {
		m_fLifetime = fLifetime;
	}

	bool HasExpired(float fTime) { 
		return fTime >= m_fDeathTime; 
	}
	
	void UpdateTime(float fElapsed) {
		if (m_fCreateTime < 0) {
			m_fCreateTime = fElapsed;
			m_fDeathTime = m_fCreateTime + m_fLifetime;
		}
		m_Shader.setUniform("g_fGameTime", fElapsed);
		m_Shader.setUniform("g_fExpiry", m_fDeathTime);
	}
};

#endif