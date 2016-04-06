#include "ProactorService.h"
#include "SingltonObject.h"
#include <assert.h>

volatile int g_currentSessionCnt = 0;

ProactorService::ProactorService()
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

ProactorService::~ProactorService( void )
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
	def__sync_fetch_and_add( &g_currentSessionCnt, 1 );
#endif
	
}

void ProactorService::open( ACE_HANDLE h, ACE_Message_Block& MessageBlock )
{
	this->handle(h);	

	if (this->m_AsyncReader.open(*this) != 0 || this->m_AsyncWriter.open(*this) != 0)
	{
		delete this;
		return;
	}	

	m_bServiceCloseFlag = false;

	if (g_currentSessionCnt > GetEngine()->GetMaxUserAccept())
	{
		if (this->handle() != ACE_INVALID_HANDLE)
			ACE_OS::closesocket(this->handle());

		this->handle(ACE_INVALID_HANDLE);

		delete this;
		return;
	}

	m_serial = ProactorServiceManagerSinglton::instance()->Register(this);	

	if (m_serial == INVALID_ID)
	{
		if (this->handle() != ACE_INVALID_HANDLE)
			ACE_OS::closesocket(this->handle());

		this->handle(ACE_INVALID_HANDLE);
		
		delete this;
		return;
	}

	RegisterTimer();

	sockaddr_in addr;
	int addrLen = sizeof(addr);

	ACE_OS::getpeername(this->handle(), (sockaddr *)&addr, &addrLen);	

	m_sessionDesc.port = ntohs(addr.sin_port);
	char* szIP = inet_ntoa(addr.sin_addr);

	if(szIP != NULL)
		m_sessionDesc.szIP = inet_ntoa(addr.sin_addr);

	ISession::OnConnect(this->m_serial, m_sessionDesc);

	PostRecv();
}

void ProactorService::PostRecv()
{
	ACE_Message_Block* pBlock;

	ACE_NEW_NORETURN(pBlock, ACE_Message_Block (2048));
	if(this->m_AsyncReader.read(*pBlock, pBlock->space()) != 0)
	{
		pBlock->release();
		ReserveClose();
	}
}

void ProactorService::handle_read_stream( const ACE_Asynch_Read_Stream::Result& result )
{
	transferredData += (int)result.bytes_transferred();
	ACE_Message_Block& block = result.message_block();
	if (!result.success() || result.bytes_transferred() == 0)
	{
		block.release();
		ReserveClose();
	}
	else
	{
		if (false == ISession::OnReceive(block.rd_ptr(), (unsigned short)block.length(), m_sessionDesc))
		{
			block.release();
			ReserveClose();
			return;
		}
//20150322 memory leak fix		
		block.release();
		receiveCount++;

		PostRecv();
	}
}

void ProactorService::handle_write_stream( const ACE_Asynch_Write_Stream::Result& result )
{
	sendCount++;
	result.message_block().release();
}

void ProactorService::RegisterTimer()
{
	ACE_Time_Value intervalTime(5,0);
	ACE_Proactor::instance()->schedule_timer(*this, 0, ACE_Time_Value::zero, intervalTime);
}

void ProactorService::UnregisterTimer()
{
	ACE_Proactor::instance()->cancel_timer(*this);
}

void ProactorService::Disconnect()
{
	if (this->handle() != ACE_INVALID_HANDLE)
		ACE_OS::closesocket(this->handle());

	this->handle(ACE_INVALID_HANDLE);

	m_bServiceCloseFlag = true;
}

void ProactorService::ReserveClose()
{	
	ProactorServiceManagerSinglton::instance()->UnRegister(m_serial);

	if(this->handle() != ACE_INVALID_HANDLE)
		ACE_OS::closesocket(this->handle());
	
	this->handle(ACE_INVALID_HANDLE);	

	ISession::OnDisconnect(this->m_serial, m_sessionDesc);

	m_bServiceCloseFlag = true;
}

void ProactorService::handle_time_out(const ACE_Time_Value& tv, const void* arg)
{
	ACE_UNUSED_ARG(tv);
	ACE_UNUSED_ARG(arg);

	if(m_bServiceCloseFlag == true)
	{			
		UnregisterTimer();
		delete this;	
	}
}

void ProactorService::SendInternal(char* pBuffer, int bufferSize)
{			
	ACE_Message_Block* pBlock = NULL;

	ACE_NEW_NORETURN(pBlock, ACE_Message_Block(bufferSize));

	pBlock->copy((const char*)pBuffer, bufferSize);
	sendData += bufferSize;

	//if(NULL == pBlock->cont())
	//{
		m_AsyncWriter.write(*pBlock, pBlock->length());
	//}
	//else
	//{
		//m_AsyncWriter.writev(*pBlock, pBlock->total_length());
	//}
}

bool ProactorService::SendRequest(BasePacket* pPacket)
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