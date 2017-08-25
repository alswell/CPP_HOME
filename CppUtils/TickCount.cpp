#include "TickCount.h"

CTickCount::CTickCount()
{
	gettimeofday(&m_tLast, 0);
}

int CTickCount::TickCount()
{
	struct timeval t;
	gettimeofday(&t, 0);
	int sec = t.tv_sec - m_tLast.tv_sec;
	int usec = t.tv_usec - m_tLast.tv_usec;
	gettimeofday(&m_tLast, 0);
	return sec * 1000000 + usec;
}

CTickCount g_tick;
int GetTickCount()
{
	return g_tick.TickCount();
}
