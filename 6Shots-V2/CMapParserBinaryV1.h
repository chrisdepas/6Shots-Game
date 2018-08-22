#ifndef __CMAPPARSERBINARYV1_H__
#define __CMAPPARSERBINARYV1_H__

#include "CBaseMapParser.h"

class CMapParserBinaryV1 : public CBaseMapParser {
	const short version[3] = {1, 0, 0};
public:

	CMapParserBinaryV1(char* szFile) : CBaseMapParser(szFile) {}

	virtual bool ParseMapFile(char* pFileBytes, int iFileLen, C6SMap& _out_map) {

	}
	void WriteMapImage();
	void WriteMapBackground();
	void WriteMapTiles();

	virtual bool WriteMapFile(char* szFile, C6SMap& mapFile) {
		FILE* file = std::fopen(szFile, "wb");
		if (!file)
			return false;

		WriteHeader(file, version[0], version[1], version[2]);
	}
};


#endif