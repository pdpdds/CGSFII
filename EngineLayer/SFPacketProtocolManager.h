#pragma once
#include "EngineLayerHeader.h"
#include <list>
#include <map>

class IPacketProtocol;

class SFPacketProtocolManager
{
	typedef std::map<int, IPacketProtocol*> mapPacketProtocol;
	typedef std::list<_ListenerInfo> listListenerInfo;
	typedef std::list<_ConnectorInfo> listConnectorInfo;

public:
	SFPacketProtocolManager();
	virtual ~SFPacketProtocolManager();
	
	int  AddPacketProtocol(IPacketProtocol* pProtocol);
	bool AddListenerInfo(int listenerId, int packetProtocolId);
	bool AddConnectorInfo(_ConnectorInfo* pInfo);
	bool SetConnectorPacketProtocol(int connectorId, int packetProtocolId);
	bool SetConnectorState(int connectorId, bool connected);

	IPacketProtocol* GetPacketProtocol(int packetProtocolId)
	{ 
		const auto& iter = m_mapPacketProtocol.find(packetProtocolId);
		if (iter != m_mapPacketProtocol.end()) 
			return iter->second; 
		return nullptr; 
	}

	IPacketProtocol* GetPacketProtocolWithListenerId(int listenerId)
	{
		for (auto& iter : m_listListenerInfo)
		{
			if (iter.listenerId == listenerId)
				return GetPacketProtocol(iter.packetProtocolId);
		}

		return nullptr;
	}

	IPacketProtocol* GetPacketProtocolWithConnectorId(int connectorId)
	{
		for (auto& iter : m_listConnectorInfo)
		{
			if (iter.connectorId == connectorId)
				return GetPacketProtocol(iter.packetProtocolId);
		}

		return nullptr;
	}

private:
	mapPacketProtocol m_mapPacketProtocol;
	listListenerInfo m_listListenerInfo;
	listConnectorInfo m_listConnectorInfo;
	static int m_protocolIdentifier;
};