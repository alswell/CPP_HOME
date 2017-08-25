#include <sys/time.h>

class CTickCount
{
	struct timeval m_tLast;
public:
	CTickCount();
	int TickCount();
};

int GetTickCount();
