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

inline void BuildNext(const char* pattern, size_t length, unsigned int* next)
{
	unsigned int i, t;

	i = 1;
	t = 0;
	next[1] = 0;

	while(i < length + 1)
	{
		while (t > 0 && pattern[i - 1] != pattern[t - 1])
			t = next[t];

		++t;
		++i;

		if(pattern[i - 1] == pattern[t - 1])
			next[i] = next[t];
		else
			next[i] = t;
	}

	//pattern末尾的结束符控制，用于寻找目标字符串中的所有匹配结果用
	while(t > 0 && pattern[i - 1] != pattern[t - 1])
		t = next[t];

	++t;
	++i;

	next[i] = t;
}

int KMP(const char* str, const char* pattern, int begin = 0)
{
	size_t str_length = strlen(str);
	if (begin >= str_length)
		return -1;

	const char* text = &str[begin];
	size_t text_length = str_length - begin;
	size_t pattern_length = strlen(pattern);
	unsigned int i, j, n;
	unsigned int next[pattern_length + 2];

	BuildNext(pattern, pattern_length, next);

	i = 0;
	j = 1;
	n = 0;

	while (pattern_length + 1 - j <= text_length - i)
	{
		if (text[i] == pattern[j - 1])
		{
			++i;
			++j;

			//发现匹配结果，将匹配子串的位置，加入结果
			if(j == pattern_length + 1)
			{
				return i - pattern_length + begin;
				//matches[n++] = i - pattern_length;
				//j = next[j];
			}
		}
		else
		{
			j = next[j];

			if(j == 0)
			{
				++i;
				++j;
			}
		}
	}

	return -1;
	//返回发现的匹配数
	//return n;
}


char CStr::white_char[] = " \r\n\t";
CStr::CStr()
{
	m_nLength = 0;
    m_nCountInc = 1;
    m_pBuff = new char[INCREAMENT];
    memset(m_pBuff, 0, INCREAMENT);
}

CStr::CStr(char c)
{
	m_nLength = 1;
	m_nCountInc = 1;
	m_pBuff = new char[INCREAMENT];
	memset(m_pBuff, 0, INCREAMENT);
	m_pBuff[0] = c;
}

CStr::CStr(const CStr & str)
{
	m_nLength = str.m_nLength;
	m_nCountInc = str.m_nCountInc;
	m_pBuff = new char[BuffSize()];
	strcpy(m_pBuff, str.m_pBuff);
}

CStr::CStr(const char *str, int n)
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

unsigned CStr::BuffSize() const
{
	return m_nCountInc * INCREAMENT;
}

unsigned CStr::Length() const
{
	return m_nLength;
}

bool CStr::Empty() const
{
	return m_nLength == 0;
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

void CStr::operator =(char c)
{
    this->Format("%c", c);
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

bool CStr::operator ==(const char* str) const
{
	return strcmp(m_pBuff, str) == 0;
}

bool CStr::operator !=(const char* str) const
{
	return strcmp(m_pBuff, str) != 0;
}

char &CStr::operator [](int i) const
{
	if (i < 0)
		i += m_nLength;
	return m_pBuff[i];
}

bool CStr::operator <(const CStr & str) const
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

CStr & CStr::operator +=(char c)
{
	return this->AppendFormat("%c", c);
}

CStr & CStr::operator +=(const char* str)
{
	return this->AppendFormat("%s", str);
}

CStr CStr::operator +(char c)
{
	CStr new_str(*this);
	return new_str += c;
}

CStr CStr::operator +(const char *str)
{
	CStr new_str(*this);
	return new_str += str;
}

CStr & CStr::Format(const char *fmt, ...)
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

CStr & CStr::AppendFormat(const char *fmt, ...)
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

int CStr::Replace(char src, char des)
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

int CStr::Find(char c, int start) const
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

int CStr::Find(const char *str, int start)
{
	if (start < 0)
		start += m_nLength;
	return KMP(m_pBuff, str, start);
}

bool CStr::StartWith(const char *str)
{
	int i = 0;
	while (str[i])
	{
		if (i >= Length() || (*this)[i] != str[i])
			return false;
		++i;
	}
	return true;
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

void CStr::Truncate(int n)
{
	m_pBuff[n] = 0;
	m_nLength = n;
}

CStr CStr::Left(unsigned n) const
{
	return CStr(m_pBuff, n);
}

CStr CStr::Right(unsigned n) const
{
	return CStr(&m_pBuff[n], Length() - n);
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

void CStr::Split(list<CStr> &lsStr, char c, unsigned num) const
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

list<CStr> CStr::Split(char c, unsigned num) const
{
	list<CStr> lsStr;
	Split(lsStr, c, num);
	return lsStr;
}

void CStr::Split(list<CStr> &lsStr, const char *str, unsigned num) const
{
	lsStr.clear();
	int n = 0;
	int p = 0;
	while (1)
	{
		n = KMP(m_pBuff, str, n);
		if (n == -1)
			break;
		lsStr.push_back(CStr(&m_pBuff[p], n - p));
		n += strlen(str);
		p = n;
	}
	if (m_pBuff[p])
		lsStr.push_back(CStr(&m_pBuff[p]));
}

list<CStr> CStr::Split(const char *str, unsigned num) const
{
	list<CStr> lsStr;
	Split(lsStr, str, num);
	return lsStr;
}

void ToHexStr(CStr& str, const void *buff, unsigned nSize)
{
	unsigned char* p = (unsigned char*)buff;
	str = "";
	for (unsigned i = 0; i < nSize; ++i)
		str.AppendFormat("%02X", p[i]);
}

void ToHexStr(CStr& str, const CStr& buff)
{
	ToHexStr(str, buff, buff.Length());
}

