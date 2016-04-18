#pragma once
#include "SFLogicDispatcher.h"
#include "SFIOCPQueue.h"
#include "TinyThread/tinythread.h"
#include <map>

typedef struct tag_ClientInfo
{
	int serial;
	int channel;
}ClientInfo;

namespace CgsfNET64Lib {

	class SFNETMultiDispatcher : public SFLogicDispatcher
	{
	public:
		SFNETMultiDispatcher(int channelCount = 1);
		virtual ~SFNETMultiDispatcher();

		typedef std::map<int, SFIOCPQueue<BasePacket>*> mapQueue;
		typedef std::map<int, tthread::thread*> mapThread;
		typedef std::map<int, ClientInfo> mapClient;

		virtual void Dispatch(BasePacket* pPacket) override;
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
}
