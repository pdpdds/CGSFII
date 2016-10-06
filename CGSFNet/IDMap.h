#pragma once
#include <ace/Map_Manager.h>
#include "Queue.h"

template <typename T>
class SFIDMap
{
	typedef ACE_Map_Manager<int, T*, ACE_Thread_Mutex> IDMap;

public:
	SFIDMap() {}
	virtual ~SFIDMap(void){}

	void Init(int maxIdCount, int offset)
	{
		m_idleIdQueue.Init(maxIdCount, offset);
	}

	int Register(T* p)
	{				
		unsigned int id = m_idleIdQueue.Pop();
		if (id == INVALID_ID)
			return INVALID_ID;
		m_IDMap.bind(id, p);
		
		return id;
	}

	void UnRegister(int id)
	{		
		T* t = NULL;
		if (-1 != m_IDMap.find(id, t))
		{
			m_IDMap.unbind(id);
			m_idleIdQueue.Push(id);
		}
	}

	T* Get(int id)
	{
		T* t = NULL;
		if (-1 == m_IDMap.find(id, t))
			return NULL;
		return t;
	}

	int Size()
	{
		return (int)m_IDMap.current_size();
	}

protected:
	IDMap m_IDMap;
	IDQueue m_idleIdQueue;
};