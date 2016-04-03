#pragma once
#include "SFBasePacketProtocol.h"
#include "SFPacketProtocol.h"
#ifndef _WIN32
#include "../BaseLayerLinux/CGSFDefine.h"
#endif

class SFPacketIOBuffer;
class SFCommand;

class SFCGSFPacketProtocol : public SFBasePacketProtocol
{
public:
	SFCGSFPacketProtocol(void);
	virtual ~SFCGSFPacketProtocol(void);

	bool Initialize(int ioBufferSize, unsigned short packetDataSize, int packetOption);

	bool Reset();
	BasePacket* GetPacket(int& ErrorCode);
	bool AddTransferredData(char* pBuffer, DWORD dwTransferred);
	bool Encode(BasePacket* pPacket, char** ppBuffer, int& bufferSize);
	static bool DisposePacket(BasePacket* pPacket);

protected:

private:
	SFPacketIOBuffer* m_pPacketIOBuffer;
};

