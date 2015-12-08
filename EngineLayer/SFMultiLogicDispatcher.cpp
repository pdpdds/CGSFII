#include "stdafx.h"
#include "SFMultiLogicDispatcher.h"
#include "SFEngine.h"
#include "SFPacketPool.h"

SFMultiLogicDispatcher::SFMultiLogicDispatcher(int channelCount)
: m_channelCount(channelCount)
{

}

SFMultiLogicDispatcher::~SFMultiLogicDispatcher()
{
}

void SFMultiLogicDispatcher::Dispatch(BasePacket* pPacket)
{		
	SFLogicGateway::GetInstance()->PushPacket(pPacket);	
}

bool SFMultiLogicDispatcher::CreateLogicSystem(ILogicEntry* pLogicEntry)
{	
	LogicEntry::GetInstance()->SetLogic(pLogicEntry);

	m_packetDistrubutor = new tthread::thread(PacketDistributorProc, this);		

	for (int index = 0; index < m_channelCount; index++)
	{
		SFIOCPQueue<BasePacket>* pQueue = new SFIOCPQueue<BasePacket>();

		tthread::thread* pThread = new tthread::thread(MultiLogicProc, pQueue);
		m_mapThread.insert(std::make_pair(index, pThread));
		m_mapQueue.insert(std::make_pair(index, pQueue));
	}
	
	return true;
}

bool SFMultiLogicDispatcher::ShutDownLogicSystem()
{
	BasePacket* pCommand = SFPacketPool::GetInstance()->Alloc();
	pCommand->SetSerial(-1);
	pCommand->SetPacketType(SFPACKET_SERVERSHUTDOWN);
	SFLogicGateway::GetInstance()->PushPacket(pCommand);	

	if (m_packetDistrubutor->joinable())
		m_packetDistrubutor->join();

	for (auto& thread : m_mapThread)
	{
		tthread::thread* pThread = thread.second;
		if (pThread->joinable())
		{
			pThread->join();
			delete pThread;
		}
	}

	m_mapThread.clear();

	for (auto& queue : m_mapQueue)
	{
		SFIOCPQueue<BasePacket>* pQueue = queue.second;		
		delete pQueue;		
	}

	m_mapQueue.clear();

	LogicEntry::GetInstance()->Destroy();

	return true;
}

void SFMultiLogicDispatcher::PacketDistributorProc(void* Args)
{
	SFMultiLogicDispatcher* pDispatcher = static_cast<SFMultiLogicDispatcher*>(Args);

	while (true)
	{
		BasePacket* pPacket = SFLogicGateway::GetInstance()->PopPacket();
		
		/*if (pPacket->GetPacketType() == SFPACKET_CONNECT)
		{

		}
		else if (pPacket->GetPacketType() == SFPACKET_DISCONNECT)
		{			
			
		}
		else */if (pPacket->GetPacketType() == SFPACKET_TIMER)
		{
			for (auto& iter : pDispatcher->m_mapQueue)
			{
				SFIOCPQueue<BasePacket>* pQueue = iter.second;
				pQueue->Push(pPacket);
			}			
		}
		else if (pPacket->GetPacketType() == SFPACKET_SERVERSHUTDOWN)
		{
			for (auto& queue : pDispatcher->m_mapQueue)
			{
				BasePacket* pCommand = SFPacketPool::GetInstance()->Alloc();
				pCommand->SetSerial(-1);
				pCommand->SetPacketType(SFPACKET_SERVERSHUTDOWN);

				SFIOCPQueue<BasePacket>* pQueue = queue.second;
				pQueue->Push(pCommand);
			}

			break;
		}
		else
		{
			const auto& iter = pDispatcher->m_mapQueue.find(0);//pPacket->GetChannelNum());

			if (iter != pDispatcher->m_mapQueue.end())
			{
				SFIOCPQueue<BasePacket>* pQueue = iter->second;
				pQueue->Push(pPacket);
			}
			else
			{
				LOG(WARNING) << "Invalid Channel Num : " << 0;//pPacket->GetChannelNum();
				ReleasePacket(pPacket);
			}
		}						
	}
}

void SFMultiLogicDispatcher::MultiLogicProc(void* Args)
{
	SFIOCPQueue<BasePacket>* pQueue = static_cast<SFIOCPQueue<BasePacket>*>(Args);
	
	LogicEntry::GetInstance()->Initialize();

	while (true)
	{
		BasePacket* pPacket = pQueue->Pop(INFINITE);

		if (pPacket->GetPacketType() == SFPACKET_SERVERSHUTDOWN)
			break;
		else
		{
			LogicEntry::GetInstance()->ProcessPacket(pPacket);
			if (pPacket->GetPacketType() != SFPACKET_DB)
			{
				ReleasePacket(pPacket);
			}
		}
	}	
}