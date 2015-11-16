// ServerListener.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "ServerListenerLogicEntry.h"
#include "SFJsonProtocol.h"
#include "SFCGSFPacketProtocol.h"

#pragma comment(lib, "EngineLayer.lib")

#define RPC_LISTEN_PORT 10000
#define GAMESERVER_LISTEN_PORT 10001
#define AUTHSERVER_LISTEN_PORT 10001

int _tmain(int argc, _TCHAR* argv[])
{
	ServerListenerLogicEntry* pLogicEntry = new ServerListenerLogicEntry();
	SFEngine::GetInstance()->Intialize(pLogicEntry);

	int protocolIndex1 = SFEngine::GetInstance()->AddPacketProtocol(new SFPacketProtocol<SFJsonProtocol>);
	int protocolIndex2 = SFEngine::GetInstance()->AddPacketProtocol(new SFPacketProtocol<SFJsonProtocol>(16384, 8192, 0));
	int protocolIndex3 = SFEngine::GetInstance()->AddPacketProtocol(new SFPacketProtocol<SFCGSFPacketProtocol>);

	int rpcListener = SFEngine::GetInstance()->AddListener(nullptr, RPC_LISTEN_PORT, protocolIndex1);
	int gameListener = SFEngine::GetInstance()->AddListener(nullptr, GAMESERVER_LISTEN_PORT, protocolIndex2);
	int authListener = SFEngine::GetInstance()->AddListener(nullptr, AUTHSERVER_LISTEN_PORT, protocolIndex3);
	
	if (false == SFEngine::GetInstance()->Start(-1))
	{
		LOG(ERROR) << "Server Start Fail";
		SFEngine::GetInstance()->ShutDown();
		return 0;
	}	

	getchar();

	SFEngine::GetInstance()->ShutDown();

	return 0;
}