#pragma once
#include <sys/time.h>
#include <time.h>
#include "str.h"

class CDateTime
{
	struct tm m_t;
	struct timeval m_tStamp;
public:
	CDateTime();
	CDateTime(long sec, long usec = 0);
	CDateTime(int y, int m, int d, int H, int M, int S);
	long operator-(const CDateTime &dt);
	CString StrFmt(const char* fmt);
	long Stamp();
	long UStamp();
	CString StrYmd();
	CString StrHMS();
	CString StrYmdHMS();
	CString StrDateTime();
};

long GetTickCount();

#define TICK_HELPER(info) CTickHelper tmp_tick_helper(info)
class CTickHelper
{
	CDateTime m_tick;
	const char* m_info;
public:
	CTickHelper(const char* info);
	~CTickHelper();
};
