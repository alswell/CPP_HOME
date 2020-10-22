#pragma once
#include "stream.h"
#include "../str.h"

class CFile : public IStream
{
	FILE* m_pFile;
public:
	CFile(const char* strFileName, const char *mode = "ab+");

	virtual unsigned TotalSize() override;
	virtual int Read(void *pBuff, unsigned nSize) override;
	virtual int Write(const void *pBuff, unsigned long nSize) override;
	virtual void Close() override;
	list<CString> ReadLines();
};
