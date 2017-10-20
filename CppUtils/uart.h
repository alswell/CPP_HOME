#ifndef __UART_H
#define __UART_H

#include "afx.h"
#include <fcntl.h>
#include <termios.h>

#include "str.h"

//  CUART uart("/dev/ttySAC3", 115200, 8, 'N', 1);
//  uart.Write("hello uart!");
class CUART
{
    int m_fd;
	CStr m_buff;
public:
	CUART();
	CUART(CStr dev, int nSpeed, int nBits, char nEvent, int nStop);

	operator bool ();

	const char *Read();
	char ReadChar();

	void Write(char c) const;
	void Write(CStr str) const;
	void Write(const void* buff, unsigned nSize) const;
	void WriteHex(const void* buff, unsigned nSize) const;
private:
    int SetOpt(int fd,int nSpeed, int nBits, char nEvent, int nStop);
};

#endif
