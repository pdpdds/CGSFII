#include "stdafx.h"
#include "SFPacketProtocolManager.h"

int SFPacketProtocolManager::m_protocolIdentifier = -1;

SFPacketProtocolManager::SFPacketProtocolManager()
{
}

SFPacketProtocolManager::~SFPacketProtocolManager()
{
	for (auto& iter : m_mapPacketProtocol)
	{
		IPacketProtocol* pProtocol = iter.second;
		delete pProtocol;
	}

	m_mapPacketProtocol.clear();
}

int SFPacketProtocolManager::AddPacketProtocol(IPacketProtocol* pProtocol)
{
	m_protocolIdentifier++;
	std::pair< std::map<int, IPacketProtocol*>::iterator, bool > result;
	result = m_mapPacketProtocol.insert(std::make_pair(m_protocolIdentifier, pProtocol));

	if (result.second == false)
		return -1;

	return m_protocolIdentifier;
}


bool SFPacketProtocolManager::AddListenerInfo(int listenerId, int packetProtocolId)
{
	_ListenerInfo info;
	info.listenerId = listenerId;
	info.packetProtocolId = packetProtocolId;

	m_listListenerInfo.push_back(info);

	LOG(INFO) << "SFPacketProtocolManager::AddListenerInfo. listenerId: " << listenerId << ", packetProtocolId: " << packetProtocolId;
	google::FlushLogFiles(google::GLOG_INFO);
	return true;	
}

bool SFPacketProtocolManager::AddConnectorInfo(_ConnectorInfo* pInfo)
{
	m_listConnectorInfo.push_back(*pInfo);

	return true;
}

bool SFPacketProtocolManager::SetConnectorPacketProtocol(int connectorId, int packetProtocolId)
{
	for (auto& iter : m_listConnectorInfo)
	{
		if (iter.connectorId == connectorId)
		{
			iter.packetProtocolId = packetProtocolId;
			return true;
		}
	}

	return false;
}

bool SFPacketProtocolManager::SetConnectorState(int connectorId, bool connected)
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

