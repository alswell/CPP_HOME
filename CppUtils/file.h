#ifndef __COMMON_FILE_H
#define __COMMON_FILE_H

#include "str.h"

class File
{
	FILE* pf;
	CStr strBuff;
public:
	File(const char* file, const char* mode);
	~File();

	int write(const char* str);
	int writeline(const char* str);
	list<CStr> readlines();
};

#endif
