#pragma once
#include <sys/time.h>

class CTickCount
{
	struct timeval m_tInit;
public:
	CTickCount();
	int TickCount();
};

int GetTickCount();

