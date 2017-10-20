#include "str.h"

#define IS_ONE_OF(obj, arr) in(obj, arr, sizeof(arr)/sizeof(arr[0]))
#define DEF_IN(cls) \
bool in(cls c, cls arr[], int n)\
{\
	for (int i = 0; i < n; ++i)\
	{\
		if (c == arr[i])\
		{\
			return true;\
		}\
	}\
	return false;\
}

DEF_IN(char)

char CStr::white_char[] = " \r\n\t";
CStr::CStr()
{
	m_nLength = 0;
	m_pBuff = new char[INCREAMENT];
	m_nCountInc = 1;
	memset(m_pBuff, 0, BuffSize());
}

CStr::CStr(const CStr & str)
{
	m_nLength = str.m_nLength;
	m_nCountInc = str.m_nCountInc;
	m_pBuff = new char[BuffSize()];
	strcpy(m_pBuff, str.m_pBuff);
}

CStr::CStr(char str[], int n)
{
	m_nLength = n == -1 ? strlen(str) : n;
	m_nCountInc = m_nLength / INCREAMENT;
	++m_nCountInc;
	m_pBuff = new char[BuffSize()];
	memset(m_pBuff, 0, BuffSize());
	if (str)
		memcpy(m_pBuff, str, m_nLength);
}

CStr::~CStr()
{
    delete[] m_pBuff;
}

unsigned CStr::BuffSize()
{
	return m_nCountInc * INCREAMENT;
}

unsigned CStr::Length()
{
	return m_nLength;
}

char* CStr::GetBuffer(int n /*= 0*/)
{
	if (n && n >= BuffSize())
	{
		m_nCountInc = n / INCREAMENT;
		++m_nCountInc;
//		char* pTemp = m_pBuff;
//		m_pBuff = new char[BuffSize()];
//		memset(m_pBuff, 0, BuffSize());
//		memcpy(m_pBuff, pTemp, m_nLength);
//		delete[] pTemp;
		delete[] m_pBuff;
		m_pBuff = new char[BuffSize()];
	}
	memset(m_pBuff, 0, BuffSize());
	return m_pBuff;
}

void CStr::ReleaseBuffer()
{
	m_nLength = strlen(m_pBuff);
}

CStr::operator const char*() const
{
	return m_pBuff;
}

void CStr::operator=(const CStr & str)
{
	m_nLength = str.m_nLength;
	m_nCountInc = str.m_nCountInc;
	if (m_pBuff)
		delete[] m_pBuff;
	m_pBuff = new char[BuffSize()];
	strcpy(m_pBuff, str.m_pBuff);
}

bool CStr::operator ==(const CStr &str) const
{
	return strcmp(m_pBuff, str.m_pBuff);
}

char CStr::operator[](int i) const
{
	if (i < 0)
		i += m_nLength;
	return m_pBuff[i];
}

bool CStr::operator <(const CStr & str) const
{
	for (int i = 0; i < m_nLength && i < str.m_nLength; ++i)
	{
		if ((*this)[i] != str[i])
		{
			return (*this)[i] < str[i];
		}
	}
	return m_nLength < str.m_nLength;
}

CStr & CStr::operator +=(char c)
{
	return this->AppendFormat("%c", c);
}

CStr & CStr::operator +=(const char* str)
{
	return this->AppendFormat("%s", str);
}

CStr & CStr::Format(char fmt[], ...)
{
    va_list vArgList;
	do
	{
		va_start(vArgList, fmt);
		m_nLength = vsnprintf(m_pBuff, BuffSize(), fmt, vArgList);
		va_end(vArgList);
		if (m_nLength != -1 && m_nLength < BuffSize())
			break;
		delete[] m_pBuff;
		++m_nCountInc;
		m_pBuff = new char[BuffSize()];
	} while (1);
	return *this;
}

CStr & CStr::AppendFormat(char fmt[], ...)
{
	va_list vArgList;
	do
	{
		va_start(vArgList, fmt);
		int r = vsnprintf(&m_pBuff[m_nLength], BuffSize() - m_nLength, fmt, vArgList);
		va_end(vArgList);
		if (r != -1 && r < BuffSize() - m_nLength)
		{
			m_nLength += r;
			break;
		}
		char* pTemp = m_pBuff;
		++m_nCountInc;
		m_pBuff = new char[BuffSize()];
		memcpy(m_pBuff, pTemp, m_nLength);
		delete[] pTemp;
	} while (1);
	return *this;
}

int CStr::Find(char c, int start)
{
	if (start < 0)
		start += m_nLength;
	unsigned i = start;
	for (int i = start; i < m_nLength; ++i)
	{
		if (m_pBuff[i] == c)
			return i;
	}
	return -1;
}

CStr CStr::SubStr(int start, int end) const
{
	if (start < 0)
		start += m_nLength;
	if (end < 0)
		end += m_nLength;
	if (start > end)
		return CStr();
	return CStr(&m_pBuff[start], end - start);
}

CStr& CStr::TrimLeft(char c)
{
	int i = 0;
	for (; i < m_nLength; ++i)
		if (!(c ? m_pBuff[i] == c : IS_ONE_OF(m_pBuff[i], white_char)))
			break;
	if (i == 0)
		return *this;

	int j = 0;
	for (; i < m_nLength; ++i, ++j)
		m_pBuff[j] = m_pBuff[i];
	m_pBuff[j] = 0;
	m_nLength = j;
	return *this;
}

CStr& CStr::TrimRight(char c)
{
	while (m_nLength)
	{
		if (c ? m_pBuff[m_nLength - 1] == c : IS_ONE_OF(m_pBuff[m_nLength - 1], white_char))
			--m_nLength;
		else
			break;
	}
	m_pBuff[m_nLength] = 0;
	return *this;
}

CStr& CStr::Trim(char c)
{
	return TrimLeft(c).TrimRight(c);
}

list<CStr> CStr::Split(char c, unsigned num) const
{
	list<CStr> lsStr;
	int temp_i = 0;
	for (int i = 0, count = 0; i < m_nLength; ++i)
	{
		if (c ? m_pBuff[i] == c : IS_ONE_OF(m_pBuff[i], white_char))
		{
			lsStr.push_back(SubStr(temp_i, i));
			temp_i = i + 1;
			++count;
			if (count == num)
				break;
		}
	}
	lsStr.push_back(SubStr(temp_i, m_nLength));
	return lsStr;
}
