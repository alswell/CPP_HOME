#include "fmt.h"
#include "type.h"

CString m_buf;
const char* fmt_str = "%s";
const char* fmt_char = "%c";
const char* fmt_int = "%d";
const char* fmt_uint = "%u";
const char* fmt_float = "%.2f";


const char * ToStr(bool b)
{
	m_buf.Format(fmt_str, b ? "TRUE!" : "FALSE!");
	return m_buf;
}

const char * ToStr(char c)
{
	m_buf.Format(fmt_char, c);
	return m_buf;
}

const char* ToStr(int i)
{
	m_buf.Format(fmt_int, i);
	return m_buf;
}

const char *ToStr(long l)
{
	m_buf.Format(fmt_int, l);
	return m_buf;
}

const char * ToStr(unsigned int u)
{
	m_buf.Format(fmt_uint, u);
	return m_buf;
}

const char * ToStr(float f)
{
	m_buf.Format(fmt_float, f);
	return m_buf;
}

const char * ToStr(double d)
{
	m_buf.Format(fmt_float, d);
	return m_buf;
}


const char *ToStr(const CSmartType &v)
{
	v.ToStr(m_buf.GetBuffer(1024), 1024);
	m_buf.ReleaseBuffer();
	return m_buf;
}

char __buff[128];
char* __(float f)
{
	sprintf(__buff, "%f", f);
	return __buff;
}
