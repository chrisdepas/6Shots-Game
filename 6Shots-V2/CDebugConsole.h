#ifndef __CDEBUGCONSOLE_H__
#define __CDEBUGCONSOLE_H__

#include "Util.h"

#define DEBUG_CONSOLE_HEIGHT_PCT 0.25f
#define DEBUG_CONSOLE_TEXT_HEIGHT 20
#define DEBUG_CONSOLE_TEXT_SPACING 6
#define DEBUG_CONSOLE_ANIMATION_TIME sf::seconds(0.5f)
#define DEBUG_CONSOLE_ANIMATION tgui::ShowAnimationType::Scale

class CGame;

class CDebugConsole {
	bool m_bHasCommands = false;
	bool m_bActive = false;
	bool m_bInit = false;
	bool m_bHiding = false, m_bShowing = false;

	tgui::Gui m_GUI;

	// Queue for command strings waiting to be parsed
	std::queue<std::string> m_CmdQueue;

	// Stored arguments for an executing cmd. Valid only in handler callback
	std::vector<std::string> m_vCurCmdArgs;

	// Console command mappings to handler functions and help strings
	std::map<std::string, std::function<void()>> m_CmdHandlers;
	std::map<std::string, std::string> m_CmdHelp;

	// Console alias mappings to command string
	std::map<std::string, std::string> m_CmdAlias;


	// Cmd/Alias Length bounds, inclusive
	const int CMD_MIN_LEN = 1;
	const int CMD_MAX_LEN = 64;

	const std::string CMD_REQUIREMENTS_STR = "- Must start with a letter\n- Must use letters and numbers only\
\n- No less than " + std::to_string(CMD_MIN_LEN) + (CMD_MIN_LEN == 1 ? " character" : " characters") + "\
\n- No more than " + std::to_string(CMD_MAX_LEN) + " characters";

	// Callback for entering console cmd
	void ReturnKeyPressed();

	// Console commands 
	void Echo(void* pThis); // Print string to console
	void Help(void* pThis); // Show help text for a command
	void ListCmd(void* pThis); // Show all commands 
	void ListAlias(void* pThis); // Show all aliases
	void RemoveAlias(void* pThis); // Delete an alias
	void Alias(void* pThis); // Insert an alias

public:
	// Create a new console. Must call Init() before using
	CDebugConsole::CDebugConsole(){}

	// Create and initialize a new console 
	CDebugConsole::CDebugConsole(CGame* pGame);

	void Init(CGame* pGame);

	void Update(CGame* pGame);
	bool IsActive() { return m_bInit && m_bActive; }
	void Draw() { if (m_bInit) m_GUI.draw(); }

	// Activate/Deactivate with animation
	void Hide(CGame* pGame, tgui::Gui* pRestoreGUI=NULL);
	void Show(CGame* pGame);

	// Activate/Deactivate without animation
	void Activate(CGame* pGame);
	void Disable(CGame* pGame, tgui::Gui* pRestoreGUI=NULL);

	// Returns true if the command/alias is valid. Must start with a letter,
	// use alphanumeric characters only, and contain 1 or more characters
	bool ValidateCommand(std::string sCmd);
	bool ValidateAliasName(std::string sCmd);
	bool ValidateAliasCommandStr(std::string sCmd);

	// Checks if a command/alias name exists
	bool CommandExists(std::string sCmd);
	bool AliasExists(std::string sAlias);

	// Helpers - Get components of console GUI
	tgui::ChatBox::Ptr GetConsolelist();
	tgui::EditBox::Ptr GetConsoleTextbox();
	tgui::Panel::Ptr GetConsolePanel();

	// Get vector for current command args
	// Undefined result if called outside of registered command handler function
	std::vector<std::string>& GetArgs() { return m_vCurCmdArgs; }
	int ArgCount() { return m_vCurCmdArgs.size(); }
	
	// Print console text. Text color defaults to white
	void AddConsoleText(std::string sText, sf::Color color = sf::Color::White);
	void AddConsoleText(char* szFmt, sf::Color color, ...);
	void AddConsoleText(char* szFmt, ...);

	// Manually send a console command 
	// This is also called when user submits a command
	// Command format  -  Strings use quotation marks, "/'. Seperate command 
	// calls are seperated by semicolons. E.g. two commands, containing strings
	// echo "hello"; echo "m'lady"
	void SubmitCommand(std::string sCmd);

	// Registers a console command handler function. Pass changing args by 
	// reference using std::ref. Console commands are not case-sensitive.
	// 
	// Function must contain a 'CDebugConsole*' as the first argument, and can
	// use AddConsoleText() and GetArgs() 
	// Example:
	//   bool fnCmd(CDebugConsole* pConsole, CGame* ptr, int 2);
	//   m_Console->RegisterConsoleFunction(pGame, int_2);
	//
	// Returns bool indicating if the command handler was added. false indicates 
	// that the command is already registered.
	// Note: Currently description isn't used. TODO!
	template <typename CallbackFn_t, typename... Args>
	bool RegisterConsoleFunction(std::string sCmd, std::string sHelpDescription, CallbackFn_t func, Args... args) {
		if (CommandExists(sCmd))
			return false;

		// Insert command handler fn and help text
		util::StrLower(sCmd);
		m_CmdHandlers[sCmd] = std::bind(func, this, args...);
		m_CmdHelp[sCmd] = sHelpDescription;

		// Remove alias for command if it exists
		if (RemoveConsoleAlias(sCmd)) {
			AddConsoleText("Removed alias '" + sCmd + "', as it matched a registered console function", sf::Color::Yellow);
		}

		return true;
	}

	// Registers an alias
	// An alias maps an input string to an invocation of other command(s)
	// Will return false and fail if the alias used is already registered as 
	// a command, and overwrite the previous alias if it already exists.
	// An alias cannot contain spaces or special characters
	bool RegisterConsoleAlias(std::string sAlias, std::string sCommandStr);

	// Removes an alias
	bool RemoveConsoleAlias(std::string sAlias);
};

#endif