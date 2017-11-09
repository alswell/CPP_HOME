#ifndef __LOG_H
#define __LOG_H
#include "str.h"

struct FMT
{
	static const char* fmt_bool;
	static const char* fmt_char;
	static const char* fmt_int;
	static const char* fmt_uint;
	static const char* fmt_float;
	static const char* fmt_double;
	static const char* Get(bool);
	static const char* Get(char);
	static const char* Get(int);
	static const char* Get(unsigned int);
	static const char* Get(float);
	static const char* Get(double);
};

class CToStr
{
	CStr m_buf;
public:
	const char* m_fmt;
	CToStr();

	template <class T>
	CToStr(T e)
	{
		m_fmt = FMT::Get(e);
	}

	template <class T>
	const char* operator[] (T e)
	{
		const char* old_fmt = m_fmt;
		m_fmt = FMT::Get(e);
		this->operator()(e);
		m_fmt = old_fmt;
		return m_buf;
	}

	const char* operator() (bool b);
	const char* operator() (char c);
	const char* operator() (int i);

	const char* operator() (unsigned int u);
	const char* operator() (float f);
	const char* operator() (double d);
};

extern CToStr _;

class LogOut
{
	CStr m_filename;

public:
	LogOut(const char* file = NULL);
	~LogOut(void);
	static LogOut* Instance();

	int Wirte(const char* str);
	void operator () (const char* str);
	LogOut& operator << (LogOut& (*func)(LogOut& out));
	LogOut& operator << (const bool b);
	LogOut& operator << (const int i);
	LogOut& operator << (const unsigned int u);
	LogOut& operator << (const float f);
	LogOut& operator << (const double d);
	LogOut& operator << (const char c);
	LogOut& operator << (const char *p);
};
LogOut& endl(LogOut& out);

extern LogOut* g_log;
void setup_log(const char* filename);
#define LOG (*g_log)

#endif
