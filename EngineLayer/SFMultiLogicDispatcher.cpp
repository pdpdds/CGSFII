#include "SFMultiLogicDispatcher.h"
#include "SFEngine.h"
#include "SFPacketPool.h"

#define DEFAULT_CHANNEL 0

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
		//SFIOCPQueue<BasePacket>* pQueue = new SFIOCPQueue<BasePacket>();
		SFLockQueue<BasePacket>* pQueue = new SFLockQueue<BasePacket>();

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
		//SFIOCPQueue<BasePacket>* pQueue = queue.second;		
		SFLockQueue<BasePacket>* pQueue = queue.second;
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
		
		if (pPacket->GetPacketType() == SFPACKET_CONNECT)
		{
			pDispatcher->RegisterClient(pPacket);
		}
		
		if (pPacket->GetPacketType() == SFPACKET_TIMER)
		{
			for (auto& iter : pDispatcher->m_mapQueue)
			{
				//SFIOCPQueue<BasePacket>* pQueue = iter.second;
				SFLockQueue<BasePacket>* pQueue = iter.second;
				pQueue->Push(pPacket);
//일단 한스레드에만 패킷을 넘기고 전체 로직 스레드에게 타이머 패킷을 보낼 수 있도록 나중에 수정한다
				continue;
			}			
		}
		else if (pPacket->GetPacketType() == SFPACKET_SERVERSHUTDOWN)
		{
			for (auto& queue : pDispatcher->m_mapQueue)
			{
				BasePacket* pCommand = SFPacketPool::GetInstance()->Alloc();
				pCommand->SetSerial(-1);
				pCommand->SetPacketType(SFPACKET_SERVERSHUTDOWN);

				//SFIOCPQueue<BasePacket>* pQueue = queue.second;
				SFLockQueue<BasePacket>* pQueue = queue.second;
				pQueue->Push(pCommand);
			}

			break;
		}
		else
		{
			ClientInfo* pInfo = pDispatcher->FindClient(pPacket->GetSerial());

			if (pInfo != nullptr)
			{
				const auto& iter = pDispatcher->m_mapQueue.find(pInfo->channel);

				if (iter != pDispatcher->m_mapQueue.end())
				{
					//SFIOCPQueue<BasePacket>* pQueue = iter->second;
					SFLockQueue<BasePacket>* pQueue = iter->second;
					pQueue->Push(pPacket);
				}
				else
				{
					LOG(WARNING) << "Invalid Channel Num : " << pInfo->channel;
					ReleasePacket(pPacket);
				}
			}
			else
			{
				LOG(WARNING) << "ClientInfo NULL : " << pPacket->GetSerial();
			}
		}

		if (pPacket->GetPacketType() == SFPACKET_DISCONNECT)
		{
			pDispatcher->UnregisterClient(pPacket);
		}
	}
}

void SFMultiLogicDispatcher::MultiLogicProc(void* Args)
{
	//SFIOCPQueue<BasePacket>* pQueue = static_cast<SFIOCPQueue<BasePacket>*>(Args);
	SFLockQueue<BasePacket>* pQueue = static_cast<SFLockQueue<BasePacket>*>(Args);
	
	LogicEntry::GetInstance()->Initialize();

	while (true)
	{
		BasePacket* pPacket = pQueue->Pop(-1);

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

void SFMultiLogicDispatcher::RegisterClient(BasePacket* pPacket)
{
	ClientInfo info;
	info.channel = DEFAULT_CHANNEL;
	info.serial = pPacket->GetSerial();
	
	m_mapClient.insert(std::make_pair(info.serial, info));
}

void SFMultiLogicDispatcher::UnregisterClient(BasePacket* pPacket)
{
	m_mapClient.erase(pPacket->GetSerial());
}

ClientInfo* SFMultiLogicDispatcher::FindClient(int serial)
{
	auto iter = m_mapClient.find(serial);

	if (iter == m_mapClient.end())
		return nullptr;

	return &iter->second;	
}

bool SFMultiLogicDispatcher::ChangeChannel(int serial, int channel)
{
	auto iter = m_mapClient.find(serial);
	
	if (iter == m_mapClient.end())
		return false;

	ClientInfo& info = iter->second;
	info.channel = channel;

	return true;
}