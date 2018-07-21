#pragma once
#include "stream.h"
#include <fcntl.h>
#include <termios.h>

#include "str.h"

//  CUART uart("/dev/ttySAC3", 115200, 8, 'N', 1);
//  uart.Write("hello uart!");
class CUART : public IStream
{
    int m_fd;
public:
	CUART();
	CUART(CStr dev, int nSpeed, int nBits, char nEvent, int nStop);

	operator bool ();

	virtual int Read(void* pBuff, unsigned nSize);
	virtual int Write(const void* pBuff, unsigned nSize);
	virtual void Close();

	char ReadChar();
private:
    int SetOpt(int fd,int nSpeed, int nBits, char nEvent, int nStop);
};

