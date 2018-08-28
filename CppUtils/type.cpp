#include "type.h"


CSmartType::ISmartType::~ISmartType()
{
}

CSmartType::CBOOL::CBOOL(bool b)
	: m_value(b)
{

}

CSmartType::ISmartType *CSmartType::CBOOL::Copy()
{
	return new CBOOL(m_value);
}

int CSmartType::CBOOL::ToStr(char *str, int n)
{
	return snprintf(str, n, "%s", m_value ? "TRUE" : "FALSE");
}

CSmartType::CINT::CINT(ssize_t i)
	: m_value(i)
{
}

CSmartType::ISmartType *CSmartType::CINT::Copy()
{
	return new CINT(m_value);
}

int CSmartType::CINT::ToStr(char *str, int n)
{
	return snprintf(str, n, "%d", m_value);
}

CSmartType::CFLOAT::CFLOAT(double d)
	: m_value(d)
{
}

CSmartType::ISmartType *CSmartType::CFLOAT::Copy()
{
	return new CFLOAT(m_value);
}

int CSmartType::CFLOAT::ToStr(char *str, int n)
{
	return snprintf(str, n, "%f", m_value);
}

CSmartType::CSTRING::CSTRING(const char *str)
{
	m_value = new char[strlen(str) + 1];
	strcpy(m_value, str);
}

CSmartType::CSTRING::~CSTRING()
{
	delete m_value;
}

CSmartType::CSTRING &CSmartType::CSTRING::operator =(const CSTRING &str)
{
	m_value = new char[strlen(str.m_value) + 1];
	strcpy(m_value, str.m_value);
	return *this;
}

CSmartType::ISmartType *CSmartType::CSTRING::Copy()
{
	return new CSTRING(m_value);
}

int CSmartType::CSTRING::ToStr(char *str, int n)
{
	return snprintf(str, n, "%s", m_value);
}

CSmartType &CSmartType::CLIST::operator [](size_t n)
{
	return m_value[n];
}

CSmartType::ISmartType *CSmartType::CLIST::Copy()
{
	CLIST* p = new CLIST;
	p->m_value = m_value;
	return p;
}

int CSmartType::CLIST::ToStr(char *str, int n)
{
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

CSmartType::ISmartType *CSmartType::CDICT::Copy()
{
	CDICT* p = new CDICT;
	p->m_value = m_value;
	return p;
}

int CSmartType::CDICT::ToStr(char *str, int n)
{
	int size;
	int count = 0;
	for (map<CString, CSmartType>::iterator it = m_value.begin(); it != m_value.end(); ++it)
	{
		size = n - count;
		if (size <= 1) break;
		count += snprintf(&str[count], size, " \"%s\": ", (const char*)it->first);

		size = n - count;
		if (size <= 1) break;
		const char* p = it->second;
		if (p)
		{
			str[count++] = '"';
			size = n - count;
			if (size <= 1) break;
		}
		count += it->second.ToStr(&str[count], size);
		if (p)
		{
			size = n - count;
			if (size <= 1) break;
			str[count++] = '"';
		}

		size = n - count;
		if (size <= 1) break;
		str[count++] = ',';
		str[count] = 0;
	}
	str[0] = '{';
	str[count - 1] = '}';
	return count;
}

CSmartType &CSmartType::CDICT::operator [](const char *key)
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
	m_pSmartTypeImpl = new CBOOL(b);
}

CSmartType::CSmartType(int i)
{
	m_pSmartTypeImpl = new CINT(i);
}

CSmartType::CSmartType(double d)
{
	m_pSmartTypeImpl = new CFLOAT(d);
}

CSmartType::CSmartType(const char *str)
{
	m_pSmartTypeImpl = new CSTRING(str);
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
	if (m_pSmartTypeImpl)
		delete m_pSmartTypeImpl;
	m_pSmartTypeImpl = e.m_pSmartTypeImpl->Copy();
	return *this;
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

CSmartType::operator bool()
{
	CBOOL* p = dynamic_cast<CBOOL*>(m_pSmartTypeImpl);
	return p ? p->m_value : false;
}

CSmartType::operator int()
{
	CINT* p = dynamic_cast<CINT*>(m_pSmartTypeImpl);
	return p ? p->m_value : 0;
}

CSmartType::operator ssize_t()
{
	CINT* p = dynamic_cast<CINT*>(m_pSmartTypeImpl);
	return p ? p->m_value : 0;
}

CSmartType::operator double()
{
	CFLOAT* p = dynamic_cast<CFLOAT*>(m_pSmartTypeImpl);
	return p ? p->m_value : 0;
}

CSmartType::operator const char *()
{
	CSTRING* p = dynamic_cast<CSTRING*>(m_pSmartTypeImpl);
	return p ? p->m_value : NULL;
}

vector<CSmartType> g_vNULL;
CSmartType::operator vector<CSmartType> &()
{
	CLIST* p = dynamic_cast<CLIST*>(m_pSmartTypeImpl);
	return p ? p->m_value : g_vNULL;
}

map<CString, CSmartType> g_dNULL;
CSmartType::operator map<CStr, CSmartType> &()
{
	CDICT* p = dynamic_cast<CDICT*>(m_pSmartTypeImpl);
	return p ? p->m_value : g_dNULL;
}

CSmartType &CSmartType::operator [](const char *key)
{
	CDICT* p = dynamic_cast<CDICT*>(m_pSmartTypeImpl);
	if (p == NULL)
		return *this;
	return (*p)[key];
}

int CSmartType::ToStr(char *str, int n) const
{
	return m_pSmartTypeImpl ? m_pSmartTypeImpl->ToStr(str, n) : -1;
}

CSmartType &CSmartType::ToList()
{
	if (m_pSmartTypeImpl)
		delete m_pSmartTypeImpl;
	m_pSmartTypeImpl = new CLIST;
	return *this;
}

CSmartType &CSmartType::ToDict()
{
	if (m_pSmartTypeImpl)
		delete m_pSmartTypeImpl;
	m_pSmartTypeImpl = new CDICT;
	return *this;
}
