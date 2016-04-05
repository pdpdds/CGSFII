#include "ACEEngine.h"
#include "ACEHeader.h"
#include "ProactorWorkerThread.h"
#include "ProactorAcceptor.h"
#include "SingltonObject.h"
#include <assert.h>
#include "BasePacket.h"

#ifdef  _WIN32
#ifdef _DEBUG
#pragma comment(lib, "aced.lib")
#else
#pragma comment(lib, "ace.lib")
#endif
#else

//#include <libcpuid/libcpuid.h>
#endif

#define PROACTOR_THREAD_GROUP_ID 9783

INetworkEngine * CreateNetworkEngine(IEngine* pEngine)
{
	return new ACEEngine(pEngine);	
}

ACEEngine::ACEEngine(IEngine* pEngine)
	: INetworkEngine(pEngine)
	, m_TimeOutHandler(this)
	, m_acceptorIndex(0)
{
	ACE::init();
	ProactorServiceManagerSinglton::instance();
}

ACEEngine::~ACEEngine(void)
{
	ACE::fini();
}

bool ACEEngine::SendRequest(BasePacket* pPacket)
{
	return ProactorServiceManagerSinglton::instance()->SendRequest(pPacket);
}

bool ACEEngine::Disconnect(int serial)
{
	ProactorServiceManagerSinglton::instance()->Disconnect(serial);
	return true;
}

bool ACEEngine::NetworkOpen()
{
	if (m_mapAcceptor.size() == 0)
		return false;

	for (auto& acceptor : m_mapAcceptor)
	{
		ProactorAcceptor* pAcceptor = acceptor.second;

		ACE_INET_Addr listen_addr;
		listen_addr.set(pAcceptor->GetPort());

		if (0 != pAcceptor->open(listen_addr, 0, 1, ACE_DEFAULT_BACKLOG, 1, 0, 1, 1, 1024))
		{
			delete pAcceptor;
			return false;
		}
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

	return ACE_Proactor::instance()->schedule_timer(m_TimeOutHandler, (void*)timerID, start, interval);
}

bool ACEEngine::CancelTimer(int timerID)
{
	if (timerID < 0)
	{
		ACE_Proactor::instance()->cancel_timer(m_TimeOutHandler);
		return true;
	}

	return ACE_Proactor::instance()->cancel_timer(timerID) == 1;
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
	ProactorAcceptor* pAcceptor = new ProactorAcceptor(this, szIP, port);

	m_acceptorIndex++;
	pAcceptor->SetAcceptorNum(m_acceptorIndex);

	m_mapAcceptor.insert(std::make_pair(m_acceptorIndex, pAcceptor));

	return m_acceptorIndex;
}

bool ACEEngine::Shutdown()
{	
	ACE_Proactor::instance()->cancel_timer(m_TimeOutHandler);

	ACE_Proactor::instance()->end_event_loop();
	
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
	/*struct cpu_raw_data_t raw;
	struct cpu_id_t data;
	if (cpu_identify(&raw, &data) < 0) {
		printf("Sorrry, CPU identification failed.\n");
		printf("Error: %s\n", cpuid_error());
		return false;
	}
	int optimalThreadCount = data.num_cores * 2;*/
	int optimalThreadCount = 4;
#endif

	m_workThreadGroupID = ACE_Thread_Manager::instance()->spawn_n(optimalThreadCount, (ACE_THR_FUNC)ProactorWorkerThread, NULL, THR_NEW_LWP, ACE_DEFAULT_THREAD_PRIORITY);

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