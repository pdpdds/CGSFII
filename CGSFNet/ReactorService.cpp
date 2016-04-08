#include "ReactorService.h"
#include "ServiceManager.h"

extern volatile int g_currentSessionCnt;

ReactorService::ReactorService()
	: m_bServiceCloseFlag(false)
{
#if defined(_WIN32)
	InterlockedDecrement((LONG *)&g_currentSessionCnt);
#elif defined(linux)
	def__sync_fetch_and_add(&g_currentSessionCnt, 1);
#endif

	transferredData = 0;
	sendData = 0;
	receiveCount = 0;
	sendCount = 0;
#ifdef _WIN32
	startTime = GetTickCount();
	elapsedTime = GetTickCount();
#endif
}

ReactorService::~ReactorService()
{

#ifdef  _WIN32
#ifdef _DEBUG 
	elapsedTime = GetTickCount() - startTime;
	printf("log result : elapsedTime : %d, transferred : %d, sendData : %d, receiveCount : %d, sendCount : %d\n", elapsedTime, transferredData, sendData, receiveCount, sendCount);
#endif
#endif //  _WIN32

#if defined(_WIN32)
	InterlockedDecrement((LONG *)&g_currentSessionCnt);
#elif defined(linux)
	def__sync_fetch_and_add(&g_currentSessionCnt, 1);
#endif

}

//Called back to handle any input received
int ReactorService::handle_input(ACE_HANDLE)
{
	//receive the data
	char receiveData[2048];
	ssize_t recvBytes = peer().recv(receiveData, 2048);
	if (recvBytes <= 0)
	{
		//ACE_DEBUG((LM_DEBUG, "%s\n", "reactor receive fail!!"));
		return -1;
	}
	receiveData[recvBytes] = 0;

	if (false == ISession::OnReceive(receiveData, recvBytes, m_sessionDesc))
	{		
		ReserveClose();
		return -1;
	}
	
	receiveCount++;	
	return 0;
}

int ReactorService::handle_close(ACE_HANDLE h, ACE_Reactor_Mask m)
{
	delete this;
	return  0;
}

int ReactorService::open(void*)
{
	if (ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>::open() == -1)
		return -1;

	this->peer().get_remote_addr(m_remote_addr);
	

	m_bServiceCloseFlag = false;

	if (g_currentSessionCnt > GetEngine()->GetMaxUserAccept())
	{		
		delete this;
		return -1;
	}

	m_serial = ReactorServiceManager::instance()->Register(this);

	if (m_serial == INVALID_ID)
	{	

		delete this;
		return -1;
	}

	RegisterTimer();

	sockaddr_in addr;
	int addrLen = sizeof(addr);	

	m_sessionDesc.port = m_remote_addr.get_port_number();
	const char* szIP = m_remote_addr.get_host_addr();

	m_sessionDesc.szIP = szIP;

	ISession::OnConnect(this->m_serial, m_sessionDesc);

	return 0;
}

void ReactorService::Disconnect()
{	
	m_bServiceCloseFlag = true;
}

void ReactorService::ReserveClose()
{
	CGSFServiceManager::instance()->UnRegister(m_serial);	

	ISession::OnDisconnect(this->m_serial, m_sessionDesc);

	m_bServiceCloseFlag = true;
}

int ReactorService::handle_timeout(const ACE_Time_Value& tv, const void* arg)
{
	ACE_UNUSED_ARG(tv);
	ACE_UNUSED_ARG(arg);

	if (m_bServiceCloseFlag == true)
	{
		UnregisterTimer();
		delete this;
	}

	return 0;
}

void ReactorService::SendInternal(char* pBuffer, int bufferSize)
{
	ACE_Message_Block* pBlock = NULL;

	ACE_NEW_NORETURN(pBlock, ACE_Message_Block(bufferSize));

	pBlock->copy((const char*)pBuffer, bufferSize);
	sendData += bufferSize;

	this->peer().send(pBlock->rd_ptr(), pBlock->length());
}

bool ReactorService::SendRequest(BasePacket* pPacket)
{
	IPacketProtocol* pProtocol = GetPacketProtocol();
	if (NULL == pProtocol)
		return false;

	char* pBuffer = NULL;
	int bufferSize = 0;
	if (false == pProtocol->Encode(pPacket, &pBuffer, bufferSize))
		return false;

	SendInternal(pBuffer, bufferSize);

	return true;
}

void ReactorService::UnregisterTimer()
{
	ACE_Reactor::instance()->cancel_timer(this);
}

void ReactorService::RegisterTimer()
{
	ACE_Time_Value intervalTime(5, 0);
	ACE_Reactor::instance()->schedule_timer(this, 0, ACE_Time_Value::zero, intervalTime);
}