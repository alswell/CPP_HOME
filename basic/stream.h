#pragma once
#include "env.h"

#define CRLF "\n"

class IStream
{
	char* m_pBuff;
	unsigned m_nSize;
public:
	IStream();
	virtual ~IStream();


	virtual int Read(void* pBuff, unsigned nSize) = 0;
	char *ReadN(unsigned nSize);
	char* ReadLine(unsigned nSize = 1024);

	virtual int Write(const void* pBuff, unsigned long nSize) = 0;
	int WriteString(const char *str);
	int WriteLine(const char *str);

	virtual void Close() = 0;
};
