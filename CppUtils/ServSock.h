#ifndef SERV_SOCK_H
#define SERV_SOCK_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "Sock.h"

class CServSock
{
	int m_fd;

public:
	CServSock(short port, int backlog = 10, const char* net_addr = "127.0.0.1");
	~CServSock();

	CSock Accept();
};

#endif
