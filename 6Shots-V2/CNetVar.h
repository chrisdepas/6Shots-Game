#ifndef __CNETVAR_H__
#define __CNETVAR_H__

class CNetVarBase {
	std::string m_sVarUID;
	int m_iID;
};

template <typename T>
class CNetVar : public CNetVarBase {
	T m_Var;

	CNetVar(T var, int iID, std::string& sVarUID);

	T Value();

	void Set(T val);
};


#endif