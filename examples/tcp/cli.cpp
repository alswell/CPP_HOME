#include "io/sock.h"
#include <iostream>
using namespace std;

int main()
{
	CSock cli;
	cli.Connect("127.0.0.1", 8866);
	cout << cli.ReadLine() << endl;
	return 0;
}
