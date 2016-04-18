#pragma once
#include "ILogicEntry.h"

class ProtoBufLogicEntry : public ILogicEntry
{
public:
	ProtoBufLogicEntry();
	virtual ~ProtoBufLogicEntry();

	virtual bool Initialize() override;
	virtual bool ProcessPacket(BasePacket* pBasePacket) override;
	virtual ILogicEntry* Clone()
	{
		return new ProtoBufLogicEntry();
	}
};

