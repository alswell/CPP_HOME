#include "Sock.h"
#include <stdio.h>


CSock::CSock(int fd)
{
	m_fd = fd;
}

CSock::CSock(const char * net_addr, short port)
{	
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

void CSock::PrintReadErr(int r)
{
	switch (r) {
	case 0:
		printf("client close!\n");
		break;
	case -1:
		printf("net error!\n");
		break;
	default:
		printf("recv err: %d\n", r);
		break;
	}
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
	int r = recv(m_fd, buff, n, 0);
	if (r <= 0) 
	{
		PrintReadErr(r);
		close(m_fd);
	}

	return r;
}

int CSock::Write(const void* buff, int n)
{
	return send(m_fd, buff, n, 0);
}
