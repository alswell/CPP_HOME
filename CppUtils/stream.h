#pragma once
#include "env.h"

#define CRLF "\n"

class IStream
{
	char* m_pBuff;
	int m_nSize;
public:
	IStream();
	~IStream();


	virtual int Read(void* pBuff, unsigned nSize) = 0;
	void* ReadN(unsigned nSize);
	char* ReadLine(int nSize = 1024);

	virtual int Write(const void* pBuff, unsigned nSize) = 0;
	int WriteString(const char *str);
	int WriteLine(const char *str);

	virtual void Close() = 0;
};
