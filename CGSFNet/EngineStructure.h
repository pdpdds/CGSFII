#pragma once
#include <string>

enum class SESSION_TYPE : int
{
	SESSION_UNKNOWN = -1,
	SESSION_LISTENER = 0,
	SESSION_CONNECTOR = 1,
};

typedef struct tag_SessionDesc
{
	SESSION_TYPE sessionType;
	int identifier;
	std::string szIP;
	unsigned short port;

	tag_SessionDesc()
	{
		sessionType = SESSION_TYPE::SESSION_UNKNOWN;
		identifier = -1;		
		port = 0;
	}

}_SessionDesc;