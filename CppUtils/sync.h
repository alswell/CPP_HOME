#ifndef __SYNC_OBJ_H
#define __SYNC_OBJ_H

#include <semaphore.h>
#include <pthread.h>

class CSemaphore
{
	sem_t m_sem;
public:
	CSemaphore(int nInit = 0);
	~CSemaphore();
	
	int Lock(int nTimeout = -1);
	int Unlock();
	
	int GetValue();
};

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

#endif
