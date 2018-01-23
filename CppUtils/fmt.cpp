#include "fmt.h"
#include "type.h"

CStr FMT::m_buf;
const char* FMT::fmt_str = "%s";
const char* FMT::fmt_char = "%c";
const char* FMT::fmt_int = "%d";
const char* FMT::fmt_uint = "%u";
const char* FMT::fmt_float = "%.2f";


const char * FMT::Fmt(bool b)
{
	m_buf.Format(fmt_str, b ? "TRUE!" : "FALSE!");
	return m_buf;
}

const char * FMT::Fmt(char c)
{
	m_buf.Format(fmt_char, c);
	return m_buf;
}

const char* FMT::Fmt(int i)
{
	m_buf.Format(fmt_int, i);
	return m_buf;
}

const char *FMT::Fmt(long l)
{
	m_buf.Format(fmt_int, l);
	return m_buf;
}

const char * FMT::Fmt(unsigned int u)
{
	m_buf.Format(fmt_uint, u);
	return m_buf;
}

const char * FMT::Fmt(float f)
{
	m_buf.Format(fmt_float, f);
	return m_buf;
}

const char * FMT::Fmt(double d)
{
	m_buf.Format(fmt_float, d);
	return m_buf;
}


const char *FMT::Fmt(const CValueType &v)
{
	switch (v.m_type) {
	case TYPE_BOOL:
		Fmt(v.GetBool());
		break;
	case TYPE_STRING:
		m_buf.Format("'%s'", v.GetString());
		break;
	case TYPE_INT:
		Fmt(v.GetInt());
		break;
	case TYPE_FLOAT:
		Fmt(v.GetFloat());
		break;
	case TYPE_DICT:
		m_buf = v.GetDict().Str();
		break;
	case TYPE_LIST:
	{
		CStr buff = "[";
		for (int i = 0; i < v.GetList().size(); ++i)
		{
			buff.AppendFormat("%s, ", Fmt(v.GetList()[i]));
		}
		buff += "]";
		m_buf = buff;
	}
		break;
	default:
		return NULL;
		break;
	}
	return m_buf;
}
