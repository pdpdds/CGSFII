#pragma once
#include "ACEHeader.h"
#include "INetworkEngine.h"

#ifdef _WIN32
class ACETimeOutHandler : public ACE_Handler	
#else
class ACETimeOutHandler : public ACE_Event_Handler
#endif // _WIN32
{
public:
	ACETimeOutHandler(INetworkEngine* pOwner){m_pOwner = pOwner;}
	virtual ~ACETimeOutHandler(void){}

	 virtual void handle_time_out (const ACE_Time_Value &tv,
                                const void *arg)
    {
		IEngine* pEngine = m_pOwner->GetEngine();
		pEngine->OnTimer(arg);
    }

protected:

private:
	INetworkEngine* m_pOwner;
};

