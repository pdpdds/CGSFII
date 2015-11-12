#include "StdAfx.h"
#include "SFBridgeThread.h"
#include "SFPacketDelaySendTask.h"
#include "SFEngine.h"

void PacketSendThread(void* Args)
{
	UNREFERENCED_PARAMETER(Args);

	while (true)
	{
		SFPacketDelaySendTask* pPacketTask = (SFPacketDelaySendTask*)SFPacketSendGateway::GetInstance()->PopTask();

		if (pPacketTask == NULL)
			break;

		pPacketTask->Execute();


		SFEngine::GetInstance()->m_delayedSendTaskPool.Release((SFPacketDelaySendTask*)pPacketTask);
	}
}