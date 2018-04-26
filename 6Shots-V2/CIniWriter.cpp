#include "stdafx.h"
#include "CIniWriter.h"
#include <stdlib.h>
#include <fstream>
#include <string>

void CIniWriter::ClearItems() {
	m_Items.clear();
}

void CIniWriter::StoreBoolItem(char* szKey, bool bValue) {
	std::string iniString = szKey;
	iniString += "=";
	iniString += bValue ? "1" : "0";
	m_Items.push_back(iniString);
}

void CIniWriter::StoreIntItem(char* szKey, int iValue) {
	std::string iniString = szKey;
	iniString += "=";
	iniString += std::to_string(iValue);
	m_Items.push_back(iniString);
}
void CIniWriter::StoreStringItem(char* szKey, char* szValue)
{
	std::string iniString = szKey;
	iniString += "=";
	iniString += szValue;
	m_Items.push_back(iniString);
}

void CIniWriter::StoreFloatItem(char* szKey, float fValue) {
	std::string iniString = szKey;
	iniString += "=";
	iniString += std::to_string(fValue);
	m_Items.push_back(iniString);
}

void CIniWriter::StoreComment(char* szComment) {
	std::string iniString = ";";
	iniString += szComment;
	m_Items.push_back(iniString);
}

void CIniWriter::StoreSectionHeader(char* szSectionName) {
	std::string iniString = "[";
	iniString += szSectionName;
	iniString += "]";
	m_Items.push_back(iniString);
}

bool CIniWriter::WriteToFile(char* szFileName)
{
	std::ofstream ini(szFileName, std::ofstream::out | std::ofstream::trunc);
	if (!ini.is_open())
		return false;

	for (unsigned int i = 0; i < m_Items.size(); i++) {
		ini << m_Items[i];
		ini << "\n";
	}

	ini.close();
	return true;
}