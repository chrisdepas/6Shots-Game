#include "stdafx.h"
#include "CIniItem.h"
#include "Util.h"
#include "CDebugLogger.h"
#include "Util.h"
/*
	Michael & Christopher De Pasquale
	Created:	Unknown, notice added 12 December 2016
	Last Edit:	24th April 2018
*/

CIniItem::CIniItem(std::string& key, std::string& value) {
	m_sKey = key;
	m_sValue = value;
}

bool CIniItem::MatchesKey(char* Key) {
	return !_stricmp(Key, m_sKey.c_str());
}

bool CIniItem::MatchesKey(std::string& key) {
	return !_stricmp(key.c_str(), m_sKey.c_str());
}

bool CIniItem::GetBool(bool& __return_bool) {
	// Get lowercase string
	std::string lowerValue = m_sValue;
	util::StrLower(lowerValue);

	if (lowerValue.length() == 1) {
		if (lowerValue.compare("1") == 0 || lowerValue.compare("t") == 0) {
			__return_bool = true;
			return true;
		}
		if (lowerValue.compare("0") == 0 || lowerValue.compare("f") == 0) {
			__return_bool = false;
			return true;
		}
	}
	else if (lowerValue.length() < 6) {
		if (lowerValue.compare("true") == 0 || lowerValue.compare("on") == 0) {
			__return_bool = true;
			return true;
		}
		if (lowerValue.compare("false") == 0 || lowerValue.compare("off") == 0) {
			__return_bool = false;
			return true;
		}
	}

	CDebugLogger::LogWarning("Failed to convert ini value '%s' for key '%s' to a bool\n", m_sValue.c_str(), m_sKey.c_str());
	return false;
}

bool CIniItem::GetInt(int& __return_int) {
	try {
		__return_int = std::stoi(m_sValue);
		return true;
	} catch (...) {
		CDebugLogger::LogWarning("Failed to convert ini value '%s' for key '%s' to an int\n", m_sValue.c_str(), m_sKey.c_str());
	}
	return false;
}

bool CIniItem::GetFloat(float& __return_float) {
	try {
		__return_float = std::stof(m_sValue);
		return true;
	} catch (...) {
		CDebugLogger::LogWarning("Failed to convert ini value '%s' for key '%s' to a float\n", m_sValue.c_str(), m_sKey.c_str());
	}
	return false;
}

std::string CIniItem::GetString() {
	return m_sValue;
}

const char* CIniItem::GetCString() {
	return m_sValue.c_str(); 
}