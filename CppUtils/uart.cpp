#include "uart.h"


CUART::CUART()
{
	m_fd = -1;
}

CUART::CUART(CStr dev, int nSpeed, int nBits, char nEvent, int nStop)
{
	m_fd = open(dev, O_RDWR | O_NOCTTY | O_NDELAY);
	if (m_fd != -1)
		SetOpt(m_fd, nSpeed, nBits, nEvent, nStop);
}

CUART::operator bool()
{
	return m_fd != -1;
}

const char* CUART::Read()
{
	m_buff = "";
	while (1)
	{
		int r;
		char c;
		do
		{
			r = read(m_fd, &c, 1);
		} while (r == 0);
		if (c == '\n')
			c = 0;

		switch (r) {
		case 1:
			m_buff += c;
			break;
		case -1:
			cout<<"uart read error"<<endl;
			return NULL;
		default:
			cout << "uart r = " << r << endl;
			break;
		}

		if (c == 0)
			break;
	}
	return m_buff;
}

char CUART::ReadChar()
{
	char c = 0;
	int r;
	do
	{
		r = read(m_fd, &c, 1);
	} while (r == 0);

	if (r != 1)
		cout << "uart r = " << r << endl;

	return c;
}

void CUART::Write(char c) const
{
	write(m_fd, &c, 1);
}

void CUART::Write(CStr str) const
{
	write(m_fd, str, str.Length());
}

void CUART::Write(const void *buff, unsigned nSize) const
{
	write(m_fd, buff, nSize);
}

void CUART::WriteHex(const void *buff, unsigned nSize) const
{
	CStr str;
	ToHexStr(str, buff, nSize);

	for (int i = 0; i < 10; ++i)
		Write('\0');
	Write('\n');
	Write(str);
	Write('\n');
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
