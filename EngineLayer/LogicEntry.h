#pragma once
#include "ILogicEntry.h"

class LogicEntry
{
public:
	LogicEntry(void);
	virtual ~LogicEntry(void);

	virtual bool ProcessPacket(BasePacket* pPacket);
	virtual bool DestroyLogic();

	void SetLogic(ILogicEntry* pLogic)
	{
		m_pLogicEntry = pLogic;
	}
	
	static LogicEntry* GetInstance();

protected:

private:
	static LogicEntry* m_pLogic;
	ILogicEntry* m_pLogicEntry;
};
