#ifndef __CGLOBALVARS_H__
#define __CGLOBALVARS_H__


/*
	Christopher De Pasquale	
	25/03/17

	Storage of variables which are only important locally.
*/

																	
#include <string>

template <typename T>
struct SGlobalVar {
	std::string m_sVarStr;

	bool Matches(std::string& str) {
		return str.compare(m_sVarStr) == 0;
	}
};
class CGlobalVars {
	std::

};

#endif