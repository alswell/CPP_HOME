#include "fmt.h"
#include "type.h"

CString g_strFmtBuf;
const char* fmt_str = "%s";
const char* fmt_char = "%c";
const char* fmt_int = "%d";
const char* fmt_uint = "%u";
const char* fmt_float = "%.2f";


const char * ToStr(bool b)
{
	g_strFmtBuf.Format(fmt_str, b ? "TRUE!" : "FALSE!");
	return g_strFmtBuf;
}

const char * ToStr(char c)
{
	g_strFmtBuf.Format(fmt_char, c);
	return g_strFmtBuf;
}

const char* ToStr(int i)
{
	g_strFmtBuf.Format(fmt_int, i);
	return g_strFmtBuf;
}

const char *ToStr(long l)
{
	g_strFmtBuf.Format(fmt_int, l);
	return g_strFmtBuf;
}

const char * ToStr(unsigned int u)
{
	g_strFmtBuf.Format(fmt_uint, u);
	return g_strFmtBuf;
}

const char * ToStr(float f)
{
	g_strFmtBuf.Format(fmt_float, f);
	return g_strFmtBuf;
}

const char * ToStr(double d)
{
	g_strFmtBuf.Format(fmt_float, d);
	return g_strFmtBuf;
}


const char *ToStr(const CSmartType &v)
{
	v.ToStr(g_strFmtBuf.GetBuffer(1024), 1024);
	g_strFmtBuf.ReleaseBuffer();
	return g_strFmtBuf;
}

const char *ToStr(const CSmartType *v)
{
	v->ToStr(g_strFmtBuf.GetBuffer(1024), 1024);
	g_strFmtBuf.ReleaseBuffer();
	return g_strFmtBuf;
}

char __buff[128];
char* __(float f)
{
	sprintf(__buff, "%f", f);
	return __buff;
}
