#pragma once
#include "SFProtobufPacketImpl.h"

template<class T>
class SFProtobufPacket : public SFProtobufPacketImpl
{
public:

	SFProtobufPacket(int serviceId, int channel = 0)
		: SFProtobufPacketImpl(serviceId, channel, &m_Packet)
		, m_channel(channel)
	{		
	}
		
	virtual ~SFProtobufPacket(void){}

	T& GetData()
	{
		return m_Packet;
	}
	
	virtual BasePacket* Clone()
	{
		SFProtobufPacket* pClone = new SFProtobufPacket<T>(getServiceId());
		pClone->CopyBaseHeader(this);
		pClone->m_Packet = m_Packet;

		return pClone;
	}

	int GetChannel() { return m_channel; }


protected:

private:
	T m_Packet;
	int m_channel;
};

