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
	char sep;
	Printer();
	Printer& SetSep(char c)
	{
		sep = c;
		return *this;
	}
	template<class T>
	Printer& operator << (T x)
	{
		Output(x);
		return *this;
	}
	template<class T>
	Printer& operator , (T x)
	{
		Output(sep);
		Output(x);
		return *this;
	}
	void EndLine();
};
extern Printer printer;
#define Println(...) (printer << __VA_ARGS__).EndLine()
#define Print_(...)  printer << __VA_ARGS__
#define PrintXX(...) (printer.SetSep(0) << __VA_ARGS__).SetSep(' ')
