#pragma once
#include <pthread.h>

class CMutex
{
	pthread_mutex_t m_lock;
public:
	CMutex();
	~CMutex();

	int Lock(int nTimeout = -1);
	int Unlock();
};

class CRWLock
{
	pthread_rwlock_t m_lock;
public:
	CRWLock();
	~CRWLock();
	
	int ReadLock(int nTimeout = -1);
	int WriteLock(int nTimeout = -1);
	int Unlock();
};

