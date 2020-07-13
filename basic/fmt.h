#pragma once

void Output(void* p);
void Output(bool b);
void Output(char c);
void Output(int x);
void Output(unsigned int x);
void Output(unsigned long x);
void Output(long l);
void Output(float x);
void Output(double d);
void Output(const char* const s);

#include <list>
#include <vector>
using namespace std;
#define OUTPUT_LIST(type) \
template <class T>\
void Output(type<T> ls)\
{\
	Output('[');\
	for (auto it = ls.begin(); it != ls.end(); ++it)\
	{\
		if (it != ls.begin())\
			Output(", ");\
		Output(*it);\
	}\
	Output(']');\
}
OUTPUT_LIST(list)
OUTPUT_LIST(vector)

template <class T>
void Output(T* p, unsigned n)
{
	Output('[');
	for (unsigned i = 0; i < n; ++i)
	{
		if (i != 0)
			Output(", ");
		Output(p[i]);
	}
	Output(']');
}

struct Printer
{
	template<class T>
	Printer& operator << (T x)
	{
		Output(x);
		return *this;
	}
	template<class T>
	Printer& operator , (T x)
	{
		Output(' ');
		Output(x);
		return *this;
	}
};
extern Printer printer;

struct FormatPrinter
{
	//const char* fmt;
	const char* p;
	void Move();
	FormatPrinter& Format(const char* str);
	template<class T>
	FormatPrinter& operator , (T x)
	{
		Output(x);
		Move();
		return *this;
	}
};
extern FormatPrinter format_printer;

#define NewLine() printer << '\n'
#define Println(...) (printer << __VA_ARGS__) << '\n'
#define Prints(...) printer << __VA_ARGS__
#define Printf(fmt, ...) format_printer.Format(fmt) , __VA_ARGS__

