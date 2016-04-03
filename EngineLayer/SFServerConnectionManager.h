#pragma once
#include <list>
//#include "SFLock.h"

class IServerConnector;

class SFServerConnectionManager
{

public:
	SFServerConnectionManager();
	virtual ~SFServerConnectionManager();

	bool SetupServerReconnectSys();
	bool LoadConnectorList(char* szFileName);
	//HANDLE m_hTimerEvent;
	bool SetConnectorState(int connectorId, bool connected);

	void AddConnectorInfo(_ConnectorInfo& connectorInfo);

protected:
	unsigned int static __stdcall ServerReconnectProc(void* arg);

private:
	std::list<_ConnectorInfo> m_listConnectorInfo;
	//HANDLE m_hThread;
	bool  m_bThreadEnd;
	//DWORD  m_dwThreadID;

	//SFLock m_Lock;
};
