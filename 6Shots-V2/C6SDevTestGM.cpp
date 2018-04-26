#include "stdafx.h"
#include "CGame.h"
#include "C6SMenuState.h"
#include "C6SDevTestGM.h"

void C6SDevTestGM::ReturnToMenu(CGame* pGame) {
	// Disable GUI handling
	pGame->m_WindowManager.SetGUI(NULL);
	pGame->m_WindowManager.SetGUIActive(false);

	/* BACK TO BLACK */
	pGame->m_StateHandler.ChangeState(C6SMenuState::Instance(), pGame);
}

void C6SDevTestGM::LoadGMPressed(CGame* pGame) {

}

void C6SDevTestGM::Cancel(CGame* pGame) {
	// Disable GUI handling
	pGame->m_WindowManager.SetGUI(NULL);
	pGame->m_WindowManager.SetGUIActive(false);

	// Return to menu
	pGame->m_StateHandler.ChangeState(C6SMenuState::Instance(), pGame);
}

void C6SDevTestGM::Init(CGame* pGame) {
	if (!IsAlreadyInitialised()) {
		/* Get valid resolutions for max bpp */
		std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
		int bppMax = -1;
		for (unsigned int i = 0; i < modes.size(); i++) {
			if ((unsigned int)bppMax > modes[i].bitsPerPixel) {
				bppMax = modes[i].bitsPerPixel; m_vResolutions.clear();
			}
			m_vResolutions.push_back(sf::Vector2i((int)modes[i].width, (int)modes[i].height));
		}


		/* Create GUI if it doesn't exist */
		m_pGui = new tgui::Gui(*pGame->m_WindowManager.GetWindow());

		/* Add GUI elements */
		try {
			tgui::Theme::Ptr theme = std::make_shared<tgui::Theme>("widgets/Black.txt");

			tgui::Layout yOffset = 200;
			/* Load gamemode path textbox */
			tgui::Label::Ptr lblLoadGM = std::make_shared<tgui::Label>();
			lblLoadGM->setText("test gamemode:");
			lblLoadGM->setSize(tgui::Layout2d(200, 20));
			lblLoadGM->setPosition(0.0f, yOffset); yOffset += 20.0f;
			lblLoadGM->setTextColor(sf::Color::White);
			m_pGui->add(lblLoadGM);

			tgui::EditBox::Ptr editLoadGM = theme->load("EditBox");
			editLoadGM->setSize(300, 30);
			editLoadGM->setPosition(0, yOffset);
			editLoadGM->setText("");
			editLoadGM->setTextSize(12);
			m_pGui->add(editLoadGM, "EditLoadGM");

			tgui::Button::Ptr btnLoadGM = theme->load("Button");
			btnLoadGM->setSize(300, 30);
			btnLoadGM->setPosition(300, yOffset); yOffset += 30.0f;
			btnLoadGM->setText("Load");
			btnLoadGM->connect("Pressed", &C6SDevTestGM::LoadGMPressed, this, pGame);
			m_pGui->add(btnLoadGM);

			/* return button */
			tgui::Button::Ptr btnRetn = theme->load("Button");
			btnRetn->setSize(200, 30);
			btnRetn->setPosition((float)pGame->m_WindowManager.GetScreenCentre().X - 300.0f, yOffset);
			btnRetn->setText("Return to Menu");
			btnRetn->setTextSize(TEXT_SIZE);
			btnRetn->connect("pressed", &C6SDevTestGM::ReturnToMenu, this, pGame);
			m_pGui->add(btnRetn);
		}
		catch (...) {
			pGame->ErrorQuit("Unable to create main menu");
		}


	}
	
	/* Enable GUI input handling */
	pGame->m_WindowManager.SetGUI(m_pGui);
	pGame->m_WindowManager.SetGUIActive(true);
}

void C6SDevTestGM::Draw(CGame* pGame){
	pGame->m_Drawing.DrawTextCentredX(pGame->GetWindowMgr(), "DEV GM", pGame->m_WindowManager.GetScreenCentre().X, 100, 64, 255, 255, 255, 255);
	m_pGui->draw();
}

void C6SDevTestGM::Cleanup(CGame* pGame){}
void C6SDevTestGM::Update(CGame* pGame) {}
void C6SDevTestGM::HandleInput(CGame* pGame) {}
void C6SDevTestGM::PauseState(){}
void C6SDevTestGM::ResumeState(){}