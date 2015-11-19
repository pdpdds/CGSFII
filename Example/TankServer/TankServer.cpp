// TankServer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "TankLogicEntry.h"
#include "TankProtocol.h"
#include "SFPacketProtocol.h"
#include "SFMultiLogicDispatcher.h"

#pragma comment(lib, "EngineLayer.lib")

int _tmain(int argc, _TCHAR* argv[])
{

	TankLogicEntry* pLogicEntry = new TankLogicEntry();
	SFEngine::GetInstance()->Intialize(pLogicEntry, new SFMultiLogicDispatcher(10));
	SFEngine::GetInstance()->AddPacketProtocol(new SFPacketProtocol<TankProtocol>);

	SFEngine::GetInstance()->Start(0);

	google::FlushLogFiles(google::GLOG_INFO);

	getchar();

	SFEngine::GetInstance()->ShutDown();

	return 0;
}

