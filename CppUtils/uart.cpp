#include "uart.h"


CUART::CUART()
{
	m_fd = -1;
}

CUART::CUART(CString dev, int nSpeed, int nBits, char nEvent, int nStop)
{
	m_fd = open(dev, O_RDWR | O_NOCTTY | O_NDELAY);
	if (m_fd != -1)
		SetOpt(m_fd, nSpeed, nBits, nEvent, nStop);
}

CUART::operator bool()
{
	return m_fd != -1;
}

int CUART::Read(void *pBuff, unsigned nSize)
{
	char* p = (char*)pBuff;
	for (int i = 0; i < nSize; ++i)
	{
		int r = 0;
		while (r == 0)
			r = read(m_fd, &p[i], 1);
		if (r < 0)
		{
			cout << "uart read error: " << r << endl;
			return -1;
		}
	}
}

int CUART::Write(const void *pBuff, unsigned nSize)
{
	return write(m_fd, pBuff, nSize);
}

void CUART::Close()
{
	if (m_fd != -1)
	{
		close(m_fd);
		m_fd = -1;
	}
}

char CUART::ReadChar()
{
	char c = -1;
	Read(&c, 1);
	return c;
}

int CUART::SetOpt(int fd, int nSpeed, int nBits, char nEvent, int nStop)
{
	struct termios newtio, oldtio;
	if (tcgetattr(fd, &oldtio) != 0)
	{
		perror("SetupSerial 1");
		return -1;
	}
	bzero(&newtio, sizeof(newtio));
	newtio.c_cflag |= CLOCAL | CREAD;
	newtio.c_cflag &= ~CSIZE;

	switch (nBits)
	{
	case 7:
		newtio.c_cflag |= CS7;
		break;
	case 8:
		newtio.c_cflag |= CS8;
		break;
	}

	switch (nEvent)
	{
	case 'O':
		newtio.c_cflag |= PARENB;
		newtio.c_cflag |= PARODD;
		newtio.c_iflag |= (INPCK | ISTRIP);
		break;
	case 'E':
		newtio.c_iflag |= (INPCK | ISTRIP);
		newtio.c_cflag |= PARENB;
		newtio.c_cflag &= ~PARODD;
		break;
	case 'N':
		newtio.c_cflag &= ~PARENB;
		break;
	}

	switch (nSpeed)
	{
	case 2400:
		cfsetispeed(&newtio, B2400);
		cfsetospeed(&newtio, B2400);
		break;
	case 4800:
		cfsetispeed(&newtio, B4800);
		cfsetospeed(&newtio, B4800);
		break;
	case 9600:
		cfsetispeed(&newtio, B9600);
		cfsetospeed(&newtio, B9600);
		break;
	case 115200:
		cfsetispeed(&newtio, B115200);
		cfsetospeed(&newtio, B115200);
		break;
	case 460800:
		cfsetispeed(&newtio, B460800);
		cfsetospeed(&newtio, B460800);
		break;
	case 921600:
		cfsetispeed(&newtio, B921600);
		cfsetospeed(&newtio, B921600);
		break;
	default:
		cfsetispeed(&newtio, B9600);
		cfsetospeed(&newtio, B9600);
		break;
	}
	if (nStop == 1)
		newtio.c_cflag &= ~CSTOPB;
	else if (nStop == 2)
		newtio.c_cflag |= CSTOPB;
	newtio.c_cc[VTIME] = 0;
	newtio.c_cc[VMIN] = 0;
	tcflush(fd, TCIFLUSH);
	if ((tcsetattr(fd, TCSANOW, &newtio)) != 0)
	{
		perror("com set error");
		return -1;
	}
	//	printf("set done!\n\r");
	return 0;
}

//void CUART::WriteHex(const void *buff, unsigned nSize) const
//{
//	CString str;
//	ToHexStr(str, buff, nSize);

//	for (int i = 0; i < 10; ++i)
//		Write("\0");
//	Write("\n");
//	Write(str);
//	Write("\n");
//}

