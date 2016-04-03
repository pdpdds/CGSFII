#include "SFSubject.h"
#include "SFObserver.h"

SFSubject::SFSubject(void)
{
}

SFSubject::~SFSubject(void)
{
}

bool SFSubject::AddObserver( SFObserver* pObject )
{
	if(true == CheckDuplicate(pObject))
		return false;

	m_Observerlist.push_back(pObject);

	return true;
}

bool SFSubject::CheckDuplicate( SFObserver* pObject )
{
	bool bDuplicate = false;
	Observerlist::iterator iter = m_Observerlist.begin();

	for(; iter != m_Observerlist.end(); iter++)
	{
		if(*iter == pObject)
		{
			bDuplicate = true;
			break;
		}
	}

	return bDuplicate;
}

bool SFSubject::DelObserver( SFObserver* pObject )
{
	Observerlist::iterator iter = m_Observerlist.begin();

	for(; iter != m_Observerlist.end(); iter++)
	{
		if(*iter == pObject)
		{
			m_Observerlist.erase(iter);
			break;
		}
	}

	return true;
}

bool SFSubject::PropagateMessage( SFMessage* pMessage )
{
	Observerlist::iterator iter = m_Observerlist.begin();

	for(; iter != m_Observerlist.end(); iter++)
	{
		SFObserver* pObserver = *iter;
		pObserver->OnMessage(this, pMessage);
	}

	return true;
}