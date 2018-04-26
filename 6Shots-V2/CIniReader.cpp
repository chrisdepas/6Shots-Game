#include "stdafx.h"
#include "CIniReader.h"
#include <stdio.h>
#include <fstream>
#include "CDebugLogger.h"
#include "Util.h"

CIniReader::CIniReader(){}
CIniReader::~CIniReader(){}

bool CIniReader::CIniSection::KeyExists(std::string& key) {
	for (unsigned int i = 0; i < m_SectionItems.size(); i++) {
		if (m_SectionItems[i].MatchesKey(key)) {
			return true;
		}
	}
	return false;
}

bool CIniReader::CIniSection::MatchesSection(char* Section) {
	return !_stricmp(Section, m_sSectionName.c_str());
}

CIniReader::CIniSection* CIniReader::GetFirstSection() {
	if (m_Sections.size() <= 0)
		return NULL;
	return &(m_Sections[0]);
}

CIniReader::CIniSection* CIniReader::GetNextSection(CIniSection* pLastSection) {
	if (!pLastSection)
		return NULL;

	for (unsigned int i = 0; i < m_Sections.size(); i++) {
		if (&(m_Sections[i]) == pLastSection) {
			if (i >= m_Sections.size()-1) {
				return NULL;
			}
			return &(m_Sections[i + 1]); 
		}
	}
	return NULL;
}
CIniItem* CIniReader::GetItem(char* Key)
{
	// Check if item is in default section first
	for (unsigned int i = 0; i < m_DefaultItems.size(); i++) {
		if (m_DefaultItems[i].MatchesKey(Key))
			return &(m_DefaultItems[i]);
	}

	// If not in default section, get first instance in other sections
	for (unsigned int i = 0; i < m_Sections.size(); i++) {
		for (unsigned int j = 0; j < m_Sections[i].m_SectionItems.size(); j++) {
			if (m_Sections[i].m_SectionItems[j].MatchesKey(Key))
				return &(m_Sections[i].m_SectionItems[j]);
		}
	}

	return NULL;
}

CIniItem* CIniReader::GetItem(char* section, char* Key)
{
	if (!section)
		return GetItem(Key);

	for (unsigned int i = 0; i < m_Sections.size(); i++) {

		if (!m_Sections[i].MatchesSection(section))
			continue;

		for (unsigned int j = 0; j < m_Sections[i].m_SectionItems.size(); j++) {
			if (m_Sections[i].m_SectionItems[j].MatchesKey(Key)) {
				return &(m_Sections[i].m_SectionItems[j]);
			}
		}
	}
	return NULL;
}

bool CIniReader::ItemExists(std::string& key, char* szSection) {
	if (!szSection) {
		for (unsigned int i = 0; i < m_DefaultItems.size(); i++) {
			if (m_DefaultItems[i].MatchesKey(key))
				return true;
		}
	} else {
		for (unsigned int i = 0; i < m_Sections.size(); i++) {

			if (szSection && !m_Sections[i].MatchesSection(szSection))
				continue;

			for (unsigned int j = 0; j < m_Sections[i].m_SectionItems.size(); j++) {
				if (m_Sections[i].m_SectionItems[j].MatchesKey(key)) {
					return true;
				}
			}

		}
	}
	return false;
}
bool CIniReader::ItemExists(char* Key, char* szSection)
{
	if (!szSection) {
		for (unsigned int i = 0; i < m_DefaultItems.size(); i++) {
			if (m_DefaultItems[i].MatchesKey(Key))
				return true;
		}
	}
	for (unsigned int i = 0; i < m_Sections.size(); i++)
	{
		if (szSection && !m_Sections[i].MatchesSection(szSection))
			continue;

		for (unsigned int j = 0; j < m_Sections[i].m_SectionItems.size(); j++) {
			if (m_Sections[i].m_SectionItems[j].MatchesKey(Key))
				return true;
		}
	}
	return false;
}

void CIniReader::ClearItems() {
	m_DefaultItems.clear();
	m_Sections.clear();
}

bool CIniReader::ParseFile(char* szFilename)
{
	ClearItems();

	std::ifstream file(szFilename);
	if (!file.is_open()) {
		CDebugLogger::LogWarning("Couldn't read settings file %s\n", szFilename);
		return false;
	}

	char* context = NULL;
	unsigned int curSection = 0;
	std::string curLine;
	while (std::getline(file, curLine))
	{
		// Skip comments 
		bool bComment = false;
		for (unsigned int i = 0; i < m_sCommentChars.length(); i++) {
			if (curLine[0] == m_sCommentChars[i]) {
				bComment = true;
				break;
			}
		} 
		if (bComment) {
			continue;
		}
		
		if (curLine[0] == SECTION_PREFIX) {

			// Remove suffix
			size_t endPos = curLine.find_first_of(SECTION_SUFFIX);
			if (endPos == std::string::npos) {
				CDebugLogger::LogWarning("Ignoring malformed Ini file section - Missing suffix character '%c'\n", SECTION_SUFFIX);
				continue;
			}
			curLine = curLine.substr(0, endPos);

			if (curLine.length() < 3) { 
				CDebugLogger::LogWarning("Ignoring malformed Ini file section - Missing section title\n");
				continue;
			}

			// Remove prefix
			curLine = curLine.substr(1, curLine.size() - 1);

			// Check if section already exists
			for (unsigned int i = 0; i < m_Sections.size(); i++) {
				if (_strcmpi(m_Sections[i].m_sSectionName.c_str(), curLine.c_str())) {
					curSection = i;
					continue;
				}
			}

			// Equivalent to m_Sections.size()-1 after insertion
			curSection = m_Sections.size();

			CIniSection newSection(curLine);
			m_Sections.push_back(newSection);

			continue;
		}

		size_t delimPos = curLine.find_first_of(ITEM_DELIM);
		if (delimPos == std::string::npos || delimPos == curLine.length() || delimPos == 0) {
			CDebugLogger::LogWarning("Ignoring malformed ini item-value pair\n\tLine: '%s'", curLine.c_str());
			continue;
		}

		// Split key-value pair over delimeter
		std::string key = curLine.substr(0, delimPos);
		std::string value = curLine.substr(delimPos + 1, curLine.length() - 1);

		util::Trim(key);
		util::Trim(value);

		if (m_Sections.size() == 0) {

			// Insert into default values 
			if (ItemExists(key)) {
				CDebugLogger::LogWarning("Skipped duplicate ini item '%s'\n", key.c_str());
				continue;
			}
			m_DefaultItems.push_back(CIniItem(key, value));

		} else {

			// Insert into current section
			if (m_Sections[curSection].KeyExists(key)) {
				CDebugLogger::LogWarning("Skipped duplicate ini item '%s' in section '%s'\n", key.c_str(), m_Sections[curSection].m_sSectionName.c_str());
				continue;
			}
			m_Sections[curSection].m_SectionItems.push_back(CIniItem(key, value));
		}
	}

	file.close();

	return true;
}

bool CIniReader::GetBoolValue(char* Key, bool& __return_bool, char* szSection)
{
	CIniItem* item = !szSection ? GetItem(Key) : GetItem(szSection, Key);
	return !item ? false : item->GetBool(__return_bool);
}

bool CIniReader::GetIntValue(char* Key, int& __return_int, char* szSection)
{
	CIniItem* item = !szSection ? GetItem(Key) : GetItem(szSection, Key);
	return !item? false : item->GetInt(__return_int);
}

bool CIniReader::GetFloatValue(char* Key, float& __return_float, char* szSection)
{
	CIniItem* item = !szSection ? GetItem(Key) : GetItem(szSection, Key);
	return !item ? false : item->GetFloat(__return_float);
}

bool CIniReader::GetStringValue(char* Key, char*& __return_string, char* szSection)
{
	CIniItem* item = !szSection ? GetItem(Key) : GetItem(szSection, Key);
	if (!item) {
		return false;
	}
	// hack
	__return_string = (char*)item->GetCString();
	return true;
}