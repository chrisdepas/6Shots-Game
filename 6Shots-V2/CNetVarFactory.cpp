#include "stdafx.h"
#include "CNetVarFactory.h"

template <typename T>
int CNetVarFactory::CreateNetVar(T var, std::string& sVarName) {
	CNetVar<T>* pNewNetvar = new CNetVar<t>(var, ++m_iLastID, sVarName);
	m_vpNetVar.push_back(pNewNetvar);

	// return ID
	return m_iLastID;
}

template <typename t>
CNetVar<t>* CNetVarFactory::GetNetVar(std::string& sVarName) {
	for (unsigned int i = 0; i < m_vpNetVar; i++) {
		if (m_vpNetVar[i]->m_sVarUID.compare(sVarName) == 0) {
			return m_vpNetVar[i];
		}
	}

	// Not found
	return NULL;
} 

template <typename t>
CNetVar<t>* CNetVarFactory::GetNetVar(int iID) {
	for (unsigned int i = 0; i < m_vpNetVar; i++) {
		if (m_vpNetVar[i]->m_iID == iID) {
			return m_vpNetVar[i];
		}
	}

	// Not found
	return NULL;
}