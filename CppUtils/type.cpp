#include "type.h"

CValueType::CValueType(EValueType t, void *v)
{
	m_type = t;
	m_value = v;
}

CValueType::~CValueType()
{
	Release();
}

CValueType::CValueType(const CValueType &v)
{
	switch (v.m_type)
	{
	case TYPE_STRING:
		SetString(v.GetString());
		break;
	case TYPE_LIST:
		SetList();
		GetList() = v.GetList();
		break;
	case TYPE_DICT:
		SetDict();
		GetDict() = v.GetDict();
		break;
	default:
		m_type = v.m_type;
		m_value = v.m_value;
		break;
	}
}

ssize_t CValueType::GetInt() const
{
	return (ssize_t)m_value;
}

void CValueType::SetInt(ssize_t i)
{
	m_type = TYPE_INT;
	m_value = (void*)i;
}

double CValueType::GetFloat() const
{
	double d;
	memcpy(&d, &m_value, sizeof(void*));
	return d;
}

void CValueType::SetFloat(double d)
{
	m_type = TYPE_FLOAT;
	memcpy(&m_value, &d, sizeof(void*));
}

bool CValueType::GetBool() const
{
	return (bool)m_value;
}

void CValueType::SetBool(bool b)
{
	m_type = TYPE_BOOL;
	m_value = (void*)b;
}

char *CValueType::GetString() const
{
	static char undefined[] = "undefined";
	if (m_value)
		return (char*)m_value;
	else
		return undefined;
}

void CValueType::SetString(const char *str)
{
	Release();
	m_value = new char[strlen(str)+1];
	strcpy((char*)m_value, str);
	m_type = TYPE_STRING;
}

vector<CValueType> &CValueType::GetList() const
{
	return *((vector<CValueType>*)m_value);
}

void CValueType::SetList()
{
	Release();
	m_type = TYPE_LIST;
	m_value = new vector<CValueType>;
}

CDict<CValueType> &CValueType::GetDict() const
{
	return *((CDict<CValueType>*)m_value);
}

void CValueType::SetDict()
{
	Release();
	m_type = TYPE_DICT;
	m_value = new CDict<CValueType>;
}

void CValueType::SetStrInt(const CStr &str)
{
	if (str == "true")
		SetBool(true);
	else if (str == "false")
		SetBool(false);
	else if (str[0] == '"' && str[-1] == '"')
		SetString(str.SubStr(1, -1));
	else if (str.Find('.') != -1)
		SetFloat(atof((const char*)str));
	else
		SetInt(atoi((const char*)str));
}

void CValueType::Release()
{
	if (m_value)
	{
		switch (m_type)
		{
		case TYPE_STRING:
			delete[] (char*)m_value;
			break;
		case TYPE_LIST:
			delete (vector<CValueType>*)m_value;
			break;
		case TYPE_DICT:
			delete (DICT(CValueType)*)m_value;
			break;
		}

		m_value = NULL;
	}
}
