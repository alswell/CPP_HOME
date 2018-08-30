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
	for (int i = begin; str[i]; ++i)
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

class CStrMgr
{
	int m_nBufLen;
	int m_nStrLen;
public:
	char* String()
	{
		return (char*)(this + 1);
	}
	static CStrMgr* GetMgr(char* p)
	{
		return (CStrMgr*)p - 1;
	}
	void Delete()
	{
		free(this);
	}
	static CStrMgr* New(int n)
	{
		CStrMgr* p = (CStrMgr*)malloc(sizeof(CStrMgr) + n);
		p->m_nBufLen = n;
		memset(p->String(), 0, n);
		p->m_nStrLen = 0;
		return p;
	}
	CStrMgr* Renew(int n)
	{
		if (n > m_nBufLen)
		{
			Delete();
			return New(n);
		}
		return this;
	}
	CStrMgr* Append(unsigned n)
	{
		CStrMgr* tmp = New(m_nBufLen + n);
		strcpy(tmp->String(), String());
		tmp->m_nStrLen = m_nStrLen;
		Delete();
		return tmp;
	}
	void SetStrLen(int n = -1)
	{
		if (n >= m_nBufLen)
			n = m_nBufLen - 1;
		if (n < 0)
			n = strlen(String());
		String()[n] = 0;
		m_nStrLen = n;
	}
	inline int GetStrLen()
	{
		return m_nStrLen;
	}
	int BuffSize()
	{
		return m_nBufLen;
	}
};


char CString::white_char[] = " \r\n\t";
CString::CString()
{
	m_pBuff = CStrMgr::New(INCREAMENT)->String();
}

CString::CString(char c)
{
	m_pBuff = CStrMgr::New(INCREAMENT)->String();
	m_pBuff[0] = c;
}

CString::CString(const CString & str)
{
	int n = str.GetLength();
	m_pBuff = CStrMgr::New(n + 1)->String();
	strcpy(m_pBuff, str.m_pBuff);
	ReleaseBuffer(n);
}

CString::CString(const char *str, int n)
{
	int len = strlen(str);
	if (n == -1 || n > len)
		n = len;
	m_pBuff = CStrMgr::New(n + 1)->String();
	if (str)
		memcpy(m_pBuff, str, n);
	ReleaseBuffer(n);
}

CString::~CString()
{
	CStrMgr::GetMgr(m_pBuff)->Delete();
}

unsigned CString::GetLength() const
{
	return CStrMgr::GetMgr(m_pBuff)->GetStrLen();
}

bool CString::Empty() const
{
	return GetLength() == 0;
}

char* CString::GetBuffer(int n /*= 0*/)
{
	m_pBuff = CStrMgr::GetMgr(m_pBuff)->Renew(n)->String();
	return m_pBuff;
}

void CString::ReleaseBuffer(int n)
{
	CStrMgr::GetMgr(m_pBuff)->SetStrLen(n);
}

void CString::operator =(char c)
{
	Format("%c", c);
}

CString::operator const char*() const
{
	return m_pBuff;
}

void CString::operator=(const CString & str)
{
	int n = str.GetLength();
	m_pBuff = CStrMgr::GetMgr(m_pBuff)->Renew(n + 1)->String();
	strcpy(m_pBuff, str.m_pBuff);
	ReleaseBuffer(n);
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
		i += GetLength();
	return m_pBuff[i];
}

bool CString::operator <(const CString & str) const
{
	return strcmp(m_pBuff, str) < 0;
	for (int i = 0; i < GetLength() && i < str.GetLength(); ++i)
	{
		if ((*this)[i] != str[i])
		{
			return (*this)[i] < str[i];
		}
	}
	return GetLength() < str.GetLength();
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
		int n = vsnprintf(m_pBuff, CStrMgr::GetMgr(m_pBuff)->BuffSize(), fmt, vArgList);
		va_end(vArgList);
		if (n != -1 && n < CStrMgr::GetMgr(m_pBuff)->BuffSize())
		{
			ReleaseBuffer(n);
			break;
		}
		m_pBuff = CStrMgr::GetMgr(m_pBuff)->Append(INCREAMENT)->String();
	} while (1);
	return *this;
}

CString & CString::AppendFormat(const char *fmt, ...)
{
	va_list vArgList;
	do
	{
		va_start(vArgList, fmt);
		int len = GetLength();
		int r = vsnprintf(&m_pBuff[len], CStrMgr::GetMgr(m_pBuff)->BuffSize() - len, fmt, vArgList);
		va_end(vArgList);
		if (r != -1 && r < CStrMgr::GetMgr(m_pBuff)->BuffSize() - len)
		{
			ReleaseBuffer(len + r);
			break;
		}
		m_pBuff = CStrMgr::GetMgr(m_pBuff)->Append(INCREAMENT)->String();
	} while (1);
	return *this;
}

int CString::Replace(char src, char des)
{
	int nCount = 0;
	for (int i = 0; i < GetLength(); ++i)
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
		start += GetLength();
	unsigned i = start;
	for (int i = start; i < GetLength(); ++i)
	{
		if (m_pBuff[i] == c)
			return i;
	}
	return -1;
}

int CString::Find(const char *str, int start)
{
	if (start < 0)
		start += GetLength();
	return KMP(m_pBuff, str, start);
}

int CString::ReverseFind(char c, int start) const
{
	if (start > 0)
		start = -start;
	for (int i = GetLength() + start; i >= 0; --i)
	{
		if (m_pBuff[i] == c)
			return i;
	}
	return -1;
}

bool CString::StartsWith(const char *str)
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
		start += GetLength();
	return CString(&m_pBuff[start], length);
}

CString CString::SubStr(int start, int end) const
{
	if (start < 0)
		start += GetLength();
	if (end < 0)
		end += GetLength();
	if (start > end)
		return CString();
	return CString(&m_pBuff[start], end - start);
}

CString CString::Left(unsigned n) const
{
	return CString(m_pBuff, n);
}

CString CString::Right(unsigned n) const
{
	return CString(&m_pBuff[GetLength() - n], n);
}

CString& CString::TrimLeft(char c)
{
	int i = 0;
	for (; i < GetLength(); ++i)
		if (!(c ? m_pBuff[i] == c : IS_ONE_OF(m_pBuff[i], white_char)))
			break;
	if (i == 0)
		return *this;

	int j = 0;
	for (; i < GetLength(); ++i, ++j)
		m_pBuff[j] = m_pBuff[i];
	ReleaseBuffer(j);
	return *this;
}

CString& CString::TrimRight(char c)
{
	int n = GetLength();
	while (n)
	{
		--n;
		if (c ? m_pBuff[n] != c : !IS_ONE_OF(m_pBuff[n], white_char))
			break;
	}
	ReleaseBuffer(n + 1);
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
	for (int i = 0, count = 0; i < GetLength(); ++i)
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
	lsStr.push_back(SubStr(temp_i, GetLength()));
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

