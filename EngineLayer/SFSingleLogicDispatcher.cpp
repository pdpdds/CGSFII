#include "stdafx.h"
#include "SFSingleLogicDispatcher.h"
#include "SFEngine.h"
#include "SFPacket.h"
#include "SFPacketPool.h"

bool SFSingleLogicDispatcher::m_bLogicEnd = false;


SFSingleLogicDispatcher::SFSingleLogicDispatcher(void)
{

}

SFSingleLogicDispatcher::~SFSingleLogicDispatcher(void)
{

}

void SFSingleLogicDispatcher::Dispatch(BasePacket* pPacket)
{			
	SFLogicGateway::GetInstance()->PushPacket(pPacket);
}

void SFSingleLogicDispatcher::LogicThreadProc(void* Args)
{
	UNREFERENCED_PARAMETER(Args);
	LogicEntry* pEntry = LogicEntry::GetInstance();
	pEntry->Initialize();

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

bool SFSingleLogicDispatcher::CreateLogicSystem(ILogicEntry* pLogicEntry)
{
	m_pLogicThread = new tthread::thread(LogicThreadProc, this);
	LogicEntry::GetInstance()->SetLogic(pLogicEntry);

	return true;
}

bool SFSingleLogicDispatcher::ShutDownLogicSystem()
{
	m_bLogicEnd = true;

	BasePacket* pCommand = SFPacketPool::GetInstance()->Alloc();
	pCommand->SetSerial(-1);
	pCommand->SetPacketType(SFPACKET_SERVERSHUTDOWN);
	SFLogicGateway::GetInstance()->PushPacket(pCommand);

	if (m_pLogicThread->joinable())
	{
		m_pLogicThread->join();
	}

	delete m_pLogicThread;

	LogicEntry::GetInstance()->Destroy();

	return true;
}

bool SFSingleLogicDispatcher::AddRPCService(IRPCService* pService)
{
	m_pRPCService = pService;
	return true;
}