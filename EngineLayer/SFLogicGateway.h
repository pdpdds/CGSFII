#pragma once


#ifdef _WIN32
#include "../CGSFNet/BasePacket.h"
#include "../BaseLayer/SFIOCPQueue.h"
#else
#include "../CGSFNetLinux/BasePacket.h"
#include "../BaseLayerLinux/SFLockQueue.h"
#endif // _WIN32

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

