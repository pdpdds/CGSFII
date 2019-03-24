#include "../EngineLayer/SFEngine.h"
#include "EchoLogicEntry.h"

EchoLogicEntry::EchoLogicEntry(void)
	: m_timerCount(0)
{
}

EchoLogicEntry::~EchoLogicEntry(void)
{
}

bool EchoLogicEntry::Initialize()
{
	return true;
}

ILogicEntry* EchoLogicEntry::Clone()
{
	return new EchoLogicEntry();
}

bool EchoLogicEntry::ProcessPacket(BasePacket* pPacket)
{
	switch (pPacket->GetPacketType())
	{	
	case SFPACKET_CONNECT:
		printf("connect : %d\n", pPacket->GetSerial());
		break;
	case SFPACKET_DISCONNECT:
		printf("disconnect : %d\n", pPacket->GetSerial());
		break;
	case SFPACKET_DATA:		
		SFEngine::GetInstance()->SendRequest(pPacket);
		break;
	case SFPACKET_TIMER:
		printf("timer call! %d\n", m_timerCount);
		m_timerCount++;
		break;
	}

	return true;
}