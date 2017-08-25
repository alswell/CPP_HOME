#include "Sock.h"

int main()
{
	CSock cli("127.0.0.1", 8866);
	cli.Write("hello", 5);
	return 0;
}
