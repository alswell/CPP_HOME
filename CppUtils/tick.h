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
	int TickCount();
	CString StrYmdHMS();
	CString StrDateTime();
};

int GetTickCount();

