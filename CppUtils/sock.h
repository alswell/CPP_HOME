#ifndef __SOCK_H
#define __SOCK_H

#include "afx.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SOCK_BUFF_SIZE 1024
class CSock
{
	friend class CServSock;
	char m_buff[SOCK_BUFF_SIZE];
	int m_fd;

public:
	CSock(int fd = -1);
	CSock(const char* net_addr, short port);
	~CSock();

	void PrintReadErr(int r);

	char* Read();
	int Read(void* buff, int n);
	int Write(const void* buff, int n);
};

class CServSock
{
	int m_fd;

public:
	CServSock(short port, int backlog = 10, const char* net_addr = "0.0.0.0");
	~CServSock();

	CSock Accept();
};

#endif
