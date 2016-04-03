#pragma once
#include "SFPacket.h"
#ifdef _WIN32
#include "../BaseLayer/SFObjectPool.h"
#else
#include "../BaseLayerLinux/SFObjectPool.h"
#endif

class SFPacketPool : public SFObjectPool<SFPacket>
{
public:
	SFPacketPool();
	virtual ~SFPacketPool();
	
	static SFPacketPool* GetInstance();

private:
	static SFPacketPool* m_pPacketPool;

};

