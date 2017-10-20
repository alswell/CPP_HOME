#include "thread.h"
#include "sock.h"
#include "tick.h"
#include <signal.h>


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
			sleep(1);
			int r = m_sock.Write("hello", 5);
			if (r != -1)
				printf("thread ID: %lu: %d; tick count: %d\n", m_tid, r, GetTickCount());
			else
				return 0;
				
		}
	}
};

int main()
{
	signal(SIGPIPE, SIG_IGN);
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
