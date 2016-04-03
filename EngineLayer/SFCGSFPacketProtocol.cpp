#include "SFCGSFPacketProtocol.h"
#include "SFCompressor.h"
#ifdef _WIN32
#include "../CGSFNet/ISession.h"
#else
#include "../CGSFNetLinux/ISession.h"
#endif
#include "SFEngine.h"
#include "SFPacketPool.h"

SFCGSFPacketProtocol::SFCGSFPacketProtocol(void)
{
	
}

SFCGSFPacketProtocol::~SFCGSFPacketProtocol(void)
{
	if(m_pPacketIOBuffer)
		delete m_pPacketIOBuffer;

	m_pPacketIOBuffer = NULL;
}

bool SFCGSFPacketProtocol::Initialize(int ioBufferSize, unsigned short packetSize, int packetOption)
{
	m_pPacketIOBuffer = new SFPacketIOBuffer();
	m_pPacketIOBuffer->AllocIOBuf(ioBufferSize);

	m_ioSize = ioBufferSize;
	m_packetSize = packetSize;
	m_packetOption = packetOption;

	return true;
}

BasePacket* SFCGSFPacketProtocol::GetPacket(int& errorCode)
{
	SFPacket* pPacket = SFPacketPool::GetInstance()->Alloc();

	pPacket->Initialize();

	if (FALSE == m_pPacketIOBuffer->GetPacket(*pPacket->GetHeader(), (char*)pPacket->GetData(), m_packetSize, errorCode))
	{
		SFPacketPool::GetInstance()->Release(pPacket);
		return NULL;
	}

	/*if (FALSE == pPacket->Decode(m_packetSize, errorCode))
	{
		PacketPoolSingleton::instance()->Release(pPacket);
		return NULL;
	}*/

	return pPacket;
}

bool SFCGSFPacketProtocol::AddTransferredData(char* pBuffer, DWORD dwTransferred)
{
	m_pPacketIOBuffer->AppendData(pBuffer, dwTransferred);

	return true;
}

bool SFCGSFPacketProtocol::Reset()
{
	m_pPacketIOBuffer->InitIOBuf();

	return true;
}

bool SFCGSFPacketProtocol::Encode(BasePacket* pPacket, char** ppBuffer, int& bufferSize)
{
	SFPacket* pSFPacket = (SFPacket*)pPacket;
	
	if (false == pSFPacket->Encode(m_packetSize, m_packetOption))
		return false;

	*ppBuffer = (char*)pSFPacket->GetHeader();
	bufferSize = pSFPacket->GetPacketSize();

	return true;
}

bool SFCGSFPacketProtocol::DisposePacket(BasePacket* pPacket)
{
	SFPacket* pSFPacket = static_cast<SFPacket*>(pPacket);

	SFASSERT(pSFPacket != NULL);
	return SFPacketPool::GetInstance()->Release(pSFPacket);
}