#pragma once

#pragma once
#include "SFProtocol.h"
#include "DataBuffer.h"
#include "SFConstant.h"
#include "SFBasePacketProtocol.h"

class ISession;
class BasePacket;

class SFProtobuf2ExtensionProtocol : public SFProtocol, public SFBasePacketProtocol
{
public:
	SFProtobuf2ExtensionProtocol(void);
	virtual ~SFProtobuf2ExtensionProtocol(void);

	bool Initialize(int ioBufferSize, unsigned short packetSize, int packetOption);

	bool Reset();
	BasePacket* GetPacket(int& ErrorCode);
	bool AddTransferredData(char* pBuffer, DWORD dwTransferred);
	bool Encode(BasePacket* pPacket, char** ppBuffer, int& bufferSize);

	virtual void disposeOutgoingPacket(BasePacket* pPacket);
	virtual void disposeIncomingPacket(BasePacket* pPacket);
	BasePacket* CreatePacket(){ return NULL; }
	static bool DisposePacket(BasePacket* pPacket);

protected:
	virtual BasePacket* CreateIncomingPacketFromPacketId(int packetId, int channel) = 0;

protected:
	virtual int encodeOutgoingPacket(BasePacket& packet);
	virtual int frameOutgoingPacket(BasePacket& packet, DataBuffer& buffer, unsigned int& nWrittenBytes);

	//
	virtual int tryDeframeIncomingPacket(DataBuffer& Buffer, BasePacket*& pPacket, int& serviceId, unsigned int& nExtractedBytes);
	virtual int decodeIncomingPacket(BasePacket* pPacket, int& packetId);

protected:

private:
	DataBuffer m_Buffer;
	DataBuffer oBuffer;
};