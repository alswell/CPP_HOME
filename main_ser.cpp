#include "Thread.h"
#include "ServSock.h"
#include "Sock.h"
#include "TickCount.h"

class CWorker : public CThreadImpl<CWorker>
{
	CSock m_sock;
public:
	CWorker(const CSock& sock)
	{
		m_sock = sock;
	}

	void* Run()
	{
		while (1)
		{
			char* p = m_sock.Read();
			if (p)
				printf("thread ID: %lu: %s; tick count: %d\n", m_tid, p, GetTickCount());
			else
				return 0;
		}
	}
};

int main()
{
	CServSock server(8866);
	printf("Server start!\n");
	while (1)
	{
		CWorker worker(server.Accept());
		worker.Start();
		printf("Accept!\n");
	}
	return 0;
}
