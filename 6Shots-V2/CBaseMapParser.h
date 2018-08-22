#ifndef __CBASEMAPPARSER_H__
#define __CBASEMAPPARSER_H__

#include <string>
#include <fstream>

class C6SMap;

struct CMapHeader {
	char fileIdentifier[8];
	short versionMajor;
	short versionMinor;
	short versionRevision;

	CMapHeader(){}
	CMapHeader(short _versionMajor, short _versionMinor, short _versionRevision) {
		char header[8] = { '\x06', 'M', 'A', 'P', 'F', 'I', 'L', 'E' };
		memcpy(fileIdentifier, header, 8);
		versionMajor = _versionMajor;
		versionMinor = _versionMinor;
		versionRevision = _versionRevision;
	}

	bool isValidHeader() {
		char header[8] = { '\x06', 'M', 'A', 'P', 'F', 'I', 'L', 'E' };
		return strncmp(fileIdentifier, header, 8) == 0;
	}
};

struct CMapSection {
	char sectionID[4];
	int sectionLength;

	int length() { return sectionLength; }
};

class CBaseMapParser {
protected:
	std::string m_sMapFile;
public:
	CBaseMapParser(char* szMapFile) {
		m_sMapFile = szMapFile;
	}

	bool WriteHeader(FILE* file, short versionMajor, short versionMinor, short revision) {
		CMapHeader header(versionMajor, versionMinor, revision);
		return fwrite(&header, sizeof(char), sizeof(header), file) == sizeof(header);
	}
	bool ReadHeader(std::ifstream& stream, CMapHeader& header) {
		CMapHeader header;
		return (stream.read((char*)&header, sizeof(CMapHeader)) && header.isValidHeader());
	}

	virtual bool ParseMapFile(C6SMap& _out_map);
	virtual bool WriteMapFile(C6SMap& mapFile);
};

#endif