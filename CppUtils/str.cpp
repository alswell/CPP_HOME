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

int KMP(const char* str, const char* pattern, int begin = 0)
{
	// fake KMP algorithm
	for (int i = 0; str[i]; ++i)
	{
		if (str[i] == pattern[0])
		{
			const char *p1 = pattern, *p2 = &str[i];
			for (; *p1 != 0 && *p1 == *p2; ++p1, ++p2);
			if (*p1 == 0)
				return i;
		}
	}
	return -1;
}


char CString::white_char[] = " \r\n\t";
CString::CString()
{
	m_nLength = 0;
	m_nCountInc = 1;
	m_pBuff = new char[INCREAMENT];
	memset(m_pBuff, 0, INCREAMENT);
}

CString::CString(char c)
{
	m_nLength = 1;
	m_nCountInc = 1;
	m_pBuff = new char[INCREAMENT];
	memset(m_pBuff, 0, INCREAMENT);
	m_pBuff[0] = c;
}

CString::CString(const CString & str)
{
	m_nLength = str.m_nLength;
	m_nCountInc = str.m_nCountInc;
	m_pBuff = new char[BuffSize()];
	strcpy(m_pBuff, str.m_pBuff);
}

CString::CString(const char *str, int n)
{
	m_nLength = n == -1 ? strlen(str) : n;
	m_nCountInc = m_nLength / INCREAMENT;
	++m_nCountInc;
	m_pBuff = new char[BuffSize()];
	memset(m_pBuff, 0, BuffSize());
	if (str)
		memcpy(m_pBuff, str, m_nLength);
}

CString::~CString()
{
	delete[] m_pBuff;
}

unsigned CString::BuffSize() const
{
	return m_nCountInc * INCREAMENT;
}

unsigned CString::GetLength() const
{
	return m_nLength;
}

bool CString::Empty() const
{
	return m_nLength == 0;
}

char* CString::GetBuffer(int n /*= 0*/)
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

void CString::ReleaseBuffer()
{
	m_nLength = strlen(m_pBuff);
}

void CString::operator =(char c)
{
	this->Format("%c", c);
}

CString::operator const char*() const
{
	return m_pBuff;
}

void CString::operator=(const CString & str)
{
	m_nLength = str.m_nLength;
	m_nCountInc = str.m_nCountInc;
	if (m_pBuff)
		delete[] m_pBuff;
	m_pBuff = new char[BuffSize()];
	strcpy(m_pBuff, str.m_pBuff);
}

bool CString::operator ==(const char* str) const
{
	return strcmp(m_pBuff, str) == 0;
}

bool CString::operator !=(const char* str) const
{
	return strcmp(m_pBuff, str) != 0;
}

char &CString::operator [](int i) const
{
	if (i < 0)
		i += m_nLength;
	return m_pBuff[i];
}

bool CString::operator <(const CString & str) const
{
	return strcmp(m_pBuff, str) < 0;
	for (int i = 0; i < m_nLength && i < str.m_nLength; ++i)
	{
		if ((*this)[i] != str[i])
		{
			return (*this)[i] < str[i];
		}
	}
	return m_nLength < str.m_nLength;
}

CString & CString::operator +=(char c)
{
	return this->AppendFormat("%c", c);
}

CString & CString::operator +=(const char* str)
{
	return this->AppendFormat("%s", str);
}

CString CString::operator +(char c)
{
	CString new_str(*this);
	return new_str += c;
}

CString CString::operator +(const char *str)
{
	CString new_str(*this);
	return new_str += str;
}

CString & CString::Format(const char *fmt, ...)
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

CString & CString::AppendFormat(const char *fmt, ...)
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

int CString::Replace(char src, char des)
{
	int nCount = 0;
	for (int i = 0; i < m_nLength; ++i)
	{
		if (m_pBuff[i] == src)
		{
			m_pBuff[i] = des;
			++nCount;
		}
	}
	return nCount;
}

int CString::Find(char c, int start) const
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

int CString::Find(const char *str, int start)
{
	if (start < 0)
		start += m_nLength;
	return KMP(m_pBuff, str, start);
}

int CString::ReverseFind(char c, int start) const
{
	for (int i = m_nLength - 1; i >= 0; --i)
	{
		if (m_pBuff[i] == c)
			return i;
	}
	return -1;
}

bool CString::StartWith(const char *str)
{
	int i = 0;
	while (str[i])
	{
		if (i >= GetLength() || (*this)[i] != str[i])
			return false;
		++i;
	}
	return true;
}

CString CString::Mid(int start, int length) const
{
	if (start < 0)
		start += m_nLength;
	if (length == -1)
		length = m_nLength - start;
	return CString(&m_pBuff[start], length);
}

CString CString::SubStr(int start, int end) const
{
	if (start < 0)
		start += m_nLength;
	if (end < 0)
		end += m_nLength;
	if (start > end)
		return CString();
	return CString(&m_pBuff[start], end - start);
}

void CString::Truncate(int n)
{
	m_pBuff[n] = 0;
	m_nLength = n;
}

CString CString::Left(unsigned n) const
{
	return CString(m_pBuff, n);
}

CString CString::Right(unsigned n) const
{
	return CString(&m_pBuff[n], GetLength() - n);
}

CString& CString::TrimLeft(char c)
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

CString& CString::TrimRight(char c)
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

CString& CString::Trim(char c)
{
	return TrimLeft(c).TrimRight(c);
}

void CString::Split(list<CString> &lsStr, char c, unsigned num) const
{
	lsStr.clear();
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
}

list<CString> CString::Split(char c, unsigned num) const
{
	list<CString> lsStr;
	Split(lsStr, c, num);
	return lsStr;
}

void CString::Split(list<CString> &lsStr, const char *str, unsigned num) const
{
	lsStr.clear();
	int n = 0;
	int p = 0;
	while (1)
	{
		n = KMP(m_pBuff, str, n);
		if (n == -1)
			break;
		lsStr.push_back(CString(&m_pBuff[p], n - p));
		n += strlen(str);
		p = n;
	}
	if (m_pBuff[p])
		lsStr.push_back(CString(&m_pBuff[p]));
}

list<CString> CString::Split(const char *str, unsigned num) const
{
	list<CString> lsStr;
	Split(lsStr, str, num);
	return lsStr;
}

void ToHexStr(CString& str, const void *buff, unsigned nSize)
{
	unsigned char* p = (unsigned char*)buff;
	str = "";
	for (unsigned i = 0; i < nSize; ++i)
		str.AppendFormat("%02X", p[i]);
}

void ToHexStr(CString& str, const CString& buff)
{
	ToHexStr(str, buff, buff.GetLength());
}

