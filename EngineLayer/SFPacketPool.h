#pragma once
#include "SFPacket.h"
#include "../BaseLayer/SFObjectPool.h"

class SFPacketPool : public SFObjectPool<SFPacket>
{
public:
	SFPacketPool();
	virtual ~SFPacketPool();
	
	static SFPacketPool* GetInstance();

private:
	static SFPacketPool* m_pPacketPool;

};

