// ServerConnector.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "ServerConnectorLogicEntry.h"
#include "SFJsonProtocol.h"
#include "SFJsonPacket.h"
#include "SFServerConnectionManager.h"
#include "AuthServerCallback.h"
#include "DatabaseServerCallback.h"
#include "SFPacketProtocol.h"

#pragma comment(lib, "EngineLayer.lib")

#define GAME_DATABASE_SERVER_1 10000
#define AUTH_SERVER_1 9000

int _tmain(int argc, _TCHAR* argv[])
{
	ServerConnectorLogicEntry* pLogicEntry = new ServerConnectorLogicEntry();

//������ Ŀ���Ϳ� ���� �ݹ� ��ü�� �߰��Ѵ�.
	AuthServerCallback* pAuthServerCallback = new AuthServerCallback();
	DatabaseServerCallback* pDatabaseServerCallback = new DatabaseServerCallback();

	SFEngine::GetInstance()->Intialize(pLogicEntry);
	int packetProtocolIndex = SFEngine::GetInstance()->AddPacketProtocol(new SFPacketProtocol<SFJsonProtocol>);

	if (false == SFEngine::GetInstance()->LoadConnectorList("ServerConnection.xml"))
		return 0;

	pLogicEntry->AddConnectorCallback(AUTH_SERVER_1, pAuthServerCallback, packetProtocolIndex);
	pLogicEntry->AddConnectorCallback(GAME_DATABASE_SERVER_1, pDatabaseServerCallback, packetProtocolIndex);

	if (false == SFEngine::GetInstance()->SetupServerReconnectSys())
		return 0;

	getchar();

	SFEngine::GetInstance()->ShutDown();

	return 0;
}

ILogicEntry* ServerConnectorLogicEntry::Clone()
{
	return new ServerConnectorLogicEntry();
}