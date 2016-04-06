#include "SFEngine.h"
#include "SFServerConnectionManager.h"
#include "SFPacketProtocolManager.h"

#include "Markup.h"
#include <process.h>

#include "../BaseLayer/Markup.h"
 
SFServerConnectionManager::SFServerConnectionManager()
: m_dwThreadID(0)
, m_bThreadEnd(false)
{
#ifdef _WIN32
	m_hThread = 0;
	m_hTimerEvent = nullptr;
#endif
}

#ifdef _WIN32
static unsigned WINAPI ServerReconnectProc(void* arg);
#else
static unsigned ServerReconnectProc(void* arg);
#endif

SFServerConnectionManager::~SFServerConnectionManager()
{
	if (m_hThread != 0)
	{
		m_bThreadEnd = true;
#ifdef _WIN32			
		WaitForSingleObject((HANDLE)m_hThread, INFINITE);
		CloseHandle((HANDLE)m_hThread);	
#else
		if (pthread_mutex_destroy(&m_lock) != 0)
		{
			printf("fail to pthread_mutex_destroy, %s\n", strerror(errno));		
		}

		if (pthread_cond_destroy(&m_event) != 0)
		{
			printf("fail to pthread_cond_destroy, %s\n", strerror(errno));			
		}		
#endif
	}
}


/*
bool SFServerConnectionManager::Save()
{
	CMarkup xml;

	xml.AddElem(L"SERVERLIST");
	xml.AddChildElem(L"SERVER");
	xml.IntoElem();
	xml.AddChildElem(L"IP", L"127.0.0.1");
	xml.AddChildElem(L"PORT", 10000);
	xml.AddChildElem(L"IDENTIFER", 10000);
	xml.AddChildElem(L"DESC", L"GAME DATABASE SERVER");
	xml.OutOfElem();

	xml.AddChildElem(L"SERVER");
	xml.IntoElem();
	xml.AddChildElem(L"IP", L"127.0.0.1");
	xml.AddChildElem(L"PORT", 10001);
	xml.AddChildElem(L"IDENTIFER", 9000);
	xml.AddChildElem(L"DESC", L"AUTH SERVER");
	xml.OutOfElem();

	std::wstring csXML = xml.GetDoc();
	xml.Save(L"ServerConnection.xml");

	wprintf(L"%s", csXML.c_str());

	return true;
}*/

bool SFServerConnectionManager::LoadConnectorList(char* szFileName)
{
	CMarkup xml;
	bool result = xml.Load(szFileName);

	if (result == false)
		return false;

	while (xml.FindChildElem("SERVER"))
	{
		_ConnectorInfo connectorInfo;
		xml.IntoElem();

		xml.FindChildElem("IP");
		connectorInfo.szIP = xml.GetChildData();

		xml.FindChildElem("PORT");
		connectorInfo.port = atoi(xml.GetChildData().c_str());

		xml.FindChildElem("IDENTIFER");
		connectorInfo.connectorId = atoi(xml.GetChildData().c_str());

		xml.FindChildElem("DESC");
		connectorInfo.szDesc = xml.GetChildData();

		xml.OutOfElem();
		
		AddConnectorInfo(connectorInfo);
	}

	return true;
}

void SFServerConnectionManager::AddConnectorInfo(_ConnectorInfo& connectorInfo)
{
	m_listConnectorInfo.push_back(connectorInfo);
	SFEngine::GetInstance()->GetPacketProtocolManager()->AddConnectorInfo(&connectorInfo);

	LOG(INFO) << "AddConnectorInfo. " << "connectID: " << connectorInfo.connectorId << ", ProtocolID: " << (int)connectorInfo.packetProtocolId << ", IP: " << connectorInfo.szIP.data() << ", Port: " << connectorInfo.port;
	google::FlushLogFiles(google::GLOG_INFO);
}

bool SFServerConnectionManager::SetupServerReconnectSys()
{
#ifdef _WIN32
	m_hTimerEvent = CreateEvent(NULL, FALSE, FALSE, L"ServerReconnectEvent");
#else	
		
	if (pthread_mutex_init(&m_lock, NULL) != 0)
	{
		printf("fail to pthread_mutex_init, %s\n", strerror(errno));		
	}

	if (pthread_cond_init(&m_event, NULL) != 0)
	{
		printf("fail to pthread_cond_init, %s\n", strerror(errno));		
	}
#endif

	LOG(INFO) << "SetupServerReconnectSys. " << "First Connect";
	google::FlushLogFiles(google::GLOG_INFO);

	for (auto& iter : m_listConnectorInfo)
	{
		_ConnectorInfo& info = iter;		

		LOG(INFO) << "connectID: " << info.connectorId << ", ProtocolID: " << (int)info.packetProtocolId << ", IP: " << info.szIP.data() << ", Port: " << info.port;
		google::FlushLogFiles(google::GLOG_INFO);

		int serial = -1;
		serial = SFEngine::GetInstance()->AddConnector(info.connectorId, (char*)info.szIP.c_str(), info.port);

		if (serial >= 0)
		{
			info.connected = true;
		}
	}

	m_hThread = _beginthreadex(0, NULL, ServerReconnectProc, this, 0, (unsigned*)&m_dwThreadID);

	if (m_hThread == 0)
	{
#ifdef _WIN32
		int errorNum = (GetLastError() == ERROR_SUCCESS) ? ERROR_MAX_THRDS_REACHED : GetLastError();
		LOG(FATAL) << "ServerReconnectProc Thread Creation Fail. Error : " << errorNum;
#else
		LOG(FATAL) << "ServerReconnectProc Thread Creation Fail. Error : " << errno;
		
#endif
		return false;
	}
	
	LOG(INFO) << "ServerReconnectProc success";

	return true;
}

#ifndef _WIN32
bool SFServerConnectionManager::WaitForSingleObject()
{	
	struct timespec ts;
	struct timeval tp;

	pthread_mutex_lock(m_lock);

	if (gettimeofday(&tp, nullptr) != 0)
	{
		printf("fail to gettimeofday, %s\n", strerror(errno));		
	}

	ts.tv_sec = tp.tv_sec;
	ts.tv_nsec = tp.tv_usec * 1000;
	ts.tv_sec += 1;

	nRet = pthread_cond_timedwait(m_event, m_lock, &ts);
	pthread_mutex_unlock(m_lock);
}
#endif

#ifdef _WIN32
static unsigned WINAPI ServerReconnectProc(void* arg)
#else
static unsigned ServerReconnectProc(void* arg)
#endif
{
	SFServerConnectionManager * pConnectionManager = reinterpret_cast<SFServerConnectionManager*>(arg);
			
#ifdef _WIN32
	while (WaitForSingleObject(pConnectionManager->GetTimerHandle(), 1000) != WAIT_OBJECT_0 && pConnectionManager->m_bThreadEnd == false)
#else
	while (WaitForSingleObject() == true && pConnectionManager->m_bThreadEnd == false)
}
#endif
	{
		for (auto& iter : pConnectionManager->GetConnectorInfo())
		{
			_ConnectorInfo& info = iter;

			if (info.connected == false)
			{
				int serial = -1;
				serial = SFEngine::GetInstance()->AddConnector(info.connectorId, (char*)info.szIP.c_str(), info.port);

				if (serial >= 0)
				{
					info.connected = true;
				}
			}			
		}
	}

	return 0;
}

bool SFServerConnectionManager::SetConnectorState(int connectorId, bool connected)
{
	for (auto& iter : m_listConnectorInfo)
	{
		_ConnectorInfo& info = iter;

		if (info.connectorId == connectorId)
		{
			info.connected = connected;
			return true;
		}
	}

	return false;
}