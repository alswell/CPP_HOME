#include "sock.h"
#include <signal.h>

struct InitSock {
	InitSock() {
		signal(SIGPIPE, SIG_IGN);
		cout << "ignore SIGPIPE" << endl;
	}
};
InitSock init_sock;

CSock::CSock()
{
	m_fd = socket(AF_INET, SOCK_STREAM, 0);
	m_nRecvFlag = 0;

	addr.sin_family = AF_INET;
}

CSock::CSock(int fd)
{
	m_fd = fd;
	m_nRecvFlag = 0;
}

CSock::operator bool()
{
	return m_fd != -1;
}

int CSock::Connect(const char * net_addr, short port)
{
	if (net_addr)
		addr.sin_addr.s_addr = inet_addr(net_addr);
	if (port)
		addr.sin_port = htons(port);

	int r = connect(m_fd, (struct sockaddr*)&addr, sizeof(addr));
	if (r)
		perror("Connect Failed:");
	return r;
}

void CSock::PrintReadErr(int r)
{
	char err_msg[1024];
	char *p;
	switch (r) {
	case 0:
		p = "remote close";
		break;
	case -1:
		p = "net error";
		break;
	default:
		p = "recv err";
		break;
	}
	sprintf(err_msg, "%s(%d)[%s:%u]", p, r, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
	perror(err_msg);
}

void CSock::SetTimeout(int nSecond)
{
	struct timeval timeout={nSecond, 0};
	setsockopt(m_fd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
}

void CSock::SetRecvFlag(int flag)
{
	m_nRecvFlag = flag;
}

void CSock::SetWaitAll()
{
	m_nRecvFlag = MSG_WAITALL;
}

int CSock::Read(void *pBuff, unsigned nSize)
{
	if (nSize == 0)
		return 0;
	int r = recv(m_fd, pBuff, nSize, m_nRecvFlag);
	if (r <= 0)
		PrintReadErr(r);

	return r;
}

int CSock::Write(const void *pBuff, unsigned nSize)
{
	if (nSize == 0)
		return 0;
	int r = send(m_fd, pBuff, nSize, 0);
	if (r == -1)
		perror("write error, close");

	return r;
}

void CSock::Close()
{
	if (m_fd != -1)
	{
		close(m_fd);
		m_fd = -1;
	}
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
