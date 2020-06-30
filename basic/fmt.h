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
#define Prints(...)  printer << __VA_ARGS__
#define Printf(fmt, ...) format_printer.Format(fmt) , __VA_ARGS__

