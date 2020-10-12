#include "sem.h"
#include "wait.h"

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
