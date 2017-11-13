#include "file.h"

//int fileno(FILE * stream)
//FILE * fdopen(int fd, const char * mode)

File::File(FILE *f)
{
	pf = f;
}


File::~File()
{
	if (pf)
	{
		fclose(pf);
		pf = NULL;
	}
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

CStr& File::read(int size)
{
	if (size < 0)
		size = 1023;
	fread(strBuff.GetBuffer(size + 1), 1, size, pf);
	strBuff.ReleaseBuffer();
	return strBuff;
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

FILE *File::Attach(FILE *f)
{
	FILE* temp = pf;
	pf = f;
	return temp;
}

FILE *File::Detach()
{
	FILE* temp = pf;
	pf = NULL;
	return temp;
}

File File::OpenFile(const char *file, const char *mode)
{
	return File(fopen(file, mode));
}

File File::OpenPipe(const char *cmd, const char *mode)
{
	return File(popen(cmd, mode));
}

