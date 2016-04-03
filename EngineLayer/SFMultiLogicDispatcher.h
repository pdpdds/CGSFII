#pragma once
#include "EngineLayerHeader.h"
#include "SFLogicDispatcher.h"
#ifdef _WIN32
#include "SFIOCPQueue.h"
#include "tinythread.h"
#else
#include "../BaseLayerLinux/SFLockQueue.h"
#include "../BaseLayerLinux/tinythread.h"
#endif
#include <map>

typedef struct tag_ClientInfo
{
	int serial;
	int channel;
}ClientInfo;

class SFMultiLogicDispatcher : public SFLogicDispatcher
{
	typedef std::map<int, SFLockQueue<BasePacket>*> mapQueue;
	typedef std::map<int, tthread::thread*> mapThread;
	typedef std::map<int, ClientInfo> mapClient;

public:
	SFMultiLogicDispatcher(int channelCount = 1);
	virtual ~SFMultiLogicDispatcher();
	
	virtual void Dispatch(BasePacket* pPacket) override;
	//virtual bool OnAuthenticate(BasePacket* pPacket) = 0;

	virtual bool CreateLogicSystem(ILogicEntry* pLogicEntry) override;
	virtual bool ShutDownLogicSystem() override;	

	int GetMaxChannelCount() { return m_channelCount; }

	void RegisterClient(BasePacket* pPacket);
	void UnregisterClient(BasePacket* pPacket);
	bool ChangeChannel(int serial, int channelId);
	ClientInfo* FindClient(int serial);

protected:

private:
	int m_channelCount;
	mapQueue m_mapQueue;
	mapThread m_mapThread;

	mapClient m_mapClient;

	tthread::thread* m_packetDistrubutor;

	static void PacketDistributorProc(void* Args);
	static void MultiLogicProc(void* Args);	
};

