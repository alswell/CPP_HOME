#include "sync.h"

#define SYNC_WAIT(sync_obj, _lock, _timedlock, nTimeout) \
	if (nTimeout == -1)\
	{\
		return _lock(&sync_obj);\
	}\
	else\
	{\
		struct timespec timeout;\
		timeout.tv_sec = nTimeout / 1000;\
		timeout.tv_nsec = nTimeout % 1000 * 1000000;\
		return _timedlock(&sync_obj, &timeout);\
	}


CSemaphore::CSemaphore(int nInit)
{
	sem_init(&m_sem, 0, nInit);
}

CSemaphore::~CSemaphore()
{
	sem_destroy(&m_sem);
}

int CSemaphore::Lock(int nTimeout)
{
	SYNC_WAIT(m_sem, sem_wait, sem_timedwait, nTimeout)
}

int CSemaphore::Unlock()
{
	return sem_post(&m_sem);
}

int CSemaphore::GetValue()
{
	int r;
	sem_getvalue(&m_sem, &r);
	return r;
}

CMutex::CMutex()
{
	pthread_mutex_init(&m_lock, 0);
}

CMutex::~CMutex()
{
	pthread_mutex_destroy(&m_lock);
}

int CMutex::Lock(int nTimeout)
{
	SYNC_WAIT(m_lock, pthread_mutex_lock, pthread_mutex_timedlock, nTimeout)
}

int CMutex::Unlock()
{
	return pthread_mutex_unlock(&m_lock); 
}

CRWLock::CRWLock()
{
	pthread_rwlock_init(&m_lock, 0);
}

CRWLock::~CRWLock()
{
	pthread_rwlock_destroy(&m_lock);
}

int CRWLock::ReadLock(int nTimeout)
{
	SYNC_WAIT(m_lock, pthread_rwlock_rdlock, pthread_rwlock_timedrdlock, nTimeout)
}

int CRWLock::WriteLock(int nTimeout)
{
	SYNC_WAIT(m_lock, pthread_rwlock_wrlock, pthread_rwlock_timedwrlock, nTimeout)
}

int CRWLock::Unlock()
{
	return pthread_rwlock_unlock(&m_lock);
}
