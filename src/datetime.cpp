#include "datetime.h"

CDateTime::CDateTime()
{
	gettimeofday(&m_tStamp, nullptr);
	localtime_r(&m_tStamp.tv_sec, &m_t);
}

CDateTime::CDateTime(long sec, long usec)
{
	m_tStamp.tv_sec = sec;
	m_tStamp.tv_usec = usec;
	localtime_r(&m_tStamp.tv_sec, &m_t);
}

CDateTime::CDateTime(int y, int m, int d, int H, int M, int S)
{
	m_t.tm_year = y - 1900;
	m_t.tm_mon = m - 1;
	m_t.tm_mday = d;
	m_t.tm_hour = H;
	m_t.tm_min = M;
	m_t.tm_sec = S;
	m_tStamp.tv_sec = mktime(&m_t);
	m_tStamp.tv_usec = 0;
}

long CDateTime::TickCount(const CDateTime& dt)
{
	long sec = dt.m_tStamp.tv_sec - m_tStamp.tv_sec;
	long usec = dt.m_tStamp.tv_usec - m_tStamp.tv_usec;
	return sec * 1000000 + usec;
}

#define FMT_TIME(fmt) \
	CString str;\
	strftime(str.GetBuffer(64), 64, fmt, &m_t);\
	str.ReleaseBuffer();\
	return str

CString CDateTime::StrFmt(const char* fmt)
{
	FMT_TIME(fmt);
}

long CDateTime::Stamp()
{
	return m_tStamp.tv_sec;
}

long CDateTime::UStamp()
{
	return m_tStamp.tv_sec * 1000000 + m_tStamp.tv_usec;
}

CString CDateTime::StrYmd()
{
	FMT_TIME("%Y%m%d");
}

CString CDateTime::StrHMS()
{
	FMT_TIME("%H%M%S");
}

CString CDateTime::StrYmdHMS()
{
	FMT_TIME("%Y%m%d%H%M%S");
}

CString CDateTime::StrDateTime()
{
	FMT_TIME("%Y-%m-%d %H:%M:%S");
}

long GetTickCount()
{
	static CDateTime g_tick;
	CDateTime dt;
	return g_tick.TickCount(dt);
}

CTickHelper::CTickHelper(const char *info)
	: m_info(info)
{

}

CTickHelper::~CTickHelper()
{
	CDateTime dt;
	long us = m_tick.TickCount(dt);
	cout << m_info << "{time cost}: " << us << endl;
}
