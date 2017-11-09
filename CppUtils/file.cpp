#include "file.h"

//int fileno(FILE * stream)
//FILE * fdopen(int fd, const char * mode)

File::File(const char *file, const char *mode)
{
	pf = fopen(file, mode);
}


File::~File()
{
	fclose(pf);
}

int File::write(const char *str)
{
	return fwrite(str, 1, strlen(str), pf);
}

int File::writeline(const char *str)
{
	int r = fwrite(str, 1, strlen(str), pf);
	fwrite("\n", 1, 1, pf);
	return r;
}

list<CStr> File::readlines()
{
	fseek(pf, 0, SEEK_END);
	unsigned nSize = ftell(pf);
	rewind(pf);
	fread(strBuff.GetBuffer(nSize + 1), 1, nSize, pf);
	strBuff.ReleaseBuffer();
	return strBuff.Split('\n');
}

