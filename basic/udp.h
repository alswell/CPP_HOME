#pragma once

#include "env.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define UDP_BUFF_SIZE 1024
class UdpCli
{
	int m_fd;
	sockaddr_in m_ad;
	char m_buff[UDP_BUFF_SIZE];
public:
	UdpCli(short port, const char* net_addr = NULL);

	void SetTimeout(int n);

	char *Recv(sockaddr_in &ad_snd);
	int Send(const char* str);
};

class UdpSer
{
	int m_fd;
	char m_buff[UDP_BUFF_SIZE];
public:
	UdpSer(short port, const char* net_addr = "0.0.0.0");

	char *Recv(sockaddr_in &ad_snd);
	int SendTo(const char* str, const char *net_addr, short port);
	int SendTo(const char* str, const sockaddr_in &ad_snd);
};

