#pragma once
#include "SFDispatch.h"
#include "../CGSFNet/BasePacket.h"
#include <functional>

class SFDBAdaptor
{
public:
	SFDBAdaptor(void);
	virtual ~SFDBAdaptor(void);

	virtual bool Initialize(_DBConnectionInfo* pInfo) = 0;
	virtual bool RegisterDBService() = 0;
	virtual bool RecallDBMsg(BasePacket* pMessage) = 0;

	bool Call(BasePacket* pMessage)
	{
		bool bResult = m_Dispatch.HandleMessage(pMessage->GetPacketID(), pMessage);

		RecallDBMsg(pMessage);
		
		return bResult;
	}

protected:
	SFDispatch<USHORT, std::tr1::function<BOOL(BasePacket*)>, BasePacket*> m_Dispatch;

private:
};

