#include "SFLogicGateway.h"

SFLogicGateway* SFLogicGateway::m_pLogicGateway = nullptr;

SFLogicGateway::SFLogicGateway(void)
{
}

SFLogicGateway::~SFLogicGateway(void)
{
}

bool SFLogicGateway::PushPacket( BasePacket* pPacket )
{
	return m_queue.Push(pPacket);
}

BasePacket* SFLogicGateway::PopPacket(int WaitTime)
{
	return m_queue.Pop(WaitTime);
}

SFLogicGateway* SFLogicGateway::GetInstance()
{
	if (m_pLogicGateway == nullptr)
		m_pLogicGateway = new SFLogicGateway();

	return m_pLogicGateway;
}