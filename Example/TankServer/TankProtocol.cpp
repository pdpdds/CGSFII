#include "stdafx.h"
#include "TankProtocol.h"
#include "SFProtobufPacket.h"
#include "Tank.pb.h"

#define DEFAULT_PACKET_ID 1

TankProtocol::TankProtocol()
{
}


TankProtocol::~TankProtocol()
{
}

BasePacket* TankProtocol::CreateIncomingPacketFromPacketId(int packetId)
{
	switch (packetId)
	{
	case DEFAULT_PACKET_ID:
		return new SFProtobufPacket<Tank::MessageHeader>(packetId);

	default:
		SFASSERT(0);
		break;
	}
	return NULL;
}