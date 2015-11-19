#include "stdafx.h"
#include "TankLogicEntry.h"


TankLogicEntry::TankLogicEntry()
{
}


TankLogicEntry::~TankLogicEntry()
{
}

bool TankLogicEntry::Initialize()
{	

	return true;
}

bool TankLogicEntry::ProcessPacket(BasePacket* pPacket)
{
	switch (pPacket->GetPacketType())
	{
	case SFPACKET_CONNECT:
		printf("connect user %d!!\n", pPacket->GetSerial());		
		break;

	case SFPACKET_DISCONNECT:
		printf("disconnect user %d!!\n", pPacket->GetSerial());		
		break;

	case SFPACKET_DATA:
		break;

	}

	return true;
}
