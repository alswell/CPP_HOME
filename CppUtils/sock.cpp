#include "sock.h"
#include <signal.h>


CSock::CSock(int fd)
{
	IgnoreSig();
	m_fd = fd;
}

CSock::CSock(const char * net_addr, short port)
{	
	IgnoreSig();
	m_fd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(net_addr);
	int r = connect(m_fd, (struct sockaddr*)&addr, sizeof(addr));
}


CSock::~CSock()
{
}

void CSock::IgnoreSig()
{
	static bool bIgn = true;
	if (bIgn)
	{
		bIgn = false;
		signal(SIGPIPE, SIG_IGN);
		cout << "ignore SIGPIPE" << endl;
	}
}

void CSock::PrintReadErr(int r)
{
	switch (r) {
	case 0:
		printf("remote close!\n");
		break;
	case -1:
		printf("net error!\n");
		break;
	default:
		printf("recv err: %d\n", r);
		break;
	}
}

void CSock::SetTimeout(int nSecond)
{
	struct timeval timeout={nSecond, 0};
	setsockopt(m_fd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
}

char * CSock::Read()
{
	int r = recv(m_fd, m_buff, SOCK_BUFF_SIZE - 1, 0);
	if (r <= 0) 
	{
		PrintReadErr(r);
		close(m_fd);
		return 0;
	}

	m_buff[r] = 0;
	return m_buff;
}

int CSock::Read(void * buff, int n)
{
	int r = recv(m_fd, buff, n, MSG_WAITALL);
	if (r <= 0) 
	{
		PrintReadErr(r);
		close(m_fd);
	}

	return r;
}

int CSock::Write(const char *str)
{
	int r = send(m_fd, str, strlen(str), 0);
	if (r == -1)
	{
		printf("write error, close!\n");
		close(m_fd);
	}
	return r;
}

int CSock::Write(const void* buff, int n)
{
	int r = send(m_fd, buff, n, 0);
	if (r == -1)
	{
		printf("write error, close!\n");
		close(m_fd);
	}
	return r;
}

CServSock::CServSock(short port, int backlog, const char* net_addr)
{
	int r;
	m_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (m_fd == -1) printf("1:%m\n"), exit(-1);
	printf("socket ok!\n");

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	inet_aton(net_addr, &addr.sin_addr);

	r = bind(m_fd, (struct sockaddr*)&addr, sizeof(addr));
	if (r == -1) printf("2:%m\n"), close(m_fd), exit(-1);
	printf("bind ok!\n");

	r = listen(m_fd, backlog);
	if (r == -1) printf("3:%m\n"), close(m_fd), exit(-1);
	printf("listen ok!\n");
}

CServSock::~CServSock()
{
}

CSock CServSock::Accept()
{
	int fd = accept(m_fd, 0, 0);
	return CSock(fd);
}
