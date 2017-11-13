#ifndef __COMMON_FILE_H
#define __COMMON_FILE_H

#include "str.h"

class File
{
	FILE* pf;
	CStr strBuff;
public:
	File(FILE* f = NULL);
	~File();

	int write(const char* str);
	int writeline(const char* str);
	CStr &read(int size = -1);
	list<CStr> readlines();

	FILE *Attach(FILE* f);
	FILE *Detach();
	static File OpenFile(const char* file, const char* mode);
	static File OpenPipe(const char* cmd, const char* mode);
};

#endif
