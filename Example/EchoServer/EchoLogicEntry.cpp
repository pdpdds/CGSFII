#ifdef _WIN32
#include "SFEngine.h"
#else
#include "../EngineLayerLinux/SFEngine.h"
#endif // _WIN32

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
	case SFPACKET_TIMER:
		printf("timer call!\n");
		break;
	}

	return true;
}