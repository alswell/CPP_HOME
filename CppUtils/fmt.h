#pragma once
#include "str.h"

class CSmartType;

extern CString m_buf;
extern const char* fmt_str;
extern const char* fmt_char;
extern const char* fmt_int;
extern const char* fmt_uint;
extern const char* fmt_float;
extern const char* fmt_double;
const char* ToStr(bool b);
const char* ToStr(char c);
const char* ToStr(int i);
const char* ToStr(long l);
const char* ToStr(unsigned int u);
const char* ToStr(float f);
const char* ToStr(double d);
const char* ToStr(const CSmartType &v);

char* __(float f);

#define _(x) ((const char*)CString(ToStr(x)))

