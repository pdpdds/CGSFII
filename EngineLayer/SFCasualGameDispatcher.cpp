#include "stdafx.h"
#include "SFCasualGameDispatcher.h"
#include "SFEngine.h"
#include "SFPacket.h"
#include "SFPacketPool.h"

bool SFCasualGameDispatcher::m_bLogicEnd = false;


SFCasualGameDispatcher::SFCasualGameDispatcher(void)
{
//캐쥬얼 게임 프레임 워크의 로직 쓰레드 수는 하나임
	m_nLogicThreadCnt = 1;	
}

SFCasualGameDispatcher::~SFCasualGameDispatcher(void)
{
}

void SFCasualGameDispatcher::Dispatch(BasePacket* pPacket)
{			
	SFLogicGateway::GetInstance()->PushPacket(pPacket);
}

void SFCasualGameDispatcher::LogicThreadProc(void* Args)
{
	UNREFERENCED_PARAMETER(Args);
	LogicEntry* pEntry = LogicEntry::GetInstance();

	while (m_bLogicEnd == false)
	{
//로직게이트웨이 큐에서 패킷을 꺼낸다.
//로직엔트리 객체의 ProcessPacket 메소드를 호출해서 패킷 처리를 수행한다.
		BasePacket* pPacket = SFLogicGateway::GetInstance()->PopPacket();
		pEntry->ProcessPacket(pPacket);

//20150113
//DB 처리를 로직엔트리 객체에 위임해서 엔진레이어가 데이터베이스 레이어에 종속되지 않도록 한다.
//DB 패킷의 수거는 로직레이어에서 처리하도록 한다.
//차후 구조를 수정할 것이다.
		if (pPacket->GetPacketType() != SFPACKET_DB)
		{
			ReleasePacket(pPacket);			
		}
	}
}

bool SFCasualGameDispatcher::CreateLogicSystem(ILogicEntry* pLogicEntry)
{		
	for (int index = 0; index < m_nLogicThreadCnt; index++)
	{
		tthread::thread* pThread = new tthread::thread(LogicThreadProc, this);
		m_mapThread.insert(std::make_pair(index, pThread));
	}

	LogicEntry::GetInstance()->SetLogic(pLogicEntry);

	return true;
}

bool SFCasualGameDispatcher::ShutDownLogicSystem()
{
	m_bLogicEnd = true;
	
	for (int i = 0; i < m_nLogicThreadCnt; i++)
	{
		BasePacket* pCommand = SFPacketPool::GetInstance()->Alloc();
		pCommand->SetSerial(-1);
		pCommand->SetPacketType(SFPACKET_SERVERSHUTDOWN);
		SFLogicGateway::GetInstance()->PushPacket(pCommand);
	}

	LogicEntry::GetInstance()->DestroyLogic();

	return true;
}

bool SFCasualGameDispatcher::AddRPCService(IRPCService* pService)
{
	m_pRPCService = pService;
	return true;
}

