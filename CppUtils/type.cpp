#include "type.h"


CSmartType NONE;
CSmartType::ISmartType::~ISmartType()
{
}

CSmartType::SmtBool::SmtBool(bool b)
	: m_value(b)
{

}

CSmartType::ISmartType *CSmartType::SmtBool::Copy()
{
	return new SmtBool(m_value);
}

int CSmartType::SmtBool::ToStr(char *str, int n)
{
	return snprintf(str, n, m_value ? "true" : "false");
}

CSmartType::SmtBool::operator double()
{
	return m_value;
}

CSmartType::SmtBool::operator int()
{
	return m_value;
}

CSmartType::SmtBool::operator bool()
{
	return m_value;
}

CSmartType::SmtInt::SmtInt(ssize_t i)
	: m_value(i)
{
}

CSmartType::ISmartType *CSmartType::SmtInt::Copy()
{
	return new SmtInt(m_value);
}

int CSmartType::SmtInt::ToStr(char *str, int n)
{
	return snprintf(str, n, "%d", m_value);
}

CSmartType::SmtInt::operator double()
{
	return m_value;
}

CSmartType::SmtInt::operator int()
{
	return m_value;
}

CSmartType::SmtInt::operator bool()
{
	return m_value;
}

CSmartType::SmtFloat::SmtFloat(double d)
	: m_value(d)
{
}

CSmartType::ISmartType *CSmartType::SmtFloat::Copy()
{
	return new SmtFloat(m_value);
}

int CSmartType::SmtFloat::ToStr(char *str, int n)
{
	return snprintf(str, n, "%f", m_value);
}

CSmartType::SmtFloat::operator double()
{
	return m_value;
}

CSmartType::SmtFloat::operator int()
{
	return m_value;
}

CSmartType::SmtFloat::operator bool()
{
	return m_value;
}

CSmartType::SmtStr::SmtStr(const char *str)
{
	m_value = new char[strlen(str) + 1];
	strcpy(m_value, str);
}

CSmartType::SmtStr::~SmtStr()
{
	delete m_value;
}

CSmartType::SmtStr &CSmartType::SmtStr::operator =(const SmtStr &str)
{
	m_value = new char[strlen(str.m_value) + 1];
	strcpy(m_value, str.m_value);
	return *this;
}

CSmartType::ISmartType *CSmartType::SmtStr::Copy()
{
	return new SmtStr(m_value);
}

int CSmartType::SmtStr::ToStr(char *str, int n)
{
	return snprintf(str, n, "\"%s\"", m_value);
}

CSmartType::SmtStr::operator double()
{
	return atof(m_value);
}

CSmartType::SmtStr::operator int()
{
	return atoi(m_value);
}

CSmartType::SmtStr::operator bool()
{
	return strlen(m_value);
}

CSmartType &CSmartType::SmtList::operator [](size_t n)
{
	return m_value[n];
}

CSmartType::ISmartType *CSmartType::SmtList::Copy()
{
	SmtList* p = new SmtList;
	p->m_value = m_value;
	return p;
}

int CSmartType::SmtList::ToStr(char *str, int n)
{
	if (m_value.size() == 0)
	{
		str[0] = '[';
		str[1] = ']';
		return 2;
	}
	int size;
	int count = 0;
	for (int i = 0; i < m_value.size(); ++i)
	{
		size = n - count;
		if (size <= 1) break;
		str[count++] = ' ';
		str[count] = 0;

		size = n - count;
		if (size <= 1) break;
		count += m_value[i].ToStr(&str[count], size);

		size = n - count;
		if (size <= 1) break;
		str[count++] = ',';
		str[count] = 0;
	}
	str[0] = '[';
	str[count - 1] = ']';
	return count;
}

CSmartType::SmtList::operator double()
{
	return m_value.size();
}

CSmartType::SmtList::operator int()
{
	return m_value.size();
}

CSmartType::SmtList::operator bool()
{
	return m_value.size();
}

CSmartType::ISmartType *CSmartType::SmtDict::Copy()
{
	SmtDict* p = new SmtDict;
	p->m_value = m_value;
	return p;
}

int CSmartType::SmtDict::ToStr(char *str, int n)
{
	if (m_value.size() == 0)
	{
		str[0] = '{';
		str[1] = '}';
		return 2;
	}
	int size;
	int count = 0;
	for (map<CString, CSmartType>::iterator it = m_value.begin(); it != m_value.end(); ++it)
	{
		size = n - count;
		if (size <= 1) break;
		count += snprintf(&str[count], size, " \"%s\": ", (const char*)it->first);

		size = n - count;
		if (size <= 1) break;
		count += it->second.ToStr(&str[count], size);
		size = n - count;
		if (size <= 1) break;
		str[count++] = ',';
		str[count] = 0;
	}
	str[0] = '{';
	str[count - 1] = '}';
	return count;
}

CSmartType::SmtDict::operator double()
{
	return m_value.size();
}

CSmartType::SmtDict::operator int()
{
	return m_value.size();
}

CSmartType::SmtDict::operator bool()
{
	return m_value.size();
}

CSmartType &CSmartType::SmtDict::operator [](const char *key)
{
	return m_value[key];
}

///////////////////////////////////////////////////////////

CSmartType::CSmartType()
{
	m_pSmartTypeImpl = NULL;
}

CSmartType::CSmartType(bool b)
{
	m_pSmartTypeImpl = new SmtBool(b);
}

CSmartType::CSmartType(int i)
{
	m_pSmartTypeImpl = new SmtInt(i);
}

CSmartType::CSmartType(double d)
{
	m_pSmartTypeImpl = new SmtFloat(d);
}

CSmartType::CSmartType(const char *str)
{
	m_pSmartTypeImpl = new SmtStr(str);
}

const CSmartType &CSmartType::GetEmptyList()
{
	static CSmartType* p = NULL;
	if (p == NULL)
	{
		p = new CSmartType;
		p->m_pSmartTypeImpl = new SmtList;
	}
	return *p;
}

const CSmartType &CSmartType::GetEmptyDict()
{
	static CSmartType* p = NULL;
	if (p == NULL)
	{
		p = new CSmartType;
		p->m_pSmartTypeImpl = new SmtDict;
	}
	return *p;
}

CSmartType::~CSmartType()
{
	if (m_pSmartTypeImpl)
	{
		delete m_pSmartTypeImpl;
		m_pSmartTypeImpl = NULL;
	}
}

CSmartType::CSmartType(const CSmartType &e)
{
	m_pSmartTypeImpl = e.m_pSmartTypeImpl ? e.m_pSmartTypeImpl->Copy() : NULL;
}

CSmartType &CSmartType::operator =(const CSmartType &e)
{
	ISmartType* pSmartTypeImpl = m_pSmartTypeImpl;
	m_pSmartTypeImpl = e.m_pSmartTypeImpl ? e.m_pSmartTypeImpl->Copy() : NULL;
	if (pSmartTypeImpl)
		delete pSmartTypeImpl;
	return *this;
}

bool CSmartType::operator ==(const CSmartType &e)
{
	return m_pSmartTypeImpl == e.m_pSmartTypeImpl;
}

bool CSmartType::operator == (const char *str)
{
	SmtStr* p = dynamic_cast<SmtStr*>(m_pSmartTypeImpl);
	return p && str ? strcmp(p->m_value, str) == 0 : false;
}

bool CSmartType::operator !=(const CSmartType &e)
{
	return m_pSmartTypeImpl != e.m_pSmartTypeImpl;
}

CSmartType &CSmartType::SmartInit(const CString& str)
{
	if (str == "true")
		*this = true;
	else if (str == "false")
		*this = false;
	else if (str[0] == '"' && str[-1] == '"')
		*this = (const char*)str.SubStr(1, -1);
	else if (str.Find('.') != -1)
		*this = atof((const char*)str);
	else
		*this = atoi((const char*)str);
}

CSmartType::operator bool() const
{
	return m_pSmartTypeImpl ? *m_pSmartTypeImpl : false;
}

CSmartType::operator int() const
{
	return m_pSmartTypeImpl ? *m_pSmartTypeImpl : 0;
}

CSmartType::operator ssize_t() const
{
	return m_pSmartTypeImpl ? (int)*m_pSmartTypeImpl : 0;
}

CSmartType::operator unsigned() const
{
	return m_pSmartTypeImpl ? (int)*m_pSmartTypeImpl : 0;
}

CSmartType::operator float() const
{
	return m_pSmartTypeImpl ? (double)*m_pSmartTypeImpl : 0;
}

CSmartType::operator double() const
{
	return m_pSmartTypeImpl ? *m_pSmartTypeImpl : 0;
}

CSmartType::operator char *() const
{
	SmtStr* p = dynamic_cast<SmtStr*>(m_pSmartTypeImpl);
	return p ? p->m_value : NULL;
}

CSmartType::operator CString () const
{
	SmtStr* p = dynamic_cast<SmtStr*>(m_pSmartTypeImpl);
	return p ? p->m_value : NULL;
}

CSmartType::operator vector<CSmartType> &()
{
	SmtList* p = dynamic_cast<SmtList*>(m_pSmartTypeImpl);
	if (p == NULL)
	{
		if (m_pSmartTypeImpl)
			delete m_pSmartTypeImpl;
		m_pSmartTypeImpl = new SmtList;
		p = (SmtList*)m_pSmartTypeImpl;
	}
	return p->m_value;
}

CSmartType::operator map<CString, CSmartType> &()
{
	SmtDict* p = dynamic_cast<SmtDict*>(m_pSmartTypeImpl);
	if (p == NULL)
	{
		if (m_pSmartTypeImpl)
			delete m_pSmartTypeImpl;
		m_pSmartTypeImpl = new SmtDict;
		p = (SmtDict*)m_pSmartTypeImpl;
	}
	return p->m_value;
}

CSmartType &CSmartType::operator [](const char *key)
{
	SmtDict* p = dynamic_cast<SmtDict*>(m_pSmartTypeImpl);
	if (p == NULL)
		return NONE;
	return (*p)[key];
}

CSmartType &CSmartType::operator [](int i)
{
	SmtList* p = dynamic_cast<SmtList*>(m_pSmartTypeImpl);
	if (p == NULL)
		return NONE;
	if (i < 0)
		i += p->m_value.size();
	if (i >= p->m_value.size() || i < 0)
		return NONE;
	return (*p)[i];
}

CSmartType &CSmartType::operator [](const CSmartType& key)
{
	SmtInt* pInt = dynamic_cast<SmtInt*>(key.m_pSmartTypeImpl);
	if (pInt)
		return (*this)[pInt->m_value];
	SmtStr* pStr = dynamic_cast<SmtStr*>(key.m_pSmartTypeImpl);
	if (dynamic_cast<SmtStr*>(key.m_pSmartTypeImpl))
		return (*this)[pStr->m_value];
	return NONE;
}

int CSmartType::ToStr(char *str, int n) const
{
	return m_pSmartTypeImpl ? m_pSmartTypeImpl->ToStr(str, n) : snprintf(str, n, "null");
}

CString CSmartType::ToStr() const
{
	CString strBuff = "null";
	if (m_pSmartTypeImpl)
	{
		m_pSmartTypeImpl->ToStr(strBuff.GetBuffer(1024), 1024);
		strBuff.ReleaseBuffer();
	}
	return strBuff;
}

