#include "stdafx.h"
#include "EchoLogicEntry.h"
#include "SFCGSFPacketProtocol.h"

#pragma comment(lib, "EngineLayer.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	EchoLogicEntry* pLogicEntry = new EchoLogicEntry();
	
	auto errorCode = SFEngine::GetInstance()->Intialize(pLogicEntry);	
	if (errorCode != NET_ERROR_CODE::SUCCESS)
	{	
		return 0;
	}

	SFEngine::GetInstance()->AddPacketProtocol(new SFPacketProtocol<SFCGSFPacketProtocol>);
	SFEngine::GetInstance()->Start();

	getchar();
	
	SFEngine::GetInstance()->ShutDown();

	return 0;
}