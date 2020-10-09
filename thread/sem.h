#pragma once
#include <semaphore.h>

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
