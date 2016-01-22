#include "stdafx.h"
#include "stdafx.h"
#include "SFProtobufExtensionProtocol.h"
#include "SFProtobufPacketImpl.h"
#include "ISession.h"
#include "SFEngine.h"

#pragma warning (disable : 4100) 

#define SignatureStart 16018
#define SignatureEnd 16108


#define nReceiveBufferSize 1024 * 8/*8192*/
#define nSendBufferSize 1024 * 8/*8192*/
SFProtobufExtensionProtocol::SFProtobufExtensionProtocol(void)
	:oBuffer(nSendBufferSize), m_Buffer(nReceiveBufferSize)
{

}


SFProtobufExtensionProtocol::~SFProtobufExtensionProtocol(void)
{

}

bool SFProtobufExtensionProtocol::Initialize(int ioBufferSize, unsigned short packetSize, int packetOption)
{
	m_ioSize = ioBufferSize;
	m_packetSize = packetSize;
	m_packetOption = packetOption;

	return true;
}

bool SFProtobufExtensionProtocol::AddTransferredData(char* pBuffer, DWORD dwTransferred)
{
	return m_Buffer.Append(pBuffer, dwTransferred);
}

bool SFProtobufExtensionProtocol::Reset()
{
	return true;
}

bool SFProtobufExtensionProtocol::Encode(BasePacket* pPacket, char** ppBuffer, int& bufferSize)
{
	unsigned int uWrittenBytes = 0;
	int iResult = serializeOutgoingPacket(*pPacket, oBuffer, uWrittenBytes);
	if (iResult != SFProtocol::Success)
	{
		return false;
	}

	unsigned int uSize = oBuffer.GetDataSize();

	oBuffer.Pop(uSize);

	*ppBuffer = oBuffer.GetBuffer();
	bufferSize = uSize;

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
int SFProtobufExtensionProtocol::decodeIncomingPacket(BasePacket* pPacket, int& serviceId)
{
	return SFProtocol::Success;
}

void SFProtobufExtensionProtocol::disposeOutgoingPacket(BasePacket* pPacket)
{
	delete pPacket;
}

void SFProtobufExtensionProtocol::disposeIncomingPacket(BasePacket* pPacket)
{
	delete pPacket;
}

bool SFProtobufExtensionProtocol::DisposePacket(BasePacket* pPacket)
{
	delete pPacket;
	return true;
}

BasePacket* SFProtobufExtensionProtocol::GetPacket(int& ErrorCode)
{
	//The Processing Loop.
	int uCommandID;
	BasePacket* pPacket = NULL;
	int iResult;
	unsigned int uExtractedBytes;

	ErrorCode = 0;

	//watch.GetElapsedTime(false);
	iResult = tryDeserializeIncomingPacket(m_Buffer, pPacket, uCommandID, uExtractedBytes);
	//iResult = pProtocol->tryDeframeIncomingPacket(pChannel->GetReceiveBuffer().GetBuffer(), pChannel->GetReceiveBuffer().GetDataSize(),uCommandID, pPacket, uExtractedBytes);
	//wcout << L"Packet deframed : " << watch.GetElapsedTime(false) << std::endl;
	if (iResult == SFProtocol::Success)
	{
		m_Buffer.Pop(uExtractedBytes);
		pPacket->SetPacketID(uCommandID);
	}
	else if (iResult == SFProtocol::eDecodingFailure)
	{
		m_Buffer.Pop(uExtractedBytes);
		ErrorCode = -1;

		return NULL;
	}

	return pPacket;
}

int SFProtobufExtensionProtocol::encodeOutgoingPacket(BasePacket& packet)
{
	SFProtobufPacketImpl& gPacket = (SFProtobufPacketImpl&)packet;

	return gPacket.Encode() ? SFProtocol::Success : SFProtocol::eEncodingFailure;
}

int SFProtobufExtensionProtocol::tryDeframeIncomingPacket(DataBuffer& Buffer, BasePacket*& pPacket, int& packetId, unsigned int& nExtractedBytes)
{
	if (Buffer.GetDataSize() < 8)
		return SFProtocol::eIncompletePacket;

	char* pBuffer = Buffer.GetBuffer();

	unsigned int sStart = 0;
	unsigned int packetLen = 0;
	packetId = 0;
	unsigned int extendedDataLen = 0;
	unsigned int sEnd = 0;

	for (int i = 0; i<2; i++)
	{
		*((BYTE*)(&sStart) + i) = pBuffer[i];
		*((BYTE*)(&packetLen) + i) = pBuffer[i + 2];
		*((BYTE*)(&packetId) + i) = pBuffer[i + 4];
		*((BYTE*)(&extendedDataLen) + i) = pBuffer[i + 6];
	}

	if (sStart != SignatureStart)
		return SFProtocol::eCorruptPacket;

	if (packetLen > Buffer.GetDataSize())
		return SFProtocol::eIncompletePacket;

	for (int i = 0; i<2; i++)
		*((BYTE*)(&sEnd) + i) = pBuffer[packetLen - 2 + i];

	if (sEnd != SignatureEnd)
		return SFProtocol::eCorruptPacket;

	char* pData = pBuffer + 8;
	int dataSize = packetLen - 10 - extendedDataLen;

	if (dataSize <= 0)
		return SFProtocol::eCorruptPacket;

	nExtractedBytes = packetLen;

	pPacket = CreateIncomingPacketFromPacketId(packetId, 0);

	SFProtobufPacketImpl& gPacket = (SFProtobufPacketImpl&)(*pPacket);

	if (pPacket == NULL)
		return SFProtocol::eUndefinedFailure;

	if (!pPacket->Decode(pData, dataSize))
	{
		disposeIncomingPacket(pPacket);
		return SFProtocol::eDecodingFailure;
	}

	pData = pBuffer + 8 + dataSize;

	if (extendedDataLen > 0)
	{
		gPacket.getExtendedData()->Append(pData, extendedDataLen);
	}

	return SFProtocol::Success;
}

int SFProtobufExtensionProtocol::frameOutgoingPacket(BasePacket& packet, DataBuffer& buffer, unsigned int& nWrittenBytes)
{
	SFProtobufPacketImpl& gPacket = (SFProtobufPacketImpl&)packet;

	nWrittenBytes = (unsigned int)(8 + gPacket.getEncodedStream()->size() + gPacket.getExtendedDataSize() + 2);

	if (nWrittenBytes > buffer.getRemainingSize())
		return SFProtocol::eInsufficientBuffer;
	//
	unsigned int sStart = SignatureStart;
	unsigned int extendedDataSize = gPacket.getExtendedDataSize();
	unsigned int packetLen = gPacket.getEncodedStreamSize() + 10 + extendedDataSize;
	unsigned int commandID = gPacket.getServiceId();

	unsigned int sEnd = SignatureEnd;

	buffer.Append((char*)&sStart, 2);
	buffer.Append((char*)&packetLen, 2);
	buffer.Append((char*)&commandID, 2);
	buffer.Append((char*)&extendedDataSize, 2);

	buffer.Append((char*)gPacket.getEncodedStream()->c_str(), gPacket.getEncodedStreamSize());
	buffer.Append((char*)gPacket.getExtendedData()->GetBuffer(), gPacket.getExtendedDataSize());

	buffer.Append((char*)&sEnd, 2);

	return SFProtocol::Success;
}