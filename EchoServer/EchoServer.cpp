#include "stdafx.h"
#include "EchoLogicEntry.h"
#include "SFCGSFPacketProtocol.h"
#include "SFMultiLogicDispatcher.h"

#pragma comment(lib, "EngineLayer.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	EchoLogicEntry* pLogicEntry = new EchoLogicEntry();
	
	auto errorCode = SFEngine::GetInstance()->Intialize(pLogicEntry);
	//auto errorCode = SFEngine::GetInstance()->Intialize(pLogicEntry, new SFMultiLogicDispatcher(10));
	if (errorCode != NET_ERROR_CODE::SUCCESS)
		return 0;

	SFEngine::GetInstance()->AddPacketProtocol(new SFPacketProtocol<SFCGSFPacketProtocol>);	
	bool result = SFEngine::GetInstance()->Start();

	if (false == result)
		return 0;

	getchar();
	
	SFEngine::GetInstance()->ShutDown();

	return 0;
}