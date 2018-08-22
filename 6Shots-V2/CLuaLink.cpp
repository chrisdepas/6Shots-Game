#include "stdafx.h"
/*#include "CLuaLink.h"
#include "CDebugLogger.h"

bool CLuaLink::InitLua() {
	// don't overwrite current state 
	if (m_pLuaState != NULL)
		return false;

	// create state
	m_pLuaState = luaL_newstate();
	if (!m_pLuaState) {
		CDebugLogger::LogError("InitLua: Failed to create new lua state\n");
		return false;
	}

	// load standard lua libraries 
	luaL_openlibs(m_pLuaState);
	
	return true;
}

void CLuaLink::DestroyLua() {
	if (m_pLuaState == NULL)
		return;

	// Close lua state 
	lua_close(m_pLuaState);

	m_pLuaState = NULL;
}

bool CLuaLink::RegisterCFunc(char* szName, lua_CFunction pFunction, char* szSDKDescription) {
	if (!ValidLuaState())
		return false;

	// Register function 
	lua_register(m_pLuaState, szName, pFunction);

	// Add to 'SDK' list
	m_vSDK.push_back( SSDKItem(szName, szSDKDescription) );

	return true;
}

void CLuaLink::RegisterMemberFunc(char* szName, void* pFunc, lua_CFunction pInstance) {
	assert(false && "registermemberfunc not implemented");
}

void RegisterGlobal(char* szName, int dwGlobal) {
	//lua_push()
}
void RegisterGlobal(char* szName, void* pGlobal) {

}

bool CLuaLink::CallExtScriptFunctionStr(char* szLuaScriptFile, char* szFunctionName, char*& _out_result) {
	// Ensure we have a valid lua state
	if (!ValidLuaState())
		InitLua();

	// load script
	if (luaL_loadfile(m_pLuaState, szLuaScriptFile) != LUA_OK) {
		CDebugLogger::LogError("CallExtScriptFunctionStr: Failed to load file %s\n", szLuaScriptFile);
		return false;
	}	

	// run script
	if (lua_pcall(m_pLuaState, 0, 0, 0) != LUA_OK) {
		CDebugLogger::LogError("CallExtScriptFunctionStr: Failed to run script\n");
		return false;
	}

	// get function
	lua_getglobal(m_pLuaState, szFunctionName);

	// call function (0 args, 1 return value)
	if (lua_pcall(m_pLuaState, 0, 1, 0) != LUA_OK) {
		CDebugLogger::LogError("CallExtScriptFunctionStr: Failed to call function\n");
		return false;
	}

	// get return val
	if (!lua_isstring(m_pLuaState, -1)) {
		CDebugLogger::LogError("CallExtScriptFunctionStr: Got non-string value\n");
		return false;
	}
	const char* val = lua_tostring(m_pLuaState, -1);

	// make copy of return val
	_out_result = (char*)malloc(strlen(val) + 1);
	memcpy(_out_result, val, strlen(val) + 1);

	return true;
}

bool CLuaLink::CallExtScriptFunctionInt(char* szLuaScriptFile, char* szFunctionName, int& _out_result) {
	// Ensure we have a valid lua state
	if (!ValidLuaState())
		InitLua();

	// load script on new state
	if (luaL_loadfile(m_pLuaState, szLuaScriptFile) != LUA_OK) {
		CDebugLogger::LogError("CallExtScriptFunctionInt: Failed to load file %s\n", szLuaScriptFile);
		return false;
	}

	// run script
	if (lua_pcall(m_pLuaState, 0, 0, 0) != LUA_OK) {
		CDebugLogger::LogError("CallExtScriptFunctionInt: Failed to run script\n");
		return false;
	}

	// get function
	lua_getglobal(m_pLuaState, szFunctionName);

	// call function (0 args, 1 return value)
	if (lua_pcall(m_pLuaState, 0, 1, 0) != LUA_OK) {
		CDebugLogger::LogError("CallExtScriptFunctionInt: Failed to call function\n");
		return false;
	}

	// get & save return val
	if (!lua_isinteger(m_pLuaState, -1)) {
		CDebugLogger::LogError("CallExtScriptFunctionInt: Got non-int value\n");
		return false;
	}
	_out_result = ( int )lua_tointeger(m_pLuaState, -1);

	return true;
}


bool CLuaLink::CallExtScriptFunction(char* szLuaScriptFile, char* szFunctionName) {
	// Ensure we have a valid lua state
	if (!ValidLuaState())
		InitLua();

	// load script on new state
	if (luaL_loadfile(m_pLuaState, szLuaScriptFile) != LUA_OK) {
		CDebugLogger::LogError("CallExtScriptFunctionInt: Failed to load file %s\n", szLuaScriptFile);
		return false;
	}

	// run script
	if (lua_pcall(m_pLuaState, 0, 0, 0) != LUA_OK) {
		CDebugLogger::LogError("CallExtScriptFunctionInt: Failed to run script\n");
		return false;
	}

	// get function
	lua_getglobal(m_pLuaState, szFunctionName);

	// call function (0 args, 0 return values)
	if (lua_pcall(m_pLuaState, 0, 0, 0) != LUA_OK) {
		CDebugLogger::LogError("CallExtScriptFunctionInt: Failed to call function\n");
		return false;
	}

	return true;
}

bool CLuaLink::DumpSDK(char* szSDKFilePath) {
	FILE* file = NULL;
	if (fopen_s(&file, szSDKFilePath, "w+") > 0) {
		// unable to open file
		CDebugLogger::LogError("CLuaLink::DumpSDK fatal error: Unable to open file %s", szSDKFilePath);
		return false;
	}

	// SDK version & name 
	fprintf(file, "\n%s SDK version %i.%i", m_sSDKName.c_str(), m_iSDKVersionMajor, m_iSDKVersionMinor);

	// Functions which must be defined
	for (unsigned int i = 0; i < EXPECTEDFUNCS_COUNT; i++) {
//		fprintf_s(file, "\nREQUIRED FUNCTION %i\n\tName:\t%s\nDescription:\t%s", i + 1, m_szRequiredFuncsName[i], m_szRequiredFuncDesc[i]);
	}

	// Functions which have been custom defined
	for (unsigned int i = 0; i < m_vSDK.size(); i++) {
		fprintf_s(file, "\nFUNCTION %i\n\tName:\t%s\nDescription:\t%s", i + 1, m_vSDK[i].sFunc.c_str(), 
			m_vSDK[i].sDesc.length() <= 0 ? "*(Missing description)" : m_vSDK[i].sDesc.c_str());
	}

	fclose( file );
	return true;
}

bool CLuaLink::PrintSDK() {
	// SDK version & name 
	fprintf(stdout, "\n%s SDK version %i.%i", m_sSDKName.c_str(), m_iSDKVersionMajor, m_iSDKVersionMinor);

	// Functions which must be defined
	for (unsigned int i = 0; i < EXPECTEDFUNCS_COUNT; i++) {
	//	fprintf_s(stdout, "\nREQUIRED FUNCTION %i\n\tName:\t%s\nDescription:\t%s", i + 1, m_szRequiredFuncsName[i], m_szRequiredFuncDesc[i]);
	}

	// Functions which have been custom defined
	for (unsigned int i = 0; i < m_vSDK.size(); i++) {
		fprintf_s(stdout, "\nFUNCTION %i\n\tName:\t%s\nDescription:\t%s", i + 1, m_vSDK[i].sFunc.c_str(),
			m_vSDK[i].sDesc.length() <= 0 ? "*(Missing description)" : m_vSDK[i].sDesc.c_str());
	}

	return true;
}*/