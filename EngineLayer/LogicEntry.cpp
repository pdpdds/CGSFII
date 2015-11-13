#include "StdAfx.h"
#include "LogicEntry.h"

LogicEntry* LogicEntry::m_pLogic = nullptr;

LogicEntry::LogicEntry(void)
: m_pLogicEntry(NULL)
{
}

LogicEntry::~LogicEntry(void)
{
}

bool LogicEntry::ProcessPacket(BasePacket* pPacket)
{
	if (m_pLogicEntry)
	{		
		_SessionDesc& desc = pPacket->GetSessionDesc();
		if (desc.sessionType <= SESSION_TYPE::SESSION_LISTENER)
			m_pLogicEntry->ProcessPacket(pPacket);
		else
		{
			m_pLogicEntry->ProcessConnectorPacket(pPacket);
		}
	}
		
	return true;
}

bool LogicEntry::DestroyLogic()
{
	if (m_pLogicEntry)
	{
		delete m_pLogicEntry;
	}

	return true;
}

LogicEntry* LogicEntry::GetInstance()
{
	if (m_pLogic == NULL)
		m_pLogic = new LogicEntry();

	return m_pLogic;
}