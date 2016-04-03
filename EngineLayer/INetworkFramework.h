#pragma once
/*#include "ISFAgent.h"

enum eNetworkFrameworkType
{
	NetworkFramework_SERVER = 0,
	NetworkFramework_CLIENT,
	NetworkFramework_MAX,
};

class ILogicEntry;
class IProactorAgent;
class SFPacket;

class INetworkFramework
{
public:
	INetworkFramework(){m_pNetworkAgent = NULL;}
	virtual ~INetworkFramework(void){}

	virtual bool Initialize(eNetworkFrameworkType Type) = 0;
	virtual bool Start(ILogicEntry* pLogic) = 0;
	virtual bool End() = 0;
	virtual bool Finally() = 0;

	virtual bool Send(int Serial, USHORT PacketID, char* pMessage, int BufSize ) = 0;
	virtual bool Send(int Serial, SFPacket* pPacket) = 0;

	ISFAgent* GetNetworkAgent(){return m_pNetworkAgent;}
	void SetNetworkFrameworkType(eNetworkFrameworkType Type){m_Type = Type;}

protected:
	ISFAgent* m_pNetworkAgent;

private:
	eNetworkFrameworkType m_Type;
	
};