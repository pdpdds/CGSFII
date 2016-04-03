#include "SFObserver.h"

#pragma warning (disable : 4100) 


SFObserver::SFObserver(void)
{
}

SFObserver::~SFObserver(void)
{
}

bool SFObserver::OnMessage( SFObject* pSender, SFMessage* pMessage )
{
	return true;
}