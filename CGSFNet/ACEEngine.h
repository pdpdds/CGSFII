#pragma once
#include "INetworkEngine.h"
#include "ProactorAcceptor.h"
#include "ReactorAcceptor.h"
#include "ACETimeOutHandler.h"
#include <map>

#ifdef ACE_PROACTOR
typedef std::map<int, ProactorAcceptor*> mapAcceptor;
#else
typedef std::map<int, ReactorAcceptor*> mapAcceptor;
#endif

class ACEEngine : public INetworkEngine
{
public:
	ACEEngine(IEngine* pEngine);
	virtual ~ACEEngine(void);

    virtual bool Shutdown() override;

	virtual bool SendRequest(BasePacket* pPacket) override;

	virtual bool Disconnect(int serial) override;

	virtual bool CheckTimerImpl() override; //default timer »ç¿ë
	virtual long AddTimer(unsigned int timerID, unsigned int startTime, unsigned int period) override;
	virtual bool CancelTimer(int timerID) override;

	virtual int AddConnector(int connectorIndex, char* szIP, unsigned short port) override;
	virtual int AddListener(char* szIP, unsigned short port) override;

	virtual bool Init() override;
	virtual bool Start() override;

protected:
	bool NetworkOpen();

	int m_acceptorIndex;
	int m_connectorIndex;
	int m_workThreadGroupID;
	mapAcceptor m_mapAcceptor;

private:
	ACETimeOutHandler m_timeOutHandler;
};