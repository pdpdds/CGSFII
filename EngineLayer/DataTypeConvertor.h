#ifndef __datatypeconverter_h__
#define __datatypeconverter_h__

#pragma once

#include <stdlib.h>
#include <string>

#pragma warning (disable : 4244) 

typedef std::basic_string<char> tstring;

namespace Datatype
{

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	inline void ToOwnValue(const char* text, T& numeric) 
	{}

	template<typename T>
	inline void ToOwnValue(const char* text, tstring& numeric) 
	{ 
		numeric = text; 
	}

	template<typename T>
	inline void ToOwnValue(const char* text, int& numeric) 
	{ 
		numeric = atoi(text); 
	}

	template<typename T>
	inline void ToOwnValue(const char* text, char& numeric) 
	{ 
		numeric = atoi(text); 
	}

	template<typename T>
	inline void ToOwnValue(const char* text, unsigned char& numeric) 
	{ 
		numeric = atoi(text);	
	}

	template<typename T>
	inline void ToOwnValue(const char* text, short& numeric) 
	{ 
		numeric = atoi(text); 
	}

	template<typename T>
	inline void ToOwnValue(const char* text, unsigned short& numeric) 
	{ 
		numeric = atoi(text);	
	}

	template<typename T>
	inline void ToOwnValue(const char* text, unsigned int& numeric) 
	{ 
		numeric = strtoul(text, NULL, 10);	
	}

	template<typename T>
	inline void ToOwnValue(const char* text, float& numeric)
	{ 
		numeric = (float)atof(text); 
	}

	template<typename T>
	inline void ToOwnValue(const char* text, double& numeric)
	{ 
		numeric = atof(text); 
	}

	template<typename T>
	inline void ToOwnValue(const char* text, bool& numeric) 
	{ 
		numeric = (strcmp(text, "true")==0);
	}


	//////////////////////////////////////////////////////////////////////////

	template<typename T>
	inline void ToString(char* dstStr, T srcValue, unsigned int bufferLen )
	{}

	template<typename T>
	inline void ToString(char* dstStr, tstring srcValue, unsigned int bufferLen ) 
	{ 
#ifdef _WIN32
		sprintf_s(dstStr, bufferLen, "%s", srcValue.c_str());
#else
		sprintf(dstStr, "%s", srcValue.c_str());
#endif
	}

	template<typename T>
	inline void ToString(char* dstStr, char* srcValue, unsigned int bufferLen ) 
	{ 
#ifdef _WIN32
		sprintf(dstStr, "%s", srcValue);
#else
#endif
	}

	template<typename T>
	inline void ToString(char* dstStr, const char* srcValue, unsigned int bufferLen ) 
	{ 
#ifdef _WIN32
		sprintf_s(dstStr, bufferLen, "%s", srcValue);
#else
		sprintf(dstStr, "%s", srcValue);
#endif
	}

	template<typename T>
	inline void ToString(char* dstStr, int srcValue, unsigned int bufferLen ) 
	{
#ifdef _WIN32
		sprintf_s(dstStr, bufferLen, "%d", srcValue);
#else
		sprintf(dstStr, "%d", srcValue);
#endif
	}

	template<typename T>
	inline void ToString(char* dstStr, char srcValue, unsigned int bufferLen ) 
	{
#ifdef _WIN32
		sprintf_s(dstStr, bufferLen, "%d", srcValue);
#else		
		sprintf_s(dstStr, "%d", srcValue);
#endif

	}

	template<typename T>
	inline void ToString(char* dstStr, unsigned char srcValue, unsigned int bufferLen ) 
	{
#ifdef _WIN32
		sprintf_s(dstStr, bufferLen, "%d", srcValue);
#else
		sprintf(dstStr, "%d", srcValue);
#endif

	}

	template<typename T>
	inline void ToString(char* dstStr, short srcValue, unsigned int bufferLen ) 
	{
#ifdef _WIN32
		sprintf_s(dstStr, bufferLen, "%d", srcValue);
#else
		sprintf(dstStr, "%d", srcValue);
#endif
	}

	template<typename T>
	inline void ToString(char* dstStr, unsigned short srcValue, unsigned int bufferLen ) 
	{
#ifdef _WIN32
		sprintf_s(dstStr, bufferLen, "%d", srcValue);
#else
		sprintf(dstStr, "%d", srcValue);
#endif
	}

	template<typename T>
	inline void ToString(char* dstStr, unsigned int srcValue, unsigned int bufferLen ) 
	{
#ifdef _WIN32
		sprintf_s(dstStr, bufferLen, "%d", srcValue);
#else
		sprintf(dstStr, "%d", srcValue);
#endif
	}

	template<typename T>
	inline void ToString(char* dstStr, float srcValue, unsigned int bufferLen ) 
	{
#ifdef _WIN32
		sprintf_s(dstStr, bufferLen, "%.4f", srcValue);
#else
		sprintf(dstStr, "%.4f", srcValue);
#endif
	}

	template<typename T>
	inline void ToString(char* dstStr, double srcValue, unsigned int bufferLen ) 
	{
#ifdef _WIN32
		sprintf_s(dstStr, bufferLen, "%.4f", srcValue);
#else
		sprintf(dstStr, "%.4f", srcValue);
#endif
	}

	template<typename T>
	inline void ToString(char* dstStr, bool srcValue, unsigned int bufferLen ) 
	{
	
#ifdef _WIN32
		if (srcValue)
			sprintf_s(dstStr, bufferLen, "true");
		else
			sprintf_s(dstStr, bufferLen, "false");
#else
		if (srcValue)
			sprintf(dstStr, "true");
		else
			sprintf(dstStr, "false");
#endif
	}
} // namespace HalfNetwork


#endif // __datatypeconverter_h__