#ifndef __CINIITEM_H__
#define __CINIITEM_H__

/*
	Michael & Christopher De Pasquale
	Created:	Unknown, notice added 12 December 2016
	Last Edit:	24th April 2018
*/

struct CIniItem
{
	std::string	m_sKey;
	std::string m_sValue;

	bool MatchesKey(char* Key); /* Check if this this is the keyname supplied */
	bool MatchesKey(std::string& key); /* Check if this this is the keyname supplied */

	// Returns bool indicating success, and output value in __return_bool
	// Valid values for bool: f/t 0/1 false/true on/off. Not case sensitive
	bool GetBool(bool& __return_bool);

	bool GetInt(int& __return_int);
	bool GetFloat(float& __return_float);
	std::string GetString();

	/* !unsafe! Returns iniItem value string c_str(), pointer becomes invalid when IniItem is destroyed. Don't save! */
	const char* GetCString(); 

	CIniItem(std::string& key, std::string& value);

};

#endif