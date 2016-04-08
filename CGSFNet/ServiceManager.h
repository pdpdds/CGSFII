#pragma once
#include "ACEHeader.h"
#include "IDMap.h"
#include "Lock.h"
#include "BasePacket.h"

class ProactorService;

template <typename LockStrategy, typename T, int maxIdCount>
class ServiceManager
{
public:
	ServiceManager(){}
	virtual ~ServiceManager(){}

	bool SendRequest(BasePacket* pPacket)
	{
		SFLockHelper lock(&m_lock);

		T* pService = m_IdMap.Get(pPacket->GetSerial());

		if (pService != NULL)
		{
			pService->SendRequest(pPacket);
		}

		return true;
	}

	bool Disconnect(int serial)
	{
		SFLockHelper lock(&m_lock);

		T* pService = m_IdMap.Get(serial);

		if (pService != NULL)
		{
			pService->Disconnect();
			return true;
		}

		return false;
	}

	bool BroadCast(int ownerSerial, int destSerial[], int destSize, char* buffer, unsigned int bufferSize)
	{
		SFLockHelper lock(&m_lock);

		for (int i = 0; i < destSize; i++)
		{
			T* pService = m_IdMap.Get(destSerial[i]);

			if (pService != NULL)
			{
				pService->SendInternal(buffer, bufferSize);
			}
		}

		return true;
	}
	
	int Register(T* p)
	{
		SFLockHelper lock(&m_lock);

		return m_IdMap.Register(p);
	}

	void UnRegister(int id)
	{
		SFLockHelper lock(&m_lock);

		m_IdMap.UnRegister(id);
	}

private:
	SFLock m_lock;
	SFIDMap<LockStrategy, T, maxIdCount> m_IdMap;
};

#define MAX_CGSF_CONCURRENT_USER 5000

#ifdef _WIN32
typedef ACE_Singleton<ServiceManager<ACE_Null_Mutex, ProactorService, MAX_CGSF_CONCURRENT_USER>, ACE_Thread_Mutex> CGSFServiceManager;
#else
typedef ACE_Singleton<ServiceManager<ACE_Null_Mutex, ReactorService, MAX_CGSF_CONCURRENT_USER>, ACE_Thread_Mutex> CGSFServiceManager;
#endif