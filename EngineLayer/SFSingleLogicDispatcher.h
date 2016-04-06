#pragma once
#include "SFLogicDispatcher.h"
#include "../BaseLayer/tinythread.h"

class SFSingleLogicDispatcher : public SFLogicDispatcher
{
	typedef std::map<int, tthread::thread*> mapThread;

public:
	SFSingleLogicDispatcher(void);
	virtual ~SFSingleLogicDispatcher(void);

	virtual void Dispatch(BasePacket* pPacket) override;
	virtual bool CreateLogicSystem(ILogicEntry* pLogicEntry) override;
	virtual bool ShutDownLogicSystem() override;

	virtual bool AddRPCService(IRPCService* pService) override;

private:
	static bool m_bLogicEnd;	
	tthread::thread* m_pLogicThread;

	IRPCService* m_pRPCService;

	static void LogicThreadProc(void* Args);
	static void RPCThreadProc(void* Args);
};

