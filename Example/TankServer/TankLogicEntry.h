#pragma once
#include "ILogicEntry.h"

class TankLogicEntry : public ILogicEntry
{
public:
	TankLogicEntry();
	virtual ~TankLogicEntry();

	virtual bool Initialize() override;
	virtual bool ProcessPacket(BasePacket* pBasePacket) override;
	virtual ILogicEntry* Clone()
	{
		return new TankLogicEntry();
	}
};

