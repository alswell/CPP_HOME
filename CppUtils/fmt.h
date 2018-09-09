#pragma once
#include "str.h"

class CSmartType;
namespace FMT
{
	extern CString m_buf;
	extern const char* fmt_str;
	extern const char* fmt_char;
	extern const char* fmt_int;
	extern const char* fmt_uint;
	extern const char* fmt_float;
	extern const char* fmt_double;
	const char* Fmt(bool b);
	const char* Fmt(char c);
	const char* Fmt(int i);
	const char* Fmt(long l);
	const char* Fmt(unsigned int u);
	const char* Fmt(float f);
	const char* Fmt(double d);
	const char* Fmt(const CSmartType &v);
}
#define _(x) FMT::Fmt(x)

char* __(float f);

