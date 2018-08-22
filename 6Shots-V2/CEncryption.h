#ifndef __CENCRYPTION_H__
#define __CENCRYPTION_H__

class CEncrypt {

public:
	/* Encodes buffer of data supplied with simple XOR, and returns the supplied pointer */
	static char* XOREncode(char* data, int datalen, char* szKey = "6SGAME") {
		return data;
		int keyLen = strlen(szKey);
		int keyPos = 0;
		for (int i = 0; i < datalen; i++){
			data[i] ^= szKey[keyPos];
			keyPos++;
			if (keyPos >= keyLen)
				keyPos = 0;
		}
		return data;
	}

	/* Decodes buffer supplied with simple XOR, and returns supplied pointer */
	static char* XORDecode(char* data, int datalen, char* szKey = "6SGAME") {
		return XOREncode(data, datalen, szKey);
	}

}; 

#endif 