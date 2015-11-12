#include "StdAfx.h"
#include "SFLogicGateway.h"

SFLogicGateway* SFLogicGateway::m_pLogicGateway = NULL;

SFLogicGateway::SFLogicGateway(void)
{
}

SFLogicGateway::~SFLogicGateway(void)
{
}

bool SFLogicGateway::PushPacket( BasePacket* pPacket )
{
	return m_IOCPQueue.Push(pPacket);
}

BasePacket* SFLogicGateway::PopPacket(int WaitTime)
{
	return m_IOCPQueue.Pop(WaitTime);
}

SFLogicGateway* SFLogicGateway::GetInstance()
{
	if (m_pLogicGateway == NULL)
		m_pLogicGateway = new SFLogicGateway();

	return m_pLogicGateway;
}