#include "fmt.h"
#include "type.h"

CString FMT::m_buf;
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


const char *FMT::Fmt(const CSmartType &v)
{
	v.ToStr(m_buf.GetBuffer(1024), 1024);
	m_buf.ReleaseBuffer();
	return m_buf;
}
