#include "fmt.h"
#include "iostream"
using namespace std;

Printer printer;
FormatPrinter format_printer;

void Output(void* p)
{
	cout << p;
}

void Output(bool b)
{
	cout << (b ? "TRUE!" : "FALSE!");
}

void Output(char c)
{
	cout << c;
}

void Output(int x)
{
	cout << x;
}

void Output(unsigned int x)
{
	cout << x;
}

void Output(unsigned long x)
{
	cout << x;
}

void Output(long l)
{
	cout << l;
}

void Output(float x)
{
	cout << x;
}

void Output(double d)
{
	cout << d;
}

void Output(const char* const s)
{
	if (s == nullptr)
		cout << "null";
	else
		cout << s;
}

void FormatPrinter::Move()
{
	while (*p)
	{
		switch (*p)
		{
		case '%':
			if ((p[1]) != '%')
			{
				//fmt = p;
				while (*(p++) != 's');
				return;
			}
			Output('%');
			++p;
			break;
		default:
			Output(*p);
		}
		++p;
	}
}

FormatPrinter& FormatPrinter::Format(const char* str)
{
	//fmt = nullptr;
	p = str;
	return *this, '\0';
}

