#include "datetime.h"

CDateTime::CDateTime()
{
	gettimeofday(&m_tStamp, nullptr);
	localtime_r(&m_tStamp.tv_sec, &m_t);
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
