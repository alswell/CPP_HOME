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
	CStr(const CStr& str);
	CStr(char str[], int n = -1);
	~CStr();

	inline unsigned BuffSize();
	unsigned Length();
	char* GetBuffer(int n = 0);
	void ReleaseBuffer();
	operator const char* () const;

	void operator = (const CStr& str);
	bool operator ==(const CStr& str) const;
	char operator [] (int i) const;
	bool operator < (const CStr& str) const;
	CStr& operator += (char c);
	CStr& operator += (const char* str);

	CStr& Format(char fmt[], ...);
	CStr& AppendFormat(char fmt[], ...);
	int Find(char c, int start = 0);
	CStr SubStr(int start, int end) const;
	CStr& TrimLeft(char c = 0);
	CStr& TrimRight(char c = 0);
	CStr& Trim(char c = 0);
	list<CStr> Split(char c = 0, unsigned num = 0) const;
};

#endif
