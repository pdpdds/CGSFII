#include "stdafx.h"
#include "ProtoBufLogicEntry.h"
#include "SampleMessage.pb.h"
#include "SFProtobufPacket.h"
#include <iostream>

ProtoBufLogicEntry::ProtoBufLogicEntry()
{
}

ProtoBufLogicEntry::~ProtoBufLogicEntry()
{
}

bool ProtoBufLogicEntry::Initialize()
{	
	return true;
}

bool ProtoBufLogicEntry::ProcessPacket(BasePacket* pPacket)
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
	{
		if (pPacket->GetPacketID() == SAMPLE::HELLO)
		{
			SFProtobufPacket<SAMPLE::Hello>* hello = (SFProtobufPacket<SAMPLE::Hello>*)pPacket;
			std::string message = hello->GetData().message();
			std::cout << message << std::endl;
		}
	}
	break;
	}

	return true;
}