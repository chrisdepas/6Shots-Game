#include "stdafx.h"
#include "Util.h"
#include <cstring>
#include <stdlib.h>

/*	Christopher De Pasquale
**	Created:	24th April 2018
**	Last Edit:	25th April 2018
*/

namespace util {

	char* strcpyalloc(char* szSourceStr) {
		int len = strlen(szSourceStr) + 1;
		char* szNewStr = (char*)malloc(len);
		memcpy(szNewStr, szSourceStr, len);
		return szNewStr;
	}

	char* strcpyalloc(char* szSourceStr, int strlen) {
		char* szNewStr = (char*)malloc(strlen + 1);
		memcpy(szNewStr, szSourceStr, strlen);
		szNewStr[strlen] = 0;
		return szNewStr;
	}

	char* strcpynew(char* szSourceStr) {
		int len = strlen(szSourceStr) + 1;
		char* szNewStr = new char[len];
		memcpy(szNewStr, szSourceStr, len);
		return szNewStr;
	}

	char* strcpynew(char* szSourceStr, int strlen) {
		char* szNewStr = new char[strlen + 1];
		memcpy(szNewStr, szSourceStr, strlen);
		szNewStr[strlen] = 0;
		return szNewStr;
	}

	int strtoint(char* szString) {
		return strtol(szString, NULL, 10);
	}

	int strtoint(char* szString, int len) {
		//  HACK - TODO: Parse each number end to start in-place, return result
		
		char* tmpbuf = new char[len + 1];
		memcpy(tmpbuf, szString, len);
		tmpbuf[len] = 0;

		int result = strtoint(tmpbuf);

		delete tmpbuf;
		return result;
	}

	char CharToLower(char c) {
		if (c >= 'A' && c <= 'Z') {
			return (c |= 1 << 5);
		}
		return c;
	}

	void StringToLower(std::string& string) {
		for (unsigned int i = 0; i < string.length(); i++) {
			string[i] = CharToLower(string[i]);
		}
	}

	void LTrim(std::string& str) {
		for (unsigned int i = 0; i < str.length(); i++) {
			if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n') {
				continue;
			}

			if (i == 0) {
				// No whitespace in string
				break;
			}
			str = str.substr(i, str.length() - i);
			break;
		}
	}

	void RTrim(std::string& str) {
		for (unsigned int i = str.length() - 1; i > 0; i--) {
			if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n') {
				continue;
			}

			if (i == str.length() - 1) {
				// No whitespace in string
				break;
			}
			str = str.substr(0, str.length() - (str.length() - 1 - i));
			break;
		}
	}

	void Trim(std::string& str) {
		RTrim(str);
		LTrim(str);
	}

}