#include "udp.h"


UdpCli::UdpCli(short port, const char *net_addr)
{
	m_fd = socket(AF_INET, SOCK_DGRAM, 0);

	m_ad.sin_family = AF_INET;
	m_ad.sin_port = htons(port);
	if (net_addr)
	{
		m_ad.sin_addr.s_addr = inet_addr(net_addr);
	}
	else
	{
		m_ad.sin_addr.s_addr = htonl(INADDR_BROADCAST);
		int on = 1;
		setsockopt(m_fd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
	}
}

void UdpCli::SetTimeout(int n)
{
	struct timeval timeout;
	timeout.tv_sec = n;
	timeout.tv_usec = 0;
	if (setsockopt(m_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1)
		cout << "udp set timer out fail" << endl;
}

char *UdpCli::Recv(sockaddr_in& ad_snd)
{
	socklen_t len = sizeof(ad_snd);
	int r = recvfrom(m_fd, m_buff, UDP_BUFF_SIZE-1, 0, (sockaddr*)&ad_snd, &len);
	if (r == -1)
		return NULL;
	m_buff[r] = 0;
	return m_buff;
}

int UdpCli::Send(const char *str)
{
	return sendto(m_fd, str, strlen(str), 0, (sockaddr*)&m_ad, sizeof(m_ad));
}

UdpSer::UdpSer(short port, const char *net_addr)
{
	m_fd = socket(AF_INET, SOCK_DGRAM, 17);

	sockaddr_in ad;
	ad.sin_family = AF_INET;
	ad.sin_port = htons(port);
	ad.sin_addr.s_addr = inet_addr(net_addr);
	bind(m_fd, (sockaddr*)&ad, sizeof(ad));
}

char *UdpSer::Recv(sockaddr_in &ad_snd)
{
	socklen_t len = sizeof(ad_snd);
	int r = recvfrom(m_fd, m_buff, UDP_BUFF_SIZE-1, 0, (sockaddr*)&ad_snd, &len);
	if (r == -1)
		return NULL;
	m_buff[r] = 0;
	return m_buff;
}

int UdpSer::SendTo(const char *str, const char *net_addr, short port)
{
	sockaddr_in ad;
	ad.sin_family = AF_INET;
	ad.sin_port = htons(port);
	ad.sin_addr.s_addr = inet_addr(net_addr);
	return sendto(m_fd, str, strlen(str), 0, (sockaddr*)&ad, sizeof(ad));
}

int UdpSer::SendTo(const char *str, const sockaddr_in &ad_snd)
{
	return sendto(m_fd, str, strlen(str), 0, (sockaddr*)&ad_snd, sizeof(ad_snd));
}
