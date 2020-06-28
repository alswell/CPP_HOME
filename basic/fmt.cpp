#include "fmt.h"
#include "iostream"
using namespace std;

Printer printer;

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

Printer::Printer()
{
	sep = ' ';
}

void Printer::EndLine()
{
	cout << endl;
}
