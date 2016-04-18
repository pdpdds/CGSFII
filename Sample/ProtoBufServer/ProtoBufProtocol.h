#pragma once
#include "SFProtobufProtocol.h"

class ProtoBufProtocol : public SFProtobufProtocol
{
public:
	ProtoBufProtocol();
	virtual ~ProtoBufProtocol();

	BasePacket* CreateIncomingPacketFromPacketId(int packetId, int channel) override;
};

