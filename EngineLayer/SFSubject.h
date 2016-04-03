#pragma once
#include <list>
#include "SFObject.h"

class SFObserver;
class SFMessage;

class SFSubject : public SFObject
{
	typedef std::list<SFObserver*> Observerlist;
public:
	SFSubject(void);
	virtual ~SFSubject(void);

	bool AddObserver(SFObserver* pObject);
	bool DelObserver(SFObserver* pObject);

	bool PropagateMessage(SFMessage* pMessage);

	int GetObserverCount(){ return (int)m_Observerlist.size();}

protected:
	bool CheckDuplicate(SFObserver* pObject);

private:
	Observerlist m_Observerlist;
};
