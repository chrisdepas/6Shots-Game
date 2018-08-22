#include "stdafx.h"
#include "C6SMultiplayerState.h"
#include "C6SMenuState.h"
#include "CGame.h"
#include "C6SMultiplayerGameState.h"
#include "ASSETS.h"

#define FONT_SIZE 24

void ToMenuFromMultiplayerLobby(CGame* pGame) {
	// Disable GUI handling
	pGame->m_WindowManager.SetGUI(NULL);
	pGame->m_WindowManager.SetGUIActive(false);

	// Return to menu
	pGame->m_StateHandler.ChangeState(C6SMenuState::Instance(), pGame);
}

void ServerToMultiplayerGameFromLobby(IEventListener* pListener, CGame* pGame) {
	// Disable GUI handling
	pGame->m_WindowManager.SetGUI(NULL);
	pGame->m_WindowManager.SetGUIActive(false);

	// Launch Game State
	pGame->m_StateHandler.ChangeState(C6SMultiplayerGameState::Instance(), pGame);
}

void C6SMultiplayerState::OnEvent(CGame* pGame, std::string evtName, void* data, size_t len) {
	/* Don't handle events from another state */
	if (!pGame->m_StateHandler.IsCurrentState(this))
		return; 

	tgui::ChatBox::Ptr chat = (tgui::ChatBox::Ptr)m_pGui->get<tgui::ChatBox>("Chat");
	if (evtName.compare("ConnectionAccepted") == 0) {
		// Display connect 
		chat->addLine("Connected to server.", sf::Color::Blue);

		// Add username
		tgui::ChatBox::Ptr players = (tgui::ChatBox::Ptr)m_pGui->get<tgui::ChatBox>("PlayerList");
		players->addLine(pGame->m_Client.GetUsername());

	} else if (evtName.compare("ConnectionRefused") == 0) {
		std::string* pStr = (std::string*)data;
		std::string conStr = "Connection refused. Reason:" + *pStr;
		chat->addLine(conStr, sf::Color::Red);

	} else if (evtName.compare("NewPlayer") == 0) {
		std::string* pStr = (std::string*)data;
		tgui::ChatBox::Ptr players = (tgui::ChatBox::Ptr)m_pGui->get<tgui::ChatBox>("PlayerList");
		players->addLine(*pStr);

	} else if (evtName.compare("NewPlayerRejected") == 0) {
		std::string* pStr = (std::string*)data;
		std::string conStr = "Refused connection for player '" + *pStr + "'";
		chat->addLine(conStr, sf::Color::Blue);

	} else if (evtName.compare("ChatMessage") == 0) {
		IEventListener::SChatEvent* pChat = (IEventListener::SChatEvent*)data;
		OnChatMessage(pChat);

	} else if (evtName.compare("ServerLaunchGame") == 0) {
		ServerToMultiplayerGameFromLobby(this, pGame);

	} 
}
void C6SMultiplayerState::OnChatMessage(IEventListener::SChatEvent* pChatEvent) {
	tgui::ChatBox::Ptr chat = (tgui::ChatBox::Ptr)m_pGui->get<tgui::ChatBox>(sf::String("Chat"));
	
	/* Get chat color */
	sf::Color chatColor = sf::Color::White;

	/* Format and display chat message */
	std::string chatLine = *pChatEvent->user + ": " + *pChatEvent->message;
	chat->addLine(chatLine, sf::Color::White);
}
void C6SMultiplayerState::Connect(CGame* pGame) {
	if (!pGame->m_Client.IsConnected()) {
		tgui::EditBox::Ptr editIP = (tgui::EditBox::Ptr)m_pGui->get<tgui::EditBox>(sf::String("IP"));
		tgui::EditBox::Ptr editUsername = (tgui::EditBox::Ptr)m_pGui->get<tgui::EditBox>(sf::String("Username"));

		// Disable server button
		tgui::Button::Ptr btnServer = (tgui::Button::Ptr)m_pGui->get<tgui::Button>(sf::String("Server"));
		btnServer->setEnabled(false);

		printf("Connecting to: %s with username %s\n", editIP->getText().toAnsiString().c_str(), editUsername->getText().toAnsiString().c_str());
		pGame->m_Client.Start((char*)editIP->getText().toAnsiString().c_str(), (char*)editUsername->getText().toAnsiString().c_str(), "");
	}
}
void C6SMultiplayerState::StartServer(CGame* pGame) {
	// Get username 
 	tgui::EditBox::Ptr editUsername = m_pGui->get<tgui::EditBox>("Username");
	std::string username = editUsername->getText().toAnsiString();	

	if (!pGame->m_Server.IsListening() && !pGame->m_Client.IsActive() && pGame->m_Server.Start(username)) {
		printf("Server started\n");

		// Disable client buttons
		tgui::Button::Ptr btnConnect = m_pGui->get<tgui::Button>("Connect");
		tgui::EditBox::Ptr editIP = m_pGui->get<tgui::EditBox>("IP");
		btnConnect->setEnabled(false);
		editIP->setEnabled(false);

		// Enable launch game button
		tgui::Button::Ptr btnPlay = m_pGui->get<tgui::Button>("BtnLaunchGame");
		btnPlay->setEnabled(true);

		// Add local player to playerlist
		tgui::ChatBox::Ptr players = m_pGui->get<tgui::ChatBox>("PlayerList");
		players->addLine(username);

		tgui::EditBox::Ptr editMap = m_pGui->get<tgui::EditBox>("MapEdit");
		if (editMap) {
			// Remove map editbox 
			m_pGui->remove(editMap);

			// Add map selector listbox
			tgui::ListBox::Ptr lstMap = tgui::ListBox::create();
			lstMap->setSize(300, 30);
			lstMap->setPosition(pGame->m_Settings.m_vScreenSize.x - 310.0f, 60.0f);
			lstMap->setTextSize(TEXT_SIZE_MEDIUM);
			for (unsigned int i = 0; i < m_vMaps.size(); i++) {
				lstMap->addItem(m_vMaps[i].sDisplayName);
			}
			lstMap->setSelectedItemByIndex(0);
			lstMap->connect("ItemSelected", &C6SMultiplayerState::OnMapSelected, this, pGame);
			m_pGui->add(lstMap, "MapList");
		}		
	}
	else {
		CDebugLogger::LogError("[SERVER] Unable to start server.\n");
	}
}
void C6SMultiplayerState::LaunchGame(CGame* pGame) {
	CDebugLogger::LogInfo("Launching game....\n");

	if (pGame->m_Server.IsListening()) {
		/* Network game launch */
		pGame->m_Server.SendGameLaunch(pGame);

		/* Raise game launch event */  
		pGame->m_EventManager.RaiseEvent(pGame, "ServerLaunchGame", 0, 0);
	}
}

void C6SMultiplayerState::OnMapSelected(CGame* pGame) {
	tgui::ListBox::Ptr editMap = m_pGui->get<tgui::ListBox>("MapList");
	unsigned int selectedID =  (unsigned int)editMap->getSelectedItemIndex();
	if (selectedID < m_vMaps.size()) {
		printf("Map was changed to %s\n", m_vMaps[selectedID].sDisplayName.c_str());
	}
}
void C6SMultiplayerState::SendChatMessage(CGame* pGame) {
	tgui::EditBox::Ptr editChat = (tgui::EditBox::Ptr)m_pGui->get<tgui::EditBox>(sf::String("ChatMsgBox"));
	printf("Chat: %s\n", editChat->getText().toAnsiString().c_str());

	std::string msg = editChat->getText();

	/* Broadcast chat on network */
	if (pGame->m_Server.IsListening()) {
		pGame->m_Server.SendChatMessage(msg, pGame);

	} else if (pGame->m_Client.IsConnected()) { 
		pGame->m_Client.SendChatMessage(msg);
	}
	
	/* Clear message box */
	editChat->setText("");
}

void C6SMultiplayerState::Init(CGame* pGame) {

	if (IsAlreadyInitialised()) {
		// Set view to correct size and position
		// Fixes case where screen has been resized after GUI is initialized
		m_pGui->setView(pGame->m_WindowManager.GetGUIView());

	} else {

		/* Check maps directory */
		if (!boost::filesystem::exists(MAPS_FILE_LOCATION) || !boost::filesystem::is_directory(MAPS_FILE_LOCATION)) {
			CDebugLogger::LogError("ERROR - MAPS DIRECTORY '%s' NOT FOUND!\n", MAPS_FILE_LOCATION);
			return;
		}
		
		/* Iterate over directory to find maps */
		for (boost::filesystem::directory_entry& x : (boost::filesystem::directory_iterator(MAPS_FILE_LOCATION))) {
			if (x.status().type() == boost::filesystem::regular_file) {
				// Get file path & extension
				std::string fileName = x.path().filename().string();
				std::string extension = x.path().filename().extension().string();
				util::StrLower(extension);

				// Skip if not a map file
				if (strcmp(extension.c_str(), C6SReleaseMap::GetMapFileExtension()) != 0)
					continue;

				CDebugLogger::LogInfo("[MultiplayerState] Added map '%s'\n", fileName.c_str());
				m_vMaps.push_back(SMapInstance(fileName, x.path().string()));
			}
		}

		// Register for network events
		pGame->m_EventManager.RegisterListener(this, "ConnectionAccepted");
		pGame->m_EventManager.RegisterListener(this, "ConnectionRefused");
		pGame->m_EventManager.RegisterListener(this, "NewPlayer");
		pGame->m_EventManager.RegisterListener(this, "NewPlayerRejected");
		pGame->m_EventManager.RegisterListener(this, "ChatMessage");
		pGame->m_EventManager.RegisterListener(this, "ServerLaunchGame");

		// Create GUI 
		m_pGui = new tgui::Gui(*pGame->m_WindowManager.GetWindow());
		sf::Vector2f scrSize = pGame->m_WindowManager.GetScreenDimensions2f();

		// Back button
		tgui::Button::Ptr btnBack = tgui::Button::create();
		btnBack->setSize(100, 30);
		btnBack->setPosition(10, 10);
		btnBack->setText("Back");
		btnBack->setTextSize(TEXT_SIZE_LARGE);
		btnBack->connect("pressed", &ToMenuFromMultiplayerLobby, pGame);
		m_pGui->add(btnBack);

		// Connect IP
		tgui::EditBox::Ptr editIP = tgui::EditBox::create();
		editIP->setSize(300, 30);
		editIP->setPosition((float)(pGame->m_Drawing.GetTextWidth("IP: ", 24) + 10), 100.0f);
		editIP->setText("");
		editIP->setTextSize(TEXT_SIZE_MEDIUM);
		m_pGui->add(editIP, "IP");

		// Username
		tgui::EditBox::Ptr editUsername = tgui::EditBox::create();
		editUsername->setSize(200, 30);
		editUsername->setPosition((float)(pGame->m_Drawing.GetTextWidth("Username: ", 24) + 10), 60);
		editUsername->setText("Chris");
		editUsername->setTextSize(TEXT_SIZE_MEDIUM);
		m_pGui->add(editUsername, "Username");

		// Map selector
		tgui::EditBox::Ptr edtMap = tgui::EditBox::create();
		edtMap->setSize(300, 30);
		edtMap->setPosition(pGame->m_Settings.m_vScreenSize.x - 310.0f, 60);
		edtMap->setTextSize(TEXT_SIZE_MEDIUM);
		edtMap->setText("<Map not selected>");
		edtMap->setEnabled(false);
		m_pGui->add(edtMap, "MapEdit");

		// Connect Button
		tgui::Button::Ptr btnConnect = tgui::Button::create();
		btnConnect->setSize(100, 30);
		btnConnect->setPosition(editIP->getAbsolutePosition().x + editIP->getSize().x + 10, 100);
		btnConnect->setText("Connect");
		btnConnect->setTextSize(TEXT_SIZE_LARGE);
		btnConnect->connect("pressed", &C6SMultiplayerState::Connect, this, pGame);
		m_pGui->add(btnConnect, "Connect");

		// Start Server button
		tgui::Button::Ptr btnServer = tgui::Button::create();
		btnServer->setSize(200, 30); 
		btnServer->setPosition(scrSize.x - 210.0f, 100);
		btnServer->setText("Start Server");
		btnServer->setTextSize(TEXT_SIZE_LARGE);
		btnServer->connect("pressed", &C6SMultiplayerState::StartServer, this, pGame);
		m_pGui->add(btnServer, "Server");

		// Chatbox
		tgui::ChatBox::Ptr chatBox = tgui::ChatBox::create();
		chatBox->setSize(scrSize.x - 340.0f, 350.0f);
		chatBox->setPosition(10, 150);
		chatBox->setTextSize(TEXT_SIZE_MEDIUM);
		m_pGui->add(chatBox, "Chat");

		// PlayerList
		tgui::ChatBox::Ptr players = tgui::ChatBox::create();
		players->setSize(300, 350.0f);
		players->setPosition(scrSize.x - 310.0f, 150);
		players->setTextSize(TEXT_SIZE_MEDIUM);
		players->setTextColor(sf::Color::White);
		players->setLinesStartFromTop(true);
		players->addLine("Player List");
		m_pGui->add(players, "PlayerList");

		// Chat message box
		tgui::EditBox::Ptr chatMsg = tgui::EditBox::create();
		chatMsg->setSize(scrSize.x - 20, 30);
		chatMsg->setPosition(10.0f, 505.0f);
		chatMsg->setText("");
		chatMsg->setTextSize(TEXT_SIZE_MEDIUM);
		chatMsg->connect("ReturnKeyPressed", &C6SMultiplayerState::SendChatMessage, this, pGame);
		m_pGui->add(chatMsg, "ChatMsgBox");

		// Start game button
		tgui::Button::Ptr btnLaunch = tgui::Button::create();
		btnLaunch->setSize(200.0f, 30.0f);
		btnLaunch->setPosition((scrSize.x / 2.0f) - 100.0f, 535.0f);
		btnLaunch->setText("Launch Game");
		btnLaunch->setTextSize(TEXT_SIZE_MEDIUM);
		btnLaunch->connect("pressed", &C6SMultiplayerState::LaunchGame, this, pGame);
		btnLaunch->setEnabled(false);
		m_pGui->add(btnLaunch, "BtnLaunchGame");
	}

	pGame->m_WindowManager.SetGUI(m_pGui);
	pGame->m_WindowManager.SetGUIActive(true);
}

void C6SMultiplayerState::Cleanup(CGame* pGame) {}
void C6SMultiplayerState::HandleInput(CGame* pGame) {}
void C6SMultiplayerState::PauseState() {}
void C6SMultiplayerState::ResumeState() {}

void C6SMultiplayerState::Draw(CGame* pGame){
	pGame->m_Drawing.DrawText(pGame->GetWindowMgr(), "Username:", 10, 60, 24, 255, 255, 255, 255);
	//Left of IP
	pGame->m_Drawing.DrawText(pGame->GetWindowMgr(), "IP:", 10, 100, 24, 255, 255, 255, 255);
	pGame->m_Drawing.DrawTextCentredX(pGame->GetWindowMgr(), "Multiplayer", pGame->m_WindowManager.GetScreenCentre().x, 10, 24, 255, 255, 255, 255);

	if (m_pGui) {
		m_pGui->draw();
	}
}

void C6SMultiplayerState::Update(CGame* pGame, float fFrameTime){
	if (pGame->m_Server.IsListening())
		pGame->m_Server.Update(pGame);

	if (pGame->m_Client.IsActive())
		pGame->m_Client.Update(pGame);
}

