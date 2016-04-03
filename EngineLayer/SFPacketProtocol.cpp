#include "SFPacketProtocol.h"
#include "SFEngine.h"

void SendLogicLayer(BasePacket* pPacket)
{
	SFEngine::GetInstance()->GetLogicDispatcher()->Dispatch(pPacket);
}