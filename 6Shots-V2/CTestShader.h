#ifndef __CTESTSHADER_H__
#define __CTESTSHADER_H__

#include "CShaderEffect.h"
#include <Windows.h>

class CTestShader : public CShaderEffect {
public:
	CTestShader() {
		printf("Loading test shader...");
		printf(m_Shader.loadFromFile("default.6svert", "test.6sfrag") ? "OK\n" : "FAIL\n");
		InitRenderState();
	}
	void UpdateTestShader() {

	}
};

class CBloomTest : public CShaderEffect {
	sf::Shader m_ShaderBlurStage1;
	sf::Shader m_ShaderBlurStage2;
	sf::Shader m_ShaderComposite;
	sf::RenderStates m_StateBlurStage1;
	sf::RenderStates m_StateBlurStage2;
	sf::RenderStates m_StateComposite;
public:
	CBloomTest() {
		printf("Loading bloom shaders...");
		printf(m_Shader.loadFromFile("default.6svert", "bloom1.6sfrag") ? "OK  " : "FAIL ");
		printf(m_ShaderBlurStage1.loadFromFile("default.6svert", "bloom2.6sfrag") ? "OK  " : "FAIL ");
		printf(m_ShaderBlurStage2.loadFromFile("default.6svert", "bloom3.6sfrag") ? "OK  " : "FAIL ");
		printf(m_ShaderComposite.loadFromFile("default.6svert", "bloom4.6sfrag") ? "OK  " : "FAIL ");
		printf("\n");
		InitRenderState();
		m_StateBlurStage1.shader = &m_ShaderBlurStage1;
		m_StateBlurStage2.shader = &m_ShaderBlurStage2;
		m_StateComposite.shader = &m_ShaderComposite;
	}
	void ApplyEffect(sf::Texture& windowTexture, sf::RenderWindow& window, sf::IntRect& scrRect) {
		sf::Texture screenData(windowTexture);
		window.draw(sf::Sprite(windowTexture, scrRect), m_RenderStates);
		windowTexture.update(window);
		window.draw(sf::Sprite(windowTexture, scrRect), m_StateBlurStage1);
		windowTexture.update(window);
		window.draw(sf::Sprite(windowTexture, scrRect), m_StateBlurStage2);
		windowTexture.update(window);
		window.draw(sf::Sprite(windowTexture, scrRect), m_StateBlurStage1);
		windowTexture.update(window);
		window.draw(sf::Sprite(windowTexture, scrRect), m_StateBlurStage2);
		windowTexture.update(window);

		m_ShaderComposite.setUniform("bloomImage", windowTexture);
		window.draw(sf::Sprite(screenData, scrRect), m_StateComposite);

		//windowTexture.update(window);
		//window.draw(sf::Sprite(windowTexture, scrRect), m_BlurState);
		//windowTexture.update(window);
		//window.draw(sf::Sprite(windowTexture, scrRect), m_BlurState);
	}
};


class CRaymarch : public CShaderEffect {
public:
	CRaymarch() {
		printf("Loading raymarch shader...");
		printf(m_Shader.loadFromFile("default.6svert", "raymarch.6sfrag") ? "OK  " : "FAIL ");
		printf("\n");
		InitRenderState();
	}
	void ApplyEffect(sf::Texture& windowTexture, sf::RenderWindow& window, sf::IntRect& scrRect) {
		if (GetAsyncKeyState(VK_F1)) {

			printf(m_Shader.loadFromFile("default.6svert", "raymarch.6sfrag") ? "OK  " : "FAIL ");
		}
		m_Shader.setUniform("g_vResolution", sf::Glsl::Vec2(window.getSize()));
		window.draw(sf::Sprite(windowTexture, scrRect), m_RenderStates);
	}
};

class CMultipassTest : public CShaderEffect {
	sf::Shader m_Shader2;
	sf::RenderStates m_RenderStates2 = sf::RenderStates::Default;
	sf::Shader m_Shader3;
	sf::RenderStates m_RenderStates3 = sf::RenderStates::Default;
public:
	void ApplyEffect(sf::Texture& windowTexture, sf::RenderWindow& window, sf::IntRect& scrRect) {
			sf::Texture screenData(windowTexture);
				// blur to seperate texture
				//window.draw(sf::Sprite(windowTexture, scrRect), m_RenderStates2);
				//windowTexture.update(window);

				// Edge detection to render outlines
			//	m_Shader.setParameter("blurredImage", windowTexture);
				//window.draw(sf::Sprite(screenData, scrRect), m_RenderStates);
				// draw bright parts to seperate texture
				window.draw(sf::Sprite(windowTexture, scrRect), m_RenderStates3);           
				//windowTexture.update(window);

				// blur texture with bright parts seperate texture
				//window.draw(sf::Sprite(windowTexture, scrRect), m_RenderStates2);
				//windowTexture.update(window);

				// combine to provide image with bloom
				//m_Shader.setParameter("blurredImage", windowTexture);
				//window.draw(sf::Sprite(screenData, scrRect), m_RenderStates);

		if (GetAsyncKeyState(VK_F1)) {
			m_Shader.loadFromFile("default.6svert", "edgeDetection.6sfrag");
			m_Shader2.loadFromFile("default.6svert", "test2.6sfrag");
		}
		if (GetAsyncKeyState(VK_F2)) {
			//m_Shader.loadFromFile("default.6svert", "bloomStage3.6sfrag");
			//m_Shader2.loadFromFile("default.6svert", "bloomStage2.6sfrag");
			m_Shader3.loadFromFile("default.6svert", "bloomStage1.6sfrag");
		}
	}
	CMultipassTest() {
		printf("Loading test multipass shaders...");
		printf(m_Shader.loadFromFile("default.6svert", "test.6sfrag") ? "OK\n" : "FAIL\n");
		printf(m_Shader2.loadFromFile("default.6svert", "test2.6sfrag") ? "OK\n" : "FAIL\n");
		printf(m_Shader3.loadFromFile("default.6svert", "bloomStage1.6sfrag") ? "OK\n" : "FAIL\n");
		InitRenderState();
		m_RenderStates2.shader = &m_Shader2;
		m_RenderStates3.shader = &m_Shader3;
	}
	void UpdateTestShader() {

	}
};


class CEdgeDetectionTest : public CShaderEffect {
	sf::Shader m_Shader2;
	sf::RenderStates m_RenderStates2 = sf::RenderStates::Default;
	sf::Shader m_Shader3;
	sf::RenderStates m_RenderStates3 = sf::RenderStates::Default;
public:
	void ApplyEffect(sf::Texture& windowTexture, sf::RenderWindow& window, sf::IntRect& scrRect) {
		return;
		sf::Texture screenData(windowTexture);
		// blur to seperate texture
		//window.draw(sf::Sprite(windowTexture, scrRect), m_RenderStates2);
		//windowTexture.update(window);

		// Edge detection to render outlines
		//	m_Shader.setParameter("blurredImage", windowTexture);
		//window.draw(sf::Sprite(screenData, scrRect), m_RenderStates);
		// draw bright parts to seperate texture
		window.draw(sf::Sprite(windowTexture, scrRect), m_RenderStates3);
		//windowTexture.update(window);

		// blur texture with bright parts seperate texture
		//window.draw(sf::Sprite(windowTexture, scrRect), m_RenderStates2);
		//windowTexture.update(window);

		// combine to provide image with bloom
		//m_Shader.setParameter("blurredImage", windowTexture);
		//window.draw(sf::Sprite(screenData, scrRect), m_RenderStates);

		if (GetAsyncKeyState(VK_F1)) {
			m_Shader.loadFromFile("default.6svert", "edgeDetection.6sfrag");
			m_Shader2.loadFromFile("default.6svert", "test2.6sfrag");
		}
		if (GetAsyncKeyState(VK_F2)) {
			//m_Shader.loadFromFile("default.6svert", "bloomStage3.6sfrag");
			//m_Shader2.loadFromFile("default.6svert", "bloomStage2.6sfrag");
			m_Shader3.loadFromFile("default.6svert", "bloomStage1.6sfrag");
		}
	}
	CEdgeDetectionTest() {
		printf("Loading Edge Detection Shaders");
		printf(m_Shader.loadFromFile("default.6svert", "test.6sfrag") ? "OK\n" : "FAIL\n");
		printf(m_Shader2.loadFromFile("default.6svert", "test2.6sfrag") ? "OK\n" : "FAIL\n");
		printf(m_Shader3.loadFromFile("default.6svert", "bloomStage1.6sfrag") ? "OK\n" : "FAIL\n");
		InitRenderState();
		m_RenderStates2.shader = &m_Shader2;
		m_RenderStates3.shader = &m_Shader3;
	}
	void UpdateTestShader() {

	}
};
#endif