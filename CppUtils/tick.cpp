#include "tick.h"

CDateTime::CDateTime()
{
	gettimeofday(&m_tStamp, 0);
	localtime_r(&m_tStamp.tv_sec, &m_t);
}

int CDateTime::TickCount()
{
	struct timeval t;
	gettimeofday(&t, 0);
	int sec = t.tv_sec - m_tStamp.tv_sec;
	int usec = t.tv_usec - m_tStamp.tv_usec;
	return sec * 1000000 + usec;
}

CString CDateTime::StrYmdHMS()
{
	CString str;
	strftime(str.GetBuffer(64), 64, "%Y%m%d%H%M%S", &m_t);
	str.ReleaseBuffer();
	return str;
}

CString CDateTime::StrDateTime()
{
	CString str;
	strftime(str.GetBuffer(64), 64, "%Y-%m-%d %H:%M:%S", &m_t);
	str.ReleaseBuffer();
	return str;
}

int GetTickCount()
{
	static CDateTime g_tick;
	return g_tick.TickCount();
}
