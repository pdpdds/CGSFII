#pragma once
#include "SFProtobufProtocol.h"

class TankProtocol : public SFProtobufProtocol
{
public:
	TankProtocol();
	virtual ~TankProtocol();

	BasePacket* CreateIncomingPacketFromPacketId(int packetId) override;
};

