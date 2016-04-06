#pragma once
#include <list>
#include "../BaseLayer/SFLock.h"

class IServerConnector;

class SFServerConnectionManager
{

public:
	SFServerConnectionManager();
	virtual ~SFServerConnectionManager();

	bool SetupServerReconnectSys();
	bool LoadConnectorList(char* szFileName);	
	bool SetConnectorState(int connectorId, bool connected);

	void AddConnectorInfo(_ConnectorInfo& connectorInfo);

	bool  m_bThreadEnd;

	std::list<_ConnectorInfo>& GetConnectorInfo() { return m_listConnectorInfo; }

#ifdef _WIN32
	HANDLE GetTimerHandle(){ return m_hTimerEvent; }
#else
	bool WaitForSingleObject();
#endif

protected:	

private:
	std::list<_ConnectorInfo> m_listConnectorInfo;		
	uintptr_t m_hThread;
	unsigned int  m_dwThreadID;
#ifdef _WIN32		
	HANDLE m_hTimerEvent;
#else
	pthread_mutex_t m_lock;
	pthread_cond_t m_event;
#endif // _WIN32


	SFLock m_Lock;
};
