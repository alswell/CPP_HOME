#include "ServSock.h"


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
