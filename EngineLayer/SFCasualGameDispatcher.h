#pragma once
#include "SFLogicDispatcher.h"
#include "TinyThread/tinythread.h"

class SFCasualGameDispatcher : public SFLogicDispatcher
{
	typedef std::map<int, tthread::thread*> mapThread;

public:
	SFCasualGameDispatcher(void);
	virtual ~SFCasualGameDispatcher(void);

	virtual void Dispatch(BasePacket* pPacket) override;
	virtual bool CreateLogicSystem(ILogicEntry* pLogicEntry) override;
	virtual bool ShutDownLogicSystem() override;

	virtual bool AddRPCService(IRPCService* pService) override;

private:
	static bool m_bLogicEnd;
	int m_nLogicThreadCnt;		
	mapThread m_mapThread;

	IRPCService* m_pRPCService;

	static void LogicThreadProc(void* Args);
	static void RPCThreadProc(void* Args);
};

