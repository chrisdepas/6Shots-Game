#ifndef __CINIWRITER_H__
#define __CINIWRITER_H__
#include <vector>

/*
Michael & Christopher De Pasquale
	Created:	Unknown, notice added 12 December 2016
	Last Edit:	24th April, 2018
*/

#define MAX_ITEM_SIZE 256

class CIniWriter
{
	std::vector<std::string> m_Items;

public:
	void ClearItems();

	void StoreBoolItem(char* szKey, bool bValue);
	void StoreIntItem(char* szKey, int iValue);
	void StoreStringItem(char* szKey, char* szValue);
	void StoreFloatItem(char* szKey, float fValue);

	void StoreComment(char* szComment);
	void StoreSectionHeader(char* szSectionName);
	
	bool WriteToFile(char* szFileName);
};

#endif