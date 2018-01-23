#ifndef __STR_H
#define __STR_H

#include "afx.h"

class CStr
{
	static char white_char[];
	static const unsigned INCREAMENT = 1024;
	char* m_pBuff;
	unsigned m_nCountInc;
	int m_nLength;
public:
	CStr();
    CStr(char c);
    CStr(const CStr& str);
	CStr(const char* str, int n = -1);
	~CStr();

	inline unsigned BuffSize() const;
	unsigned Length() const;
	bool Empty() const;
	char* GetBuffer(int n = 0);
	void ReleaseBuffer();
	operator const char* () const;

    void operator = (char c);
    void operator = (const CStr& str);
	bool operator ==(const char *str) const;
	bool operator !=(const char *str) const;
	char& operator [] (int i) const;
	bool operator < (const CStr& str) const;
	CStr& operator += (char c);
	CStr& operator += (const char* str);
	CStr operator +(char c);
	CStr operator + (const char *str);

	CStr& Format(const char* fmt, ...);
    CStr& AppendFormat(const char* fmt, ...);
	int Replace(char src, char des);
	int Find(char c, int start = 0) const;
	int Find(const char *str, int start = 0);
	bool StartWith(const char* str);
	CStr SubStr(int start, int end) const;
	void Truncate(int n);
	CStr Left(unsigned n) const;
	CStr Right(unsigned n) const;
	CStr& TrimLeft(char c = 0);
	CStr& TrimRight(char c = 0);
	CStr& Trim(char c = 0);
	void Split(list<CStr>& lsStr, char c = 0, unsigned num = 0) const;
	list<CStr> Split(char c = 0, unsigned num = 0) const;
	void Split(list<CStr>& lsStr, const char *str, unsigned num = 0) const;
	list<CStr> Split(const char *str, unsigned num = 0) const;
};

void ToHexStr(CStr& str, const void *buff, unsigned nSize);
void ToHexStr(CStr& str, const CStr& buff);

#endif
