#ifndef __TICK_COUNT_H
#define __TICK_COUNT_H

#include <sys/time.h>

class CTickCount
{
	struct timeval m_tLast;
public:
	CTickCount();
	int TickCount();
};

int GetTickCount();

#endif
