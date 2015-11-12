#include "stdafx.h"
#include "SFPacketPool.h"
	
SFPacketPool* SFPacketPool::m_pPacketPool = nullptr;

SFPacketPool::SFPacketPool()
{
}


SFPacketPool::~SFPacketPool()
{
}

SFPacketPool* SFPacketPool::GetInstance()
{
	if (m_pPacketPool == nullptr)
		m_pPacketPool = new SFPacketPool();

	return m_pPacketPool;
}
