#pragma once
#include "SFLogicDispatcher.h"
#include "SFIOCPQueue.h"
#include "BasePacket.h"
#include "TinyThread/tinythread.h"
#include <map>

class SFMultiLogicDispatcher : public SFLogicDispatcher
{
	typedef std::map<int, SFIOCPQueue<BasePacket>*> mapQueue;
	typedef std::map<int, tthread::thread*> mapThread;
	typedef std::map<int, int> mapSerialChannel;

public:
	SFMultiLogicDispatcher(int channelCount = 1);
	virtual ~SFMultiLogicDispatcher();
	
	virtual void Dispatch(BasePacket* pPacket) override;
	//virtual bool OnAuthenticate(BasePacket* pPacket) = 0;

	virtual bool CreateLogicSystem(ILogicEntry* pLogicEntry) override;
	virtual bool ShutDownLogicSystem() override;	

	int GetMaxChannelCount() { return m_channelCount; }

protected:

private:
	int m_channelCount;
	mapQueue m_mapQueue;
	mapThread m_mapThread;

	mapSerialChannel m_mapSerialChannel;

	tthread::thread* m_packetDistrubutor;

	static void PacketDistributorProc(void* Args);
	static void MultiLogicProc(void* Args);
	static bool m_bLogicEnd;
};

