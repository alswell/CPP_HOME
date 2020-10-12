#ifndef __QUEUE_H
#define __QUEUE_H

#include "../env.h"
#include "lock.h"

template <class T>
class CArrayQueue
{
	CSemaphore m_semSolid;
	CSemaphore m_semHollow;
	CMutex m_mtPop;
	CMutex m_mtPush;
	T* m_pQ;
	int m_nFront;
	int m_nBack;
	int m_nSize;
public:
	CArrayQueue(int nSize)
		: m_semSolid(0)
		, m_semHollow(nSize)
	{
		m_pQ = new T[nSize];
		m_nSize = nSize;

		m_nFront = m_nSize - 1;
		m_nBack = 0;
	}
	~CArrayQueue()
	{
		delete[] m_pQ;
	}
	
	void Pop(T& e)
	{
		m_mtPop.Lock();
		m_semSolid.Lock();
		m_nFront++;
		m_nFront %= m_nSize;
		e = m_pQ[m_nFront];
		m_semHollow.Unlock();
		m_mtPop.Unlock();
	}
	void Push(const T& e)
	{
		m_mtPush.Lock();
		m_semHollow.Lock();
		m_pQ[m_nBack] = e;
		m_nBack++;
		m_nBack %= m_nSize;
		m_semSolid.Unlock();
		m_mtPush.Unlock();
	}
};

template <class T>
class CListQueue
{
	CSemaphore m_sem;
	CMutex m_mt;
	list<T> m_ls;
public:
	CListQueue()
		: m_sem(0)
	{
	}

	void Pop(T& e)
	{
		m_mt.Lock();
		m_sem.Lock();
		e = m_ls.front();
		m_ls.pop_front();
		m_mt.Unlock();
	}
	void Push(const T& e)
	{
		m_mt.Lock();
		m_ls.push_back(e);
		m_sem.Unlock();
		m_mt.Unlock();
	}
};

#endif
