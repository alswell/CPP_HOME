#pragma once
#include "stream.h"
#include "str.h"

class CFile : public IStream
{
	FILE* m_pFile;
public:
	CFile(const char* strFileName, const char *mode = "ab+");

	virtual int Read(void *pBuff, unsigned nSize);
	virtual int Write(const void *pBuff, unsigned long nSize);
	virtual void Close();
	list<CString> ReadLines();
};
