#include "ACEEngine.h"
#include "ACEHeader.h"
#include "WorkerThread.h"
#include "ProactorAcceptor.h"
#include "ReactorAcceptor.h"
#include <assert.h>
#include "BasePacket.h"
#include "ServiceManager.h"

#ifdef  _WIN32
#ifdef _DEBUG
#pragma comment(lib, "aced.lib")
#else
#pragma comment(lib, "ace.lib")
#endif
#else
#include <unistd.h>
#endif

#define PROACTOR_THREAD_GROUP_ID 9783

INetworkEngine * CreateNetworkEngine(IEngine* pEngine)
{
	return new ACEEngine(pEngine);	
}

ACEEngine::ACEEngine(IEngine* pEngine)
	: INetworkEngine(pEngine)
	, m_timeOutHandler(this)
	, m_acceptorIndex(0)
{
	ACE::init();
	CGSFServiceManager::instance()->Init(5000, 1);
}

ACEEngine::~ACEEngine(void)
{
	ACE::fini();
}

bool ACEEngine::SendRequest(BasePacket* pPacket)
{
	return CGSFServiceManager::instance()->SendRequest(pPacket);
}

bool ACEEngine::Disconnect(int serial)
{
	CGSFServiceManager::instance()->Disconnect(serial);
	return true;
}

bool ACEEngine::NetworkOpen()
{
	if (m_mapAcceptor.size() == 0)
		return false;

	for (auto& acceptor : m_mapAcceptor)
	{
#ifdef ACE_PROACTOR
		ProactorAcceptor* pAcceptor = acceptor.second;
		ACE_INET_Addr listen_addr;
		listen_addr.set(pAcceptor->GetPort());

		if (0 != pAcceptor->open(listen_addr, 0, 1, ACE_DEFAULT_BACKLOG, 1, 0, 1, 1, 1024))
		{
			delete pAcceptor;
			return false;
		}
#else
		ReactorAcceptor* pAcceptor = acceptor.second;
		ACE_Addr addr(pAcceptor->GetAddress().get_port_number());		

		if (0 != pAcceptor->open(addr))
		{
			delete pAcceptor;
			return false;
		}
#endif		
	}

	return true;
}

bool ACEEngine::CheckTimerImpl()
{
	return true;
}

long ACEEngine::AddTimer(unsigned int timerID, unsigned int startTime, unsigned int period)
{
	ACE_Time_Value interval(period / 1000, (period % 1000) * 1000);
	ACE_Time_Value start(startTime / 1000, (startTime % 1000) * 1000);

#ifdef ACE_PROACTOR
	return ACE_Proactor::instance()->schedule_timer(m_timeOutHandler, (void*)timerID, start, interval);
#else
	return ACE_Reactor::instance()->schedule_timer(&m_timeOutHandler, (void*)timerID, start, interval);
#endif
}

bool ACEEngine::CancelTimer(int timerID)
{
	if (timerID < 0)
	{
#ifdef ACE_PROACTOR
		ACE_Proactor::instance()->cancel_timer(m_timeOutHandler);
#else
		return ACE_Reactor::instance()->cancel_timer(&m_timeOutHandler);
#endif
		return true;
	}

#ifdef ACE_PROACTOR
	return ACE_Proactor::instance()->cancel_timer(timerID) == 1;
#else
	return ACE_Reactor::instance()->cancel_timer(timerID) == 1;
#endif
}

int ACEEngine::AddConnector(int connectorIndex, char* szIP, unsigned short port)
{
	ACE_SOCK_Stream* stream = new ACE_SOCK_Stream();
	ACE_INET_Addr connectAddr(port, szIP);
	ACE_SOCK_Connector connector;
	int result = connector.connect(*stream, connectAddr);
	if (-1 == result)
		return -1;

	_SessionDesc sessionDesc;
	sessionDesc.identifier = connectorIndex;
	sessionDesc.sessionType = SESSION_TYPE::SESSION_CONNECTOR;

	ProactorService* pService = new ProactorService();
	pService->SetOwner(this);
	pService->SetSessionDesc(sessionDesc);

	ACE_Message_Block mb;
	pService->open(stream->get_handle(), mb);
	delete stream;
	stream = NULL;

	return pService->GetSerial();
}

int ACEEngine::AddListener(char* szIP, unsigned short port)
{
#ifdef ACE_PROACTOR
	ProactorAcceptor* pAcceptor = new ProactorAcceptor(this, szIP, port);

	m_acceptorIndex++;
	pAcceptor->SetAcceptorNum(m_acceptorIndex);

	m_mapAcceptor.insert(std::make_pair(m_acceptorIndex, pAcceptor));
#else
	ACE_INET_Addr addr(port);
	ReactorAcceptor* pAcceptor = new ReactorAcceptor(this, addr);

	m_acceptorIndex++;
	pAcceptor->SetAcceptorNum(m_acceptorIndex);

	m_mapAcceptor.insert(std::make_pair(m_acceptorIndex, pAcceptor));
#endif

	return m_acceptorIndex;
}

bool ACEEngine::Shutdown()
{	
#ifdef ACE_PROACTOR
	ACE_Proactor::instance()->cancel_timer(m_timeOutHandler);
	ACE_Proactor::instance()->end_event_loop();	
#else
	ACE_Reactor::instance()->cancel_timer(&m_timeOutHandler);
	ACE_Reactor::instance()->end_event_loop();
#endif
	
	ACE_Thread_Manager::instance()->wait_grp(m_workThreadGroupID);		
	
	return true;
}

bool ACEEngine::Init()
{	
#ifdef _WIN32
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	int optimalThreadCount = si.dwNumberOfProcessors * 2;
#else
	int optimalThreadCount = sysconf(_SC_NPROCESSORS_ONLN);
#endif

#ifdef ACE_PROACTOR
	m_workThreadGroupID = ACE_Thread_Manager::instance()->spawn_n(optimalThreadCount, (ACE_THR_FUNC)ProactorWorkerThread, NULL, THR_NEW_LWP, ACE_DEFAULT_THREAD_PRIORITY);
#else
	m_workThreadGroupID = ACE_Thread_Manager::instance()->spawn_n(optimalThreadCount, (ACE_THR_FUNC)ReactorWorkerThread, NULL, THR_NEW_LWP, ACE_DEFAULT_THREAD_PRIORITY);
#endif

	if (m_workThreadGroupID == -1)
	{
		assert(0);
		return false;
	}

	return true;
}

bool ACEEngine::Start()
{	
	return NetworkOpen();
}