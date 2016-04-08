#pragma once
#include "ACEHeader.h"
#include <ace/Message_Block.h>
#include <ace/Svc_Handler.h>
#include "EngineStructure.h"
#include "BasePacket.h"

#include "ISession.h"

class ReactorService : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>, public ISession
{
public:
	ReactorService();
	virtual ~ReactorService();

	//Called back to handle any input received
	virtual int handle_input(ACE_HANDLE) override;
	virtual int handle_close(ACE_HANDLE h, ACE_Reactor_Mask m) override;
	virtual int handle_timeout(const ACE_Time_Value &tv, const void *act) override;
	virtual int open(void* = 0) override;
	
	//Used by the reactor to determine the underlying handle
	ACE_HANDLE get_handle()  const
	{
		return this->peer_.get_handle();
	}

	virtual bool SendRequest(BasePacket* pPacket) override;
	void SendInternal(char* pBuffer, int BufferSize);
	void Disconnect();

	void SetSerial(int serial) { m_serial = serial; }
	int GetSerial() { return m_serial; }

	void SetSessionDesc(_SessionDesc& desc) { m_sessionDesc = desc; };

	ACE_SOCK_Stream& peer()
	{
		return this->peer_;
	}

protected:
	void RegisterTimer();
	void UnregisterTimer();
	void ReserveClose();

private:
	ACE_SOCK_Stream peer_;

	int m_serial;
	ACE_INET_Addr m_remote_addr;
	_SessionDesc m_sessionDesc;

	bool m_bServiceCloseFlag;

	int transferredData;
	int sendData;
	int receiveCount;
	int sendCount;
	unsigned int startTime;
	unsigned int elapsedTime;
};

