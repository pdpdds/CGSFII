#pragma once

class SFObject;
class SFMessage;

class SFObserver
{
public:
	SFObserver(void);
	virtual ~SFObserver(void);

	virtual bool OnMessage(SFObject* pSender, SFMessage* pMessage);
};
