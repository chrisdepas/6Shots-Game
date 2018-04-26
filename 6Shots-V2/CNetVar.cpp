#include "stdafx.h"
#include "CNetVar.h"


template <typename T>
CNetVar<T>::CNetVar(T var, int iID, std::string& sVarUID) {
	m_Var = var;
	m_iID = iID;
	m_sVarUID = sVarUID;
}

template <typename T>
T CNetVar<T>::Value() {
	return m_Var;
}

template <typename T>
void CNetVar<T>::Set(T val) {
	m_Var = val;
}