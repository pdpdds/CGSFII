#include "stdafx.h"
#include "SFNETDispatcher.h"
#include "SFEngine.h"
#include "SFPacket.h"
#include "LogicEntry.h"
#include "SFLogicGateway.h"
#include "SFPacketPool.h"


namespace CgsfNET64Lib { 

	bool SFNETDispatcher::m_bLogicEnd = false;
	
	typedef std::map<int, tthread::thread*> mapThread;
	mapThread m_mapThread;


	SFNETDispatcher::SFNETDispatcher(void)
	{
		m_nLogicThreadCnt = 1;
	}

	SFNETDispatcher::~SFNETDispatcher(void)
	{
	}

	void SFNETDispatcher::Init(int threadCount)
	{
		m_nLogicThreadCnt = threadCount;
	}

	void SFNETDispatcher::Dispatch(BasePacket* pPacket)
	{
		SFLogicGateway::GetInstance()->PushPacket(pPacket);
	}

	void SFNETDispatcher::LogicThreadProc(void* Args)
	{
		UNREFERENCED_PARAMETER(Args);
		
		while (m_bLogicEnd == false)
		{
			//로직게이트웨이 큐에서 패킷을 꺼낸다.
			//로직엔트리 객체의 ProcessPacket 메소드를 호출해서 패킷 처리를 수행한다.
			BasePacket* pPacket = SFLogicGateway::GetInstance()->PopPacket();

			LogicEntry::GetInstance()->ProcessPacket(pPacket);

			ReleasePacket(pPacket);
		}
	}

	bool SFNETDispatcher::ReleasePacket(BasePacket* pPacket)
	{
		//사용한 패킷을 수거한다. 패킷의 타입에 따라 릴리즈 형태가 다름
		switch (pPacket->GetPacketType())
		{
		case SFPACKET_DATA:
			SFEngine::GetInstance()->ReleasePacket(pPacket);
			break;
		case SFPACKET_CONNECT:
		case SFPACKET_DISCONNECT:
		case SFPACKET_TIMER:
		case SFPACKET_SHOUTER:
		case SFPACKET_SERVERSHUTDOWN:
			delete pPacket;
			break;

		case SFPACKET_DB:
			SFASSERT(0);
			break;

		default:
			SFASSERT(0);
			return false;
		}

		return true;
	}
	
	bool SFNETDispatcher::CreateLogicSystem(ILogicEntry* pLogicEntry)
	{
		for (int index = 0; index < m_nLogicThreadCnt; index++)
		{
			tthread::thread* pThread = new tthread::thread(LogicThreadProc, this);
			m_mapThread.insert(std::make_pair(index, pThread));
		}


		LogicEntry::GetInstance()->SetLogic(pLogicEntry);

		return true;
	}

	bool SFNETDispatcher::ShutDownLogicSystem()
	{
		m_bLogicEnd = true;

		for (int i = 0; i < m_nLogicThreadCnt; i++)
		{
			BasePacket* pCommand = SFPacketPool::GetInstance()->Alloc();
			pCommand->SetSerial(-1);
			pCommand->SetPacketType(SFPACKET_SERVERSHUTDOWN);
			SFLogicGateway::GetInstance()->PushPacket(pCommand);			
		}

		for (auto thread : m_mapThread)
		{
			tthread::thread* pThread = thread.second;
			if (pThread->joinable())
			{
				pThread->join();
			}

			delete pThread;
		}

		m_mapThread.clear();

		LogicEntry::GetInstance()->DestroyLogic();

		return true;
	}

	

}