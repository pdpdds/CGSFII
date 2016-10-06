#pragma once
#include "ACEHeader.h"
#include "IDMap.h"
#include "Lock.h"
#include "BasePacket.h"

class ProactorService;
class ReactorService;

template <typename T>
class ServiceManager
{
public:
	ServiceManager(){}
	virtual ~ServiceManager(){}

	void Init(int maxIdCount, int offset)
	{
		m_IdMap.Init(maxIdCount, offset);
	}

	bool SendRequest(BasePacket* pPacket)
	{
		T* pService = m_IdMap.Get(pPacket->GetSerial());

		if (pService != NULL)
		{
			pService->SendRequest(pPacket);
		}

		return true;
	}

	bool Disconnect(int serial)
	{

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
		return m_IdMap.Register(p);
	}

	void UnRegister(int id)
	{		
		m_IdMap.UnRegister(id);
	}

private:	
	SFIDMap<T> m_IdMap;
};

typedef ACE_Singleton<ServiceManager<ProactorService>, ACE_Null_Mutex> CGSFServiceManager;
typedef ACE_Singleton<ServiceManager<ReactorService>, ACE_Null_Mutex> ReactorServiceManager;

