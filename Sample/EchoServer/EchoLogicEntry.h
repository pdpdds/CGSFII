#pragma once
#include "../EngineLayer/ILogicEntry.h"

class BasePacket;

class EchoLogicEntry : public ILogicEntry
{
public:
	EchoLogicEntry(void);
	virtual ~EchoLogicEntry(void);

	virtual bool Initialize() override;
	virtual bool ProcessPacket(BasePacket* pBasePacket) override;
	virtual ILogicEntry* Clone() override;

private:
	int m_timerCount;
};

