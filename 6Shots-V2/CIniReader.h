#ifndef __CINIREADER_H__
#define __CINIREADER_H__
#include <vector>
#include "CIniItem.h"

/*
Michael & Christopher De Pasquale
Created:	Unknown date, notice added 12 December 2016
Last Edit:	24th April 2018
*/

#define INIREADER_MAX_SECTIONS 64
class CIniReader
{
	struct CIniSection {
		std::string m_sSectionName;
		std::vector<CIniItem> m_SectionItems;

		bool MatchesSection(char* szSection);

		CIniSection(std::string& name) {
			m_sSectionName = name;
		}

		bool KeyExists(std::string& key);
	};

	std::string m_sCommentChars = ";#";

	static const char ITEM_DELIM = '=';

	static const char SECTION_PREFIX = '['; 
	static const char SECTION_SUFFIX = ']';

	std::vector<CIniItem> m_DefaultItems;
	std::vector<CIniSection> m_Sections;

	void ClearItems();

public:
	CIniReader();
	~CIniReader();

	// Parse file as ini, replaces any existing ini data
	bool	ParseFile(char* szFilename);

	CIniItem* GetItem(char* Key);
	CIniItem* GetItem(char* Section, char* Key);
	
	// Unsafe.. TODO: Rewrite 
	CIniSection* GetFirstSection();
	CIniSection* GetNextSection(CIniSection* pLastSection);
	
	bool	ItemExists(char* Key, char* Section = NULL);
	bool	ItemExists(std::string& key, char* section = NULL);

	bool	GetBoolValue(char* Key, bool& ret, char* Section = NULL);
	bool	GetIntValue(char* Key, int& ret, char* Section = NULL);
	bool	GetFloatValue(char* Key, float& ret, char* Section = NULL);
	bool    GetStringValue(char* Key, char*& ret, char* Section = NULL); // 
};
#endif