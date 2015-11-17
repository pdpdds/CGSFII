#include "stdafx.h"
#include "SFEngine.h"
#include <stdlib.h>  
#include "ILogicDispatcher.h"
#include "SFSessionService.h"
#include "SFSingleLogicDispatcher.h"
#include "SFUtil.h"
#include "SFServerConnectionManager.h"
#include "SFPacketProtocolManager.h"
#include "XMl/StringConversion.h"

#pragma comment(lib, "BaseLayer.lib")

SFEngine* SFEngine::m_pEngine = nullptr;

SFEngine::SFEngine()
	: m_pNetworkEngine(nullptr)
	, m_engineHandle(nullptr)
	, m_pServerConnectionManager(nullptr)	
{		

	google::InitGoogleLogging("CGSFII");
	m_config.Read(L"EngineConfig.xml");

#ifdef _DEBUG
	_putenv_s("GLOG_logbufsecs", "0");
	google::LogToStderr();
#endif

	m_pPacketProtocolManager = new SFPacketProtocolManager();
}

SFEngine::~SFEngine(void)
{		
	if (m_pNetworkEngine)
		delete m_pNetworkEngine;

	if (m_engineHandle)
		FreeLibrary(m_engineHandle);
}

SFEngine* SFEngine::GetInstance()
{
	if (m_pEngine == nullptr)
		m_pEngine = new SFEngine();

	return m_pEngine;
}

NET_ERROR_CODE SFEngine::CreateEngine(char* szModuleName)
{
	m_engineHandle = ::LoadLibraryA(szModuleName);

	if (m_engineHandle == nullptr)
		return NET_ERROR_CODE::ENGINE_INIT_CREAT_ENGINE_LOAD_DLL_FAIL;

	CREATENETWORKENGINE *pfunc;
	pfunc = (CREATENETWORKENGINE*)::GetProcAddress(m_engineHandle, "CreateNetworkEngine");
	m_pNetworkEngine = pfunc(this);

	if (m_pNetworkEngine == nullptr)
		return NET_ERROR_CODE::ENGINE_INIT_CREAT_ENGINE_FUNC_NULL;

	if(false == m_pNetworkEngine->Init())
		return NET_ERROR_CODE::ENGINE_INIT_CREAT_ENGINE_INIT_FAIL;
	
	return NET_ERROR_CODE::SUCCESS;
}

bool SFEngine::AddRPCService(IRPCService* pService)
{
	return m_pLogicDispatcher->AddRPCService(pService);	
}

ISessionService* SFEngine::CreateSessionService(_SessionDesc& desc)
{
	IPacketProtocol* pSourceProtocol = nullptr;
	if (desc.sessionType == SESSION_TYPE::SESSION_LISTENER)
		pSourceProtocol = m_pPacketProtocolManager->GetPacketProtocolWithListenerId(desc.identifier);
	else
		pSourceProtocol = m_pPacketProtocolManager->GetPacketProtocolWithConnectorId(desc.identifier);

	if (pSourceProtocol == nullptr)
	{
		SFASSERT(0);
	}
		
	IPacketProtocol* pCloneProtocol = pSourceProtocol->Clone();
	return new SFSessionService(pCloneProtocol);
}

void SFEngine::SetLogFolder()
{
	WCHAR szFilePath[MAX_PATH] = { 0, };
	GetModuleFileName(nullptr, szFilePath, MAX_PATH);

	WCHAR* path = SFUtil::ExtractPathInfo(szFilePath, SFUtil::PATH_DIR);
	SetCurrentDirectory(path);

	std::wstring szLogPath = path;
	szLogPath += L"Log\\";

	CreateDirectory(szLogPath.c_str(), nullptr);
	
	std::wstring szLogInfo = szLogPath + L"INFO_";
	std::wstring szLogWarning = szLogPath + L"WARNING_";
	std::wstring szLogError = szLogPath + L"ERROR_";

	google::SetLogDestination(google::GLOG_INFO, (char*)StringConversion::ToASCII(szLogInfo).c_str());
	google::SetLogDestination(google::GLOG_WARNING, (char*)StringConversion::ToASCII(szLogWarning).c_str());
	google::SetLogDestination(google::GLOG_ERROR, (char*)StringConversion::ToASCII(szLogError).c_str());

	LOG(INFO) << "Log Destination " << (char*)StringConversion::ToASCII(szLogPath).c_str();	
}

NET_ERROR_CODE SFEngine::Intialize(ILogicEntry* pLogicEntry, ILogicDispatcher* pDispatcher)
{
	LOG(INFO) << "Engine Initialize... ";

	SetLogFolder();	

	if (NULL == pLogicEntry)
		return NET_ERROR_CODE::ENGINE_INIT_LOGIC_ENTRY_NULL;

	if (pDispatcher == nullptr)
	{
		pDispatcher = new SFSingleLogicDispatcher();
	}

	SetLogicDispathcer(pDispatcher);

	m_pServerConnectionManager = new SFServerConnectionManager();

	LOG(INFO) << "Logic Entry Initialize";
	if (false == pLogicEntry->Initialize())
	{
		LOG(ERROR) << "LogicEntry Intialize Fail!!";
		return NET_ERROR_CODE::ENGINE_INIT_LOGIC_ENTRY_FAIL;
	}
	
	
	_EngineConfig* pInfo = m_config.GetConfigureInfo();
	
	LOG(INFO) << "Basic Port: " << pInfo->serverPort;

	std::string szNetworkEngineName = StringConversion::ToASCII(pInfo->engineName);
	LOG(INFO) << "NetworkEngine Create : " << szNetworkEngineName.c_str();

	LOG(INFO) << "MaxAccept : " << pInfo->maxAccept;

	if (pInfo->maxAccept <= 0 || pInfo->maxAccept > MAX_USER_ACCEPT)
	{
		LOG(WARNING) << "MaxAccept Value Abnormal : " << pInfo->maxAccept;
		SetMaxUserAccept(DEFALUT_USER_ACCEPT);
	}
	else
		SetMaxUserAccept(pInfo->maxAccept);
		
	auto errorCode = CreateEngine((char*)szNetworkEngineName.c_str());
	if (errorCode != NET_ERROR_CODE::SUCCESS)
	{
		LOG(ERROR) << "NetworkEngine : " << szNetworkEngineName.c_str() << " Creation FAIL!!";
		return errorCode;
	}

	LOG(INFO) << "NetworkEngine : " << szNetworkEngineName.c_str() << " Creation Success!!";

	if (false == pDispatcher->CreateLogicSystem(pLogicEntry))
	{
		LOG(ERROR) << "Logic System Creation FAIL!!";
		return NET_ERROR_CODE::ENGINE_INIT_CREAT_LOGIC_SYSTEM_FAIL;
	}	

	LOG(INFO) << "Engine Initialize Complete!! ";
	return NET_ERROR_CODE::SUCCESS;
}

////////////////////////////////////////////////////////////////////
//Add Timer
////////////////////////////////////////////////////////////////////
bool SFEngine::AddTimer(int timerID, DWORD period, DWORD delay)
{
	_TimerInfo Timer;
	Timer.TimerID = timerID;
	Timer.Period = period;
	Timer.StartDelay = delay;

	if(GetNetworkEngine()->CheckTimerImpl())
	{
		long internelTimerId = GetNetworkEngine()->AddTimer(timerID, delay, period);

		if (internelTimerId < 0)
		{
			LOG(ERROR) << "Timer Creation FAIL!!";
			return FALSE;
		}

		m_mapTimer.insert(std::make_pair(timerID, internelTimerId));

		LOG(INFO) << "TimerId : " << internelTimerId << "Creation Success!!";
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////
//Cancel Timer
//타이머 아이디가 음수일 경우 등록된 모든 타이머를 취소한다.
////////////////////////////////////////////////////////////////////
bool SFEngine::CancelTimer(int timerID)
{
	mapTimer::iterator iter;
	bool bResult = false;

	if (timerID >= 0)
	{
		iter = m_mapTimer.find(timerID);

		if (iter == m_mapTimer.end())
			return false;

		bResult = GetNetworkEngine()->CancelTimer(iter->second);
	}
	else
	{
		bResult = GetNetworkEngine()->CancelTimer(timerID);
	}
	
	if (bResult == false)
	{
		LOG(INFO) << "Timer Cancel Fail. Id : " << timerID;
		return false;
	}

	if (timerID < 0)
		m_mapTimer.clear();
	else
		m_mapTimer.erase(timerID);

	return true;
}

bool SFEngine::Start(int protocolId)
{
	bool bResult = false;
	_EngineConfig* pInfo = m_config.GetConfigureInfo();
	LOG(INFO) << "Engine Starting... IP : " << (char*)StringConversion::ToASCII(pInfo->serverIP).c_str() << " Port : " << pInfo->serverPort;
	char* serverIP = (char*)StringConversion::ToASCII(pInfo->serverIP).c_str();
	short port = pInfo->serverPort;

	if (protocolId >= 0)
	{
		IPacketProtocol* pProtocol = m_pPacketProtocolManager->GetPacketProtocol(protocolId);

		if (pProtocol == nullptr)
		{
			LOG(ERROR) << "Engine Start Fail. PacketProtocol : " << protocolId;
			return false;
		}

		int listenerId = -1;
		
		if (pInfo->serverPort != 0)
		{			
			listenerId = AddListener(serverIP, port, protocolId);

			if (listenerId <= 0)
			{
				LOG(ERROR) << "Engine Start Fail. m_pNetworkEngine->AddListener fail. " << serverIP << ":" << port;
				return false;
			}
		}
	}

//리스너가 하나라도 등록되어 있지 않은 false를 리턴한다.
	bResult = m_pNetworkEngine->Start();

	if (bResult == false)
	{
		LOG(ERROR) << "Engine Start Fail!! " << serverIP << ":" << port;;
		return false;
	}
	LOG(INFO) << "Engine Start!!";

	return true;
}

bool SFEngine::ShutDown()
{
	// 올바르게 종료되는지 조사하기 위해 각 단계별로 로그를 남깁니다.
	LOG(INFO) << "Engine Shut Down!!";	
		
	m_pLogicDispatcher->ShutDownLogicSystem();
	LOG(INFO) << "Engine Shut Down Step (1) ShutDownLogicSystem";		

	if (m_pNetworkEngine)
	{
		m_pNetworkEngine->Shutdown();
		LOG(INFO) << "Engine Shut Down Step (2) m_pNetworkEngine->Shutdown()";		
				
		//delete m_pNetworkEngine;
	}
	
	if (m_pServerConnectionManager)
	{
		delete m_pServerConnectionManager;
		LOG(INFO) << "Engine Shut Down Step (3) delete Server Connecton Manager";
	}
	
	delete this;
	LOG(INFO) << "Engine Shut Down Step (4) Engine Delete";

	google::ShutdownGoogleLogging();
	
	return true;
}

bool SFEngine::OnConnect(int serial, _SessionDesc& desc)
{
	BasePacket* pPacket = new BasePacket();
	pPacket->SetSessionDesc(desc);
	pPacket->SetPacketType(SFPACKET_CONNECT);
	pPacket->SetSerial(serial);

	m_pLogicDispatcher->Dispatch(pPacket);
	
	return true;
}

bool SFEngine::OnDisconnect(int serial, _SessionDesc& desc)
{
	BasePacket* pPacket = new BasePacket();
	pPacket->SetSessionDesc(desc);
	pPacket->SetPacketType(SFPACKET_DISCONNECT);
	pPacket->SetSerial(serial);

	m_pLogicDispatcher->Dispatch(pPacket);
	
	return true;
}

bool SFEngine::OnTimer(const void *arg)
{
	int timerId = (int)arg;

	BasePacket* pPacket = new BasePacket();
	pPacket->SetPacketType(SFPACKET_TIMER);
	pPacket->SetSerial(timerId);

	m_pLogicDispatcher->Dispatch(pPacket);

	return true;
}

bool SFEngine::SendRequest(BasePacket* pPacket)
{
	return GetNetworkEngine()->SendRequest(pPacket);
}

bool SFEngine::Disconnect(int serial)
{
	return GetNetworkEngine()->Disconnect(serial);
}

int SFEngine::AddConnector(int connectorId, char* szIP, unsigned short port)
{
	return GetNetworkEngine()->AddConnector(connectorId, szIP, port);
}

int SFEngine::AddListener(char* szIP, unsigned short port, int packetProtocolId)
{
	int listenerId = GetNetworkEngine()->AddListener(szIP, port);

	if (listenerId)
	{
		m_pPacketProtocolManager->AddListenerInfo(listenerId, packetProtocolId);		
	}

	return listenerId;
}

bool SFEngine::LoadConnectorList(WCHAR* szFileName)
{
	return m_pServerConnectionManager->LoadConnectorList(szFileName);
}

bool SFEngine::SetupServerReconnectSys()
{
	return m_pServerConnectionManager->SetupServerReconnectSys();
}

int SFEngine::AddPacketProtocol(IPacketProtocol* pProtocol)
{
	return m_pPacketProtocolManager->AddPacketProtocol(pProtocol);
}

void SFEngine::SendToLogic(BasePacket* pMessage)
{
	SFLogicGateway::GetInstance()->PushPacket(pMessage);
}

bool SFEngine::ReleasePacket(BasePacket* pPacket)
{
	pPacket->Release();
	return true;
}