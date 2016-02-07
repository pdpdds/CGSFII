#include "StdAfx.h"
#include "LogicEntry.h"
#include <boost/thread/tss.hpp>

LogicEntry* LogicEntry::m_pLogic = nullptr;
static boost::thread_specific_ptr<ILogicEntry> sLogicEntry;

LogicEntry::LogicEntry(void)
: m_pLogicEntry(NULL)
{
}

LogicEntry::~LogicEntry(void)
{
}

bool LogicEntry::Initialize()
{
	if (!sLogicEntry.get())
	{
		sLogicEntry.reset(m_pLogicEntry->Clone());
		LOG(INFO) << "LogicEntry Init. threadId :" << GetCurrentThreadId();
		
		if (false == sLogicEntry->Initialize())
		{
			LOG(ERROR) << "LogicEntry Intialize Fail!!";
			return false;
		}
	}

	return true;
}

bool LogicEntry::ProcessPacket(BasePacket* pPacket)
{		
	_SessionDesc& desc = pPacket->GetSessionDesc();
	if (desc.sessionType <= SESSION_TYPE::SESSION_LISTENER)
		sLogicEntry->ProcessPacket(pPacket);
	else
	{
		sLogicEntry->ProcessConnectorPacket(pPacket);
	}

	return true;
}

bool LogicEntry::DestroyLogic()
{
	if (sLogicEntry.get())
	{
		sLogicEntry.release();
	}

	return true;
}

bool LogicEntry::Destroy()
{
	if (m_pLogicEntry)
	{
		delete m_pLogicEntry;
	}

	if (m_pLogic)
		delete m_pLogic;

	return true;
}

LogicEntry* LogicEntry::GetInstance()
{
	if (m_pLogic == NULL)
		m_pLogic = new LogicEntry();

	return m_pLogic;
}