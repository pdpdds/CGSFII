#pragma once
#ifdef _WIN32
#include "ILogicEntry.h"
#else
#include "../EngineLayerLinux/ILogicEntry.h"
#endif

class BasePacket;

class EchoLogicEntry : public ILogicEntry
{
public:
	EchoLogicEntry(void);
	virtual ~EchoLogicEntry(void);

	virtual bool Initialize() override;
	virtual bool ProcessPacket(BasePacket* pBasePacket) override;
	virtual ILogicEntry* Clone() override;
};

