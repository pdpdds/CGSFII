#include "stdafx.h"
#include "SFPacketSendGateway.h"

SFPacketSendGateway* SFPacketSendGateway::m_pPacketSendGateway = NULL;

SFPacketSendGateway::SFPacketSendGateway(void)
{
}

SFPacketSendGateway* SFPacketSendGateway::GetInstance()
{
	if (m_pPacketSendGateway == NULL)
		m_pPacketSendGateway = new SFPacketSendGateway();

	return m_pPacketSendGateway;
}


SFPacketSendGateway::~SFPacketSendGateway(void)
{
}

bool SFPacketSendGateway::PushTask(IPacketTask* pPacketTask)
{
	return m_IOCPQueue.Push(pPacketTask);
}

IPacketTask* SFPacketSendGateway::PopTask(int WaitTime)
{
	return m_IOCPQueue.Pop(WaitTime);
}
