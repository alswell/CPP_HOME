#pragma once

#include "env.h"

class CString
{
	static char white_char[];
	static const unsigned INCREAMENT = 1024;
	char* m_pBuff;
public:
	CString();
	CString(char c);
	CString(const CString& str);
	CString(const char* str, int n = -1);
	~CString();

	unsigned GetLength() const;
	bool Empty() const;
	char* GetBuffer(int n = 0);
	void ReleaseBuffer(int n = -1);
	operator const char* () const;

	void operator = (char c);
	void operator = (const CString& str);
	bool operator ==(const char *str) const;
	bool operator !=(const char *str) const;
	char& operator [] (int i) const;
	bool operator < (const CString& str) const;
	CString& operator += (char c);
	CString& operator += (const char* str);
	CString operator +(char c);
	CString operator + (const char *str);

	CString& Format(const char* fmt, ...);
	CString& AppendFormat(const char* fmt, ...);
	int Replace(char src, char des);
	int Find(char c, int start = 0) const;
	int Find(const char *str, int start = 0);
	int ReverseFind(char c, int start = -1) const;
	bool StartsWith(const char* str);
	CString Mid(int start, int length = -1) const;
	CString SubStr(int start, int end) const;
	CString Left(unsigned n) const;
	CString Right(unsigned n) const;
	CString& TrimLeft(char c = 0);
	CString& TrimRight(char c = 0);
	CString& Trim(char c = 0);
	void Split(list<CString>& lsStr, char c = 0, unsigned num = 0) const;
	list<CString> Split(char c = 0, unsigned num = 0) const;
	void Split(list<CString>& lsStr, const char *str, unsigned num = 0) const;
	list<CString> Split(const char *str, unsigned num = 0) const;
};

void ToHexStr(CString& str, const void *buff, unsigned nSize);
void ToHexStr(CString& str, const CString& buff);

