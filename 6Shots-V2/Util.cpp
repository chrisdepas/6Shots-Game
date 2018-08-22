#include "stdafx.h"
#include "Util.h"

/*	Christopher De Pasquale
**	Created:	24th April 2018
**	Last Edit:	25th April 2018
*/

namespace std {

};

namespace util {
	/* int ToIntFast(float floatVal) {
		int result;

		// Load float into FPU stack
		__asm FLD floatVal; 
		// Convert to signed int in truncation rounding mode, store in result
		__asm FISTTP result; 

		return result;
	}*/

	sf::Color toSFMLColor(const b2Color& c) {
		return sf::Color(
			(sf::Uint8)(c.r * 255),
			(sf::Uint8)(c.g * 255),
			(sf::Uint8)(c.b * 255),
			(sf::Uint8)(c.a * 255)
			);
	}

	sf::Vector2f toSFMLVec2f(const b2Vec2& v) {
		return sf::Vector2f(v.x, v.y);
	}

	sf::Vector2i toSFMLVec2i(const b2Vec2& v) {
		return sf::Vector2i((int)v.x, (int)v.y);
	}

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

	bool StrInt(std::string str, int& _out_int) {
		try {
			Trim(str);
			_out_int = std::stoi(str);
			return true;
		} catch (...) {
			return false;
		}
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

	bool CharAlphaNum(char c) {
		return CharAlpha(c) || CharNum(c);
	}

	bool CharAlpha(char c) {
		return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
	}

	bool CharNum(char c) {
		return (c >= '0' && c <= '9');
	}

	std::string& StrLower(std::string& string) {
		for (unsigned int i = 0; i < string.length(); i++) {
			string[i] = CharToLower(string[i]);
		}
		return string;
	}

	std::vector<std::string> StrSplit(std::string& str, char delim) {
		std::vector<std::string> subStrs;

		const char* cStr = str.c_str();
		char* wordStart = (char*)cStr;
		char* strPos = (char*)cStr;
		char* strEnd = wordStart + str.length();

		while (strPos < strEnd) {

			if (*strPos == delim) {
				// Copy string before delimeter, if present
				if (strPos - wordStart > 0) {
					subStrs.push_back(std::string(wordStart, 0, strPos - wordStart));
				}

				// Skip until next non-delimeter character 
				while (*strPos == delim && strPos < strEnd) { strPos++; }
				wordStart = strPos;

				// Return substring list if at end of string
				// Insert entire string if no delimeters are found
				if (strPos >= strEnd) {
					//if (subStrs.size() <= 0)
					//	subStrs.push_back(str);
					return subStrs;
				}
			}
			else {
				strPos++;
			}
		}

		// Copy last string, given string ends on non-delimeter char
		if (strPos - wordStart > 0) {
			subStrs.push_back(std::string(wordStart, 0, strPos - wordStart));
		}

		return subStrs;
	}

	//
	bool StrSplitQuotation(std::string& str, std::vector<std::string>& result, char* szQuotationChars) {
		int quotationStart, len = str.length(), quotLen = strlen(szQuotationChars);
		char curQuotation = 0;
		const char* szString = str.c_str();

		for (int i = 0; i < len; i++) {

			for (int j = 0; j < quotLen; j++) {
				if (szString[i] == szQuotationChars[j]) {
					// Found quotation, skip to end
					quotationStart = i;
					i++;
					while (i < len && szString[i] != szQuotationChars[j])
						i++;

					// if end of string reached, quotation isn't terminated
					if (i >= len)
						return false;

					result.push_back(std::string(szString, quotationStart+1, i-quotationStart-1));
				}
			}
		}
		// Copy input into output if string contains no quotations
		if (result.size() <= 0) {
			result.push_back(str);
		}
		return true;
	}

	// Splits a string by quoted substrings, then by a delimeter (e.g. ' ')
	// Given input: "echo "This is an echo'd command" RED", result is:
	// {"echo", "This is an echo'd command", "RED"}
	bool StrSplitQuoteDelim(std::string& str, std::vector<std::string>& _out_result, char delim, char* szQuotationChars) {
		int quotationStart = -1, len = str.length(), quotLen = strlen(szQuotationChars);
		char curQuotation = 0;
		const char* szString = str.c_str();

		for (int i = 0; i < len; i++) {

			for (int j = 0; j < quotLen; j++) {
				if (szString[i] == szQuotationChars[j]) {
					quotationStart = i;
					curQuotation = szQuotationChars[j];
				}
			}

			if (quotationStart > 0) {
				// Found quotation, skip to end
				i++;
				while (i < len && szString[i] != curQuotation)
					i++;

				// if end of string reached, quotation isn't terminated
				if (i >= len)
					return false;

				int substrlen = i - quotationStart - 1;
				if (substrlen > 0)
					_out_result.push_back(std::string(szString, quotationStart + 1, i - quotationStart - 1));

				quotationStart = 0;

			} else if (szString[i] != delim) {
				// unquoted string without delimeter, skip to end and copy word
				quotationStart = i;
				while (i < len && szString[i] != delim) { i++; }

				if (i >= len)
					i = len;

				int substrlen = i - quotationStart;
				if (substrlen > 0)
					_out_result.push_back(std::string(szString, quotationStart, substrlen));

				quotationStart = 0;
			}
		}

		// Copy input into output if string contains no quotations
		if (_out_result.size() <= 0)
			_out_result.push_back(str);
		return true;
	}

	// Splits a string by quoted substrings, then by a delimeter (e.g. ' ')
	// Given input: "echo "This is an echo'd command" RED", result is:
	// {"echo", "This is an echo'd command", "RED"}
	bool StrSplitQuoteDelim2(std::string& str, std::vector<std::string>& _out_result, char delim, char* szQuotationChars) {
		int quotationStart, len = str.length(), quotLen = strlen(szQuotationChars);
		char curQuotation = 0;
		const char* szString = str.c_str();
		bool bFoundQuote = false;

		for (int i = 0; i < len; i++) {

			for (int j = 0; j < quotLen; j++) {
				if (szString[i] == szQuotationChars[j]) {
					// Found quotation, skip to end
					quotationStart = i;
					i++;
					while (i < len && szString[i] != szQuotationChars[j])
						i++;

					// if end of string reached, quotation isn't terminated
					if (i >= len)
						return false;

					_out_result.push_back(std::string(szString, quotationStart + 1, i - quotationStart - 1));
					bFoundQuote = true;
					break;
				}
			}
			if (bFoundQuote) {
				bFoundQuote = false;
				continue;
			}

			// Text is not a quotation or delimeter, copy word
			if (szString[i] != delim) {
				quotationStart = i;
				while (i < len && szString[i] != delim)
					i++;
				if (i >= len) {
					// Reached end of string without a delimeter, copy from start to end of string
					int substrlen = len - quotationStart;
					if (substrlen > 0)
						_out_result.push_back(std::string(szString, quotationStart, substrlen));
				}
				else {
					if (i - quotationStart > 0)
						_out_result.push_back(std::string(szString, quotationStart, i - quotationStart));
				}

				continue;
			}
		}
		// Copy input into output if string contains no quotations
		if (_out_result.size() <= 0) {
			_out_result.push_back(str);
		}
		return true;
	}

	bool StrAlphaNumeric(std::string& str) {
		for (unsigned int i = 0; i < str.length(); i++) {
			if (!CharAlphaNum(str[i])) 
				return false;
		}
		return true;
	}

	bool StrAlpha(std::string& str) {
		for (unsigned int i = 0; i < str.length(); i++) {
			if (!CharAlpha(str[i]))
				return false;
		}
		return true;
	}

	bool StrNum(std::string& str) {
		for (unsigned int i = 0; i < str.length(); i++) {
			if (!CharNum(str[i]))
				return false;
		}
		return true;
	}

	bool StrContains(std::string strHaystack, std::string& strNeedle) {
		return strHaystack.find(strNeedle) != std::string::npos;
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
	
	bool FileExists(std::string& sFilePath) {
		std::ifstream fStream(sFilePath);
		return fStream.good();
	}
}