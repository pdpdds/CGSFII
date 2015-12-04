// TankServer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "ProtoBufLogicEntry.h"
#include "ProtoBufProtocol.h"
#include "SFPacketProtocol.h"
#include "SFMultiLogicDispatcher.h"

#pragma comment(lib, "EngineLayer.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	ProtoBufLogicEntry* pLogicEntry = new ProtoBufLogicEntry();
	
	auto errorCode = SFEngine::GetInstance()->Intialize(pLogicEntry, new SFMultiLogicDispatcher(10));
	if (errorCode != NET_ERROR_CODE::SUCCESS)
		return 0;

	SFEngine::GetInstance()->AddPacketProtocol(new SFPacketProtocol<ProtoBufProtocol>);

	bool result = SFEngine::GetInstance()->Start();

	if (false == result)
		return 0;

	google::FlushLogFiles(google::GLOG_INFO);

	getchar();

	SFEngine::GetInstance()->ShutDown();	

	return 0;
}