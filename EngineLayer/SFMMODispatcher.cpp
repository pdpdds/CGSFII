#include "stdafx.h"
#include "SFMMODispatcher.h"
#include "SFEngine.h"

bool SFMMODispatcher::m_bLogicEnd = false;

SFMMODispatcher::SFMMODispatcher(int channelCount)
: m_channelCount(channelCount)
{

}


SFMMODispatcher::~SFMMODispatcher()
{
}

void SFMMODispatcher::Dispatch(BasePacket* pPacket)
{		
	SFLogicGateway::GetInstance()->PushPacket(pPacket);	
}

bool SFMMODispatcher::CreateLogicSystem(ILogicEntry* pLogicEntry)
{
	UNREFERENCED_PARAMETER(pLogicEntry);

	m_packetDistrubutor = new tthread::thread(PacketDistributorProc, this);		

	for (int index = 0; index < m_channelCount; index++)
	{
		SFIOCPQueue<BasePacket>* pQueue = new SFIOCPQueue<BasePacket>();

		tthread::thread* pThread = new tthread::thread(MMOLogicProc, pQueue);
		m_mapThread.insert(std::make_pair(index, pThread));
		m_mapQueue.insert(std::make_pair(index, pQueue));
	}
	
	return true;
}

bool SFMMODispatcher::ShutDownLogicSystem()
{
	return true;
}

void SFMMODispatcher::PacketDistributorProc(void* Args)
{
	SFMMODispatcher* pMMODispatcher = static_cast<SFMMODispatcher*>(Args);

	//SFEngine* pEngine = SFEngine::GetInstance();

	while (m_bLogicEnd == false)
	{
		BasePacket* pPacket = SFLogicGateway::GetInstance()->PopPacket();
		
		if (false == pMMODispatcher->OnAuthenticate(pPacket))
		{
			ReleasePacket(pPacket);
			continue;
		}
				
		const auto& iter = pMMODispatcher->m_mapQueue.find(pPacket->GetChannelNum());

		if (iter == pMMODispatcher->m_mapQueue.end())
		{
			SFASSERT(0);
			ReleasePacket(pPacket);
			continue;
		}

		SFIOCPQueue<BasePacket>* pQueue = iter->second;
		pQueue->Push(pPacket);
	}
}

void SFMMODispatcher::MMOLogicProc(void* Args)
{
	SFIOCPQueue<BasePacket>* pQueue = static_cast<SFIOCPQueue<BasePacket>*>(Args);

	while (m_bLogicEnd == false)
	{
		BasePacket* pPacket = pQueue->Pop(INFINITE);

		if (pPacket)
		{
			LogicEntry::GetInstance()->ProcessPacket(pPacket);
			if (pPacket->GetPacketType() != SFPACKET_DB)
			{
				ReleasePacket(pPacket);
			}
		}
	}
}