#pragma once
#ifdef _WIN32
#include "../CGSFNet/ISessionService.h"
#include "../CGSFNet/IPacketProtocol.h"
#else
#include "../CGSFNetLinux/ISessionService.h"
#include "../CGSFNetLinux/IPacketProtocol.h"
#endif

class SFSessionService : public ISessionService
{
public:
	SFSessionService(IPacketProtocol* pProtocol) : ISessionService(pProtocol){}
	virtual ~SFSessionService(void){}

	virtual bool OnReceive(char* pData, unsigned short Length, _SessionDesc& desc) override;
	//virtual bool SendRequest(ISession *pSession, BasePacket* pPacket) override;

protected:

private:
	
};