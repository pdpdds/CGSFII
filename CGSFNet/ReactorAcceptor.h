#pragma once
#include "ACEHeader.h"
#include "ReactorService.h"
#include <string>
#include <ace/Acceptor.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/Svc_Handler.h>

class ReactorAcceptor :public ACE_Acceptor<ReactorService, ACE_SOCK_ACCEPTOR>
{
public:
	ReactorAcceptor(INetworkEngine* pOwner, ACE_INET_Addr &addr)
	{
		m_pOwner = pOwner;
		m_addr = addr;			
	}
	virtual ~ReactorAcceptor() {}
	
	int open(ACE_Addr &addr)
	{
		if (ACE_Acceptor<ReactorService, ACE_SOCK_ACCEPTOR>::open(m_addr) == -1)
			ACE_ERROR_RETURN((LM_ERROR, "reactor acceptor open fail"), 1);
		return 0;
	}

	//Overload the handle input method
	virtual int handle_input(ACE_HANDLE handle) override
	{
		//Client has requested connection to server.
		//Create a handler to handle the connection
		ReactorService *eh;
		ACE_NEW_RETURN(eh, ReactorService, -1);
		ACE_INET_Addr cliaddr;

		//Accept the connection "into" the Event Handler
		eh->SetOwner(m_pOwner);
		if (this->acceptor().accept(eh->peer(),//stream
			&cliaddr,//remote address
			0,//timeout
			1) == -1)//restart if interrupted
			ACE_DEBUG((LM_ERROR, "Error in connection \n"));

		_SessionDesc sessionDesc;
		sessionDesc.identifier = m_acceptorNum;
		sessionDesc.sessionType = SESSION_TYPE::SESSION_LISTENER;			
		eh->SetSessionDesc(sessionDesc);
		eh->open(this);

		ACE_DEBUG((LM_DEBUG, "new client accepted %s\n", cliaddr.get_host_addr()));		

		//Register the input event handler for reading 
		ACE_Reactor::instance()->register_handler(eh, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::CONNECT_MASK);
		
		return 0;
	}	

	int handle_close(ACE_HANDLE h, ACE_Reactor_Mask m) {
		close();
		delete this;
		return 0;
	}

	ACE_INET_Addr& GetAddress() { return m_addr; }
	void SetAcceptorNum(int acceptorNum) { m_acceptorNum = acceptorNum; }

private:	 
	ACE_INET_Addr m_addr;
	int m_acceptorNum;
	INetworkEngine* m_pOwner;
};
