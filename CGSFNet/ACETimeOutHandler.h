#pragma once
#include "ACEHeader.h"
#include "INetworkEngine.h"

#ifdef ACE_PROACTOR
class ACETimeOutHandler : public ACE_Handler	
#else
class ACETimeOutHandler : public ACE_Event_Handler
#endif // _WIN32
{
public:
	ACETimeOutHandler(INetworkEngine* pOwner){m_pOwner = pOwner;}
	virtual ~ACETimeOutHandler(void){}

#ifdef ACE_PROACTOR
	 virtual void handle_time_out (const ACE_Time_Value &tv,
                                const void *arg)
#else
	virtual int handle_timeout(const ACE_Time_Value &tv, const void *arg)
#endif
    {
		IEngine* pEngine = m_pOwner->GetEngine();
		pEngine->OnTimer(arg);

#ifndef ACE_PROACTOR
		return 0;
#endif
    }

protected:

private:
	INetworkEngine* m_pOwner;
};

