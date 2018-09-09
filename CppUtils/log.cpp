#include "log.h"


#define WRITE(e) \
	Wirte(_(e))

CLog::CLog(const char *file)
{
	if (file)
		m_filename = file;
}

CLog::~CLog()
{

}

DEF_INSTANCE(CLog)

int CLog::Wirte(const char *str)
{
	unsigned len = strlen(str);
	if (m_filename.Empty())
	{
		cout << str << " ";
		return len;
	}
	else
	{
		FILE* pf = fopen(m_filename, "a");
		fwrite(str, 1, len, pf);
		fwrite(" ", 1, 1, pf);
		fclose(pf);
	}
}

void CLog::operator()(const char *str)
{
	Wirte(str);
}

CLog& CLog::operator<<(CLog& (*func)(CLog& out))
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

CLog & CLog::operator<<(const bool b)
{
	WRITE(b);
	return *this;
}

CLog& CLog::operator<<(const int i)
{
	WRITE(i);
	return *this;
}

CLog & CLog::operator<<(const unsigned int u)
{
	WRITE(u);
	return *this;
}

CLog & CLog::operator<<(const float f)
{
	WRITE(f);
	return *this;
}

CLog & CLog::operator<<(const double d)
{
	WRITE(d);
	return *this;
}

CLog & CLog::operator<<(const char c)
{
	WRITE(c);
	return *this;
}

CLog & CLog::operator<<(const char *p)
{
	Wirte(p);
	return *this;
}

CLog& endl(CLog& out)
{
	return out << "\n";
}

CLog* g_log = CLog::Instance();


void setup_log(const char *filename)
{
	int init = 1;
	if (init)
	{
		init = 0;
		g_log = new CLog(filename);
	}
}
