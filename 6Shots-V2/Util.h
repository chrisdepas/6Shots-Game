#ifndef __UTIL_H__
#define __UTIL_H__

#include <string>

/*	Christopher De Pasquale

**	Helper functions & useful functions
**
**	Created:	24th April 2018
**	Last Edit:	25th April 2018
*/

namespace util {
	/* Allocate memory for a new string, copy its contents, and return the pointer. Must free() the pointer. */
	char* strcpyalloc(char* szSourceStr);
	char* strcpyalloc(char* szSourceStr, int strlen);

	/* Allocate memory for a new string, copy its contents, and return the pointer. Must delete the pointer. */
	char* strcpynew(char* szSourceStr);
	char* strcpynew(char* szSourceStr, int strlen);

	/* Returns int value of a base-10 numeric null-terminated string */
	int strtoint(char* szString);

	/* Returns int value of a base-10 numeric string, for a given length */
	int strtoint(char* szString, int len);

	/* Returns lowercase char if c is an uppercase char, otherwise returns c */
	char CharToLower(char c);

	/* Convert std::string to lowercase */
	void StringToLower(std::string& string);

	/* Case-insensitive std::string equality check */
	bool StrCaseCmp(std::string& s1, std::string& s2);

	/* Remove whitespace (Space, tabs, and newline chars)*/
	void LTrim(std::string& str);
	void RTrim(std::string& str);
	void Trim(std::string& str);
}

#endif