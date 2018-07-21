#pragma once

#include "env.h"
#include "stream.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SOCK_BUFF_SIZE 1024
class CSock : public IStream
{
	friend class CServSock;
	int m_fd;
	sockaddr_in addr;

public:
	CSock(int fd = -1);
	CSock(const char* net_addr, short port);

	operator bool();
	int Connect();
	void PrintReadErr(int r);
	void SetTimeout(int nSecond);

	virtual int Read(void* pBuff, unsigned nSize);
	virtual int Write(const void* pBuff, unsigned nSize);
	virtual void Close();
};

class CServSock
{
	int m_fd;

public:
	CServSock(short port, int backlog = 10, const char* net_addr = "0.0.0.0");
	~CServSock();

	CSock Accept();
};
