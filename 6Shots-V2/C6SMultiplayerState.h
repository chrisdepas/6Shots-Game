#ifndef __C6SMULTIPLAYERSTATE_H__
#define __C6SMULTIPLAYERSTATE_H__

#include "CGameState.h"
#include "IEventListener.h"

class C6SMultiplayerState : public CGameState, public IEventListener {
	tgui::Gui* m_pGui;

	/* Map location path */
	std::string MAPS_FILE_LOCATION = "./MAP/"; 

	/* Map storage struct */
	struct SMapInstance {
		std::string sPath;		// Full path
		std::string sDisplayName;// Nice name
		SMapInstance(const std::string& path, const std::string& displayName) {
			sPath = path;
			sDisplayName = displayName;
		}
		SMapInstance() {}
	};
	/* Maps */
	std::vector<SMapInstance> m_vMaps;

	/**/
	const int TEXT_SIZE_LARGE = 24;
	const int TEXT_SIZE_MEDIUM = 16;
	const int TEXT_SIZE_SMALL = 12;

public:
	void C6SMultiplayerState::Init(CGame* pGame);
	void C6SMultiplayerState::Cleanup(CGame* pGame);

	void C6SMultiplayerState::Draw(CGame* pGame);
	void C6SMultiplayerState::Update(CGame* pGame, float fFrameTime);
	void C6SMultiplayerState::HandleInput(CGame* pGame);

	void C6SMultiplayerState::PauseState();
	void C6SMultiplayerState::ResumeState();

	// GUI Callbacks
	void C6SMultiplayerState::Connect(CGame* pGame);
	void C6SMultiplayerState::StartServer(CGame* pGame);
	void C6SMultiplayerState::LaunchGame(CGame* pGame);
	void C6SMultiplayerState::OnMapSelected(CGame* pGame);
	void C6SMultiplayerState::OnChatMessage(IEventListener::SChatEvent* pChatEvent);
	void C6SMultiplayerState::SendChatMessage(CGame* pGame);

	void C6SMultiplayerState::OnEvent(CGame* pGame, std::string evtName, void* data, size_t len);
	 
	static C6SMultiplayerState* Instance()
	{
		static C6SMultiplayerState m_MenuState;
		return &m_MenuState;
	}
};

#endif 