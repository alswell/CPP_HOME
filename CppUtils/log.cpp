#include "log.h"

const char* FMT::fmt_bool = "%s";
const char* FMT::fmt_char = "%c";
const char* FMT::fmt_int = "%d";
const char* FMT::fmt_uint = "%u";
const char* FMT::fmt_float = "%.2f";
const char* FMT::fmt_double = "%.2f";

const char * FMT::Get(bool)
{
	return fmt_bool;
}

const char * FMT::Get(char)
{
	return fmt_char;
}

const char* FMT::Get(int)
{
	return fmt_int;
}

const char * FMT::Get(unsigned int)
{
	return fmt_uint;
}

const char * FMT::Get(float)
{
	return fmt_float;
}

const char * FMT::Get(double)
{
	return fmt_double;
}

CToStr::CToStr()
{
	m_fmt = NULL;
}

const char * CToStr::operator()(bool b)
{
	m_buf.Format(m_fmt, b ? "TRUE!" : "FALSE!");
	return m_buf;
}

const char * CToStr::operator()(char c)
{
	m_buf.Format(m_fmt, c);
	return m_buf;
}

const char* CToStr::operator()(int i)
{
	m_buf.Format(m_fmt, i);
	return m_buf;
}

const char * CToStr::operator()(unsigned int u)
{
	m_buf.Format(m_fmt, u);
	return m_buf;
}

const char * CToStr::operator()(float f)
{
	m_buf.Format(m_fmt, f);
	return m_buf;
}

const char* CToStr::operator()(double d)
{
	m_buf.Format(m_fmt, d);
	return m_buf;
}

CToStr _;

#define WRITE(e) \
	Wirte(_[e])
//#define WRITE(e) \
//	CToStr to_str(e);\
//	Wirte(to_str(e))

LogOut::LogOut(const char *file)
{
	if (file)
		m_filename = file;
}

LogOut::~LogOut()
{

}

DEF_INSTANCE(LogOut)

int LogOut::Wirte(const char *str)
{
	unsigned len = strlen(str);
	if (m_filename.Empty())
	{
		cout << str;
		return len;
	}
	else
	{
		FILE* pf = fopen(m_filename, "a");
		fwrite(str, 1, len, pf);
		fclose(pf);
	}
}

void LogOut::operator()(const char *str)
{
	Wirte(str);
}

LogOut& LogOut::operator<<(LogOut& (*func)(LogOut& out))
{
	if (m_filename.Empty())
	{
		cout << endl;
		return *this;
	}
	else
	{
		return func(*this);
	}
}

LogOut & LogOut::operator<<(const bool b)
{
	WRITE(b);
	return *this;
}

LogOut& LogOut::operator<<(const int i)
{
	WRITE(i);
	return *this;
}

LogOut & LogOut::operator<<(const unsigned int u)
{
	WRITE(u);
	return *this;
}

LogOut & LogOut::operator<<(const float f)
{
	WRITE(f);
	return *this;
}

LogOut & LogOut::operator<<(const double d)
{
	WRITE(d);
	return *this;
}

LogOut & LogOut::operator<<(const char c)
{
	WRITE(c);
	return *this;
}

LogOut & LogOut::operator<<(const char *p)
{
	Wirte(p);
	return *this;
}

LogOut& endl(LogOut& out)
{
	return out << "\n";
}

LogOut* g_log = LogOut::Instance();


void setup_log(const char *filename)
{
	int init = 1;
	if (init)
	{
		init = 0;
		g_log = new LogOut(filename);
	}
}
