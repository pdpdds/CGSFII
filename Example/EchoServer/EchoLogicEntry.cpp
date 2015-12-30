#include "stdafx.h"
#include "EchoLogicEntry.h"

EchoLogicEntry::EchoLogicEntry(void)
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
	}

	return true;
}