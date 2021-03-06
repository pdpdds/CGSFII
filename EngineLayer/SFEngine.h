#pragma once
#include "EngineLayerHeader.h"
#include "SFConfigure.h"
#include "../BaseLayer/tinythread.h"
#include "../BaseLayer/SFObjectPool.h"
#include <vector>

class SFServerConnectionManager;
class SFPacketProtocolManager;

class SFEngine : public IEngine
{
	typedef std::map<int, long> mapTimer;

public:
	virtual ~SFEngine(void);

	static SFEngine* GetInstance();	

	bool Start(int protocolId = 0);
	bool ShutDown();

	NET_ERROR_CODE Intialize(ILogicEntry* pLogicEntry, ILogicDispatcher* pDispatcher = nullptr);
		
	virtual ISessionService* CreateSessionService(_SessionDesc& desc) override;
		
	virtual bool OnConnect(int serial, _SessionDesc& desc) override;
	virtual bool OnDisconnect(int serial, _SessionDesc& desc) override;
	virtual bool OnTimer(const void *arg) override;

	bool ReleasePacket(BasePacket* pPacket);
	void SendToLogic(BasePacket* pMessage);
	bool Disconnect(int serial);

	ILogicDispatcher* GetLogicDispatcher(){ return m_pLogicDispatcher; }	
	INetworkEngine* GetNetworkEngine(){ return m_pNetworkEngine; }

//Packet Protocol Manager
	SFPacketProtocolManager* GetPacketProtocolManager(){ return m_pPacketProtocolManager; }
	int AddPacketProtocol(IPacketProtocol* pProtocol);

	bool AddTimer(int timerID, DWORD period, DWORD delay);
	bool CancelTimer(int timerID);

	bool SendRequest(BasePacket* pPacket);	

	void SetLogFolder();
	SFConfigure* GetConfig(){ return &m_config;}
	void SetConfig(SFConfigure& Config){ m_config = Config; }	

//Server Connection Manager
	SFServerConnectionManager* GetServerConnectionManager(){ return m_pServerConnectionManager; }
	bool SetupServerReconnectSys();
	bool LoadConnectorList(char* szFileName);
	int  AddListener(char* szIP, unsigned short port, int packetProtocolId);
	int  AddConnector(int connectorId, char* szIP, unsigned short port);

//Remote Procedure Call System
	bool AddRPCService(IRPCService* pService);	

protected:	
	NET_ERROR_CODE CreateEngine(char* szModuleName);
	
private:
	SFEngine();	
	void SetLogicDispathcer(ILogicDispatcher* pDispatcher){m_pLogicDispatcher = pDispatcher;}

	static SFEngine* m_pEngine; //Framework Singleton
	void* m_engineHandle; //NetworkEngine Handle
	INetworkEngine* m_pNetworkEngine; //NetworkEngine Object
	ILogicDispatcher* m_pLogicDispatcher; //Logic Dispatcher
	SFPacketProtocolManager* m_pPacketProtocolManager; //Protocol Manager
	SFConfigure m_config; //Engine Configuration
		
	SFServerConnectionManager* m_pServerConnectionManager; //Server Connection Manager
	mapTimer m_mapTimer; //Timer System

};