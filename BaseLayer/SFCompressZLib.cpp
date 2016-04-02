#include "SFCompressZLib.h"

#ifdef _WIN32
#include <zlib/zlib.h>
#else
#include <zlib.h>
#endif

SFCompressZLib::SFCompressZLib(void)
{
}

SFCompressZLib::~SFCompressZLib(void)
{
}

bool SFCompressZLib::Compress(char* pDest, int& destLen, unsigned char* pSrc, int srcLen)
{
	int result = compress((unsigned char*)pDest,(uLongf*)&destLen, (const unsigned char*)pSrc, srcLen);

	return result >= 0;
}

bool SFCompressZLib::Uncompress(unsigned char* pDest, int& destLen, unsigned char* pSrc, int srcLen)
{
	int result = uncompress((unsigned char*)pDest, (uLongf*)&destLen, (const unsigned char*)pSrc, srcLen);

	return result >= 0;
}