#include "stdafx.h"
#include "ProtoBufProtocol.h"
#include "SFProtobufPacket.h"
#include "SampleMessage.pb.h"

#define DEFAULT_PACKET_ID 1

ProtoBufProtocol::ProtoBufProtocol()
{
}


ProtoBufProtocol::~ProtoBufProtocol()
{
}

BasePacket* ProtoBufProtocol::CreateIncomingPacketFromPacketId(int packetId)
{
	switch (packetId)
	{
	case SAMPLE::HELLO:
		return new SFProtobufPacket<SAMPLE::Hello>(packetId);

	default:
		SFASSERT(0);
		break;
	}
	return NULL;
}