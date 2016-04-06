#pragma once
#include "../CGSFNet/BasePacket.h"
#ifdef _WIN32
#include "../BaseLayer/SFIOCPQueue.h"
#endif
#include "../BaseLayer/SFLockQueue.h"

class SFLogicGateway
{
public:
	SFLogicGateway(void);
	virtual ~SFLogicGateway(void);

	static SFLogicGateway* GetInstance();

	bool PushPacket(BasePacket* pPacket);
	BasePacket* PopPacket(int WaitTime = -1);

private:
#ifdef _WIN32
	SFIOCPQueue<BasePacket> m_queue;
#else
	SFLockQueue<BasePacket> m_queue;
#endif
	static SFLogicGateway* m_pLogicGateway;
	
};

