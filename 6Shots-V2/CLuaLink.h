#ifndef __CLUALINK_H__
#define __CLUALINK_H__
#include <lua.hpp>
#include <vector>

/*
	Christopher De Pasquale
	24/03/2017
*/

/*
	EXPECTED LUA FUNCTIONS 
	THESE FUNCTIONS MUST BE PRESENT AND ARE USED TO 
	CREATE GAMEMODES ETC 
*/
#define EXPECTEDFUNCS_COUNT 2
#define EXPECTEDFUNCS_NAME {"initGamemode", "gamemodeTick(timeDelta)"}
#define EXPECTEDFUNCS_DESC {"Called when gamemode is initialised", "Called each tick"}
/*
	END EXPECTED FUNCS
*/

class CLuaLink {
	lua_State* m_pLuaState = NULL;

	// Struct which represents a SDK function 
	struct SSDKItem {
		std::string sFunc; std::string sDesc;

		SSDKItem(char* szFunc, char*szDesc) {
			sFunc = szFunc; sDesc = szDesc;
		}
	};

	// SDK Game name
	std::string m_sSDKName = "";

	// List of required functions with name/description pairs
	int m_iRequiredFuncCount = EXPECTEDFUNCS_COUNT;
	//static const char* m_szRequiredFuncsName[EXPECTEDFUNCS_COUNT] = EXPECTEDFUNCS_NAME;
	//	static const char* m_szRequiredFuncDesc[EXPECTEDFUNCS_COUNT] = EXPECTEDFUNCS_DESC;


	// SDK Versioning
	int m_iSDKVersionMajor = 0;
	int m_iSDKVersionMinor = 0;

	std::vector<SSDKItem> m_vSDK;

public:

	// Function name to init gamemode
	const char* GAMEMODE_INIT_FUNCTION = "initGamemode";

	// Function name to get gamemode type
	const char* GAMEMODE_TYPE_FUNCTION = "getGamemodeType";

	const char* GAMEMODE_REGISTER_HANDLER_FUNCTION = "registerHandler";

	/* Init lua state */
	bool InitLua();

	/* Destroy lua state */
	void DestroyLua();

	/* Check if theres a current and valid lua state */
	bool ValidLuaState() { return m_pLuaState != NULL; }

	/* Set information for SDK */
	void SetSDKInformation(char* szGameName, int iVersionMajor, int iVersionMinor) {
		m_sSDKName = szGameName; m_iSDKVersionMajor = iVersionMajor; m_iSDKVersionMinor = iVersionMinor;
	}

	/* Register a C function in lua */
	bool RegisterCFunc(char* szName, lua_CFunction pFunction, char* szSDKDescription = "");

	/* Register a C Member function in lua */
	void RegisterMemberFunc(char* szName, void* pFunc, lua_CFunction pInstance);

	void RegisterGlobal(char* szName, int dwGlobal);
	void RegisterGlobal(char* szName, void* pGlobal);

	/* Run a LUA script from file */
	void RunFile(char* szLuaScriptFile);

	/* Call LUA function from a script, get result as string. Function must take 0 args.
	if TRUE, then _out_result has been allocated as a string, and needs to be free(d)*/
	bool CallExtScriptFunctionStr(char* szLuaScriptFile, char* szFunctionName, char*& _out_result);

	/* Call LUA function from a script, get result as int. Function must take 0 args. Returns success bool*/
	bool CallExtScriptFunctionInt(char* szLuaScriptFile, char* szFunctionName, int& _out_result);

	/* Call LUA function from script, ignore result (void function required) */
	bool CallExtScriptFunction(char* szLuaScriptFile, char* szFunctionName);

	/* Dump all registered functions as an 'SDK' */
	bool DumpSDK(char* szSDKFilePath);

	/* Dump all register */
	bool CLuaLink::PrintSDK();
};

#endif