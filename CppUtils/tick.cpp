#include "tick.h"

CTickCount::CTickCount()
{
	gettimeofday(&m_tInit, 0);
}

int CTickCount::TickCount()
{
	struct timeval t;
	gettimeofday(&t, 0);
	int sec = t.tv_sec - m_tInit.tv_sec;
	int usec = t.tv_usec - m_tInit.tv_usec;
	return sec * 1000000 + usec;
}

CTickCount g_tick;
int GetTickCount()
{
	return g_tick.TickCount();
}
