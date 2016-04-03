#pragma once
#ifdef _WIN32
#include "../BaseLayer/SFIOCPQueue.h"
#else
#include "../BaseLayerLinux/SFTSSyncQueue.h"
#endif // _WIN32


class BasePacket;

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
	SFTSSyncQueue<BasePacket> m_queue;
#endif
	static SFLogicGateway* m_pLogicGateway;
	
};

