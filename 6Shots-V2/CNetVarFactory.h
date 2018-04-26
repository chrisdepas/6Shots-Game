#ifndef __CNETVARFACTORY_H__
#define __CNETVARFACTORY_H__

#include "CNetVar.h"
#include <vector>

class INetVarListener {
	// NetVar with iID has changed -> send this out!
	virtual void OnNetVarChange(int iID) = 0;
};

class CNetVarFactory {
	// array of netvars 
	std::vector<CNetVarBase*> m_vpNetVar;
	
	// last uniquely assigned netvar ID
	int m_iLastID = 48879;

	// Listener
	INetVarListener* pListener;

public:

	// Initialiser - sets callback for netvar change
	void Init(INetVarListener* pListener);

	// Create netvar
	template <typename t>
	int CreateNetVar(t var, std::string& sVarName);

	// Get netvar by name
	template <typename t>
	CNetVar<t>* GetNetVar(std::string& sVarName);

	// Get netvar by id
	template <typename t>
	CNetVar<t>* GetNetVar(int iID);
};

#endif