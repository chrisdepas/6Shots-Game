#include "stdafx.h"
#include "CDebugConsole.h"
#include "CGame.h"
#include "GUIFactory.h"

CDebugConsole::CDebugConsole(CGame* pGame) {
	Init(pGame);
}

void CDebugConsole::Init(CGame* pGame) {
	if (m_bInit)
		return;
	m_GUI.setTarget(*pGame->m_WindowManager.GetWindow());

	// Create console panel
	auto backgroundPanel = GUIFactory::PanelScale(m_GUI, sf::Vector2f(1.0f, DEBUG_CONSOLE_HEIGHT_PCT));
	tgui::Layout2d panelSize = backgroundPanel->getSize();
	m_GUI.add(backgroundPanel, "ConsolePanel");

	// Add cmd list
	auto consoleList = GUIFactory::Chatbox();
	consoleList->setSize(tgui::bindWidth(backgroundPanel), tgui::bindHeight(backgroundPanel) - (DEBUG_CONSOLE_TEXT_HEIGHT + DEBUG_CONSOLE_TEXT_SPACING));
	consoleList->setTextColor(sf::Color::White);
	backgroundPanel->add(consoleList, "ConsoleList");

	// Add comamnd textbox
	auto consoleTextbox = GUIFactory::EditboxMedium();
	consoleTextbox->setSize(tgui::bindWidth(consoleList), DEBUG_CONSOLE_TEXT_HEIGHT + DEBUG_CONSOLE_TEXT_SPACING);
	consoleTextbox->setTextSize(DEBUG_CONSOLE_TEXT_HEIGHT);
	consoleTextbox->setPosition(0, tgui::bindHeight(consoleList));
	consoleTextbox->connect("ReturnKeyPressed", &CDebugConsole::ReturnKeyPressed, this);
	backgroundPanel->add(consoleTextbox, "ConsoleTextbox");

	// Add base console commands
	RegisterConsoleFunction("echo", "Print typed text to console", &CDebugConsole::Echo, this);
	RegisterConsoleFunction("help", "Show description of a command", &CDebugConsole::Help, this);
	RegisterConsoleFunction("alias", "Register a command which executes other commands. Usage: \n\talias aliascmd \"echo 'testing';echo 'aliases'\"", &CDebugConsole::Alias, this);
	RegisterConsoleFunction("delalias", "Remove an existing alias. Usage:\n\tdelalias aliascmd", &CDebugConsole::RemoveAlias, this);
	RegisterConsoleFunction("listalias", "Show a list of all alias names and commands", &CDebugConsole::ListAlias, this);
	RegisterConsoleFunction("listcmd", "Show a list of all registered commands", &CDebugConsole::ListCmd, this);

	m_bInit = true;
}

void CDebugConsole::Show(CGame* pGame) {
	/*if (m_bShowing) return; m_bShowing = true;*/
	Activate(pGame);
	auto consoleTextbox = GetConsoleTextbox();
	if (consoleTextbox) {
		consoleTextbox->setFocused(true);
	}
	/*auto consolePanel = GetConsolePanel(); if (consolePanel) consolePanel->showWithEffect(DEBUG_CONSOLE_ANIMATION, DEBUG_CONSOLE_ANIMATION_TIME);*/
}

void CDebugConsole::Hide(CGame* pGame, tgui::Gui* pRestoreGUI) {
	/*if (m_bHiding) return; m_bHiding = true;*/
	Disable(pGame, pRestoreGUI);
	/*pGame->m_Scheduler.ScheduleEvent(DEBUG_CONSOLE_ANIMATION_TIME.asSeconds(), &CDebugConsole::Disable, this, pGame, pRestoreGUI);*/
	/*auto consolePanel = GetConsolePanel(); if (consolePanel) consolePanel->hideWithEffect(DEBUG_CONSOLE_ANIMATION, DEBUG_CONSOLE_ANIMATION_TIME);*/
}

void CDebugConsole::Activate(CGame* pGame) {
	pGame->m_WindowManager.SetGUI(&m_GUI);
	pGame->m_WindowManager.SetGUIActive(true);
	m_bActive = true;//m_bShowing = false;
}

void CDebugConsole::Disable(CGame* pGame, tgui::Gui* pRestoreGUI) {
	m_bActive = false;//m_bHiding = false;
	pGame->m_WindowManager.SetGUI(pRestoreGUI);
	pGame->m_WindowManager.SetGUIActive(true);
}

void CDebugConsole::Update(CGame* pGame) {
	while (!m_CmdQueue.empty()) {
		// Parse args and command
		std::vector<std::string> args;
		if (!util::StrSplitQuoteDelim(m_CmdQueue.front(), args)) {
			AddConsoleText("Invalid command string '" + m_CmdQueue.front() + "', command has unmatched quotation", sf::Color::Red);
			continue;
		}

		// Split command and args
		std::string cmd = util::StrLower(args[0]);
		args.erase(args.begin());
		m_vCurCmdArgs = args;

		// Find command & call handler function
		auto mapFn = m_CmdHandlers.find(cmd);
		if (mapFn != m_CmdHandlers.end())
			mapFn->second();
		else
			AddConsoleText("Invalid command '" + cmd + "'", sf::Color::Red);

		m_CmdQueue.pop();
	}
}

void CDebugConsole::ReturnKeyPressed() {
	if (!m_bInit)
		return;
	auto consoleText = GetConsoleTextbox();
	if (consoleText == NULL) {
		CDebugLogger::LogError("Failed to get console textbox while submitting console command\n");
	} else {
		SubmitCommand(consoleText->getText());		
		consoleText->setText("");
	}
}

void CDebugConsole::SubmitCommand(std::string cmd) {
	if (!m_bInit)
		return;

	AddConsoleText("> " + cmd);
	auto cmds = util::StrSplit(cmd, ';');

	for (unsigned int i = 0; i < cmds.size(); i++) {
		// Remove whitespace
		util::Trim(cmds[i]);

		if (cmds[i].length() > 0) {
			m_CmdQueue.push(cmds[i]);
		}
	}
}

void CDebugConsole::AddConsoleText(std::string sText, sf::Color color) {
	if (!m_bInit)
		return;

	auto consoleList = GetConsolelist();
	if (consoleList == NULL) {
		CDebugLogger::LogError("Failed to get console list while adding console text\n");
		return;
	}
	consoleList->addLine(sText, color);
}

#define CONSOLE_EXTRA_BUFFER_SPACING 512
void CDebugConsole::AddConsoleText(char* fmt, sf::Color color, ...) {
	if (!m_bInit)
		return;

	auto consoleList = GetConsolelist();
	if (consoleList == NULL) {
		CDebugLogger::LogError("Failed to get console list while adding console text\n");
		return;
	}

	/* Init buffer as formatted string */
	int bufLen = CONSOLE_EXTRA_BUFFER_SPACING + strlen(fmt);
	char* consoleBuffer = new char[bufLen];

	va_list args;
	va_start(args, fmt);
	int result = vsnprintf_s(consoleBuffer, bufLen, bufLen - 1, fmt, args);
	va_end(args);

	if (result <= 0) {
		CDebugLogger::LogError("Failed to add console text - vsnprintf returned 0\n");
		return;
	}
	else if (result >= bufLen) {
		CDebugLogger::LogWarning("Tried to add console text which was too large, output has been trimmed\n");
		consoleBuffer[bufLen - 1] = NULL;
	}
	else {
		consoleBuffer[result] = NULL;
	}

	consoleList->addLine(consoleBuffer, color);
}

void CDebugConsole::AddConsoleText(char* fmt, ...) {
	if (!m_bInit)
		return;

	auto consoleList = GetConsolelist();
	if (consoleList == NULL) {
		CDebugLogger::LogError("Failed to get console list while adding console text\n");
		return;
	}

	/* Init buffer as formatted string */
	int bufLen = CONSOLE_EXTRA_BUFFER_SPACING + strlen(fmt);
	char* consoleBuffer = new char[bufLen];

	va_list args;
	va_start(args, fmt);
	int result = vsnprintf_s(consoleBuffer, bufLen, bufLen - 1, fmt, args);
	va_end(args);

	if (result <= 0) {
		CDebugLogger::LogError("Failed to add console text - vsnprintf returned 0\n");
		return;
	}
	else if (result >= bufLen) {
		CDebugLogger::LogWarning("Tried to add console text which was too large, output has been trimmed\n");
		consoleBuffer[bufLen - 1] = NULL;
	}
	else {
		consoleBuffer[result] = NULL;
	}

	consoleList->addLine(consoleBuffer, sf::Color::White);
}

bool CDebugConsole::RegisterConsoleAlias(std::string sAlias, std::string sCmdStr) {
	if (!m_bInit)
		return false;

	if (!ValidateAliasName(sAlias))
		return false;
	if (!ValidateAliasCommandStr(sCmdStr))
		return false;
	if (CommandExists(sAlias))
		return false;

	// Insert or overwrite alias
	m_CmdAlias[util::StrLower(sAlias)] = sCmdStr;
	AddConsoleText("Successfully registered alias", sf::Color::Green);
	return true;
}

bool CDebugConsole::RemoveConsoleAlias(std::string sAlias) {
	if (!m_bInit)
		return false;

	auto findAlias = m_CmdAlias.find(util::StrLower(sAlias));
	if (findAlias == m_CmdAlias.end())
		return false;

	m_CmdAlias.erase(findAlias);
	return true;
}


/*** Console Command Handler Functions ***/

void CDebugConsole::Echo(void* pThis) {
	auto args = GetArgs();
	if (args.size() == 1)
		AddConsoleText(args[0]);
	else
		AddConsoleText("Error: Expected 1 argument for 'echo', recieved " + std::to_string(args.size()), sf::Color::Red);	
}

void CDebugConsole::Help(void* pThis) {
	if (ArgCount() != 1) {
		AddConsoleText("Error: Expected 1 argument for 'help', recieved " + std::to_string(ArgCount()), sf::Color::Red);
		return;
	} 

	std::string helpCmd = util::StrLower(GetArgs()[0]);
	auto searchHelp = m_CmdHelp.find(helpCmd);

	if (searchHelp != m_CmdHelp.end()) {
		AddConsoleText(searchHelp->second, sf::Color::Yellow);
	} else {
		AddConsoleText("Error: Couldn't find help text for command '" + helpCmd + "'", sf::Color::Red);
	}
}

void CDebugConsole::ListCmd(void* pThis) {
	if (ArgCount() > 1) {
		AddConsoleText("Error: Expected optional filter string as only argument", sf::Color::Red);
		return;
	}

	std::string cmp = (ArgCount() == 1 ? util::StrLower(GetArgs()[0]) : "");
	AddConsoleText("Listing all commands" + (cmp.empty() ? "" : " which match filter: " + cmp));

	for (auto it = m_CmdHandlers.begin(); it != m_CmdHandlers.end(); it++) {
		if (cmp.empty() || util::StrContains(it->first, cmp))
			AddConsoleText("\t" + it->first);
	}
}

void CDebugConsole::ListAlias(void* pThis) {
	if (ArgCount() > 1) {
		AddConsoleText("Error: Expected optional filter string as only argument", sf::Color::Red);
		return;
	} 
	
	std::string cmp = (ArgCount() == 1 ? util::StrLower(GetArgs()[0]) : "");
	AddConsoleText("Listing all aliases" + (cmp.empty() ? "" : " which match filter: " + cmp));

	for (auto it = m_CmdAlias.begin(); it != m_CmdAlias.end(); it++) {
		if (cmp.empty() || util::StrContains(it->first, cmp))
			AddConsoleText("\t" + it->first + "\n\t\t" + it->second);
	}
}

void CDebugConsole::RemoveAlias(void* pThis) {
	if (ArgCount() != 1) {
		AddConsoleText("Error - Expected 1 argument, recieved " + std::to_string(ArgCount()), sf::Color::Red);
		return;
	}
	std::string alias = GetArgs()[0];
	if (!RemoveConsoleAlias(alias)) {
		AddConsoleText("Error - Failed to delete alias '" + alias + "', alias may not exist", sf::Color::Red);
	}
}

void CDebugConsole::Alias(void* pThis) {
	if (ArgCount() != 2) {
		AddConsoleText("Error: Expected 2 arguments for 'alias', recieved " + std::to_string(ArgCount()), sf::Color::Red);
		return;
	}
	auto args = GetArgs();
	if (!RegisterConsoleAlias(args[0], args[1])) 
		AddConsoleText("Error: Registering console alias failed. \
					   \nEnsure alias name doesn't match an existing command, and matches the following criteria:\
					   \n" + CMD_REQUIREMENTS_STR, sf::Color::Red);	
}

/*** Helper Functions ***/

tgui::Panel::Ptr CDebugConsole::GetConsolePanel() {
	return m_GUI.get<tgui::Panel>("ConsolePanel");
}

tgui::ChatBox::Ptr CDebugConsole::GetConsolelist() {
	tgui::Panel::Ptr consolePanel = m_GUI.get<tgui::Panel>("ConsolePanel");
	if (!consolePanel)
		return NULL;
	return consolePanel->get<tgui::ChatBox>("ConsoleList");
}

tgui::EditBox::Ptr CDebugConsole::GetConsoleTextbox() {
	tgui::Panel::Ptr consolePanel = m_GUI.get<tgui::Panel>("ConsolePanel");
	if (!consolePanel)
		return NULL;
	return consolePanel->get<tgui::EditBox>("ConsoleTextbox");
}

bool CDebugConsole::ValidateAliasName(std::string sAlias) {
	return (sAlias.length() > 0 && util::StrAlphaNumeric(sAlias) && util::CharAlpha(sAlias[0]));
}

bool CDebugConsole::ValidateAliasCommandStr(std::string sCmd) {
	return (sCmd.length() > 0 && util::StrAlphaNumeric(sCmd) && util::CharAlpha(sCmd[0]));
}

bool CDebugConsole::ValidateCommand(std::string sCmd) {
	return (sCmd.length() > 0 && util::StrAlphaNumeric(sCmd) && util::CharAlpha(sCmd[0]));
}

bool CDebugConsole::CommandExists(std::string sCmd) {
	util::StrLower(sCmd);
	return (m_CmdHandlers.find(sCmd) != m_CmdHandlers.end());
} 

bool CDebugConsole::AliasExists(std::string sAlias) {
	util::StrLower(sAlias);
	return (m_CmdAlias.find(sAlias) != m_CmdAlias.end());
}